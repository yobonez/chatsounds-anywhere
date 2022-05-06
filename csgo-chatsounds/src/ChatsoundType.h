#pragma once
#include <iostream>
#include <string>

struct ChatsoundType
{
	ChatsoundType(int len, std::string k, std::string val);

	int length;
	std::string key, value;
};

bool sort_by_size(const ChatsoundType& lhs, const ChatsoundType& rhs);
