#include "ChatsoundType.h"

ChatsoundType::ChatsoundType(std::string k, std::string val)
	:  key(k), value(val)
{};

bool sort_by_size(const ChatsoundType& lhs, const ChatsoundType& rhs)
{
	return lhs.key.length() > rhs.key.length();
}
