#pragma once
#include "soloud.h"
#include "soloud_wav.h"
#include <vector>
#include <string>
#include <iostream>
#include "ChatsoundType.h"
#include <random>
#include <filesystem>
#include <array>

enum EffectFlags {
	NO_EFFECTS = 1,
	HAS_EFFECTS = 2
};

class SLChatsoundPlayer
{
public:
	SLChatsoundPlayer();
	~SLChatsoundPlayer();

	void add_chatsounds(std::string name, std::vector<ChatsoundType>& chatsounds_ref);
	void play_chatsounds(std::vector<std::pair<std::string, std::array<int, 4>>> chatsounds, enum EffectFlags ef);

private:
	void play(std::vector<ChatsoundType> t_b_p, enum EffectFlags& ef_ref);

	SoLoud::Soloud sl;
	SoLoud::Wav wav;						// will be used when there are no VST effects to apply
	std::vector<SoLoud::Wav> wav_container; // will be used when there is a need to apply some VST effects on a single chatsound or more
	std::vector<ChatsoundType> chatsound_cache;

	std::random_device rd;
};

