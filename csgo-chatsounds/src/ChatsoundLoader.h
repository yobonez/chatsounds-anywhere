#pragma once
#include <vector>
#include <string>

#include "nlohmann/single_include/json.hpp"
#include "ChatsoundType.h"

using json = nlohmann::json;

class ChatsoundLoader
{
public:
	std::vector<ChatsoundType> Scan();
private:
	std::string json_file_name = "./chatsound-paths-default.json";
	bool CheckPathFileExists();
	void CorrectlyParseJsonObject(std::string& temp_stem_path, std::string& temp_path, json& json_object);
};

