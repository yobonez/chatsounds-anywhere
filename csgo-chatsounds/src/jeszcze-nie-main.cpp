/*#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
namespace fs = std::filesystem;

int main()
{
	nlohmann::json j;
	std::string path("C:/Users/bonzo/Desktop/csgo-chatsounds/chatsounds-polskisandbox/sounds/chatsounds");
	std::string ext(".ogg");

	fs::path json_path{ "C:/Users/bonzo/Desktop/csgo-chatsounds/chatsounds-polskisandbox/sounds" };
	json_path /= "test.json";
	fs::create_directories(json_path.parent_path());

	std::ofstream ofs(json_path);

	for (auto& p : fs::recursive_directory_iterator(path))
	{
		if (p.path().extension() == ext)
			j[p.path().stem().string()] = p.path().string();
	}

	ofs << j.dump(4);
	
	ofs.close();
	return 0;
}*/

/*
#include <iostream>
#include "SDL2ChatSounds.h"
#include <nlohmann/json.hpp>

int main()
{
	SDL2ChatSounds se;
	se.addChatSound("C:/Users/bonzo/Desktop/csgo-chatsounds/chatsounds-polskisandbox/sounds/chatsounds/jaszczur/daje do myslenia.ogg");
	se.addChatSound("C:/Users/bonzo/Desktop/csgo-chatsounds/chatsounds-polskisandbox/sounds/chatsounds/jaszczur/czyzby pan byl homofobem.ogg");

	/*
	short int choice = 0;

	while (choice != -1)
	{
		choice = 0;

		std::cout << "Which sound to play: /n => ";
		std::cin >> choice;

		se.playChatSound(choice);
		std::cin.clear();
	}
	

	return 0;
}*/