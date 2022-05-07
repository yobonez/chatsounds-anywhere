#include "ChatsoundType.h"

ChatsoundType::ChatsoundType(int len, std::string k, std::string val)
	: length(len), key(k), value(val)
{};

bool sort_by_size(const ChatsoundType& lhs, const ChatsoundType& rhs)
{
	return lhs.length > rhs.length;
}
