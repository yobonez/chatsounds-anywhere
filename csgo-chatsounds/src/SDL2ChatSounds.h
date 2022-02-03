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
	void playChatSound(std::string name) const;

private:
	//bigbrain
	//std::map<std::string, std::string> mChatSoundPathList; //name, path
	std::map<std::string, Mix_Chunk*> mChatSoundBank; // name, chunk
};


