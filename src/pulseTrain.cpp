#include <iostream>
#include <string>

#include "pulseTrain.hpp"

std::ostream& operator<<(std::ostream & os, const freqPulse& pulse) {
	os << "Pulse " << pulse.myFrequency << "MHz for " << pulse.myDuration << "ns with amplitude of " << pulse.myAmplitude;
	os << " and phase " << pulse.myPhase << "*pi";
	os << (pulse.myMarkStart ? ", with " : ", without ") << "initial marker and ";
	os << (pulse.myMarkDuration ? "with " : "without ") << "duration marker.";
	return os;
}

std::string freqPulse::getMarkerChars(const unsigned int numPoints, unsigned int startMarkerPoints = 1){
	std::string markerChars = "";
	markerChars.reserve(numPoints+1);

	if ( startMarkerPoints > numPoints ) startMarkerPoints = numPoints;

	if ( ! myMarkStart ) {
		unsigned char durationOnly = (myMarkDuration ? 0x01 : 0x00);
		for (unsigned int i = 0; i < numPoints; i++) markerChars += durationOnly;
	} else {
		unsigned char startPart	= (myMarkDuration ? 0x03 : 0x02);
		unsigned char endPart	= (myMarkDuration ? 0x01 : 0x00);
		unsigned int i;

		for (i = 0; i < startMarkerPoints; i++) markerChars += startPart;
		for (i = startMarkerPoints; i < numPoints; i++) markerChars += endPart;
	}

	return markerChars;
}
