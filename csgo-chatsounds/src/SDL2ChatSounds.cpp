#include "SDL2ChatSounds.h"
#include <SDL.h>
#include <filesystem>
#include <windows.h>
#include <random>

#include <thread>

SDL2ChatSounds::SDL2ChatSounds()
{
	SDL_Init(SDL_INIT_AUDIO);

	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 8;
	int audio_buffers = 4096;
	
	int flags = MIX_INIT_OGG;

	if (Mix_Init(flags) != flags)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
		exit(-1);
	}
	else {
		//std::cout << SDL_GetAudioDeviceName(0, 1) << std::endl;
		Mix_OpenAudioDevice(audio_rate, audio_format, audio_channels, audio_buffers, "CABLE Input (VB-Audio Virtual Cable)", SDL_AUDIO_ALLOW_ANY_CHANGE);
		//Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
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
			{
				path = p.path().string();
				mChatSoundBank.insert(std::make_pair(name, Mix_LoadWAV(path.c_str())));
			}
		}
	}
}

void SDL2ChatSounds::playChatSounds(std::vector<std::string> chatsounds) const
{
	Mix_Volume(-1, 30);
	unsigned short int amount_playing;

	for (int i = 0; i < 8; i++)
	{
		if (Mix_Playing(i) == 0)
		{
			amount_playing = i;
			break;
		}
	}

	for (auto& name : chatsounds)
	{
		std::vector<std::pair<std::string, Mix_Chunk*>> duplicates;
		for (auto& keyval : mChatSoundBank)
		{
			if (keyval.first == name)
			{
				duplicates.push_back(std::make_pair(name, keyval.second));
			}
		}

		//https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(0, duplicates.size() - 1);

		auto random_integer = uni(rng);

		Mix_Chunk* tmp = duplicates[random_integer].second;

		while (Mix_Playing(amount_playing) != 0) // sam jestes uninitialized VS dzbanie
		{
			SDL_Delay(300);
		}
		Mix_PlayChannel(amount_playing, tmp, 0);
		std::cout << "Sound played: " << duplicates[random_integer].first << std::endl;
	}

	for (int i = 0; i < 8; i++)
	{
		if (Mix_Playing(i))
		{
			std::cout << "Channel " << i << ": currently playing" << std::endl;
		}
		else
		{
			std::cout << "Channel " << i << ": not playing" << std::endl;
		}
	}
}

