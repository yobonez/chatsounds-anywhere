#pragma once
#include <iostream>
#include <string>

struct ChatsoundType
{
	int length;
	std::string key, value;
	ChatsoundType(int len, std::string k, std::string val);
};

bool sort_by_size(const ChatsoundType& lhs, const ChatsoundType& rhs);

