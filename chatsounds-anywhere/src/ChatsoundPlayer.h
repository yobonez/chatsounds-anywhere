#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <filesystem>
#include <array>
#include <thread>
#include <map>

#include "soloud.h"
#include "soloud_wav.h"
#include "utils.h"
#include "ChatsoundType.h"
#include "ChatsoundEffects.h"

class ChatsoundPlayer
{
public:
	ChatsoundPlayer();
	~ChatsoundPlayer();

	void add_chatsounds(std::vector<ChatsoundType>& toAdd_ref);

	void play_chatsounds(std::vector<std::pair<std::string, std::array<int, 4>>> toPlay);

	void gc();
	void gc(SoLoud::Wav* wav_container_ref);

	void stopall_playing();

	std::vector<ChatsoundType> chatsound_cache;

private:
	void play(std::vector<std::pair<ChatsoundType, std::array<int, 4>>> t_b_p);
	SoLoud::Soloud sl;
	ChatsoundEffects effects;

	//std::array<SoLoud::Wav, size> wav_container; 

	std::vector<SoLoud::handle> wav_handles;
	std::random_device rd;
};

