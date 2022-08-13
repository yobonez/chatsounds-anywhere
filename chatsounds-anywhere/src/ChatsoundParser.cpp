#include "ChatsoundParser.h"
#include "ChatsoundLoader.h"
#include "ChatsoundPlayer.h"

ChatsoundParser::ChatsoundParser()
{
	chatsounds = initialize();
}

// TODO: initialize with selected audio device
std::vector<ChatsoundType> ChatsoundParser::initialize() 
{
	ChatsoundLoader loader;
	return loader.Scan();
}
void ChatsoundParser::parse(std::string content_copy)
{
	if (content_copy == "sh")
	{
		chatsound_player.stopall_playing();
		return;
	}

	std::vector<ChatsoundType> toAdd; // get rid of ChatsoundType, it starts to complicate things

	std::vector<std::pair<std::string, std::array<int, 4>>> toPlay;

	ChatsoundModifiers modifiers;
	std::array<int, 4> params_and_args = { -1, 0, 0, 0 };

	while (content_copy != "")
	{
		for (int i = 0; i < toPlay.size() + 1; i++)
		{
			for (auto& chtsnd : chatsounds)
			{
				std::string chatsound_name = chtsnd.key;
				std::regex rgx("\\b^(" + chatsound_name + ")\\b");
				std::smatch match;

				if (content_copy == "")
					break;

				if (std::regex_search(content_copy, match, rgx) && match.str(0) != "")
				{

					if (toAdd.size() == 0)
						toAdd.emplace_back(chtsnd.key, chtsnd.value);
					else
					{
						if (std::find(toAdd.begin(), toAdd.end(), chtsnd) == toAdd.end())
							toAdd.emplace_back(chtsnd.key, chtsnd.value);
					} // prevent adding EXACTLY the same chatsounds to cache (i. e. the same key and value)

					std::string current_chtsnd_name = chatsound_name;

					ChatsoundType next_chtsnd = *(&chtsnd + 1);
					std::string_view next_chtsnd_name = next_chtsnd.key;

					if (current_chtsnd_name == next_chtsnd_name)
					{
						continue;
					} // continue adding other duplicates of chatsounds with the same name but with other paths to cache

					params_and_args = modifiers.search(content_copy, chatsound_name);
					int id = params_and_args[0];

					/*if (id < 0)*/ content_copy = std::regex_replace(content_copy, rgx, "");

					content_copy = Utils::trim(content_copy);

					toPlay.emplace_back(chtsnd.key, params_and_args);
					break;
				}
			}
		}

		for (char character : content_copy)
		{
			if (content_copy[0] == ' ')
			{
				content_copy.erase(0, 1);
				break;
			}
			else
			{
				content_copy.erase(0, 1);
			}
		}
	} // while(content_copy != "")

	chatsound_player.add_chatsounds(toAdd);

	chatsound_player.play_chatsounds(toPlay);
}