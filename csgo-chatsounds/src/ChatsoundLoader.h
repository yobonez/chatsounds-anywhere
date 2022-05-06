#pragma once
#include <vector>
#include <string>

#include "nlohmann/single_include/json.hpp"
#include "ChatsoundType.h"

class ChatsoundLoader
{
public:
	std::vector<ChatsoundType> Scan();
private:
	bool CheckExists();
	//void CorrectlyParseJsonObject(std::string& temp_stem_path, std::string& temp_path, json& json_object);  WIP
};

