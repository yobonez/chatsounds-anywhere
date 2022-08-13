#include "ChatsoundLoader.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include "single_include/json.hpp"

#include "ChatsoundType.h"

// BIG TODO: Standardize .ogg files to stop SoX from complaining and stopping sound
// or match file parameters on the fly (?)

using json = nlohmann::json;
namespace fs = std::filesystem;

std::vector<ChatsoundType> ChatsoundLoader::Scan()
{
	json json_chatsounds;
	std::vector<ChatsoundType> chatsounds;


	if (!CheckPathFileExists())
	{
		fs::path path("Z:\\chatsounds-polskisandbox-metastruct\\sound\\chatsounds\\autoadd");
		std::string ext(".ogg");

		std::string temp;

		std::string currently_scanned_dir;
		std::string current_parent_dir;
		std::string current_parent_dir_path;

		std::string root_dir = path.stem().string();

		std::string omit;

		std::cout << "No chatsounds list found.\nPreparing chatsounds list from selected root directory...\n";
		for (auto& p : fs::recursive_directory_iterator(path))
		{
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
		std::cout << "Writing to " + json_file_name + "...\n";
		writer << json_chatsounds.dump(4, ' ', true, json::error_handler_t::ignore); // ignore all chatsounds containing language specific characters, nlohmann-json can't handle it
																					 // maybe i will make a fix to this someday
		std::cout << "Done.\n";
		writer.close();
	}
	else
	{
		std::ifstream reader(json_file_name);
		std::cout << "Reading chatsounds list " + json_file_name + "...\n";
		reader >> json_chatsounds;
		reader.close();
		std::cout << "Done.\n";
	}

	for (auto it = json_chatsounds.begin(); it != json_chatsounds.end(); ++it)
	{
		if (it->is_array())
		{
			for (auto& subit : it->items())
			{
				chatsounds.emplace_back(it.key(), subit.value());
			}
		}
		else
			chatsounds.emplace_back(it.key(), it.value());
	}

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
