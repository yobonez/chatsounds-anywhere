#include <iostream>
#include <cmath>
#include "Effects.h"

//#include "libsndfile/include/sndfile.h"

//#include "RubberBandStretcher.h"

//#include "BerVibrato/BerVibrato.h"

//using namespace RubberBand;


void trigonometric_sound_callback(int channel, void* stream, int length, void* user_data)
{

	//for (int i = 0; i < length - 1; i++)
	//{
		/*
		if (i % 2 != 0)
		{
			de_interleaved_data[i] = 0;
		}
		else {
			de_interleaved_data[i] = data[i];
		}
		data[i] = de_interleaved_data[i];
		*/
		// jakis prymitywny panning mi tu wyszed³
		
	//}
	
	//RubberBandStretcher e(44100, (size_t)8, RubberBandStretcher::Option::OptionProcessRealTime | RubberBandStretcher::Option::OptionPitchHighSpeed);
	//e.reset();

}