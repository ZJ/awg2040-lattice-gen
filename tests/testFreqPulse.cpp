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

// Test possible marker combos on 2,1
// Test lengths being correct

int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
