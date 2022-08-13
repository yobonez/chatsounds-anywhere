#pragma once

#ifdef CHATSOUNDS_DLL
#include <iostream>
class Chatsounds
{
public:
	__declspec(dllexport) void Parse(std::string& input);
};
#endif