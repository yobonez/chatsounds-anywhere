#include "ChatsoundEffects.h"

void ChatsoundEffects::apply_effects(SoLoud::Wav* wav, std::array<int, 4> params_args)
{
	if (params_args[1] == 0)
	{
		return;
	}
	else if (params_args[1] == 1) // echo filter
	{
		float delay, decay;
		delay = params_args[2];
		decay = params_args[3];

		if (delay != 0 && decay != 0)
		{
			ef.setParams(delay, decay);
		}
		else
			ef.setParams(0.3, 0.7); // default args

		wav->setFilter(0, &ef);
		//             ^
		// jeden wav moze miec wiecej niz jeden efekt, potem se dorób jeszcze to jakoœ
	}
}

// make another applyeffects function but using threads and wav_handles from SLChatsoundPlayer.cpp
 