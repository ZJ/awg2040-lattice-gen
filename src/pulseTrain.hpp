#ifndef PULSE_TRAIN_HPP_20150819
#define PULSE_TRAIN_HPP_20150819
#include <iostream>


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
		bool operator ==(const freqPulse & otherPulse) {
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

#endif //PULSE_TRAIN_HPP_20150819
