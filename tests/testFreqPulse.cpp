#include <gtest/gtest.h>
#include "../src/pulseTrain.hpp"

TEST(FreqPulseDefaultConstructor, ExpectedValues) {
	freqPulse testPulse;
	EXPECT_EQ((double) 0, testPulse.getFrequency());
	EXPECT_EQ(double(1), testPulse.getAmplitude());
	EXPECT_EQ(double(0), testPulse.getDuration());
	EXPECT_FALSE(testPulse.getMarkStart());
	EXPECT_FALSE(testPulse.getMarkDuration());
}

TEST(FreqPulseExplicitConstructor, DefaultOptionalValues) {
	freqPulse testPulse = freqPulse(1.0,1.0,1.0);

	EXPECT_FALSE(testPulse.getMarkStart());
	EXPECT_FALSE(testPulse.getMarkDuration());

	testPulse = freqPulse(1.0,1.0,1.0,true);
	EXPECT_TRUE(testPulse.getMarkStart());
	EXPECT_FALSE(testPulse.getMarkDuration());
}

TEST(FreqPulseEquality, SelfEqual) {
	freqPulse testPulse;
	EXPECT_TRUE(testPulse==testPulse);
}

TEST(freqPulseEquality, DiffNotEqual) {
	freqPulse testPulse, notTestPulse;

	notTestPulse = freqPulse(2,1.0,0,false,false);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "frequency.";
	notTestPulse = freqPulse(0,0.5,0,false,false);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "amplitude.";
	notTestPulse = freqPulse(0,1.0,1,false,false);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "duration.";
	notTestPulse = freqPulse(0,1.0,0,true,false);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "start marker.";
	notTestPulse = freqPulse(0,1.0,0,false,true);
	EXPECT_FALSE(testPulse==notTestPulse) << " Missed " << "duration marker.";

}

int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
