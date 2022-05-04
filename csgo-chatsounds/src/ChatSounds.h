#pragma once

#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <map>

class ChatSounds
{
public:
	ChatSounds();
	~ChatSounds();

	void addChatSound(std::string name, std::vector<std::pair<std::string, std::string>>& chatsound_paths);
	void playChatSounds(std::vector<std::pair<std::string, short int>> chatsounds) const;

private:
	std::multimap<std::string, Mix_Chunk*> mChatSoundBank; // name, MixChunk*
};


