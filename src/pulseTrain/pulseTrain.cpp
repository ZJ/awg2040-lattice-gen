#include <iostream>
#include <string>
#include <cmath>

#include "pulseTrain.hpp"

std::ostream& operator<<(std::ostream & os, const freqPulse& pulse) {
	os << "Pulse " << pulse._frequency << "MHz for " << pulse._duration << "ns with amplitude of " << pulse._amplitude;
	os << " and phase " << pulse._phase << "*pi";
	os << (pulse._markStart ? ", with " : ", without ") << "initial marker and ";
	os << (pulse._markDuration ? "with " : "without ") << "duration marker.";
	return os;
}

std::string freqPulse::markerChars(const unsigned int numPoints, unsigned int startMarkerPoints){
	std::string markerChars = "";
	markerChars.reserve(numPoints+1);

	if ( startMarkerPoints > numPoints ) startMarkerPoints = numPoints;

	if ( ! _markStart ) {
		unsigned char durationOnly = (_markDuration ? 0x01 : 0x00);
		for (unsigned int i = 0; i < numPoints; i++) markerChars += durationOnly;
	} else {
		unsigned char startPart	= (_markDuration ? 0x03 : 0x02);
		unsigned char endPart	= (_markDuration ? 0x01 : 0x00);
		unsigned int i;

		for (i = 0; i < startMarkerPoints; i++) markerChars += startPart;
		for (i = startMarkerPoints; i < numPoints; i++) markerChars += endPart;
	}

	return markerChars;
}

unsigned int freqPulse::numPoints(const double samplePeriod, const bool nearestHalfCycle) {
	if (! nearestHalfCycle) return ((unsigned int) _duration/samplePeriod);

	double halfCycles = round(_duration * _frequency * 2.0 * 0.001 /*ns*MHz*/) / 2.0;
	return (unsigned int) floor(halfCycles * (1000.0/_frequency)/samplePeriod);
}


std::string freqPulse::waveChars(const double samplePeriod, const unsigned int numPoints) {
	unsigned char waveBuffer[numPoints];
	const double TwoPi = 6.28318530717958647692; // NOT tau, that's just wrong
	const double phasePerSample = TwoPi * _frequency * samplePeriod * 0.001 /* ns * MHz*/;

	for (unsigned int i = 0; i < numPoints; i++) {
		static double point;
		point = 127.0  + (127.0 * _amplitude * sin(phasePerSample * ((double) i) + _phase));
		waveBuffer[i] = (unsigned char) round(point);
	}

	return std::string((char *) waveBuffer, numPoints);
}

std::string pulseTrain::markerChars(const double samplePeriod, const unsigned int numPoints, const bool nearestHalfCycle, const unsigned int startMarkerPoints) {
	std::string firstString  = "";
	std::string secondString = "";
	
	firstString.reserve(numPoints+1);
	secondString.reserve(numPoints+1);

	int shiftAmount = ((int) round(_cyclicShift/samplePeriod));
	unsigned int firstLength = ((shiftAmount >= 0) ? (numPoints - shiftAmount) : (-shiftAmount))%numPoints;
	
	std::deque<freqPulse>::iterator thisPulse = _pulses.begin();
	for (thisPulse = _pulses.begin(); thisPulse != _pulses.end(); thisPulse++) {
		firstString += thisPulse->markerChars(thisPulse->numPoints(samplePeriod, nearestHalfCycle), startMarkerPoints);
		if ( firstString.length() > firstLength ) {
			secondString = firstString.substr(firstLength);
			firstString = firstString.erase(firstLength);
			thisPulse++;
			break;
		}
	}
	
	for (; thisPulse != _pulses.end(); thisPulse++) {
		secondString += thisPulse->markerChars(thisPulse->numPoints(samplePeriod, nearestHalfCycle), startMarkerPoints);
	}

	secondString += firstString;

	return secondString;
}

std::string pulseTrain::waveChars(const double samplePeriod, const unsigned int numPoints, const bool nearestHalfCycle) {
	std::string firstString  = "";
	std::string secondString = "";
	
	firstString.reserve(numPoints+1);
	secondString.reserve(numPoints+1);

	int shiftAmount = ((int) round(_cyclicShift/samplePeriod));
	unsigned int firstLength = ((shiftAmount >= 0) ? (numPoints - shiftAmount) : (-shiftAmount))%numPoints;

	std::deque<freqPulse>::iterator thisPulse = _pulses.begin();
	for (thisPulse = _pulses.begin(); thisPulse != _pulses.end(); thisPulse++) {
		firstString += thisPulse->waveChars(samplePeriod, thisPulse->numPoints(samplePeriod, nearestHalfCycle));
		if ( firstString.length() > firstLength ) {
			secondString = firstString.substr(firstLength);
			firstString = firstString.erase(firstLength);
			thisPulse++;
			break;
		}
	}
	
	for (; thisPulse != _pulses.end(); thisPulse++) {
		secondString += thisPulse->waveChars(samplePeriod, thisPulse->numPoints(samplePeriod, nearestHalfCycle));
	}

	secondString += firstString;

	return secondString;
}

unsigned int pulseTrain::numPoints(const double samplePeriod, const bool nearestHalfCycle) {
	unsigned int totalPoints = 0;

	for (std::deque<freqPulse>::iterator thisPulse = _pulses.begin(); thisPulse != _pulses.end(); thisPulse++) {
		totalPoints += thisPulse->numPoints(samplePeriod, nearestHalfCycle);
	}

	return totalPoints;
}
