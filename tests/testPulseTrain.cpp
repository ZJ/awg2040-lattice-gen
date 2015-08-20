#include <queue>
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
		firstPulse = freqPulse(10, 20, 30, true, true);
		secondPulse = freqPulse(1, 2, 3, false, true);
		initialQueue.push(firstPulse);
		initialQueue.push(secondPulse);
		initialShift = 3.14;

		testTrain = pulseTrain(initialShift, initialQueue);
	}
	double initialShift;
	pulseTrain testTrain;
	freqPulse firstPulse, secondPulse;
	std::queue<freqPulse> initialQueue;
};

TEST_F(defaultPulseTrain, CorrectContents) {
	queue<freqPulse> emptyPulseQueue;

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
	initialQueue.pop();
	testTrain.pop();
	EXPECT_EQ(initialQueue, testTrain.getPulses());
}

TEST_F(filledPulseTrain, push) {
	freqPulse toPush(3.1,4,5,false,true);
	initialQueue.push(toPush);
	testTrain.pushPulse(toPush);;
	EXPECT_EQ(initialQueue, testTrain.getPulses());
}

TEST_F(filledPulseTrain, empty) {
	EXPECT_FALSE(testTrain.empty());
	EXPECT_EQ(initialQueue.empty(), testTrain.empty());
	initialQueue.pop();
	testTrain.pop();
	initialQueue.pop();
	testTrain.pop();
	EXPECT_TRUE(testTrain.empty());
	EXPECT_EQ(initialQueue.empty(), testTrain.empty());
}

TEST(pulseTrainExplicitConstructor, JustDelay) {
	pulseTrain testTrain(1.0);
	queue<freqPulse> emptyPulseQueue;

	EXPECT_EQ(1.0, testTrain.getShift());
	EXPECT_EQ(emptyPulseQueue, testTrain.getPulses());
}

TEST(pulseTrainExplicitConstructor, FullConstructor) {
	queue<freqPulse> pulseQueue;
	pulseQueue.push(freqPulse());

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

int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
