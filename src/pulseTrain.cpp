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
	unsigned char waveBuffer[numPoints];
	const double TwoPi = 6.28318530717958647692; // NOT tau, that's just wrong
	const double phasePerSample = TwoPi * myFrequency * samplePeriod * 0.001 /* ns * MHz*/;

	for (unsigned int i = 0; i < numPoints; i++) {
		static double point;
		point = 127.0  + (127.0 * myAmplitude * sin(phasePerSample * ((double) i) + myPhase));
		waveBuffer[i] = (unsigned char) round(point);
	}

	return std::string((char *) waveBuffer, numPoints);
}

std::string pulseTrain::getMarkerChars(const double samplePeriod, const unsigned int numPoints, const bool nearestHalfCycle, const unsigned int startMarkerPoints) {
	std::string firstString  = "";
	std::string secondString = "";
	
	firstString.reserve(numPoints+1);
	secondString.reserve(numPoints+1);

	int shiftAmount = ((int) round(myCyclicShift/samplePeriod));
	int firstLength = ((shiftAmount >= 0) ? (numPoints - shiftAmount) : (-shiftAmount))%numPoints; 
	
	std::deque<freqPulse>::iterator thisPulse = myPulses.begin();
	for (thisPulse = myPulses.begin(); thisPulse != myPulses.end(); thisPulse++) {
		firstString += thisPulse->getMarkerChars(thisPulse->getNumPoints(samplePeriod, nearestHalfCycle), startMarkerPoints);
		if ( firstString.length() > firstLength ) {
			secondString = firstString.substr(firstLength);
			firstString = firstString.erase(firstLength);
			thisPulse++;
			break;
		}
	}
	
	for (thisPulse; thisPulse != myPulses.end(); thisPulse++) {
		secondString += thisPulse->getMarkerChars(thisPulse->getNumPoints(samplePeriod, nearestHalfCycle), startMarkerPoints);
	}

	secondString += firstString;

	return secondString;
}

std::string pulseTrain::getWaveChars(const double samplePeriod, const unsigned int numPoints, const bool nearestHalfCycle) {
	// STUB
	return "";
}

unsigned int pulseTrain::getNumPoints(const double samplePeriod, const bool nearestHalfCycle) {
	unsigned int totalPoints = 0;

	for (std::deque<freqPulse>::iterator thisPulse = myPulses.begin(); thisPulse != myPulses.end(); thisPulse++) {
		totalPoints += thisPulse->getNumPoints(samplePeriod, nearestHalfCycle);
	}

	return totalPoints;
}
