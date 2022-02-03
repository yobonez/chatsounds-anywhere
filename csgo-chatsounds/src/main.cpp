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

void parseChatsounds (std::string readyContent_copy, std::vector<std::pair<size_t, std::string>>& nameAndPath_ref, SDL2ChatSounds& sdl2_chatsounds_ref)
{
	std::vector<std::string> toPlay;

	while (readyContent_copy != "")
	{
		for (auto& chtsnd : nameAndPath_ref)
		{
			std::string chatsound = chtsnd.second;
			std::regex rgx("\\b(" + chatsound + ")\\b");
			std::smatch match;

			if (std::regex_search(readyContent_copy, match, rgx) == true)
			{
 				std::cout << "Match: " << match.str(0) << std::endl;
				 
				if (match.str(0) != "" && std::find(toPlay.begin(), toPlay.end(), match.str(0)) == toPlay.end()) // preventing duplication
				{
					toPlay.push_back(match.str(0));
					std::string newContent = std::regex_replace(readyContent_copy, rgx, "", std::regex_constants::format_first_only);
					readyContent_copy = newContent;
					readyContent_copy = trim(readyContent_copy);
					//readyContent_copy.erase(0, 1);
					std::cout << "dupa" << std::endl;
				}
			}
		}
		break;
	}

	for (auto& chatsound_query : toPlay)
	{
		sdl2_chatsounds_ref.addChatSound(chatsound_query);
		sdl2_chatsounds_ref.playChatSound(chatsound_query);
	}
}

int main ()
{
	SDL2ChatSounds sdl2_chatsounds;

	std::vector<std::pair<size_t, std::string>> nameAndSize;
	vectorSortDescending(nameAndSize);

	// Windows api stuff
	DWORD procId = GetProcId(L"csgo.exe");
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"shaderapidx9.dll");
	HANDLE hProcess = 0;

	hProcess = OpenProcess(PROCESS_VM_READ, NULL, procId);

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x0008983C;
	std::cout << "DPB: " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	std::vector<unsigned int> chatOffsets = { 0x10, 0xB4, 0x204, 0x2A8, 0x114, 0x0 };
	uintptr_t chatAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, chatOffsets);

	std::cout << "CA: " << "0x" << std::hex << chatAddr << std::endl;
	

	// Actual code that i am finally making
	std::regex normalChatMessagePattern("<[^>]*>");
	std::regex readyMessageContentPattern("^[^:]+:\s*");

	char message[512];
	std::string prevMessage;

	while (true)
	{
		ReadProcessMemory(hProcess, (BYTE*)chatAddr, &message, sizeof(message), nullptr);
		std::string msg = (std::string)message;												// IT IS KURWA '\0' TERMINATED VISUAL STUDIO SKLEJ DUPE
		
		if (msg != prevMessage)
		{
			std::string normalChatMessage = std::regex_replace(msg, normalChatMessagePattern, "");
			std::cout << normalChatMessage << std::endl;

			std::string readyContent = std::regex_replace(normalChatMessage, readyMessageContentPattern, "");
			readyContent.erase(0, 1);
			std::cout << readyContent << std::endl;

			parseChatsounds(readyContent, nameAndSize, sdl2_chatsounds);
		}

		prevMessage = msg;
		Sleep(100);
	}

	return 0;
}