#include <iostream>
#include <cmath>
#include "Effects.h"
#include "BerVibrato/BerVibrato.h"

void vibrato_callback(int channel, void* stream, int length, void* user_data)
{
	//https://github.com/Bershov/Vibrato-effect/
	BerVibrato vibrato;
	vibrato.initialize(44100);
	vibrato.setDepth(3);
	vibrato.setFrequency(5);

	auto* data = reinterpret_cast<int16_t*>(stream);
	length /= 2;
	for (unsigned short i = 0; i < length - 1; i++)
	{
		data[i] = vibrato.processOneSample(data[i]);
	}
}