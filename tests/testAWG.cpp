#include <gtest/gtest.h>

#include "../src/awg.hpp"

#include "../src/pulseTrain.hpp"

TEST(AwgClass, ComPort) {
	awg testAwg;

	EXPECT_EQ(std::string("COM1"),testAwg.comPort());

	testAwg.comPort(2);
	EXPECT_EQ(std::string("COM2"),testAwg.comPort());

	testAwg.comPort("com1");
	EXPECT_EQ(std::string("com1"),testAwg.comPort());
}

#define S(s) s, sizeof s - 1

TEST(AwgClass, ProgrammingString) {
	freqPulse	testPulse(100, 0.0, 5.0, 0, true);
	pulseTrain	testTrain(0.0);
	testTrain.pushPulse(testPulse);

	awg testAwg(testTrain, 19200, 1000.0, "COM42", "testName.txt");
	const std::string expectedProgramming(S("DATA:DESTINATION \"testName.txt\"\nDATA:WIDTH 1\nCURVE #15\x7F\x7F\x7F\x7F\x7F\nMARKER:DATA #15\x2\0\0\0\0\nCLOCK:FREQUENCY 1000.000000MHz\nCH1:WAVEFORM \"testName.txt\"\n"));
	EXPECT_EQ(expectedProgramming, testAwg.programmingString());
}

int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
