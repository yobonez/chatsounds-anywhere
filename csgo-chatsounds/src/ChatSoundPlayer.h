#pragma once

#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <map>
#include "ChatsoundType.h"

class ChatSoundPlayer
{
public:
	ChatSoundPlayer();
	~ChatSoundPlayer();

	void addChatSound(std::string name, std::vector<ChatsoundType>& chatsounds_ref);
	void playChatSound(std::vector<std::pair<std::string, short int>> chatsounds) const;

private:
	std::multimap<std::string, Mix_Chunk*> mChatSoundBank; // name, MixChunk*
};


