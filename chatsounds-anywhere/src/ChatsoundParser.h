#pragma once
#include <vector>
#include <string>
#include "ChatsoundType.h"
#include "ChatsoundModifiers.h"
#include "ChatsoundPlayer.h"


class ChatsoundParser
{
public:
	ChatsoundParser();
	void parse(std::string content_copy);
private:
	std::vector<ChatsoundType> chatsounds;
	std::vector<ChatsoundType> initialize();
	ChatsoundPlayer chatsound_player;
};

