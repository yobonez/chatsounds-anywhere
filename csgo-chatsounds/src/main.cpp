#include <iostream>
#include <vector>
#include <regex>
#include <filesystem>
#include <windows.h>

#include "proc.h"
#include "utils.h"
#include "ChatsoundLoader.h"
#include "ChatsoundType.h"
#include "Modifiers.h"

#include "SLChatsoundPlayer.h"

namespace fs = std::filesystem;

/*Returns chat memory address and a handle to the process*/
std::pair<HANDLE, uintptr_t> getChatAddr() 
{
	std::pair<HANDLE, uintptr_t> hProcess_and_ChatAddr;

	DWORD procId = GetProcId(L"csgo.exe");
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"panorama.dll");
	HANDLE hProcess = 0;

	hProcess = OpenProcess(PROCESS_VM_READ, NULL, procId);

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x0023B09C;

	std::vector<unsigned int> chatOffsets = { 0x14C, 0x74, 0xBC, 0x24, 0x2A8, 0x124, 0x0 };
	uintptr_t chatAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, chatOffsets);

	hProcess_and_ChatAddr = std::make_pair(hProcess, chatAddr);

	return hProcess_and_ChatAddr;
}


void parseChatsounds(std::string content_copy, std::vector<ChatsoundType>& chatsounds_ref, SLChatsoundPlayer& chtsndplayer_ref)
{
	std::vector<ChatsoundType> toAdd; // get rid of ChatsoundType, it starts to complicate things

	std::vector<std::pair<std::string, std::array<int, 4>>> toPlay;

	Modifiers modifiers;
	std::array<int, 4> params_and_args = { -1, 0, 0, 0 };

	while (content_copy != "")
	{
		for (int i = 0; i < toPlay.size() + 1; i++)
		{
			for (auto& chtsnd : chatsounds_ref)
			{
				std::string chatsound_name = chtsnd.key;
				std::regex rgx("\\b^(" + chatsound_name + ")\\b");
				std::smatch match;

				if (content_copy == "")
					break;

				if (std::regex_search(content_copy, match, rgx) && match.str(0) != "")
				{
					params_and_args = modifiers.search(content_copy, chatsound_name);
					int id = params_and_args[0];
					
					if (toAdd.size() == 0)
						toAdd.emplace_back(chtsnd.key, chtsnd.value);
					else
					{
						if (std::find(toAdd.begin(), toAdd.end(), chtsnd) == toAdd.end())
							toAdd.emplace_back(chtsnd.key, chtsnd.value);
					} // prevent adding EXACTLY the same chatsounds to cache (i. e. the same key and value)

					std::string current_chtsnd_name = chatsound_name;

					ChatsoundType next_chtsnd = *(&chtsnd + 1);
					std::string next_chtsnd_name = next_chtsnd.key;

					if (current_chtsnd_name == next_chtsnd_name)
					{
						continue;
					} // continue adding other duplicates of chatsounds with the same name but with other paths to cache
					
					/*if (id < 0)*/ content_copy = std::regex_replace(content_copy, rgx, "");

					content_copy = Utils::trim(content_copy);

					toPlay.emplace_back(chtsnd.key, params_and_args);
					break;
				}
			}
		}

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
	} // while(content_copy != "")

	chtsndplayer_ref.add_chatsounds(toAdd);

	chtsndplayer_ref.play_chatsounds(toPlay);
}

int main ()
{
	SLChatsoundPlayer chatsound_player;
	std::pair<HANDLE, uintptr_t> hProcess_and_ChatAddr = getChatAddr();

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
			std::smatch match;
			std::string normal_chat_message = std::regex_replace(msg, message_pattern, "");
			std::string content;

			if (std::regex_search(normal_chat_message, match, message_content_pattern))
			{
				content = std::regex_replace(normal_chat_message, message_content_pattern, "");
				content.erase(0, 1);

				std::cout << "Input: " << content << std::endl;

				if (content == "sh")
				{
					prevMessage = msg;
					chatsound_player.stopall_playing();
				}
				parseChatsounds(content, chatsounds, chatsound_player);
			}
			else
			{
				Sleep(100);
				continue;
			};
		}

		prevMessage = msg;
		Sleep(100);
	}

	return 0;
}