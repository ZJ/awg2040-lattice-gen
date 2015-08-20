#ifndef PULSE_TRAIN_HPP_20150819
#define PULSE_TRAIN_HPP_20150819
#include <iostream>
#include <queue>
class freqPulse {
	public:
		freqPulse() {
			myFrequency = myDuration = 0;
			myAmplitude = 1.0;
			myMarkStart = myMarkDuration = false;
		}
		freqPulse(double setFreq, double setAmp, double setDur, bool setStart=false, bool setMarkAll=false) {
			myFrequency = setFreq;
			myAmplitude = setAmp;
			myDuration  = setDur;
			myMarkStart = setStart;
			myMarkDuration = setMarkAll;
		}
		// Read Accessors
		inline double	getFrequency()    const {return myFrequency;}
		inline double	getAmplitude()    const {return myAmplitude;}
		inline double	getDuration()     const {return myDuration;}
		inline bool	getMarkStart()    const {return myMarkStart;}
		inline bool	getMarkDuration() const {return myMarkDuration;}
		// Overload Operators
		bool operator ==(const freqPulse & otherPulse) const{
			return ((myFrequency == otherPulse.getFrequency()) 
				&& (myAmplitude == otherPulse.getAmplitude())
				&& (myDuration == otherPulse.getDuration())
				&& (myMarkStart == otherPulse.getMarkStart())
				&& (myMarkDuration == otherPulse.getMarkDuration()));
		}
		friend std::ostream & operator<<(std::ostream & os, const freqPulse& pulse);
	private:
		double	myFrequency;
		double	myAmplitude;
		double	myDuration;
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
