#include <gtest/gtest.h>
#include "../src/options.hpp"

namespace {

class CmdLineOptionsTest : public ::testing::Test {
protected:
CmdLineOptions opts;
};

TEST_F(CmdLineOptionsTest, DefaultConstructor) {
EXPECT_EQ(false, opts.getHelp());
EXPECT_EQ(false, opts.getTemplate());
EXPECT_EQ(false, opts.getDebug());
EXPECT_EQ(false, opts.getQuiet());
}
} // end namespace

int main(int argc, char * argv[]) {
::testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
} 
