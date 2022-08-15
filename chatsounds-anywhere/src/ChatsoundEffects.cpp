#include "ChatsoundEffects.h"
#include "ChatsoundModifiers.h"

double ChatsoundEffects::apply_effects(SoLoud::Wav* wav, std::array<int, 4> params_args)
{
	if (params_args[1] == 0)
	{
		return 0.0;
	}

	else if (params_args[1] == ChatsoundModifiers::Effect::ECHO) // echo filter
	{
		double return_decay = 0.0;
		float delay, decay;
		delay = params_args[2];
		decay = params_args[3];

		if (delay != 0 && decay != 0)
		{
			echof.setParams(delay, decay);
			return_decay = decay;
		}
		else
		{
			echof.setParams(0.3, 0.7); // default args
			return_decay = 0.7;
		}

		wav->setFilter(0, &echof);
		return return_decay;
	}

	else if (params_args[1] == ChatsoundModifiers::Effect::REVERB)
	{
		/*float mode, room_size, damp, width;

		mode = params_args[2];
		room_size = params_args[3];
		damp = params_args[4];
		width = params_args[5];

		if (room_size != 0 && damp != 0 && width != 0)
		{
			reverbf.setParams(mode, room_size, damp, width);
		}
		*/

		reverbf.setParams(1, 0.5, 0.5, 1);
		wav->setFilter(1, &reverbf);
		
		return 0;
	}

	else if (params_args[1] == ChatsoundModifiers::Effect::ROBOTIZE)
	{
		robotizef.setParams(80, 3);
		wav->setFilter(2, &robotizef);

		return 0;
	}

	else if (params_args[1] == ChatsoundModifiers::Effect::FFT)
	{
		wav->setFilter(3, &fftf);

		return 0;
	}

	else if (params_args[1] == ChatsoundModifiers::Effect::FLANGER)
	{
		flangerf.setParams(0.023, 4);
		wav->setFilter(4, &flangerf);

		return 0;
	}
}

// make another applyeffects function but using threads and wav_handles from ChatsoundPlayer.cpp
 