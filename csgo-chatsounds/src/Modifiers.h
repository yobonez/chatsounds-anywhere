#pragma once

#include <regex>
#include <iostream>
#include <array>

class Modifiers
{
public:
	std::array<int, 4> search(std::string& input, std::string curr_chatsound);
	void clear_modifiers(std::string& input, std::regex& clr_rgx);
private:
	int find_id(std::string& all_modifiers_ref, std::smatch& match, std::regex& rgx, std::regex& clr_rgx);
	int find_echo(std::string& all_modifiers_ref, std::smatch& match, std::regex& rgx, std::regex& clr_rgx);
};

