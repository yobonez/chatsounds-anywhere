#pragma once

#include <SDL_mixer.h>
#include <vector>
#include <map>
#include <iostream>

class SDL2ChatSounds
{
public:
	SDL2ChatSounds();
	~SDL2ChatSounds();

	void addChatSound(std::string name);//, std::string path);
	void playChatSounds(std::vector<std::string> chatsounds) const;

private:
	std::multimap<std::string, Mix_Chunk*> mChatSoundBank; // name, chunk
};


