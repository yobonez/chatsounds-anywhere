#include <windows.h>
#include <filesystem>
#include <random>
#include <SDL.h>
#include "ChatSoundPlayer.h"
#include "ChatsoundEffects.h"
#include "ChatsoundType.h"

//TODO: Implement SoLoud

ChatSoundPlayer::ChatSoundPlayer()
{
	//SoLoud::Soloud soloud;
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
		//std::cout << SDL_GetAudioDeviceName(0, 1) << std::endl;
		Mix_OpenAudioDevice(audio_rate, audio_format, audio_channels, audio_buffers, "CABLE Input (VB-Audio Virtual Cable)", SDL_AUDIO_ALLOW_SAMPLES_CHANGE);
		//Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
		Mix_Volume(-1, 8);
	}
}
ChatSoundPlayer::~ChatSoundPlayer()
{
	Mix_Quit();
	SDL_Quit();
}

void ChatSoundPlayer::addChatSound(std::string name, std::vector<ChatsoundType>& chatsounds_ref)
{
	if (mChatSoundBank.find(name) == mChatSoundBank.end())
	{
		std::string path;
		
		for (auto& keyval : chatsounds_ref)
		{
			if (keyval.key == name)
			{
				path = keyval.value;
				mChatSoundBank.insert(std::make_pair(name, Mix_LoadWAV(path.c_str())));
			}
		}
	}
}

void ChatSoundPlayer::playChatSound(std::vector<std::pair<std::string, short int>> chatsounds) const
{
	unsigned short int amount_playing = 0;

	for (int i = 0; i < 8; i++)
	{
		if (Mix_Playing(i) == 0)
		{
			amount_playing = i;
			break;
		}
	}

	for (auto& chatsound : chatsounds)
	{
		std::string chatsound_name = chatsound.first;
		short int chatsound_id_tag = chatsound.second;

		int random_integer = chatsound_id_tag - 1;

		// adding chatsound duplicates to a new container with name, and MixChunk*
		// to randomly choose one of them later.
		std::vector<std::pair<std::string, Mix_Chunk*>> duplicates;

		for (auto& keyval : mChatSoundBank)
		{
			if (keyval.first == chatsound_name)
			{
				duplicates.push_back(std::make_pair(chatsound_name, keyval.second));
			}
		}

		if (chatsound_id_tag < 0)
		{
			//https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> uni(0, duplicates.size() - 1);

			random_integer = uni(rng);

			Mix_Chunk* tmp = duplicates[random_integer].second;
		}
		
		if (random_integer > duplicates.size()) return;

		Mix_Chunk* tmp = duplicates[random_integer].second;

		while (Mix_Playing(amount_playing) != 0)
		{
			SDL_Delay(300);
		}
		//Mix_RegisterEffect(amount_playing, trigonometric_sound_callback, nullptr, nullptr);
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

