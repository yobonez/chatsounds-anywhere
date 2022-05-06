#include "Tags.h"
#include "utils.h"

// '#(num)' Tag
int Tags::search_id(std::string& input, std::string curr_chatsound)
{
	std::string whole_chatsound;

	std::regex rgx("^(" + curr_chatsound + ")#[0-9]+");
	std::smatch match;

	std::regex rgx_id("[0-9]+");
	std::smatch match_id;

	if (std::regex_search(input, match, rgx))
	{
		whole_chatsound = match.str(0);
		input = std::regex_replace(input, rgx, "");
		input = Utils::trim(input);
	}
	if (std::regex_search(whole_chatsound, match_id, rgx_id))
	{
		short int id = std::stoi(match_id.str(0));
		return id;
	}
	
	return -1;
}

// probably a temporary solution
// "temporary solutions sometimes become permanent problems"
void Tags::remove_tags(std::string& input)
{
	std::regex rgx("^(#[0-9]+)");
	input = std::regex_replace(input, rgx, "");
	input = Utils::trim(input);
}