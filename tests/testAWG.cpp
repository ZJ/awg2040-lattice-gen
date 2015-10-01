#include <gtest/gtest.h>

#include "../src/awg.hpp"

#include "../src/pulseTrain.hpp"


class AWGConstructors : public ::testing::Test {
	protected:
};

class AWGAcessors : public ::testing::Test {
	protected:
};

int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
