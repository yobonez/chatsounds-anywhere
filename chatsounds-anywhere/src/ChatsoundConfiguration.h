#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

#pragma once
class ChatsoundConfiguration
{
private:
	std::vector<std::string> valid_names = { "root_dir_path", "chatsound_paths_file_name"};
public:
	struct ConfigurationEntry
	{
		std::string name;
		std::string value;
	};
	std::vector<ConfigurationEntry> entries;

	void LoadConfiguration();
	ConfigurationEntry GetEntry(std::string name);
};

