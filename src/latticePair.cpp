#include "latticePair.hpp"

awgPair::awgPair(unsigned long baudRate, double clockFrequency) {
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

void latticePair::addPulse(double pulseFrequency, double pulseAmplitude, double pulseDuration, double pulsePhaseDifference, bool setStart) {
	_masterAWG.outputPulses.pushPulse(freqPulse(pulseFrequency, pulseAmplitude, pulseDuration, pulsePhaseDifference/2.0, setStart));
	_slaveAWG.outputPulses.pushPulse(freqPulse(pulseFrequency, pulseAmplitude, pulseDuration, -pulsePhaseDifference/2.0, setStart));
}

bool latticePair::processLine(const std::string &pulseSpecLine, bool firstLine) {
	double	pulseFrequency, pulseAmplitude, pulseDuration, pulsePhaseDiff;
	std::istringstream pulseLine(pulseSpecLine);
	char delim;

	// Check if WS or just a comment, if it skip the line
	pulseLine >> std::skipws >> delim;
	if ('#' == delim || pulseLine.eof()) return true;
	pulseLine.putback(delim); // Shouldn't have removed it
	
	// Read CSV line, return false if misformatted
	pulseLine >> pulseFrequency >> delim;
	if (',' != delim) return false;
	pulseLine	>> pulseAmplitude	>> delim;
	if (',' != delim) return false;
	pulseLine	>> pulseDuration	>> delim;
	if (',' != delim) return false;
	pulseLine	>> pulsePhaseDiff	>> delim;
	if ((!pulseLine.eof()) && ('#' != delim)) return false;	

	addPulse(pulseFrequency, pulseAmplitude, pulseDuration, pulsePhaseDiff, firstLine);
	return true;
}
