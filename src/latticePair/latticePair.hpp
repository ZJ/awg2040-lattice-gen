#ifndef LATTICEPAIR_HPP_2015_11_11
#define LATTICEPAIR_HPP_2015_11_11
#include "../awg/awg.hpp"
#include "../pulseTrain/pulseTrain.hpp"

#include <string>
#include <sstream>
// no interface for defining pulses, that's for a sub-class.

class awgPair {
	public:
		// Constructor
		awgPair(unsigned long baudRate=19200, double clockFrequency=1024.0);
		// Accessors
		void	slaveDelay(double newSlaveDelay)	{_slaveAWG.outputPulses.shift(newSlaveDelay);};
		double	slaveDelay()	{return _slaveAWG.outputPulses.shift();};
		void	slaveAmplitude(double newSlaveAmplitude) {_slaveAWG.outputAmplitude(newSlaveAmplitude);};
		double	slaveAmplitude()	{return _slaveAWG.outputAmplitude();};
		void	masterAmplitude(double newMasterAmplitude) {_masterAWG.outputAmplitude(newMasterAmplitude);};
		double	masterAmplitude()	{return _masterAWG.outputAmplitude();};
		unsigned long  baudRate()	{return _masterAWG.baudRate();};
		double	clockFrequency()	{return _masterAWG.clockFrequency();};
		double	sampleRate()	{return _masterAWG.sampleRate();};
		double	startPulseLength()	{return _masterAWG.startPulseLength();};
		// Managing COM ports
		std::string	slaveComPort()	{return _slaveAWG.comPort();};
		std::string masterComPort()	{return _masterAWG.comPort();};
		void	slaveComPort(std::string &newComPort) {_slaveAWG.comPort(newComPort);};
		void	slaveComPort(double newComPort) {_slaveAWG.comPort(newComPort);};
		void	masterComPort(std::string &newComPort) {_masterAWG.comPort(newComPort);};
		void	masterComPort(double newComPort) {_masterAWG.comPort(newComPort);};
		// Accessors needing implementation
		void	baudRate(unsigned long newBaudRate);
		void	clockFrequency(double newClockFrequency);
		void	sampleRate(double newSampleRate);
		void	startPulseLength(double newStartPulseLength);
		// Programming strings
		std::string	slaveProgrammingString()	{return _slaveAWG.programmingString();};
		std::string	masterProgrammingString()	{return _masterAWG.programmingString();};
	protected:
		awg	_masterAWG;
		awg	_slaveAWG;
		bool _usedFirstPulse;
};

class latticePair : public awgPair {
	public:
		latticePair(unsigned long baudRate=19200, double clockFrequency=1024.0) : awgPair(baudRate, clockFrequency) {};
		void	addPulse(double pulseFrequency, double pulseAmplitude, double pulseDuration, double pulsePhaseDifference, bool setStart=false);
		bool processLine(const std::string &pulseSpecLine, bool markFirstLine=false);
};

#endif //LATTICEPAIR_HPP_2015_11_11

