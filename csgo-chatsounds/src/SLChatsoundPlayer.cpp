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

void SLChatsoundPlayer::play(std::vector<ChatsoundType> t_b_p)
{
	//std::array<SoLoud::Wav, size> wav_container;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 10);

	std::string command;

	int tbpsize = t_b_p.size();

	double delay = 0.0;
	int t_b_p_index = -1;
	SoLoud::handle temp_handle;
	SoLoud::Wav* wav_container = new SoLoud::Wav[tbpsize]; // i used an array of SoLoud::Wav's here bcuz std::vector for some reason doesn't work
														   // and a play() from each element doesn't play until it hits the last one that is the
														   // only one that plays

	for (auto& keyval : t_b_p)
	{
		int random_int = uni(rng);
		t_b_p_index++;

		if (t_b_p_index == 0)
		{
			wav_container[t_b_p_index].load(keyval.value.c_str());
			//std::cout << "one delay: " << wav_container[t_b_p_index].getLength() << std::endl;
			delay = delay + wav_container[t_b_p_index].getLength(); // delay (length of silence) for the next chatsound that will play technically at the same time,
																	// but in practice it will be still one after another, and with that
																	// i can do whatever effects for each of them in the container
			//std::cout << delay << std::endl;
		}
		else
		{
			SoLoud::Wav check_length;

			command = std::format("sox -n -r 44100 -c 2 \"tmpchatsound-silence-{}.ogg\" trim 0.0 {}", random_int, delay);
			system(command.c_str());

			command = std::format("sox \"tmpchatsound-silence-{}.ogg\"", random_int) + " \"" + keyval.value + "\" " + std::format("\"tmpchatsound-{}-{}.ogg\"", keyval.key, random_int);
			system(command.c_str());

			wav_container[t_b_p_index].load((std::format("tmpchatsound-{}-{}.ogg", keyval.key, random_int)).c_str());
			//std::cout << "one delay: " << wav_container[t_b_p_index].getLength() << std::endl;

			check_length.load(keyval.value.c_str());
			delay = delay + check_length.getLength();
			//std::cout << delay << std::endl;
		}

		sl.play(wav_container[t_b_p_index]);
	}

	std::thread deleter(Utils::wavcontainer_deleter, wav_container, std::ref(this->sl));
	deleter.detach();
	// it frees memory after ABSOLUTELY EVERYTHING STOPPED, i need to fix it sometime 
	// if its possible to check for each sound in soloud, for now idk

	/*for (int i = 0; i < wav_container.size(); i++)
	{
		temp_handle = sl.play(wav_container[i]);
		wav_handles.push_back(temp_handle);
	}*/
}

void SLChatsoundPlayer::play_chatsounds(std::vector<std::pair<std::string, std::array<int, 4>>> chatsounds)
{
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

			to_be_played.emplace_back(duplicates[selected]); // random
		}
		else
			to_be_played.emplace_back(duplicates[selected]); // explicitly selected

		if (selected > duplicates.size()) return;

	}
	play(to_be_played);
}

SLChatsoundPlayer::~SLChatsoundPlayer()
{
	sl.deinit();
}
