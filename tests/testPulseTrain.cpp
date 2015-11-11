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
		secondPulse(freqPulse(130, 1.0, 10, 0.78539816339744830962, false, true)),
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

	EXPECT_EQ(0, myTrain.shift());
	EXPECT_EQ(emptyPulseQueue, myTrain.pulses());
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
	EXPECT_EQ(initialQueue, testTrain.pulses());
}

TEST_F(filledPulseTrain, push) {
	freqPulse toPush(3.1,4,5,false,true);
	initialQueue.push_back(toPush);
	testTrain.pushPulse(toPush);;
	EXPECT_EQ(initialQueue, testTrain.pulses());
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

	EXPECT_EQ(1.0, testTrain.shift());
	EXPECT_EQ(emptyPulseQueue, testTrain.pulses());
}

TEST(pulseTrainExplicitConstructor, FullConstructor) {
	deque<freqPulse> pulseQueue;
	pulseQueue.push_back(freqPulse());

	pulseTrain testTrain(1.0, pulseQueue);

	EXPECT_EQ(1.0, testTrain.shift());
	EXPECT_EQ(pulseQueue, testTrain.pulses());
}

TEST(pulseTrainShift, SetShift) {
	pulseTrain testTrain;
	double testShift = 3.14;
	testTrain.shift(testShift);
	EXPECT_EQ(testShift, testTrain.shift());
}

TEST_F(pulseTrainOutputs, numPoints) {
	EXPECT_EQ(expectPtsExact, testTrain.numPoints(samplePeriodGHz, false));
	EXPECT_EQ(expectPtsExact, testTrain.numPoints(samplePeriodFull, false));
	EXPECT_EQ(expectPtsGHz,   testTrain.numPoints(samplePeriodGHz));
	EXPECT_EQ(expectPtsFull,  testTrain.numPoints(samplePeriodFull));
}

TEST_F(pulseTrainOutputs, markerChars) {
	unsigned int startLength = 5;
	string expectedMarker  = string(startLength, (char) 0x02) + string(10 - startLength, (char) 0x00) + string(11, (char) 0x01);
	unsigned int testLen = expectedMarker.length();
	string shiftMarkerGHz  = expectedMarker.substr(testLen-expectShiftGHz)  + expectedMarker.substr(0, testLen-expectShiftGHz);
	string shiftMarkerFull = expectedMarker.substr(testLen-expectShiftFull) + expectedMarker.substr(0, testLen-expectShiftFull);
	string negMarkerGHz = expectedMarker.substr(expectShiftGHz ) + expectedMarker.substr(0, expectShiftGHz);
	string negMarkerFull= expectedMarker.substr(expectShiftFull) + expectedMarker.substr(0, expectShiftFull);

	EXPECT_EQ(expectedMarker, testTrain.markerChars(samplePeriodGHz,  expectPtsGHz,  true, startLength));
	EXPECT_EQ(expectedMarker, testTrain.markerChars(samplePeriodFull, expectPtsFull, true, startLength));
	
	EXPECT_EQ(expectedMarker.substr(0, expectPtsExact), testTrain.markerChars(samplePeriodGHz,  expectPtsExact, false, startLength));
	EXPECT_EQ(expectedMarker.substr(0, expectPtsExact), testTrain.markerChars(samplePeriodFull, expectPtsExact, false, startLength));

	testTrain.shift(shiftSize);

	EXPECT_EQ(shiftMarkerGHz,  testTrain.markerChars(samplePeriodGHz,  expectPtsGHz,  true, startLength));
	EXPECT_EQ(shiftMarkerFull, testTrain.markerChars(samplePeriodFull, expectPtsFull, true, startLength));

	testTrain.shift(-shiftSize);

	EXPECT_EQ(negMarkerGHz,  testTrain.markerChars(samplePeriodGHz,  expectPtsGHz,  true, startLength));
	EXPECT_EQ(negMarkerFull, testTrain.markerChars(samplePeriodFull, expectPtsFull, true, startLength));
}

TEST_F(pulseTrainOutputs, waveChars) {
	string expectExactGHz  = string("\x7F\xCA\xF8\xF8\xCA\x7F\x34\x06\x06\x34\xD9\xFE\xD3\x73\x1B\x02\x38\x9B\xEC\xF9", 20);
	string expectExactFull = string("\x7F\xC8\xF7\xF9\xD0\x88\x3E\x0B\x02\x27\xD9\xFE\xD7\x7A\x21\x00\x2C\x8A\xE1\xFD", 20);
	string expectGHz       = string("\x7F\xCA\xF8\xF8\xCA\x7F\x34\x06\x06\x34\xD9\xFE\xD3\x73\x1B\x02\x38\x9B\xEC\xF9\xB9", 21);
	string expectFull      = string("\x7F\xC8\xF7\xF9\xD0\x88\x3E\x0B\x02\x27\xD9\xFE\xD7\x7A\x21\x00\x2C\x8A\xE1\xFD\xCD", 21);
	string expectShiftGHz  = string("\xEC\xF9\xB9\x7F\xCA\xF8\xF8\xCA\x7F\x34\x06\x06\x34\xD9\xFE\xD3\x73\x1B\x02\x38\x9B", 21);
	string expectShiftFull = string("\x8A\xE1\xFD\xCD\x7F\xC8\xF7\xF9\xD0\x88\x3E\x0B\x02\x27\xD9\xFE\xD7\x7A\x21\x00\x2C", 21);
	string expectNegGHz    = string("\xF8\xCA\x7F\x34\x06\x06\x34\xD9\xFE\xD3\x73\x1B\x02\x38\x9B\xEC\xF9\xB9\x7F\xCA\xF8", 21);
	string expectNegFull   = string("\xD0\x88\x3E\x0B\x02\x27\xD9\xFE\xD7\x7A\x21\x00\x2C\x8A\xE1\xFD\xCD\x7F\xC8\xF7\xF9", 21);
	
	EXPECT_EQ(expectExactGHz,  testTrain.waveChars(samplePeriodGHz,  expectPtsExact, false));
	EXPECT_EQ(expectExactFull, testTrain.waveChars(samplePeriodFull, expectPtsExact, false));

	EXPECT_EQ(expectGHz,  testTrain.waveChars(samplePeriodGHz,  expectPtsGHz,  true));
	EXPECT_EQ(expectFull, testTrain.waveChars(samplePeriodFull, expectPtsFull, true));

	testTrain.shift(shiftSize);
	EXPECT_EQ(expectShiftGHz,  testTrain.waveChars(samplePeriodGHz,  expectPtsGHz,  true));
	EXPECT_EQ(expectShiftFull, testTrain.waveChars(samplePeriodFull, expectPtsFull, true));
	
	testTrain.shift(-shiftSize);
	EXPECT_EQ(expectNegGHz,  testTrain.waveChars(samplePeriodGHz,  expectPtsGHz,  true));
	EXPECT_EQ(expectNegFull, testTrain.waveChars(samplePeriodFull, expectPtsFull, true));
}


int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
