#include "ChatsoundLoader.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include "nlohmann/single_include/json.hpp"

#include "ChatsoundType.h"

using json = nlohmann::json;
namespace fs = std::filesystem;
std::vector<ChatsoundType> ChatsoundLoader::Scan()
{
	json json_chatsounds;
	std::vector <ChatsoundType> chatsounds;

	bool exists = CheckExists();

	if (exists == false)
	{
		//gowno znowu nie dziala napraw


		//std::string path("C:\\Users\\bonzo\\Desktop\\csgo-chatsounds\\chatsounds\\sounds\\chatsounds");
		std::string path("Z:\\garrysmod-chatsounds\\sound\\chatsounds\\autoadd");
		//std::string path("C:\\Users\\bonzo\\Desktop\\garrysmod-chatsounds\\sound\\chatsounds\\autoadd");
		//std::string path("C:/Users/bonzo/Desktop/garrysmod-chatsounds/sound/chatsounds/autoadd");
		std::string ext(".ogg");

		std::string temp;

		std::string currently_scanned_dir;
		std::string current_parent_dir;
		std::string current_parent_dir_path;

		std::string omit;
		int count = 0;

		for (auto& p : fs::recursive_directory_iterator(path))
		{
			count++;
			current_parent_dir_path = p.path().parent_path().string();
			current_parent_dir = p.path().parent_path().stem().string();

			if (omit == p.path().parent_path().string())
				continue;

			if (current_parent_dir == "autoadd")
			{
				currently_scanned_dir = p.path().stem().string();
			}

			// if current path isn't folder and current_parent_dir isn't "chatsounds" THEN add
			if (p.is_directory() != true && current_parent_dir != "autoadd" && current_parent_dir == currently_scanned_dir && p.path().extension() == ext)
			{
				temp = p.path().stem().string();

				// ZR�B JAKO FUNKCJE
				// ZR�B JAKO FUNKCJE
				if (json_chatsounds.contains(temp))
				{
					json empty_explict;
					json empty_array_explict;

					if (json_chatsounds[temp].is_array())
					{
						empty_explict = json_chatsounds[temp];
						empty_explict.emplace_back(p.path().string());
						json_chatsounds[temp] = empty_explict;
					}
					if (json_chatsounds[temp].is_string())
					{
						empty_array_explict.emplace_back(json_chatsounds[temp]);
						empty_array_explict.emplace_back(p.path().string());
						json_chatsounds[temp] = empty_array_explict;
					}
				}
				else json_chatsounds[temp] = p.path().string();
				// ZR�B JAKO FUNKCJE
				// ZR�B JAKO FUNKCJE
			}

			// if child is a folder THEN scan folder and add all
			else if (p.is_directory() != true && current_parent_dir != "autoadd" && current_parent_dir != currently_scanned_dir)
			{
				if (p.path().stem().string() == "dialup")
				{
					std::cout << "fart xd";
				}
				omit = p.path().parent_path().string();
				std::string temp = current_parent_dir;

				json empty_array_explicit = json::array();

				for (auto& subp : fs::recursive_directory_iterator(current_parent_dir_path))
				{
					// nlohmann json lib silently avoids duplicate keys so i need to count them somehow in other way

					// ZR�B JAKO FUNKCJE
					// ZR�B JAKO FUNKCJE
					if (json_chatsounds.contains(temp))
					{
						json empty_explict;
						json empty_array_explict;

						if (json_chatsounds[temp].is_array())
						{
							empty_explict = json_chatsounds[temp];
							empty_explict.emplace_back(subp.path().string());
							json_chatsounds[temp] = empty_explict;
						}
						if (json_chatsounds[temp].is_string())
						{
							empty_array_explict.emplace_back(json_chatsounds[temp]);
							empty_array_explict.emplace_back(subp.path().string());
							json_chatsounds[temp] = empty_array_explict;
						}
					}
					else
					{
						empty_array_explicit.emplace_back(subp.path().string());
						json_chatsounds[temp] = empty_array_explicit;
					}
					// ZR�B JAKO FUNKCJE
					// ZR�B JAKO FUNKCJE
				}
			}
		}

		std::ofstream writer;
		writer.open("./file.json");
		writer << json_chatsounds.dump(4, ' ', true, json::error_handler_t::replace);
		writer.close();
	}
	else
	{
		std::ifstream reader("./file.json");
		reader >> json_chatsounds;
		reader.close();
	}

	for (auto it = json_chatsounds.begin(); it != json_chatsounds.end(); ++it)
	{
		if (it->is_array())
		{
			for (auto& subit : it->items())
			{
				chatsounds.emplace_back(it.key().size(), it.key(), subit.value());
			}
		}
		else {
			chatsounds.emplace_back(it.key().size(), it.key(), it.value());
		}
	}

	// znajdz jakis sposob zeby zmienialo polskie znaki na znaki ascii 
	// ale bez skanowania kazdego oddzielnego chatsounda, bo to zabija
	// performance pewnie

	std::sort(chatsounds.begin(), chatsounds.end(), sort_by_size);
	return chatsounds;
}

bool ChatsoundLoader::CheckExists()
{
	return fs::exists("./file.json");
}

// WIP
/*
void ChatsoundLoader::CorrectlyParseJsonObject(std::string& temp_stem_path, std::string& temp_path, json& json_object)
{
	if (json_object.contains(temp_stem_path))
	{
		json empty_explict;
		json empty_array_explict;

		if (json_object[temp_stem_path].is_array())
		{
			empty_explict = json_object[temp_stem_path];
			empty_explict.emplace_back(temp_path);
			json_object[temp_stem_path] = empty_explict;
		}
		if (json_object[temp_stem_path].is_string())
		{
			empty_array_explict.emplace_back(json_object[temp_stem_path]);
			empty_array_explict.emplace_back(temp_path);
			json_object[temp_stem_path] = empty_array_explict;
		}
	}
	else json_object[temp_stem_path] = temp_path;
}
*/