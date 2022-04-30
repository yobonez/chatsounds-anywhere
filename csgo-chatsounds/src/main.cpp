#include <iostream>
#include <vector>
#include <regex>
#include <filesystem>
#include <windows.h>

#include "SDL2ChatSounds.h"
#include "proc.h"

namespace fs = std::filesystem;

// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
std::string trim(const std::string& s)
{
	if (s != "") {
		auto start = s.begin();
		while (start != s.end() && std::isspace(*start)) {
			start++;
		}

		auto end = s.end();
		do {
				end--;
		} while (std::distance(start, end) > 0 && std::isspace(*end));

		return std::string(start, end + 1);
	}

	else {
		return "";
	}
}

/*Gets the nameAndSize vector with chatsounds name lenghts and names itself and fills it.
  Returns vector with names and paths that will be used in later code to not
  search for the same chatsounds over again*/
std::multimap<std::string, std::string> loadAndSortDescending (std::vector<std::pair<size_t, std::string>>& nameAndSize_ref)
{
	// chatsoundy sa zapisywane z nazw¹ samego numerka np. 1.ogg w przypadku gdy to jest podfolder z roznymi
	// wersjami tego samego chatsounda, trzeba podmienic na ta sama nazwe podfolderu wszystkie numerki
	std::multimap<std::string, std::string> chatsound_paths;

	std::string path("C:/Users/bonzo/Desktop/csgo-chatsounds/chatsounds/sounds/chatsounds");
	std::string ext(".ogg");

	std::string temp;

	std::string currently_scanned_dir;
	std::string current_parent_dir;
	std::string current_parent_dir_path;

	std::string omit;

	for (auto& p : fs::recursive_directory_iterator(path))
	{

		current_parent_dir_path = p.path().parent_path().string();
		current_parent_dir = p.path().parent_path().stem().string();

		if (omit == p.path().parent_path().string()) 
			continue;

		if (current_parent_dir == "chatsounds")
		{
			currently_scanned_dir = p.path().stem().string();
		}

		// if current path isn't folder and current_parent_dir isn't "chatsounds" THEN add
		if (p.is_directory() != true && current_parent_dir != "chatsounds" && current_parent_dir == currently_scanned_dir && p.path().extension() == ext)
		{
			temp = p.path().stem().string(); // ogg file in this case
			chatsound_paths.insert(std::make_pair(temp, p.path().string()));
			nameAndSize_ref.push_back(std::make_pair(temp.size(), temp));
		}


		// if child is a folder THEN scan folder and add all
		else if (p.is_directory() != true && current_parent_dir != "chatsounds" && current_parent_dir != currently_scanned_dir)
		{
			omit = p.path().parent_path().string();
			std::string temp = current_parent_dir;
			for (auto& subp : fs::recursive_directory_iterator(current_parent_dir_path))
			{
				chatsound_paths.insert(std::make_pair(current_parent_dir, subp.path().string()));
			}
			nameAndSize_ref.push_back(std::make_pair(temp.size(), temp));
		}
	}
	std::sort(nameAndSize_ref.begin(), nameAndSize_ref.end(), std::greater<std::pair<size_t, std::string>>());
	return chatsound_paths;
}
																																// TODO: przerob te funkcje tak, ¿eby cos zwracaly i by je
																																// mozna bylo uzyc w mainie a nie robiæ million callbacków
																																// z jednej funkcji do drugiej, bo sie robi potezny syf
																																// glupie nawyki z pythona
void parseChatsounds (std::string content_copy, std::vector<std::pair<size_t, std::string>>& nameAndSize_ref, SDL2ChatSounds& sdl2_chatsounds_ref, std::multimap<std::string, std::string>& chatsound_paths_ref)
{
	std::string toAnalyzeLater = content_copy;

	std::vector<std::string> toPlay;
	std::vector<std::string> toPlay_sorted;

	while (content_copy != "")
	{
		for (auto& chtsnd : nameAndSize_ref)
		{
			std::string chatsound = chtsnd.second;
			std::regex rgx("\\b(" + chatsound + ")\\b");
			std::smatch match;

			if (content_copy == "")
				break;

			if (std::regex_search(content_copy, match, rgx) == true && match.str(0) != "")
			{
				toPlay.push_back(match.str(0));
				content_copy = std::regex_replace(content_copy, rgx, "", std::regex_constants::format_first_only);
				content_copy = trim(content_copy);
			}
		}
		break;
	}


	// ustalanie kolejnoœci chatsoundów, tak jak na czacie
	while (toAnalyzeLater != "")
	{
		for (unsigned short int i = 0; i < toPlay.size() + 1; i++)
		{
			for (auto& x : toPlay)
			{
				std::regex rgx("\\b(^" + x + ")\\b");
				std::smatch match;

				if (std::regex_search(toAnalyzeLater, match, rgx) && match.str(0) != "")
				{
					toAnalyzeLater = std::regex_replace(toAnalyzeLater, rgx, "");
					toAnalyzeLater = trim(toAnalyzeLater);
					toPlay_sorted.push_back(x);
				}
			}
		}

		// top 10 stupid fixes
		// or is it?
		for (char character : toAnalyzeLater)
		{
			if (toAnalyzeLater[0] == ' ')
				toAnalyzeLater.erase(0, 1);
			else if (character == ' ')
				break;
			else
				toAnalyzeLater.erase(0, 1);
		}
	}

	for (std::string& chatsound_query : toPlay_sorted)
	{
		sdl2_chatsounds_ref.addChatSound(chatsound_query, chatsound_paths_ref);
	}

	sdl2_chatsounds_ref.playChatSounds(toPlay_sorted);
}

int main ()
{
	SDL2ChatSounds sdl2_chatsounds;

	std::vector<std::pair<size_t, std::string>> nameAndSize;
	std::multimap<std::string, std::string> chatsound_paths = loadAndSortDescending(nameAndSize);

	// Windows api stuff
	DWORD procId = GetProcId(L"csgo.exe");
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"panorama.dll");
	//uintptr_t moduleBase2 = GetModuleBaseAddress(procId, L"client.dll");
	HANDLE hProcess = 0;

	hProcess = OpenProcess(PROCESS_VM_READ, NULL, procId);

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x0023B0AC;
	//std::cout << "DPB: " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	std::vector<unsigned int> chatOffsets = { 0x14C, 0x74, 0xA4, 0x204, 0x2A8, 0x100, 0x0 };
	uintptr_t chatAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, chatOffsets);

	//std::cout << "CA: " << "0x" << std::hex << chatAddr << std::endl;

	std::regex normalChatMessagePattern("<[^>]*>");
	std::regex MessageContentPattern("^[^:]+:\s*");

	char message[512];
	std::string prevMessage;

	while (true)
	{
		ReadProcessMemory(hProcess, (BYTE*)chatAddr, &message, sizeof(message), nullptr);
		std::string msg = (std::string)message;
		
		if (msg != prevMessage)
		{
			std::string normalChatMessage = std::regex_replace(msg, normalChatMessagePattern, "");
			//std::cout << normalChatMessage << std::endl;


			std::string content = std::regex_replace(normalChatMessage, MessageContentPattern, "");
			content.erase(0, 1);

			std::cout << "\"Raw\" Input: " << content << std::endl;

			if (content == "sh")
			{
				prevMessage = msg;

				Mix_HaltChannel(-1);
				//Mix_Resume(-1);
				Sleep(100);
				continue;
			}

			parseChatsounds(content, nameAndSize, sdl2_chatsounds, chatsound_paths);
		}

		prevMessage = msg;
		Sleep(100);
	}

	return 0;
}