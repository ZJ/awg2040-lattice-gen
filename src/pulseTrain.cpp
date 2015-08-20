#include <iostream>

#include "pulseTrain.hpp"

std::ostream& operator<<(std::ostream & os, const freqPulse& pulse) {
	os << "Pulse " << pulse.myFrequency << "MHz for " << pulse.myDuration << "ns with amplitude of " << pulse.myAmplitude;
	os << (pulse.myMarkStart ? ", with " : ", without ") << "initial marker and ";
	os << (pulse.myMarkDuration ? "with " : "without ") << "duration marker.";
	return os;
}
