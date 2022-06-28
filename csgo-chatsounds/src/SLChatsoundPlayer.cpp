#include "SLChatsoundPlayer.h"

namespace fs = std::filesystem;

SLChatsoundPlayer::SLChatsoundPlayer()
{
	sl.init();
}

// add chatsound to cache
void SLChatsoundPlayer::add_chatsounds(ChatsoundType chatsound)
{
	if (chatsound_cache.size() == 0)
	{
		chatsound_cache.emplace_back(chatsound.key, chatsound.value);
		return;
	}

	for (auto& keyval : chatsound_cache)
	{
		if (chatsound.key == keyval.key)
			return;
		else
			chatsound_cache.emplace_back(chatsound.key, chatsound.value);
	}
}

void SLChatsoundPlayer::play(std::vector<std::pair<ChatsoundType, std::array<int,4>>> t_b_p)
{
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 20);

	std::string command;

	int tbpsize = t_b_p.size();

	double delay = 0.0;
	int t_b_p_index = -1;

	SoLoud::Wav* wav_container = new SoLoud::Wav[tbpsize]; // i used an array of SoLoud::Wav's here bcuz std::vector for some reason doesn't work
														   // and a play() from each element doesn't play until it hits the last one that is the
														   // only one that plays

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

	if (wav_container_cleaner_active == false)
	{
		std::thread deleter(Utils::wavcontainer_deleter, wav_container, std::ref(this->sl), std::ref(this->wav_container_cleaner_active), std::ref(this->wav_handles));
		deleter.detach();
	}
	// it frees memory after ABSOLUTELY EVERYTHING STOPPED, i need to fix it sometime 
	// for now idk

	/*for (int i = 0; i < wav_container.size(); i++)
	{
		temp_handle = sl.play(wav_container[i]);
		wav_handles.push_back(temp_handle);
	}*/
}

void SLChatsoundPlayer::play_chatsounds(std::vector<std::pair<ChatsoundType, std::array<int, 4>>> chatsounds)
{
	std::vector<std::pair<ChatsoundType, std::array<int, 4>>> to_be_played;
	for (auto& chatsound : chatsounds)
	{
		std::string chatsound_name = chatsound.first.key;    //     ~~~~~~~| - future arguments,
														 //	    ^		   unused for now, i just need to imagine the picture of how it will work
		int chatsound_id_modifier = chatsound.second[0]; // {X, X, X, X}
													     //  ^ 

		int selected_id = chatsound_id_modifier - 1;

		// adding chatsound duplicates to a new container of type ChatsoundType (name, path)
		// to randomly choose one of them later.
		std::vector<std::pair<ChatsoundType, std::array<int,4>>> duplicates;


		for (auto& keyval : chatsound_cache)
		{
			if (keyval.key == chatsound_name)
				duplicates.emplace_back(chatsound.first, chatsound.second);
		}


		if (chatsound_id_modifier < 0)
		{
			int min = 0;
			int max = duplicates.size() - 1;

			//https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> uni(min, max);

			selected_id = uni(rng);

			to_be_played.emplace_back(duplicates[selected_id]); // random
		}
		else
			to_be_played.emplace_back(duplicates[selected_id]); // explicitly selected

		if (selected_id > duplicates.size()) return;

	}
	play(to_be_played);
}

void SLChatsoundPlayer::stopall_playing()
{
	sl.stopAll();
}

SLChatsoundPlayer::~SLChatsoundPlayer()
{
	sl.deinit();
}
// finally i can do some effects