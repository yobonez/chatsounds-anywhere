#include "ChatsoundEffects.h"
#include "ChatsoundModifiers.h"

double ChatsoundEffects::apply_effects(SoLoud::Wav* wav, std::array<int, 2> params)
{
	double return_decay = 0.0;
	if (params[1] == 0)
	{
		return_decay = 0.0;
	}

	if ((params[1] & ChatsoundModifiers::Effect::ECHO) != 0) // echo filter
	{
		/*float delay, decay;
		delay = params_args[2];
		decay = params_args[3];

		if (delay != 0 && decay != 0)
		{
			echof.setParams(delay, decay);
			return_decay = decay;
		}
		*/
		//else
		//{
		echof.setParams(0.3, 0.7); // default args
		return_decay = return_decay + 0.7;
		//}

		wav->setFilter(0, &echof);
	}

	if ((params[1] & ChatsoundModifiers::Effect::REVERB) != 0)
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
		
		return_decay = return_decay + 0;
	}

	if ((params[1] & ChatsoundModifiers::Effect::ROBOTIZE) != 0)
	{
		robotizef.setParams(80, 3);
		wav->setFilter(2, &robotizef);

		return_decay = return_decay + 0;
	}

	if ((params[1] & ChatsoundModifiers::Effect::FFT) != 0)
	{
		wav->setFilter(3, &fftf);

		return_decay = return_decay + 0;
	}

	if ((params[1] & ChatsoundModifiers::Effect::FLANGER) != 0)
	{
		flangerf.setParams(0.023, 4);
		wav->setFilter(4, &flangerf);

		return_decay = return_decay + 0;
	}
	return return_decay;
}
 