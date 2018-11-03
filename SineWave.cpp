#include <math.h>

#include "SineWave.h"


SineWave::SineWave(unsigned int khzFreq, unsigned int amplitude) :
	time(0), khzFreq(khzFreq), amplitude(amplitude) {
}


float SineWave::getNextValue() {
	double period = 1. / (khzFreq * 1000);
	return sin(period * (time += period / (khzFreq * 1000))) * amplitude;
}