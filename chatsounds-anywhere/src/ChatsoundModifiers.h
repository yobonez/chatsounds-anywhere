#pragma once

#include <regex>
#include <iostream>
#include <array>

class ChatsoundModifiers
{
public:
	enum Effect
	{
		ECHO = 1,
		REVERB = 2,
		ROBOTIZE = 4,
		FFT = 8,
		FLANGER = 16,
		PITCH = 32
	};
	std::array<int, 2> search(std::string input, std::string& curr_chatsound);
	void clear_modifiers(std::string& input, std::regex& clr_rgx);
private:
	int find_id(std::string& all_modifiers_ref, std::smatch& match, std::regex& rgx, std::regex& clr_rgx);
	/*int find_echo(std::string& all_modifiers_ref, std::smatch& match, std::regex& rgx, std::regex& clr_rgx);*/
	void find_effects(std::string& all_modifiers_ref, std::smatch& match, std::array<std::regex, 6>& effect_rgxs_ref, std::array<int, 2>& params);
};

