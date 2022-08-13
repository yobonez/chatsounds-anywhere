#ifdef CHATSOUNDS_DLL

#include <iostream>
#include "ChatsoundParser.h"
#include "include/Chatsounds.h"

ChatsoundParser parser;

void Chatsounds::Parse(std::string& input)
{
	parser.parse(input);
}

#endif

