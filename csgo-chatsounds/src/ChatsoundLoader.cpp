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
	std::vector<ChatsoundType> chatsounds;


	if (CheckPathFileExists() == false)
	{
		//fs::path path("C:\\Users\\bonzo\\Desktop\\csgo-chatsounds\\chatsounds\\sounds\\chatsounds");
		fs::path path("Z:\\chatsounds-polskisandbox-metastruct\\sound\\chatsounds\\autoadd");
		//fs::path path("C:\\Users\\bonzo\\Desktop\\garrysmod-chatsounds\\sound\\chatsounds\\autoadd");

		//fs::path path("Z:\\garrysmod-chatsounds\\sound\\chatsounds\\autoadd");
		std::string ext(".ogg");

		std::string temp;

		std::string currently_scanned_dir;
		std::string current_parent_dir;
		std::string current_parent_dir_path;

		std::string root_dir = path.stem().string();

		std::string omit;
		int count = 0;

		for (auto& p : fs::recursive_directory_iterator(path))
		{
			count++;
			current_parent_dir_path = p.path().parent_path().string();
			current_parent_dir = p.path().parent_path().stem().string();

			if (omit == p.path().parent_path().string())
				continue;

			if (current_parent_dir == root_dir)
			{
				currently_scanned_dir = p.path().stem().string();
			}

			// if current path isn't folder and current_parent_dir isn't root_dir THEN add
			if (p.is_directory() != true && current_parent_dir != root_dir && current_parent_dir == currently_scanned_dir && p.path().extension() == ext)
			{
				temp = p.path().stem().string();
				std::string temp_path = p.path().string();

				CorrectlyParseJsonObject(temp, temp_path, json_chatsounds);
			}

			// if current path is a folder THEN scan it and add all contents
			else if (p.is_directory() != true && current_parent_dir != root_dir && current_parent_dir != currently_scanned_dir)
			{
				omit = p.path().parent_path().string();
				std::string temp = current_parent_dir;

				json empty_array_explicit = json::array();

				for (auto& subp : fs::recursive_directory_iterator(current_parent_dir_path))
				{
					// nlohmann json lib silently avoids duplicate keys so i need to count them somehow in other way

					std::string temp_subpath = subp.path().string();

					CorrectlyParseJsonObject(temp, temp_subpath, json_chatsounds);
				}
			}
		}

		std::ofstream writer;
		writer.open(json_file_name);
		writer << json_chatsounds.dump(4, ' ', true, json::error_handler_t::replace);
		writer.close();
	}
	else
	{
		std::ifstream reader(json_file_name);
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

bool ChatsoundLoader::CheckPathFileExists()
{
	return fs::exists(json_file_name);
}


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
