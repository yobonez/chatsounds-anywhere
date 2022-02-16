#include <iostream>
#include <vector>
#include <regex>
#include <filesystem>
#include <windows.h>
#include <thread>

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

void vectorSortDescending (std::vector<std::pair<size_t, std::string>>& nameAndSize_ref)
{
	std::string path("C:/Users/bonzo/Desktop/csgo-chatsounds/chatsounds-polskisandbox/sounds/chatsounds");
	std::string ext(".ogg");

	std::string temp;

	for (auto& p : fs::recursive_directory_iterator(path))
	{
		if (p.path().extension() == ext)
			temp = p.path().stem().string();
		nameAndSize_ref.push_back(std::make_pair(temp.size(), temp));
	}

	std::sort(nameAndSize_ref.begin(), nameAndSize_ref.end(), std::greater<std::pair<size_t, std::string>>());
}

void parseChatsounds (std::string content_copy, std::vector<std::pair<size_t, std::string>>& nameAndSize_ref, SDL2ChatSounds& sdl2_chatsounds_ref)
{
	std::string toAnalyzeLater = content_copy;

	std::vector<std::string> toPlay;
	std::vector<std::string> toPlay_sorted;

	while (content_copy != "")
	{
		//unsigned short int hits; // duplicate chatsounds on list


		for (auto& chtsnd : nameAndSize_ref)
		{
			std::string chatsound = chtsnd.second;
			std::regex rgx("\\b(" + chatsound + ")\\b");
			std::smatch match;

			if (content_copy == "")
				break;

			if (std::regex_search(content_copy, match, rgx) == true)
			{
 				//std::cout << "Match: " << match.str(0) << std::endl;
				 
				if (match.str(0) != "") //&& std::find(toPlay.begin(), toPlay.end(), match.str(0)) == toPlay.end()) // preventing duplication
				{
					toPlay.push_back(match.str(0));
					content_copy = std::regex_replace(content_copy, rgx, "", std::regex_constants::format_first_only);
					content_copy = trim(content_copy);
				}
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

				if (std::regex_search(toAnalyzeLater, match, rgx))
				{
					if (match.str(0) != "")
					{
						toAnalyzeLater = std::regex_replace(toAnalyzeLater, rgx, "");
						toAnalyzeLater = trim(toAnalyzeLater);
						toPlay_sorted.push_back(x);
					}
				}
			}
		}

		// top 10 most stupid fixes
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

	for (auto& chatsound_query : toPlay_sorted)
	{
		sdl2_chatsounds_ref.addChatSound(chatsound_query);
	}

	sdl2_chatsounds_ref.playChatSounds(toPlay_sorted);
}

int main ()
{
	SDL2ChatSounds sdl2_chatsounds;

	std::vector<std::pair<size_t, std::string>> nameAndSize;
	vectorSortDescending(nameAndSize);

	// Windows api stuff
	DWORD procId = GetProcId(L"csgo.exe");
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"shaderapidx9.dll");
	//uintptr_t moduleBase2 = GetModuleBaseAddress(procId, L"client.dll");
	HANDLE hProcess = 0;

	hProcess = OpenProcess(PROCESS_VM_READ, NULL, procId);

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x0008983C;
	//std::cout << "DPB: " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	std::vector<unsigned int> chatOffsets = { 0x10, 0xB4, 0x204, 0x2A8, 0x114, 0x0 };
	uintptr_t chatAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, chatOffsets);

	//std::cout << "CA: " << "0x" << std::hex << chatAddr << std::endl;
	

	// Actual code that i am finally making
	std::regex normalChatMessagePattern("<[^>]*>");
	std::regex MessageContentPattern("^[^:]+:\s*");

	char message[512];
	std::string prevMessage;

	while (true)
	{
		ReadProcessMemory(hProcess, (BYTE*)chatAddr, &message, sizeof(message), nullptr);
		std::string msg = (std::string)message;												// IT IS KURWA '\0' TERMINATED VISUAL STUDIO SKLEJ DUPE
		
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
				Mix_Resume(-1);
				Sleep(750);
				continue;
			}

			parseChatsounds(content, nameAndSize, sdl2_chatsounds);
		}

		prevMessage = msg;
		Sleep(100);
	}

	return 0;
}