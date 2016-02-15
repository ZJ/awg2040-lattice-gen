#ifndef PULSE_TRAIN_HPP_20150819
#define PULSE_TRAIN_HPP_20150819
#include <iostream>
#include <string>
#include <deque>
#include <cmath>

class freqPulse {
	public:
		freqPulse() {
			_frequency = _duration = _phase = 0;
			_amplitude = 1.0;
			_markStart = _markDuration = false;
		}
		freqPulse(double setFreq, double setAmp, double setDur, double setPhase=0.0, bool setStart=false, bool setMarkAll=false) {
			_frequency = setFreq;
			_amplitude = setAmp;
			_duration  = setDur;
			_phase     = setPhase;
			_markStart = setStart;
			_markDuration = setMarkAll;
		}
		// Read Accessors
		inline double	frequency()    const {return _frequency;}
		inline double	amplitude()    const {return _amplitude;}
		inline double	duration()     const {return _duration;}
		inline double   phase()        const {return _phase;}
		inline bool	markStart()    const {return _markStart;}
		inline bool	markDuration() const {return _markDuration;}
		// Overload Operators
		bool operator ==(const freqPulse & otherPulse) const{
			return ((_frequency == otherPulse.frequency()) 
				&& (_amplitude == otherPulse.amplitude())
				&& (_duration == otherPulse.duration())
				&& (_phase == otherPulse.phase())
				&& (_markStart == otherPulse.markStart())
				&& (_markDuration == otherPulse.markDuration()));
		}
		friend std::ostream & operator<<(std::ostream & os, const freqPulse& pulse);
		// Class Methods
		std::string markerChars(const unsigned int numPoints, unsigned int startMarkerPoints = 1);
		std::string waveChars(const double samplePeriod, const unsigned int numPoints);
		unsigned int numPoints(const double samplePeriod, const bool nearestHalfCycle = true);
	private:
		double	_frequency;
		double	_amplitude;
		double	_duration;
		double  _phase;
		bool	_markStart;
		bool	_markDuration;
};

class pulseTrain {
	public:
		// Constructors
		inline pulseTrain(double shift=0, std::deque<freqPulse> pulseQueue=std::deque<freqPulse>()) {
			_cyclicShift  = shift;
			_pulses = pulseQueue;
		};
		// Restricted set of deque methods for freqPulse deque
		inline void pop() {_pulses.pop_front();};
		inline void pushPulse(const freqPulse& toPush) {_pulses.push_back(toPush);};
		inline freqPulse& front() {return _pulses.front();};
		inline bool empty() const {return _pulses.empty();};
		inline std::deque<freqPulse>::size_type size() const {return _pulses.size();};
		// Class Methods
		std::string markerChars(const double samplePeriod, const unsigned int numPoints, const bool nearestHalfCycle = true, const unsigned int startMarkerPoints = 1);
		std::string waveChars(const double samplePeriod, const unsigned int numPoints, const bool nearestHalfCycle = true);
		unsigned int numPoints(const double samplePeriod, const bool nearestHalfCycle = true);
		// Accessors
		inline std::deque<freqPulse> pulses() const {return _pulses;};
		inline double shift() const {return _cyclicShift;};
		inline void shift(const double shift) {_cyclicShift=shift;};
	private:
		double _cyclicShift;
		std::deque<freqPulse> _pulses;
};
#endif //PULSE_TRAIN_HPP_20150819
