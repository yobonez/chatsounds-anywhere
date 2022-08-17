#include "ChatsoundConfiguration.h"

void ChatsoundConfiguration::LoadConfiguration()
{
	std::ifstream confFile("chatsounds-config.txt");
	if (confFile.is_open())
	{
		std::string line;
		while (std::getline(confFile, line))
		{
			line.erase(std::remove_if(line.begin(), line.end(), std::isspace), line.end());

			if (line[0] == '#' || line.empty())
				continue;

			size_t delimiter = line.find("=");
			std::string name = line.substr(0, delimiter);
			std::string value = line.substr(delimiter + 1);
			for (auto& check_name : valid_names)
			{
				if (name == check_name)
				{
					ConfigurationEntry cfgEntry = { name, value };
					entries.push_back(cfgEntry);
				}
				else
					continue;
			}
		}
	}
	else
	{
		std::ofstream confFile;
		confFile.open("./chatsounds-config.txt");
		confFile << "# This path will be used when generating list of audio files and its paths. Use \"/\" slashes."
					"\nroot_dir_path = X:/my-example/path"
					"\n\n# This is name of the list"
					"\nchatsound_paths_file_name = chatsound-paths.json";
		confFile.close();

		std::cout << "Config file was created. Edit it and run program again.\n";
		system("pause");
		exit(0);
	}
}

ChatsoundConfiguration::ConfigurationEntry ChatsoundConfiguration::GetEntry(std::string name)
{
	for (auto& entry : entries)
	{
		if (entry.name == name)
			return entry;
	}
	
	return { "error-name", "error-value" };
}
