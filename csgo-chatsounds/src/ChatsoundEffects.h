#include <iostream>
#include <array>

#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_echofilter.h"


class ChatsoundEffects 
{
public:
	void apply_effects(SoLoud::Wav* wav, std::array<int,4> params_args);
private:
	SoLoud::EchoFilter ef;
};
