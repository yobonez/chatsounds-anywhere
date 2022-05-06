#include <iostream>
#include <vector>
#include <regex>
#include <filesystem>
#include <windows.h>

#include "ChatSoundPlayer.h"
#include "proc.h"
#include "ChatsoundLoader.h"
#include "utils.h"
#include "Tags.h"
#include "ChatsoundType.h"

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
	- input message, 
	- vector of chatsounds,
	- ChatSoundPlayer object to run everything after processing input,
*/
void parseChatsounds (std::string content_copy, std::vector<ChatsoundType>& chatsounds_ref, ChatSoundPlayer& chatsound_player_ref)
{
	std::vector<std::pair<std::string, short int>> toPlay;

	Tags tags;

	short int id;
	while (content_copy != "")
	{
		for (unsigned short int i = 0; i < toPlay.size() + 1; i++)
		{
			for (auto& chtsnd : chatsounds_ref)
			{
				std::string chatsound = chtsnd.key;
				std::regex rgx("\\b^(" + chatsound + ")\\b");
				std::smatch match;

				if (content_copy == "")
					break;

				if (std::regex_search(content_copy, match, rgx) && match.str(0) != "")
				{
					id = tags.search_id(content_copy, chatsound);
					if (id < 0) content_copy = std::regex_replace(content_copy, rgx, "");


					content_copy = Utils::trim(content_copy);
					toPlay.emplace_back(std::make_pair(chatsound, id));

					tags.remove_tags(content_copy);

					std::cout << "Current chatsound: " << chatsound << " | id: " << id << std::endl;
				}
			}
		}
		// top 10 stupid fixes
		// or is it?
		for (char character : content_copy)
		{
			if (content_copy[0] == ' ')
			{
				content_copy.erase(0, 1);
				break;
			}
			else
			{
				content_copy.erase(0, 1);
			}
		}
	}

	for (auto& chatsound : toPlay)
	{
		chatsound_player_ref.addChatSound(chatsound.first, chatsounds_ref);
	}

	chatsound_player_ref.playChatSound(toPlay);
}

int main ()
{
	std::pair<HANDLE, uintptr_t> hProcess_and_ChatAddr = getChatAddr();

	ChatSoundPlayer chatsound_player;
	ChatsoundLoader loader;
	std::vector<ChatsoundType> chatsounds = loader.Scan();


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
			parseChatsounds(content, chatsounds, chatsound_player);
		}

		prevMessage = msg;
		Sleep(100);
	}

	return 0;
}