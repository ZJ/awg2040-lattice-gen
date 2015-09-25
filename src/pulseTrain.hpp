#ifndef PULSE_TRAIN_HPP_20150819
#define PULSE_TRAIN_HPP_20150819
#include <iostream>
#include <string>
#include <queue>
#include <cmath>

class freqPulse {
	public:
		freqPulse() {
			myFrequency = myDuration = myPhase = 0;
			myAmplitude = 1.0;
			myMarkStart = myMarkDuration = false;
		}
		freqPulse(double setFreq, double setAmp, double setDur, double setPhase=0.0, bool setStart=false, bool setMarkAll=false) {
			myFrequency = setFreq;
			myAmplitude = setAmp;
			myDuration  = setDur;
			myPhase     = setPhase;
			myMarkStart = setStart;
			myMarkDuration = setMarkAll;
		}
		// Read Accessors
		inline double	getFrequency()    const {return myFrequency;}
		inline double	getAmplitude()    const {return myAmplitude;}
		inline double	getDuration()     const {return myDuration;}
		inline double   getPhase()        const {return myPhase;}
		inline bool	getMarkStart()    const {return myMarkStart;}
		inline bool	getMarkDuration() const {return myMarkDuration;}
		// Overload Operators
		bool operator ==(const freqPulse & otherPulse) const{
			return ((myFrequency == otherPulse.getFrequency()) 
				&& (myAmplitude == otherPulse.getAmplitude())
				&& (myDuration == otherPulse.getDuration())
				&& (myPhase == otherPulse.getPhase())
				&& (myMarkStart == otherPulse.getMarkStart())
				&& (myMarkDuration == otherPulse.getMarkDuration()));
		}
		friend std::ostream & operator<<(std::ostream & os, const freqPulse& pulse);
		// Class Methods
		std::string getMarkerChars(const unsigned int numPoints, unsigned int startMarkerPoints = 1);
		std::string getWaveChars(const double samplePeriod, const double numPoints);
		unsigned int getNumPoints(const double samplePeriod, const bool nearestHalfCycle = true);
	private:
		double	myFrequency;
		double	myAmplitude;
		double	myDuration;
		double  myPhase;
		bool	myMarkStart;
		bool	myMarkDuration;
};

class pulseTrain {
	public:
		// Constructors
		inline pulseTrain(double shift=0, std::queue<freqPulse> pulseQueue=std::queue<freqPulse>()) {
			myCyclicShift  = shift;
			myPulses = pulseQueue;
		};
		// Restricted set of queue methods for freqPulse queue
		inline void pop() {myPulses.pop();};
		inline void pushPulse(const freqPulse& toPush) {myPulses.push(toPush);};
		inline freqPulse& front() {return myPulses.front();};
		inline bool empty() const {return myPulses.empty();};
		inline std::vector<freqPulse>::size_type size() const {return myPulses.size();};
		// Accessors
		inline std::queue<freqPulse> getPulses() const {return myPulses;};
		inline double getShift() const {return myCyclicShift;};
		inline void setShift(const double shift) {myCyclicShift=shift;};
	private:
		double myCyclicShift;
		std::queue<freqPulse> myPulses;
};
#endif //PULSE_TRAIN_HPP_20150819
