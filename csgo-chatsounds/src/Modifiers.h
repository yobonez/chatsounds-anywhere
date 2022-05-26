#pragma once

#include <regex>
#include <iostream>

class Modifiers
{
public:
	int search_id(std::string& input, std::string curr_chatsound);
	void remove_modifiers(std::string& input);
};

