#include "latticePair.hpp"

#define HALF_PI 1.57079632679489661923132

awgPair::awgPair(unsigned long baudRate, double clockFrequency) : _usedFirstPulse(false) {
	_masterAWG.clockFrequency(clockFrequency);
	_masterAWG.baudRate(baudRate);
	_masterAWG.waveName(std::string("MASTER.WFM"));

	_slaveAWG.clockFrequency(clockFrequency);
	_slaveAWG.baudRate(baudRate);
	_slaveAWG.waveName(std::string("SLAVE.WFM"));
}

void awgPair::baudRate(unsigned long newBaudRate) {
	_masterAWG.baudRate(newBaudRate);
	_slaveAWG.baudRate(newBaudRate);
}

void awgPair::clockFrequency(double newClockFrequency) {
	_masterAWG.clockFrequency(newClockFrequency);
	_slaveAWG.clockFrequency(newClockFrequency);
}

void awgPair::sampleRate(double newSampleRate) {
	_masterAWG.sampleRate(newSampleRate);
	_slaveAWG.sampleRate(newSampleRate);
}

void awgPair::startPulseLength(double newStartPulseLength) {
	_masterAWG.startPulseLength(newStartPulseLength);
	_slaveAWG.startPulseLength(newStartPulseLength);
}

// Phase by HALF_PI because shift each in opposite directions by half the requested difference (for the half) time PI because input is [-1,1]
void latticePair::addPulse(double pulseFrequency, double pulseAmplitude, double pulseDuration, double pulsePhaseDifference, bool setStart) {
	_masterAWG.outputPulses.pushPulse(freqPulse(pulseFrequency, pulseAmplitude, pulseDuration, pulsePhaseDifference * HALF_PI, setStart));
	_slaveAWG.outputPulses.pushPulse(freqPulse(pulseFrequency, pulseAmplitude, pulseDuration, -pulsePhaseDifference * HALF_PI, setStart));
}

bool latticePair::processLine(const std::string &pulseSpecLine, bool markFirstLine) {
	double	pulseFrequency, pulseAmplitude, pulseDuration, pulsePhaseDiff;
	std::istringstream pulseLine(pulseSpecLine);
	char delim;

	// Check if WS or just a comment, if it skip the line
	pulseLine >> std::skipws >> delim;
	if ('#' == delim || pulseLine.eof()) return true;
	pulseLine.putback(delim); // Shouldn't have removed it

	// Check if we're spec-ing a SLAVE setting
	if ( 'S' == pulseLine.peek() ) {
		std::string checkSetting;
		pulseLine >> checkSetting >> pulseDuration;
		if ( pulseLine.fail() ) return false;
		pulseLine >> delim;
		if ((!pulseLine.eof()) && ('#' != delim)) return false;

		if ( std::string("SLAVE_SHIFT") == checkSetting ) {
			slaveDelay(pulseDuration);
			return true;
		} else if ( std::string("SLAVE_AMPLITUDE") == checkSetting ) {
			slaveAmplitude(pulseDuration);
			return true;
		} else {
			return false;
		}
	}

	// Check if we're spec-ing a MASTER setting
	if ( 'M' == pulseLine.peek() ) {
		std::string checkSetting;
		pulseLine >> checkSetting >> pulseDuration;
		if ( pulseLine.fail() ) return false;
		pulseLine >> delim;
		if ((!pulseLine.eof()) && ('#' != delim)) return false;

		if ( std::string("MASTER_AMPLITUDE") == checkSetting ) {
			masterAmplitude(pulseDuration);
			return true;
		} else {
			return false;
		}
	}

	// Check if we're spec-ing a MASTER setting
	if ( 'C' == pulseLine.peek() ) {
		std::string checkSetting;
		pulseLine >> checkSetting >> pulseDuration;
		if ( pulseLine.fail() ) return false;
		pulseLine >> delim;
		if ((!pulseLine.eof()) && ('#' != delim)) return false;

		if ( std::string("CLOCK_FREQUENCY") == checkSetting ) {
			clockFrequency(pulseDuration);
			return true;
		} else {
			return false;
		}
	}

	// Read CSV line, return false if misformatted
	pulseLine >> pulseFrequency >> delim;
	if ( !pulseLine.good() || ',' != delim) return false;
	pulseLine	>> pulseAmplitude	>> delim;
	if ( !pulseLine.good() || ',' != delim) return false;
	pulseLine	>> pulseDuration	>> delim;
	if ( !pulseLine.good() || ',' != delim) return false;
	pulseLine	>> pulsePhaseDiff;
	if ( pulseLine.fail() ) return false;
	pulseLine	>> delim;
	if ((!pulseLine.eof()) && ('#' != delim)) return false;

	addPulse(pulseFrequency, pulseAmplitude, pulseDuration, pulsePhaseDiff, markFirstLine && (!_usedFirstPulse));
	_usedFirstPulse = true;
	return true;
}
