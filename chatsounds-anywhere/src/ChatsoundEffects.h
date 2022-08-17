#include <iostream>
#include <array>

#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_echofilter.h"
#include "soloud_freeverbfilter.h"
#include "soloud_robotizefilter.h"
#include "soloud_fftfilter.h"
#include "soloud_flangerfilter.h"


class ChatsoundEffects 
{
public:
	double apply_effects(SoLoud::Wav* wav, std::array<int,2> params);
private:
	SoLoud::EchoFilter echof;
	SoLoud::FreeverbFilter reverbf;
	SoLoud::RobotizeFilter robotizef;
	SoLoud::FFTFilter fftf;
	SoLoud::FlangerFilter flangerf;
};
