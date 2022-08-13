#include "ChatsoundPlayer.h"

namespace fs = std::filesystem;

ChatsoundPlayer::ChatsoundPlayer()
{
	sl.init();
}

void ChatsoundPlayer::add_chatsounds(std::vector<ChatsoundType>& toAdd_ref)
{
	for (auto& keyval : toAdd_ref)
		chatsound_cache.emplace_back(keyval.key, keyval.value);
}

void ChatsoundPlayer::play(std::vector<std::pair<ChatsoundType, std::array<int, 4>>> t_b_p)
{
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 20);

	std::string command;

	int t_b_p_size = t_b_p.size();

	double delay = 0.0;
	int t_b_p_index = -1;

	SoLoud::Wav* wav_container = new SoLoud::Wav[t_b_p_size];

	for (auto& keyval : t_b_p)
	{
		int random_int = uni(rng);
		t_b_p_index++;

		if (t_b_p_index == 0)
		{
			wav_container[t_b_p_index].load(keyval.first.value.c_str());
			delay = delay + wav_container[t_b_p_index].getLength();
		}
		else
		{
			SoLoud::Wav check_length;

			command = std::format("sox -n -r 44100 -c 2 \"tmpchatsound-silence-{}.ogg\" trim 0.0 {}", random_int, delay);
			system(command.c_str());

			command = std::format("sox \"tmpchatsound-silence-{}.ogg\"", random_int) + " \"" + keyval.first.value + "\" " + std::format("\"tmpchatsound-{}-{}.ogg\"", keyval.first.key, random_int);
			system(command.c_str());

			wav_container[t_b_p_index].load((std::format("tmpchatsound-{}-{}.ogg", keyval.first.key, random_int)).c_str());

			check_length.load(keyval.first.value.c_str());
			delay = delay + check_length.getLength();
		}

		effects.apply_effects(&wav_container[t_b_p_index], keyval.second);

		SoLoud::handle temp_handle;
		temp_handle = sl.play(wav_container[t_b_p_index]);

		wav_handles.emplace_back(temp_handle);
	}

	std::thread deleter(Utils::garbage_collector, wav_container, std::ref(this->sl), /*std::ref(this->garbage_collector_active),*/ std::ref(this->wav_handles));
	deleter.detach();
}

void ChatsoundPlayer::play_chatsounds(std::vector<std::pair<std::string, std::array<int, 4>>> toPlay)
{
	std::vector<std::pair<ChatsoundType, std::array<int, 4>>> to_be_played;

	for (auto& chatsound : toPlay)
	{
		std::string chatsound_name = chatsound.first;
		std::array<int, 4> params_args = chatsound.second;
														 //     ~~~~~~~| - future arguments,
														 //	    ^		   unused for now
		int chatsound_id_modifier = params_args[0];      // {X, X, X, X}
													     //  ^ 

		int selected_id = chatsound_id_modifier - 1;

		// adding chatsound duplicates to a new container of type ChatsoundType
		// to randomly choose one of them later.
		std::vector<ChatsoundType> duplicates;


		for (auto& keyval : chatsound_cache)
		{
			if (keyval.key == chatsound_name)
				duplicates.emplace_back(keyval);
		}


		if (chatsound_id_modifier < 0)
		{
			int min = 0;
			int max = duplicates.size() - 1;

			//https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> uni(min, max);

			selected_id = uni(rng);

			to_be_played.emplace_back(duplicates[selected_id], params_args); // random
		}
		else
			to_be_played.emplace_back(duplicates[selected_id], params_args); // explicitly selected

		if (selected_id > duplicates.size()) return;

	}
	play(to_be_played);
}

void ChatsoundPlayer::stopall_playing()
{
	sl.stopAll();
}

ChatsoundPlayer::~ChatsoundPlayer()
{
	sl.deinit();
}