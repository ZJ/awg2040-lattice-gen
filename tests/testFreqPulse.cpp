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

int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
