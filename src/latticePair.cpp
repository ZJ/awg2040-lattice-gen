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
