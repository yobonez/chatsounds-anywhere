#ifdef CHATSOUNDS_DLL

#include <iostream>
#include "ChatsoundParser.h"
#include "ChatsoundConfiguration.h"
#include "../include/Chatsounds.h"

ChatsoundParser parser;
void Chatsounds::Parse(std::string input)
{
	parser.parse(input);
}

void Chatsounds::PrintAvailableInputDevices(bool exit_after_print)
{
	ChatsoundConfiguration::PrintAudioDevices();
	if (exit_after_print) exit(0);
}

#endif

