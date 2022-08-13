#pragma once
#include <iostream>
#include <string>
#include <array>

struct ChatsoundType
{
	std::string key, value;

	ChatsoundType(std::string k, std::string val);

	bool operator==(const ChatsoundType& rhs) const;
};

bool sort_by_size(const ChatsoundType& lhs, const ChatsoundType& rhs);

