#include <iostream>
#include <vector>
#include <regex>
#include <filesystem>
#include <windows.h>

#include "ChatSounds.h"
#include "proc.h"
#include "utils.h"
#include "Tags.h"

namespace fs = std::filesystem;

/*Returns chat memory address and a handle to the process*/
std::pair<HANDLE, uintptr_t> getChatAddr() 
{
	std::pair<HANDLE, uintptr_t> hProcess_and_ChatAddr;

	DWORD procId = GetProcId(L"csgo.exe");
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"panorama.dll");
	HANDLE hProcess = 0;

	hProcess = OpenProcess(PROCESS_VM_READ, NULL, procId);

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x0023B0AC;
	//std::cout << "DPB: " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	std::vector<unsigned int> chatOffsets = { 0x14C, 0x74, 0xA4, 0x204, 0x2A8, 0x100, 0x0 };
	uintptr_t chatAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, chatOffsets);
	//std::cout << "CA: " << "0x" << std::hex << chatAddr << std::endl;

	hProcess_and_ChatAddr = std::make_pair(hProcess, chatAddr);

	return hProcess_and_ChatAddr;
}

/*Takes: 
	- vector<name length, name> of each chatsound. 
Loads all chatsounds to it and sorts them.
Returns chatsound_paths multimap<name, path> for future use in ChatSounds object's method*/
std::vector<std::pair<std::string, std::string>> loadAndSort (std::vector<std::pair<size_t, std::string>>& size_and_name_ref)
{
	std::vector<std::pair<std::string, std::string>> chatsound_paths;

	std::string path("C:/Users/bonzo/Desktop/csgo-chatsounds/chatsounds/sounds/chatsounds");
	//std::string path("Z:/garrysmod-chatsounds/sound/chatsounds/autoadd");
	//std::string path("C:/Users/bonzo/Desktop/garrysmod-chatsounds/sound/chatsounds/autoadd");
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
			chatsound_paths.push_back(std::make_pair(temp, p.path().string()));
			size_and_name_ref.push_back(std::make_pair(temp.size(), temp));
		}

		// if child is a folder THEN scan folder and add all
		else if (p.is_directory() != true && current_parent_dir != "chatsounds" && current_parent_dir != currently_scanned_dir)
		{
			omit = p.path().parent_path().string();
			std::string temp = current_parent_dir;
			for (auto& subp : fs::recursive_directory_iterator(current_parent_dir_path))
			{
				chatsound_paths.push_back(std::make_pair(current_parent_dir, subp.path().string()));
			}
			size_and_name_ref.push_back(std::make_pair(temp.size(), temp));
		}
	}
	std::sort(size_and_name_ref.begin(), size_and_name_ref.end(), std::greater<std::pair<size_t, std::string>>());
	return chatsound_paths;
}

/*Takes: 
	- input message, 
	- vector of chatsound names and its lengths,
	- ChatSounds object to run everything after processing input,
	- vector with chatsound names and paths to pass it to method of ChatSounds object*/
void parseChatsounds (std::string content_copy,
	// TODO: zrob loadera normalnego z zewnetrznego pliku a nie
	std::vector<std::pair<size_t, std::string>>& size_and_name_ref,
	ChatSounds& chatsounds_ref,
	std::vector<std::pair<std::string, std::string>>& chatsound_paths_ref)
{
	std::string toAnalyzeLater = content_copy;

	std::vector<std::string> toPlay;
	std::vector<std::pair<std::string, short int>> toPlay_sorted;

	// catch existing chatsounds from input message
	while (content_copy != "")
	{
		for (auto& chtsnd : size_and_name_ref)
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
				content_copy = Utils::trim(content_copy);
			}
		}
		break;
	}


	short int id;
	// BRAND NEW BUG: jakims cudem mozna powtorzyc chatsounda tylko 4 razy w jednym inpucie,
	// wiecej sie nie da
	// sorts catched chatsounds to play them in the order made by input message
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
					Tags tags;
					id = tags.search_id(toAnalyzeLater, x);

					toAnalyzeLater = std::regex_replace(toAnalyzeLater, rgx, "");

					toAnalyzeLater = Utils::trim(toAnalyzeLater);
					toPlay_sorted.emplace_back(std::make_pair(x, id));

					//std::cout << "Current chatsound: " << x << " | id: " << id << std::endl;
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

	for (auto& chatsound : toPlay_sorted)
	{
		chatsounds_ref.addChatSound(chatsound.first, chatsound_paths_ref);
	}

	chatsounds_ref.playChatSounds(toPlay_sorted); //zamien multimap na cos innego, multimap jest nieposortowany
}

int main ()
{
	std::pair<HANDLE, uintptr_t> hProcess_and_ChatAddr = getChatAddr();

	ChatSounds chatsounds;

	std::vector<std::pair<size_t, std::string>> size_and_name;

	// json jakis walnij zeby za kazdym razem nie musial skanowac plikow
	std::vector<std::pair<std::string, std::string>> chatsound_paths = loadAndSort(size_and_name);


	std::regex message_pattern("<[^>]*>");
	std::regex message_content_pattern("^[^:]+:\s*");

	char message[512];
	std::string prevMessage;

	while (true)
	{
		ReadProcessMemory(hProcess_and_ChatAddr.first,			// handle
						  (BYTE*)hProcess_and_ChatAddr.second,  // chat memory address
						  &message, 
						  sizeof(message), 
						  nullptr);

		std::string msg = (std::string)message;
		
		if (msg != prevMessage)
		{
			std::string normal_chat_message = std::regex_replace(msg, message_pattern, "");

			std::string content = std::regex_replace(normal_chat_message, message_content_pattern, "");
			content.erase(0, 1);

			std::cout << "Input: " << content << std::endl;

			if (content == "sh")
			{
				prevMessage = msg;

				Mix_HaltChannel(-1);
				//Mix_Resume(-1);
				Sleep(100);
				continue;
			}

			parseChatsounds(content, size_and_name, chatsounds, chatsound_paths);
		}

		prevMessage = msg;
		Sleep(100);
	}

	return 0;
}