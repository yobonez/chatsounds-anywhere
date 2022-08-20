#pragma once

#include <iostream>
class Chatsounds
{
public:
	/**
		This function must be used in some sort of infinite loop.
		It will take single message string and take care of the rest for you.
		***
		You just need to find out for yourself how to get the chat message and parse it here every once a while in a loop.
	*/
	__declspec(dllexport) void Parse(std::string input);

	/**
		This function prints out all available input devices you would want to
		use with chatsounds.
	 
		bool exit_after_print:
		- true: exit after printing
	 */
	__declspec(dllexport) static void PrintAvailableInputDevices(bool exit_after_print);
};