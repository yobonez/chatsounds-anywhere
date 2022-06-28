#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <filesystem>
#include <array>
#include <thread>
#include "soloud.h"
#include "soloud_wav.h"
#include "utils.h"
#include "ChatsoundType.h"
#include "ChatsoundEffects.h"

class SLChatsoundPlayer
{
public:
	SLChatsoundPlayer();
	~SLChatsoundPlayer();

	void add_chatsounds(ChatsoundType name);

	void play_chatsounds(std::vector<std::pair<ChatsoundType, std::array<int, 4>>> chatsounds);

	void stopall_playing();

private:
	void play(std::vector<std::pair<ChatsoundType, std::array<int, 4>>> t_b_p);
	SoLoud::Soloud sl;
	ChatsoundEffects effects;

	//std::array<SoLoud::Wav, size> wav_container; 

	std::vector<SoLoud::handle> wav_handles;
	std::vector<ChatsoundType> chatsound_cache;

	bool wav_container_cleaner_active = false;
	std::random_device rd;
};

