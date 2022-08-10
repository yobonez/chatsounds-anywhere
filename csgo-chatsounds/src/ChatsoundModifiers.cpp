#include "ChatsoundModifiers.h"
#include "utils.h"

/* 
   parameters_and_arguments[0] // id
   parameters_and_arguments[1] // 0 - no effect, 
								  1 - effect, 
								  2 - effect arg 1
								  3 - effect arg 2
   parameters_and_arguments[2] 
   parameters_and_arguments[3] 
   parameters_and_arguments[4] ...
*/
std::array<int, 4> ChatsoundModifiers::search(std::string input, std::string& curr_chatsound)
{
	std::array<int, 4> parameters_and_arguments = { -1, 0, 0, 0 };
	std::string all_modifiers;

	std::regex rgx("^(" + curr_chatsound + ")(#[0-9]+|:echo|:reverb|:robotize|:fft|:flanger)+");
	std::smatch match;

	// Patterns to get modifiers later one by one
	std::regex rgx_id("([0-9]+)$"); // it doesn't generate any bugs (probably), because the if statement below already checks if the id has "#"
									// it's just a note for me, cuz i was wondering, why it WORKS
	std::regex rgx_echo("(:echo)$");
	std::regex rgx_reverb("(:reverb)$");
	std::regex rgx_robotize(":(robotize)$");
	std::regex rgx_fft(":(fft)$");
	std::regex rgx_flanger(":(flanger)$");


	std::array<std::regex, 5> effect_rgxs;

	effect_rgxs[0] = rgx_echo;
	effect_rgxs[1] = rgx_reverb;
	effect_rgxs[2] = rgx_robotize;
	effect_rgxs[3] = rgx_fft;
	effect_rgxs[4] = rgx_flanger;

	if (std::regex_search(input, match, rgx))
	{
		std::regex rgx_ch("^(" + curr_chatsound + ")");
		all_modifiers = match.str(0);
		all_modifiers = std::regex_replace(all_modifiers, rgx_ch, "");
		input = std::regex_replace(input, rgx, "");
		input = Utils::trim(input);
	}

	// Regexes for clearing the modifiers - some will differ from these that are for detecting, some will not
	std::regex clr_rgx_id("(#[0-9]+)$");
	std::regex clr_rgx_echo = rgx_echo; 

	// this maybe could be done without any loops, let's try something different later
	// for now this stays with quick if statement fix 
	while (all_modifiers != "")
	{
		if (parameters_and_arguments[0] == -1)
			parameters_and_arguments[0] = find_id(all_modifiers, match, rgx_id, clr_rgx_id);	   // id

		find_effects(all_modifiers, match, effect_rgxs, parameters_and_arguments);
			//parameters_and_arguments[1] = find_echo(all_modifiers, match, rgx_echo, clr_rgx_echo); // 0 - no echo effect, 1 - echo effect with default args, 2 - echo effect with custom args
	}
	//parameters_and_arguments[2] =;
	//parameters_and_arguments[3] =;

	return parameters_and_arguments;
}

int ChatsoundModifiers::find_id(std::string& all_modifiers_ref , std::smatch& match, std::regex& rgx, std::regex& clr_rgx)
{
	if (std::regex_search(all_modifiers_ref, match, rgx))
	{
		short int id = std::stoi(match.str(0));
		clear_modifiers(all_modifiers_ref, clr_rgx);
		return id;
	}
	return -1;
}

/*int ChatsoundModifiers::find_echo(std::string& all_modifiers_ref, std::smatch& match, std::regex& rgx, std::regex& clr_rgx)
{
	if (std::regex_search(all_modifiers_ref, match, rgx))
	{
		clear_modifiers(all_modifiers_ref, clr_rgx);
		return 1;
	}
	// if something return 2;
	return 0;
}*/

void ChatsoundModifiers::find_effects(std::string& all_modifiers_ref, std::smatch& match, std::array<std::regex, 5>& effect_rgxs_ref, std::array<int, 4>& p_a)
{
	int eff_id = 0;
	for (auto& effect_rgx : effect_rgxs_ref)
	{
		if (std::regex_search(all_modifiers_ref, match, effect_rgx))
		{
			clear_modifiers(all_modifiers_ref, effect_rgx);
			switch (eff_id)
			{
			case 0:
				p_a[1] |= Effect::ECHO;
				break;
			case 1:
				p_a[1] |= Effect::REVERB;
				break;
			case 2:
				p_a[1] |= Effect::ROBOTIZE;
				break;
			case 3:
				p_a[1] |= Effect::FFT;
				break;
			case 4:
				p_a[1] |= Effect::FLANGER;
				break;
			}
		}
		eff_id++;
	}
}

void ChatsoundModifiers::clear_modifiers(std::string& all_modifiers_ref, std::regex& clr_rgx)
{
	all_modifiers_ref = std::regex_replace(all_modifiers_ref, clr_rgx, "");
	all_modifiers_ref = Utils::trim(all_modifiers_ref);
}

