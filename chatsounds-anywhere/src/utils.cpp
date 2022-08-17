#include "utils.h"
#include <filesystem>

namespace fs = std::filesystem;

// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
std::string Utils::trim(const std::string& s)
{
	if (s != "") {
		auto start = s.begin();
		while (start != s.end() && std::isspace(*start)) {
			start++;
		}

		auto end = s.end();
		do {
			end--;
		} while (std::distance(start, end) > 0 && std::isspace(*end));

		return std::string(start, end + 1);
	}

	else {
		return "";
	}
}

void Utils::garbage_collector(SoLoud::Wav* wav_cntr, SoLoud::Soloud& sl_ref, std::vector<SoLoud::handle>& hndls)
{
	while (sl_ref.getActiveVoiceCount() > 0)
	{
		Sleep(300);
	}

	hndls.clear();

	delete[] wav_cntr;

	for (auto& file : fs::recursive_directory_iterator("./"))
	{
		std::string name = file.path().stem().string();
		size_t found = name.find("tmpchatsound");
		if (found != std::string::npos)
		{ 
			fs::remove(file);
			// there's a case this code crashes entire app, check it out later
		}
	}
	return;
}
