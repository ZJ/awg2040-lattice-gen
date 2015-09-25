#include <sstream>
using namespace std;

#include <gtest/gtest.h>

#include "../src/pulseTrain.hpp"

TEST(FreqPulseDefaultConstructor, ExpectedValues) {
	freqPulse testPulse;
	EXPECT_EQ((double) 0, testPulse.getFrequency());
	EXPECT_EQ(double(1), testPulse.getAmplitude());
	EXPECT_EQ(double(0), testPulse.getDuration());
	EXPECT_EQ(double(0), testPulse.getPhase());
	EXPECT_FALSE(testPulse.getMarkStart());
	EXPECT_FALSE(testPulse.getMarkDuration());
}

TEST(FreqPulseExplicitConstructor, DefaultOptionalValues) {
	freqPulse testPulse = freqPulse(1.0,1.0,1.0);
	EXPECT_EQ(double(0.0),testPulse.getPhase());
	EXPECT_FALSE(testPulse.getMarkStart());
	EXPECT_FALSE(testPulse.getMarkDuration());
	
	testPulse = freqPulse(1.0,1.0,1.0,-1.0);
	EXPECT_EQ(double(-1.0),testPulse.getPhase());
	EXPECT_FALSE(testPulse.getMarkStart());
	EXPECT_FALSE(testPulse.getMarkDuration());

	testPulse = freqPulse(1.0,1.0,1.0,-1.0,true);
	EXPECT_EQ(double(-1.0),testPulse.getPhase());
	EXPECT_TRUE(testPulse.getMarkStart());
	EXPECT_FALSE(testPulse.getMarkDuration());
}

TEST(FreqPulseEquality, SelfEqual) {
	freqPulse testPulse;
	EXPECT_TRUE(testPulse==testPulse);
}

TEST(freqPulseEquality, DiffNotEqual) {
	freqPulse testPulse, notTestPulse;

	notTestPulse = freqPulse(2,1.0,0,0,false,false);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "frequency.";
	notTestPulse = freqPulse(0,0.5,0,0,false,false);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "amplitude.";
	notTestPulse = freqPulse(0,1.0,1,0,false,false);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "duration.";
	notTestPulse = freqPulse(0,1.0,1,-1,false,false);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "phase.";
	notTestPulse = freqPulse(0,1.0,0,0,true,false);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "start marker.";
	notTestPulse = freqPulse(0,1.0,0,0,false,true);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "duration marker.";

}

TEST(freqPulseInsertion, CompareDefault) {
	freqPulse testPulse;
	stringstream result;
	stringstream expectedResult;
	expectedResult << "Pulse " << (double) 0.0 << "MHz for " << (double) 0.0 << "ns with amplitude of " << (double) 1.0;
	expectedResult << " and phase " << double(0.0) << "*pi";
	expectedResult << ", without initial marker and without duration marker.";

	result << testPulse;
	EXPECT_EQ(expectedResult.str(), result.str());
}

TEST(freqPulseMarkers, NoMarkers) {
	freqPulse testPulse;
	string expectedResult = "";
	
	EXPECT_EQ(expectedResult, testPulse.getMarkerChars(0,0));
}

TEST(freqPulseMarkers, ShortenStart) {
	freqPulse testPulse;
	string expectedResult("\0\0",2);

	EXPECT_EQ(expectedResult, testPulse.getMarkerChars(2,4));
}

TEST(freqPulseMarkers, CorrectBytes) {
	freqPulse testPulse(0, 1.0, 0, 0, false, false);
	string expectedResult("\0\0",2);
	EXPECT_EQ(expectedResult, testPulse.getMarkerChars(2,1));

	testPulse = freqPulse(0, 1.0, 0, 0, true,  false);
	expectedResult = string("\2\0",2);
	EXPECT_EQ(expectedResult, testPulse.getMarkerChars(2,1));
	
	testPulse = freqPulse(0, 1.0, 0, 0, false, true );
	expectedResult = string("\1\1",2);
	EXPECT_EQ(expectedResult, testPulse.getMarkerChars(2,1));
	
	testPulse = freqPulse(0, 1.0, 0, 0, true,  true );
	expectedResult = string("\3\1",2);
	EXPECT_EQ(expectedResult, testPulse.getMarkerChars(2,1));
}

TEST(freqPulseMarkers, CorrectNumPoints) {
	freqPulse testPulse;
	
	for (unsigned int expectLength = 2; expectLength < 16;   expectLength++) {
		EXPECT_EQ(expectLength, testPulse.getMarkerChars(expectLength, 1).length());
	}
}

TEST(freqPulseMarkers, CorrectStartPoints) {
	freqPulse testPulse(0, 1.0, 0, 0, true, true);
	unsigned int numPoints = 16;

	for (unsigned int startLength = 1; startLength <= numPoints; startLength++) {
		EXPECT_EQ(startLength, testPulse.getMarkerChars(numPoints, startLength).find_last_of(string("\3")) + 1);
	}
}

TEST(freqPulseGetNumPoints, ExactPoints) {
	freqPulse testPulse(100.0, 1.0, 7.0);
	const double samplePeriodGHz = 1.0;
	const double samplePeriod1024MHz = 1.0/1.024;
	unsigned int expectedGHzPointCount = 7;

	EXPECT_EQ(expectedGHzPointCount, testPulse.getNumPoints(samplePeriodGHz, false));
	
	testPulse = freqPulse(100.0, 1.0, 130.0);
	expectedGHzPointCount = 130;
	unsigned int expected1024MHzPointCount = 133;
	EXPECT_EQ(expectedGHzPointCount, testPulse.getNumPoints(samplePeriodGHz, false));
	EXPECT_EQ(expected1024MHzPointCount, testPulse.getNumPoints(samplePeriod1024MHz, false));
}

TEST(freqPulseGetNumPoints, HalfCyclePoints) {
	freqPulse testPulse(100.0, 1.0, 113.0);
	const double samplePeriodGHz = 1.0;
	const double samplePeriod1024MHz = 1.0/1.024;

	unsigned int expectedCycles = 11.5;
	unsigned int expectedGHzPts = 115;
	unsigned int expected1024MHzPts = 117;
	EXPECT_EQ(expectedGHzPts, testPulse.getNumPoints(samplePeriodGHz));
	EXPECT_EQ(expected1024MHzPts, testPulse.getNumPoints(samplePeriod1024MHz)); 
}

int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
