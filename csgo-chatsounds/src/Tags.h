#pragma once

#include <regex>
#include <iostream>

class Tags
{
public:
	int search_id(std::string& input, std::string curr_chatsound);
	void remove_tags(std::string& input);
};

