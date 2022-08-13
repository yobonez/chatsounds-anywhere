#include "ChatsoundType.h"

ChatsoundType::ChatsoundType(std::string k, std::string val)
	: key(k), value(val)
{}

bool ChatsoundType::operator==(const ChatsoundType & rhs) const
{
	if (key == rhs.key && value == rhs.value) return true;
	else
		return false;
}

bool sort_by_size(const ChatsoundType& lhs, const ChatsoundType& rhs)
{
	return lhs.key.length() == rhs.key.length() ? lhs.key < rhs.key : lhs.key.length() > rhs.key.length();
}
