#pragma once
#include <iostream>
#include <vector>
#include "soloud.h"
#include "soloud_wav.h"

namespace Utils {
	std::string trim(const std::string& s);
	void wavcontainer_deleter(SoLoud::Wav* wav_cntr, SoLoud::Soloud& sl, bool& cleaner_active, std::vector<SoLoud::handle>& hndls);
}
