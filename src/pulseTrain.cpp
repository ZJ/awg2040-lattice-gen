#include <iostream>
#include <string>
#include <cmath>

#include "pulseTrain.hpp"

std::ostream& operator<<(std::ostream & os, const freqPulse& pulse) {
	os << "Pulse " << pulse.myFrequency << "MHz for " << pulse.myDuration << "ns with amplitude of " << pulse.myAmplitude;
	os << " and phase " << pulse.myPhase << "*pi";
	os << (pulse.myMarkStart ? ", with " : ", without ") << "initial marker and ";
	os << (pulse.myMarkDuration ? "with " : "without ") << "duration marker.";
	return os;
}

std::string freqPulse::getMarkerChars(const unsigned int numPoints, unsigned int startMarkerPoints){
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

unsigned int freqPulse::getNumPoints(const double samplePeriod, const bool nearestHalfCycle) {
	if (! nearestHalfCycle) return ((unsigned int) myDuration/samplePeriod);

	double halfCycles = round(myDuration * myFrequency * 2.0 * 0.001 /*ns*MHz*/) / 2.0;
	return (unsigned int) floor(halfCycles * (1000.0/myFrequency)/samplePeriod);
}


std::string freqPulse::getWaveChars(const double samplePeriod, const unsigned int numPoints) {
	return "";
}
