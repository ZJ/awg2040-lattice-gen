#ifndef AWG_2015_09_28
#define AWG_2015_09_28
#include <string>
#include <cstdio>

#include "pulseTrain.hpp"

class awg {
	public:
		// Constructor
		awg(
			pulseTrain outputPulses=pulseTrain(),
			unsigned long baudRate=19200,
			double clockFrequency=1024.0,
			std::string COMPort=std::string("COM1"),
			std::string waveName=std::string("awgWave.txt")
		)
			: _outputPulses(outputPulses)
			, _baudRate(baudRate)
			, _clockFrequency(clockFrequency)
			, _sampleRate(1000.0/clockFrequency)
			, _waveName(waveName)
			, _COMPort(COMPort)
		{ };
		// Write Accessors
		inline void clockFrequency(double newFreq) {
			if (newFreq < 0) newFreq = -newFreq;

			_clockFrequency = newFreq;
			_sampleRate     = 1000.0/newFreq;
		}
		inline void sampleRate(double newSampleRate) {
			if (newSampleRate < 0) newSampleRate = -newSampleRate;

			_sampleRate     = newSampleRate;
			_clockFrequency = 1000.0/newSampleRate;
		}
		inline void COMPort(const std::string newCOM) {
			_COMPort = newCOM;
		}
		inline void COMPort(const unsigned int newCOMNum) {
			char buffer[16];
			snprintf(buffer, 16, "COM%u", newCOMNum);
			_COMPort = buffer;
		}
		inline void waveName(const std::string newName) {
			_waveName = newName;
		}
		inline void baudRate(const unsigned int newBaudRate) {
			_baudRate = newBaudRate;
		}
		inline void outputPulses(const pulseTrain newPulseTrain) {
			_outputPulses = newPulseTrain;
		}
		// Read Accessors
		inline unsigned long	baudRate()	{return _baudRate;}
		inline std::string	waveName()	{return _waveName;}
		inline std::string	COMPort()	{return _COMPort;}
		inline double	clockFrequency()	{return _clockFrequency;}
		inline double	sampleRate()	{return _sampleRate;}
		inline pulseTrain	outputPulses()	{return _outputPulses;}
	private:
		pulseTrain    _outputPulses;
		unsigned long _baudRate;
		double        _clockFrequency;
		double        _sampleRate;
		std::string   _waveName;
		std::string   _COMPort;
};

#endif //AWG_2015_09_28
