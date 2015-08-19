#include <string>
#include <tr1/tuple>
using namespace std;

#include <gtest/gtest.h>
#include "../src/options.hpp"

namespace {

TEST(CmdLineOptionsDefConstructor, ExpectedValues) {
CmdLineOptions opts;

EXPECT_FALSE(opts.getHelp());
EXPECT_FALSE(opts.getTemplate());
EXPECT_FALSE(opts.getDebug());
EXPECT_FALSE(opts.getQuiet());
EXPECT_EQ("", opts.getInputPath());
}

TEST(CmdLineOptionsDefConstructor, MatchesExplicit) {
CmdLineOptions opts;
CmdLineOptions expected = CmdLineOptions(false,false,false,false,"");

EXPECT_EQ(expected, opts);
}

#ifdef GTEST_HAS_COMBINE
// Setup the command line processing
// Do it with tuples so we get all possible combos easily

using ::testing::ValuesIn;
using ::testing::Bool;

string inputPathStrings[2] = {"", "ExplicitConstructorPath"};

class CmdLineOptionsConstructor : public ::testing::TestWithParam< tr1::tuple<bool, bool, bool, bool, string> > {
protected:
  CmdLineOptions* expectedOptions;
  bool helpIn, tempIn, debugIn, quietIn;
  string inputIn;

virtual void SetUp() {
  tr1::tie(helpIn,tempIn,debugIn,quietIn,inputIn) = GetParam();
  expectedOptions = new CmdLineOptions(helpIn, tempIn, debugIn, quietIn, inputIn);
}

virtual void TearDown() {
delete expectedOptions;
expectedOptions = NULL;
}

};

TEST_P(CmdLineOptionsConstructor, SetsCorrectly) {
if ( helpIn ) {
EXPECT_TRUE(expectedOptions->getHelp());
} else {
EXPECT_FALSE(expectedOptions->getHelp());
}

if ( tempIn ) {
EXPECT_TRUE(expectedOptions->getTemplate());
} else {
EXPECT_FALSE(expectedOptions->getTemplate());
}

if ( debugIn ) {
EXPECT_TRUE(expectedOptions->getDebug());
} else {
EXPECT_FALSE(expectedOptions->getDebug());
}

if ( quietIn ) {
EXPECT_TRUE(expectedOptions->getQuiet());
} else {
EXPECT_FALSE(expectedOptions->getQuiet());
}

EXPECT_EQ(inputIn, expectedOptions->getInputPath());
}

INSTANTIATE_TEST_CASE_P(AllValidCombinations, CmdLineOptionsConstructor, ::testing::Combine( Bool(), Bool(), Bool(), Bool(), ValuesIn(inputPathStrings) ) );
#else
TEST(DummyTest, CombineIsNoteSupportedOnThisPlatform) {ASSERT_TRUE(false);}
#endif // GTEST_HAS_COMBINE

} // end namespace

int main(int argc, char * argv[]) {
::testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
} 
