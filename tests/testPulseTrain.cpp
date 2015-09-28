#include <deque>
using namespace std;

#include <gtest/gtest.h>

#include "../src/pulseTrain.hpp"

class defaultPulseTrain : public ::testing::Test {
	protected:
		pulseTrain myTrain;
};

class filledPulseTrain : public ::testing::Test {
	protected:
	virtual void SetUp() {
		firstPulse = freqPulse(10, 20, 30, 0.0, true, true);
		secondPulse = freqPulse(1, 2, 3, 0.0, false, true);
		initialQueue.push_back(firstPulse);
		initialQueue.push_back(secondPulse);
		initialShift = 3.14;

		testTrain = pulseTrain(initialShift, initialQueue);
	}
	double initialShift;
	pulseTrain testTrain;
	freqPulse firstPulse, secondPulse;
	std::deque<freqPulse> initialQueue;
};

class pulseTrainOutputs : public ::testing::Test {
	protected:
	pulseTrainOutputs() :
		firstPulse(freqPulse(100, 1.0, 10, 0.0, true, false)),
		secondPulse(freqPulse(130, 1.0, 10, 1.57079632679, false, true)),
		samplePeriodGHz(1.0),
		samplePeriodFull(1.0/1.024),
		expectPtsGHz(21),
		expectPtsFull(21),
		expectPtsExact(20),
		expectShiftGHz(3),
		expectShiftFull(4),
		shiftSize(3.45) {
			testTrain.pushPulse(firstPulse);
			testTrain.pushPulse(secondPulse);
		};
	pulseTrain testTrain;
	freqPulse firstPulse, secondPulse;
	const double samplePeriodGHz;
	const double samplePeriodFull;
	const unsigned int expectPtsGHz;
	const unsigned int expectPtsFull;
	const unsigned int expectPtsExact;
	const unsigned int expectShiftGHz;
	const unsigned int expectShiftFull;
	const double shiftSize;
};

TEST_F(defaultPulseTrain, CorrectContents) {
	deque<freqPulse> emptyPulseQueue;

	EXPECT_EQ(0, myTrain.getShift());
	EXPECT_EQ(emptyPulseQueue, myTrain.getPulses());
}

TEST_F(filledPulseTrain, size) {
	EXPECT_EQ(initialQueue.size(), testTrain.size());
}

TEST_F(filledPulseTrain, front) {
	EXPECT_EQ(initialQueue.front(), testTrain.front());
}

TEST_F(filledPulseTrain, pop) {
	initialQueue.pop_front();
	testTrain.pop();
	EXPECT_EQ(initialQueue, testTrain.getPulses());
}

TEST_F(filledPulseTrain, push) {
	freqPulse toPush(3.1,4,5,false,true);
	initialQueue.push_back(toPush);
	testTrain.pushPulse(toPush);;
	EXPECT_EQ(initialQueue, testTrain.getPulses());
}

TEST_F(filledPulseTrain, empty) {
	EXPECT_FALSE(testTrain.empty());
	EXPECT_EQ(initialQueue.empty(), testTrain.empty());
	initialQueue.pop_front();
	testTrain.pop();
	initialQueue.pop_front();
	testTrain.pop();
	EXPECT_TRUE(testTrain.empty());
	EXPECT_EQ(initialQueue.empty(), testTrain.empty());
}

TEST(pulseTrainExplicitConstructor, JustDelay) {
	pulseTrain testTrain(1.0);
	deque<freqPulse> emptyPulseQueue;

	EXPECT_EQ(1.0, testTrain.getShift());
	EXPECT_EQ(emptyPulseQueue, testTrain.getPulses());
}

TEST(pulseTrainExplicitConstructor, FullConstructor) {
	deque<freqPulse> pulseQueue;
	pulseQueue.push_back(freqPulse());

	pulseTrain testTrain(1.0, pulseQueue);

	EXPECT_EQ(1.0, testTrain.getShift());
	EXPECT_EQ(pulseQueue, testTrain.getPulses());
}

TEST(pulseTrainShift, SetShift) {
	pulseTrain testTrain;
	double testShift = 3.14;
	testTrain.setShift(testShift);
	EXPECT_EQ(testShift, testTrain.getShift());
}

TEST_F(pulseTrainOutputs, getNumPoints) {
	EXPECT_EQ(expectPtsExact, testTrain.getNumPoints(samplePeriodGHz, false));
	EXPECT_EQ(expectPtsExact, testTrain.getNumPoints(samplePeriodFull, false));
	EXPECT_EQ(expectPtsGHz,   testTrain.getNumPoints(samplePeriodGHz));
	EXPECT_EQ(expectPtsFull,  testTrain.getNumPoints(samplePeriodFull));
}

TEST_F(pulseTrainOutputs, getMarkerChars) {
	unsigned int startLength = 5;
	string expectedMarker  = string(startLength, (char) 0x02) + string(10 - startLength, (char) 0x00) + string(11, (char) 0x01);
	unsigned int testLen = expectedMarker.length();
	string shiftMarkerGHz  = expectedMarker.substr(testLen-expectShiftGHz)  + expectedMarker.substr(0, testLen-expectShiftGHz);
	string shiftMarkerFull = expectedMarker.substr(testLen-expectShiftFull) + expectedMarker.substr(0, testLen-expectShiftFull);
	string negMarkerGHz = expectedMarker.substr(expectShiftGHz ) + expectedMarker.substr(0, expectShiftGHz);
	string negMarkerFull= expectedMarker.substr(expectShiftFull) + expectedMarker.substr(0, expectShiftFull);

	EXPECT_EQ(expectedMarker, testTrain.getMarkerChars(samplePeriodGHz,  expectPtsGHz,  true, startLength));
	EXPECT_EQ(expectedMarker, testTrain.getMarkerChars(samplePeriodFull, expectPtsFull, true, startLength));
	
	EXPECT_EQ(expectedMarker.substr(0, expectPtsExact), testTrain.getMarkerChars(samplePeriodGHz,  expectPtsExact, false, startLength));
	EXPECT_EQ(expectedMarker.substr(0, expectPtsExact), testTrain.getMarkerChars(samplePeriodFull, expectPtsExact, false, startLength));

	testTrain.setShift(shiftSize);

	EXPECT_EQ(shiftMarkerGHz,  testTrain.getMarkerChars(samplePeriodGHz,  expectPtsGHz,  true, startLength));
	EXPECT_EQ(shiftMarkerFull, testTrain.getMarkerChars(samplePeriodFull, expectPtsFull, true, startLength));

	testTrain.setShift(-shiftSize);

	EXPECT_EQ(negMarkerGHz,  testTrain.getMarkerChars(samplePeriodGHz,  expectPtsGHz,  true, startLength));
	EXPECT_EQ(negMarkerFull, testTrain.getMarkerChars(samplePeriodFull, expectPtsFull, true, startLength));
}

TEST_F(pulseTrainOutputs, getWaveChars) {
	string expectExactGHz  = "\x7F\xCA\xF8\xF8\xCA\x7F\x34\x06\x06\x34\xD9\xFE\xD3\x73\x1B\x02\x38\x9B\xEC\xF9";
	string expectExactFull = "\x7F\xC8\xF7\xF9\xD0\x88\x3E\x0B\x02\x27\xD9\xFE\xD7\x7A\x21\x00\x2C\x8A\xE1\xFD";
	string expectGHz       = "\x7F\xCA\xF8\xF8\xCA\x7F\x34\x06\x06\x34\xD9\xFE\xD3\x73\x1B\x02\x38\x9B\xEC\xF9\xB9";
	string expectFull      = "\x7F\xC8\xF7\xF9\xD0\x88\x3E\x0B\x02\x27\xD9\xFE\xD7\x7A\x21\x00\x2C\x8A\xE1\xFD\xCD";
	string expectShiftGHz  = "\xEC\xF9\xB9\x7F\xCA\xF8\xF8\xCA\x7F\x34\x06\x06\x34\xD9\xFE\xD3\x73\x1B\x02\x38\x9B";
	string expectShiftFull = "\x8A\xE1\xFD\xCD\x7F\xC8\xF7\xF9\xD0\x88\x3E\x0B\x02\x27\xD9\xFE\xD7\x7A\x21\x00\x2C";
	string expectNegGHz    = "\xF8\xCA\x7F\x34\x06\x06\x34\xD9\xFE\xD3\x73\x1B\x02\x38\x9B\xEC\xF9\xB9\x7F\xCA\xF8";
	string expectNegFull   = "\xD0\x88\x3E\x0B\x02\x27\xD9\xFE\xD7\x7A\x21\x00\x2C\x8A\xE1\xFD\xCD\x7F\xC8\xF7\xF9";
	
	EXPECT_EQ(expectExactGHz,  testTrain.getWaveChars(samplePeriodGHz,  expectPtsExact, false));
	EXPECT_EQ(expectExactFull, testTrain.getWaveChars(samplePeriodFull, expectPtsExact, false));

	EXPECT_EQ(expectGHz,  testTrain.getWaveChars(samplePeriodGHz,  expectPtsGHz,  true));
	EXPECT_EQ(expectFull, testTrain.getWaveChars(samplePeriodFull, expectPtsFull, true));

	testTrain.setShift(shiftSize);
	EXPECT_EQ(expectShiftGHz,  testTrain.getWaveChars(samplePeriodGHz,  expectPtsGHz,  true));
	EXPECT_EQ(expectShiftFull, testTrain.getWaveChars(samplePeriodFull, expectPtsFull, true));
	
	testTrain.setShift(-shiftSize);
	EXPECT_EQ(expectNegGHz,  testTrain.getWaveChars(samplePeriodGHz,  expectPtsGHz,  true));
	EXPECT_EQ(expectNegFull, testTrain.getWaveChars(samplePeriodFull, expectPtsFull, true));
}


int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
