#include "Modifiers.h"
#include "utils.h"

/* 
   parameters_and_arguments[0] // id
   parameters_and_arguments[1] // 0 - no echo effect, 
								  1 - echo effect with default args, 
								  2 - echo effect with custom args (not implemented)
   parameters_and_arguments[2] // first echo arg
   parameters_and_arguments[3] // second echo arg
   parameters_and_arguments[4] ...
*/
std::array<int, 4> Modifiers::search(std::string& input, std::string curr_chatsound)
{
	std::array<int, 4> parameters_and_arguments = { -1, 0, 0, 0 };
	std::string all_modifiers;

	std::regex rgx("^(" + curr_chatsound + ")(#[0-9]+|:echo)+"); // curr_chatsound chyba nie jest potrzebny, w main.cpp ju¿ jest okreœlone, jaki chatsound zostanie u¿yty
	std::smatch match;

	std::regex rgx_id("([0-9]+)$"); // it doesn't generate any bugs (probably), because the if statement below already checks if the id has "#"
									// it's just a note for me, cuz i was wondering, why it WORKS
	std::regex rgx_echo("(:echo)$");

	if (std::regex_search(input, match, rgx))
	{
		std::regex rgx_ch("^(" + curr_chatsound + ")");
		all_modifiers = match.str(0);
		all_modifiers = std::regex_replace(all_modifiers, rgx_ch, "");
		input = std::regex_replace(input, rgx, "");
		input = Utils::trim(input);
	}

	while (all_modifiers != "")
	{
		std::regex clr_rgx_id("(#[0-9]+)$");
		std::regex clr_rgx_echo = rgx_echo; 

		parameters_and_arguments[0] = find_id(all_modifiers, match, rgx_id, clr_rgx_id);	 // id
		parameters_and_arguments[1] = find_echo(all_modifiers, match, rgx_echo, clr_rgx_echo); // 0 - no echo effect, 1 - echo effect with default args, 2 - echo effect with custom args
	}
	//parameters_and_arguments[2] =;
	//parameters_and_arguments[3] =;

	return parameters_and_arguments;
}

int Modifiers::find_id(std::string& all_modifiers_ref , std::smatch& match, std::regex& rgx, std::regex& clr_rgx)
{
	if (std::regex_search(all_modifiers_ref, match, rgx))
	{
		short int id = std::stoi(match.str(0));
		clear_modifiers(all_modifiers_ref, clr_rgx);
		return id;
	}
	return -1;
}

int Modifiers::find_echo(std::string& all_modifiers_ref, std::smatch& match, std::regex& rgx, std::regex& clr_rgx)
{
	if (std::regex_search(all_modifiers_ref, match, rgx))
	{
		clear_modifiers(all_modifiers_ref, clr_rgx);
		return 1;
	}
	return 0;
}

void Modifiers::clear_modifiers(std::string& all_modifiers_ref, std::regex& clr_rgx)
{
	all_modifiers_ref = std::regex_replace(all_modifiers_ref, clr_rgx, "");
	all_modifiers_ref = Utils::trim(all_modifiers_ref);
}

