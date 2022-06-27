#include <iostream>
#include <vector>
#include <regex>
#include <filesystem>
#include <windows.h>

#include "proc.h"
#include "utils.h"
//#include "ChatSoundPlayer.h"
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
	//std::cout << "DPB: " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	std::vector<unsigned int> chatOffsets = { 0x14C, 0x74, 0xBC, 0x24, 0x2A8, 0x124, 0x0 };
	uintptr_t chatAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, chatOffsets);
	//std::cout << "CA: " << "0x" << std::hex << chatAddr << std::endl;

	hProcess_and_ChatAddr = std::make_pair(hProcess, chatAddr);

	return hProcess_and_ChatAddr;
}


void parseChatsounds(std::string content_copy, std::vector<ChatsoundType>& chatsounds_ref, SLChatsoundPlayer& slcht_ref)
{
	std::vector<std::pair<std::string, std::array<int, 4>>> toPlay;

	Modifiers modifiers;
	std::array<int, 4> params_and_args = { -1, 0, 0, 0 };
	bool has_effects = false;

	while (content_copy != "")
	{
		for (unsigned short int i = 0; i < toPlay.size() + 1; i++)
		{
			for (auto& chtsnd : chatsounds_ref)
			{
				std::string chatsound = chtsnd.key;
				std::string chatsound_path = chtsnd.value;
				std::regex rgx("\\b^(" + chatsound + ")\\b");
				std::smatch match;

				if (content_copy == "")
					break;

				if (std::regex_search(content_copy, match, rgx) && match.str(0) != "")
				{
					params_and_args = modifiers.search(content_copy, chatsound); // tried to get it by refence, but it got thanos snapped out of reality bcuz optimization on release mode
					int id = params_and_args[0];
					int hasEcho = params_and_args[1];

					if (id < 0) content_copy = std::regex_replace(content_copy, rgx, "");

					content_copy = Utils::trim(content_copy);
					toPlay.emplace_back(std::make_pair(chatsound, params_and_args));

					std::cout << "Current chatsound: " << chatsound << " | id: " << id << ", hasEcho: " << hasEcho << std::endl;
					
					// check for effects
					for (int i = 1; i < params_and_args.size(); i++)
					{
						if (params_and_args[i] > 0)
							has_effects = true;
						break;
					}
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
	}

	for (auto& chatsound : toPlay)
	{
		//chatsound_player_ref.addChatSound(chatsound.first, chatsounds_ref);
		slcht_ref.add_chatsounds(chatsound.first, chatsounds_ref);
	}

	slcht_ref.play_chatsounds(toPlay);
	//chatsound_player_ref.playChatSound(toPlay);
}

int main ()
{
	SLChatsoundPlayer chatsound_player;
	std::pair<HANDLE, uintptr_t> hProcess_and_ChatAddr = getChatAddr();

	//ChatSoundPlayer chatsound_player;
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