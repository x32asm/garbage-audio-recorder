#ifndef GUARD_SINE_WAVE_H
#define GUARD_SINE_WAVE_H
#pragma once

class SineWave {
public:
	SineWave(unsigned int, unsigned int);

	float getNextValue();
private:
	double time;
	unsigned int khzFreq;
	unsigned int amplitude;
};

#endif //GUARD_SINE_WAVE_H