#include <gtest/gtest.h>
#include <string>

#include "../src/latticePair.hpp"

class LatticePairProcessLine : public testing::Test {
	protected:
		latticePair	testPair;
		std::string testString;
};

TEST_F(LatticePairProcessLine, GoodLine) {
	testString = "1.0, 2.1, 3.2, -0.9 #this is a comment\n";

	EXPECT_TRUE(testPair.processLine(testString, true));
	EXPECT_TRUE(testPair.processLine(testString, false));
}

TEST_F(LatticePairProcessLine, BadThreeArgs) {
	testString = "1.0, 2.1, 3.2 #this is a comment\n";

	EXPECT_FALSE(testPair.processLine(testString, true));
	EXPECT_FALSE(testPair.processLine(testString, false));
}

TEST_F(LatticePairProcessLine, BadTwoArgs) {
	testString = "1.0, 2.1 #this is a comment\n";

	EXPECT_FALSE(testPair.processLine(testString, true));
	EXPECT_FALSE(testPair.processLine(testString, false));
}

TEST_F(LatticePairProcessLine, BadOneArg) {
	testString = "1.0";

	EXPECT_FALSE(testPair.processLine(testString, true));
	EXPECT_FALSE(testPair.processLine(testString, false));
}

TEST_F(LatticePairProcessLine, GoodTrailingWhitespace) {
	testString = "1.0, 2.1, 3.2, -0.99   \n";

	EXPECT_TRUE(testPair.processLine(testString, true));
	EXPECT_TRUE(testPair.processLine(testString, false));
}

TEST_F(LatticePairProcessLine, GoodNoEOL) {
	testString = "1.0, 2.1, 3.2, -0.99";

	EXPECT_TRUE(testPair.processLine(testString, true));
	EXPECT_TRUE(testPair.processLine(testString, false));
}

TEST_F(LatticePairProcessLine, GoodWSLine) {
	testString = "  \t\n";

	EXPECT_TRUE(testPair.processLine(testString, true));
	EXPECT_TRUE(testPair.processLine(testString, false));
}

TEST_F(LatticePairProcessLine, GoodCommentLine) {
	testString = "  # Comments are OK if alone.";

	EXPECT_TRUE(testPair.processLine(testString, true));
	EXPECT_TRUE(testPair.processLine(testString, false));
}

TEST_F(LatticePairProcessLine, GoodNullLine) {
	testString = "";
	EXPECT_TRUE(testPair.processLine(testString, true));
	EXPECT_TRUE(testPair.processLine(testString, false));
}

TEST_F(LatticePairProcessLine, BadExtraArg) {
	testString = "1.0, 2.1, 3.2, 1.1, 4.4 #this is a comment\n";

	EXPECT_FALSE(testPair.processLine(testString, true));
	EXPECT_FALSE(testPair.processLine(testString, false));
}

int main(int argc, char * argv[] ) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
