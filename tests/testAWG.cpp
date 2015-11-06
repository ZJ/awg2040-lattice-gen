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

int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
