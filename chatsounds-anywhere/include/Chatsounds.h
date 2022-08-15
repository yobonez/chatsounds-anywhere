#pragma once

#ifdef CHATSOUNDS_DLL
#include <iostream>
class Chatsounds
{
public:
	/**
		This function must be used in some sort of infinite loop.
		It will take single message string and take care of the rest for you.
		***
		You just need to find out for yourself how to get the chat message and parse it here every once a while in a loop.

		//TODO: repo with example code
	*/
	__declspec(dllexport) void Parse(std::string& input);
};
#endif