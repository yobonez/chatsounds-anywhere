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

class SLChatsoundPlayer
{
public:
	SLChatsoundPlayer();
	~SLChatsoundPlayer();

	void add_chatsounds(std::string name, std::vector<ChatsoundType>& chatsounds_ref);

	void play_chatsounds(std::vector<std::pair<std::string, std::array<int, 4>>> chatsounds);

	void stopall_playing();

private:
	void play(std::vector<ChatsoundType> t_b_p);
	SoLoud::Soloud sl;

	//std::array<SoLoud::Wav, size> wav_container; 

	std::vector<SoLoud::handle> wav_handles;
	std::vector<ChatsoundType> chatsound_cache;

	bool wav_container_cleaner_active = false;
	std::random_device rd;
};

