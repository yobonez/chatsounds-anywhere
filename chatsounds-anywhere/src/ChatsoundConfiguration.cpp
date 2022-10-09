#include "ChatsoundConfiguration.h"
#include "soloud.h"
#include "utils.h"

#include <WinNls32.h>

// all of this... to just print out devices with language-specific characters in windows. GLOBALIZATION MOMENT
void ChatsoundConfiguration::PrintAudioDevices()
{
	wchar_t szISOLang[5] = { 0 };
	wchar_t szISOCountry[5] = { 0 };

	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SISO639LANGNAME, szISOLang, sizeof(szISOLang) / sizeof(wchar_t));
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SISO3166CTRYNAME, szISOCountry, sizeof(szISOCountry) / sizeof(wchar_t));

	std::wstring wstr_lang_locale = (std::wstring)szISOLang + L"_" + (std::wstring)szISOCountry + L".UTF-8";
	std::string str_lang_locale = std::string(wstr_lang_locale.begin(), wstr_lang_locale.end());

	setlocale(LC_ALL, str_lang_locale.c_str());

	std::cout << "Available input devices: \n Default: \"default\"\n";
	SoLoud::Soloud::getAudioDevices();
	std::cout << "\n";
}

void ChatsoundConfiguration::LoadConfiguration()
{
	std::ifstream confFile("chatsounds-config.txt");
	if (confFile.is_open())
	{
		std::string line;
		while (std::getline(confFile, line))
		{
			//line.erase(std::remove_if(line.begin(), line.end(), std::isspace), line.end());

			if (line[0] == '#' || line.empty())
				continue;

			size_t delimiter = line.find("=");
			std::string name = line.substr(0, delimiter);
			std::string value = line.substr(delimiter + 1);

			name = Utils::trim(name);
			value = Utils::trim(value);

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
					"\nchatsound_paths_file_name = chatsound-paths.json"
					"\n\n# This is name of the input device"
					"\naudio_input = default";
		confFile.close();

		std::cout << "Config file was created. Copy name of your desired input device. Edit config and run program again.\n";
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
