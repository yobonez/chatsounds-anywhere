#include "SDL2ChatSounds.h"
#include <SDL.h>
#include <filesystem>

SDL2ChatSounds::SDL2ChatSounds()
{
	SDL_Init(SDL_INIT_AUDIO);

	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;
	
	int flags = MIX_INIT_OGG;

	if (Mix_Init(flags) != flags)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
		exit(-1);
	}
	else {
		//Mix_OpenAudioDevice(audio_rate, audio_format, audio_channels, audio_buffers, "CABLE Input (VB-Audio Virtual Cable)", SDL_AUDIO_ALLOW_ANY_CHANGE);
		Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
	}
}
SDL2ChatSounds::~SDL2ChatSounds()
{
	Mix_Quit();
	SDL_Quit();
}

void SDL2ChatSounds::addChatSound(std::string name)//, std::string path)
{
	if (mChatSoundBank.find(name) == mChatSoundBank.end())
	{
		std::string path;
		std::string search_path("C:/Users/bonzo/Desktop/csgo-chatsounds/chatsounds-polskisandbox/sounds/chatsounds");

		for (auto& p : std::filesystem::recursive_directory_iterator(search_path))
		{
			if (p.path().filename() == name + ".ogg")
				path = p.path().string();
		}

		Mix_Chunk* tmp = Mix_LoadWAV(path.c_str());

		if (tmp != nullptr)
		{
			mChatSoundBank.insert(std::make_pair(name, tmp));
			std::cout << (mChatSoundBank.size() - 1) << ". Sound loaded: " << name << " -> " << path << '\n';
		}
		else
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
		}
	}
}

void SDL2ChatSounds::playChatSound(std::string name) const
{
	for (const auto& keyval : mChatSoundBank)
	{
		if (keyval.first == name) {
			Mix_PlayChannel(-1, keyval.second, 0);
			std::cout << "Sound played: " << keyval.first << std::endl;
		}
	}
}

