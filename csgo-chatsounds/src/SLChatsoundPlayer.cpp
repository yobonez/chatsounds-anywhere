#include "SLChatsoundPlayer.h"

namespace fs = std::filesystem;

SLChatsoundPlayer::SLChatsoundPlayer()
{
	sl.init();
}

// add chatsound to cache
void SLChatsoundPlayer::add_chatsounds(std::string name, std::vector<ChatsoundType>& chatsounds_ref)
{
	for (auto& keyval : chatsound_cache)
	{
		if (name == keyval.key)
			return;
	}

	for (auto& keyval : chatsounds_ref)
	{
		if (name == keyval.key)
			chatsound_cache.emplace_back(keyval.key, keyval.value);
	}
}

void SLChatsoundPlayer::play(std::vector<ChatsoundType> t_b_p, enum EffectFlags& ef_ref)
{
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 10);

	int random_int = uni(rng);
	std::string random_int_str = std::to_string(random_int);

	std::string out_filename = "chatsound-output" + random_int_str + ".ogg";

	std::string command = "sox";
	if (ef_ref == EffectFlags::NO_EFFECTS)
	{
		for (auto& keyval : t_b_p)
			command = command + " " + "\"" + keyval.value + "\"";

		command = command + " " + out_filename;

		system(command.c_str());

		wav.load(out_filename.c_str());
		sl.play(wav);
	}
}

void SLChatsoundPlayer::play_chatsounds(std::vector<std::pair<std::string, std::array<int, 4>>> chatsounds, enum EffectFlags ef)
{
	// koncept dzia³a, musisz zestandaryzowaæ wszystkie pliki w ChatsoundLoader na 44,1kHz i tak¹
	// sam¹ iloœc kana³ów!!!!!!
	std::vector<ChatsoundType> to_be_played;
	for (auto& chatsound : chatsounds)
	{
		std::string chatsound_name = chatsound.first;    //     ~~~~~~~| - future arguments,
														 //	    ^		   unused for now, i just need to imagine the picture of how it will work
		int chatsound_id_modifier = chatsound.second[0]; // {X, X, X, X}
													     //  ^ id

		int selected = chatsound_id_modifier - 1;

		// adding chatsound duplicates to a new container of type ChatsoundType (name, path)
		// to randomly choose one of them later.
		std::vector<ChatsoundType> duplicates;


		for (auto& keyval : chatsound_cache)
		{
			if (keyval.key == chatsound_name)
				duplicates.emplace_back(chatsound_name, keyval.value);
		}


		if (chatsound_id_modifier < 0)
		{
			//https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> uni(0, duplicates.size() - 1);

			selected = uni(rng);

			to_be_played.emplace_back(duplicates[selected]);
		}
		else
			to_be_played.emplace_back(duplicates[selected]); // here actually not random

		if (selected > duplicates.size()) return;

	}
	play(to_be_played, ef);
}

SLChatsoundPlayer::~SLChatsoundPlayer()
{
	sl.deinit();
}
