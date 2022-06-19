#pragma once
#include <iostream>
#include <string>

struct ChatsoundType
{
	std::string key, value;
	ChatsoundType(std::string k, std::string val);
};

bool sort_by_size(const ChatsoundType& lhs, const ChatsoundType& rhs);

