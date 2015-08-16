#include <gtest/gtest.h>
#include <string>
#include "../src/options.hpp"

namespace {

class CmdLineOptionsTest : public ::testing::Test {
protected:
CmdLineOptions opts;
};

TEST_F(CmdLineOptionsTest, DefaultConstructor) {
EXPECT_FALSE(opts.getHelp());
EXPECT_FALSE(opts.getTemplate());
EXPECT_FALSE(opts.getDebug());
EXPECT_FALSE(opts.getQuiet());
EXPECT_EQ("", opts.getInputPath());
}

TEST_F(CmdLineOptionsTest, ExplicitConstructor) {
CmdLineOptions helpSet(true,false,false,false,"");
EXPECT_TRUE(helpSet.getHelp());
EXPECT_FALSE(helpSet.getTemplate());
EXPECT_FALSE(helpSet.getDebug());
EXPECT_FALSE(helpSet.getQuiet());
EXPECT_EQ("", helpSet.getInputPath());

CmdLineOptions tempSet(false,true,false,false,"");
EXPECT_FALSE(tempSet.getHelp());
EXPECT_TRUE(tempSet.getTemplate());
EXPECT_FALSE(tempSet.getDebug());
EXPECT_FALSE(tempSet.getQuiet());
EXPECT_EQ("", tempSet.getInputPath());

CmdLineOptions debugSet(false,false,true,false,"");
EXPECT_FALSE(debugSet.getHelp());
EXPECT_FALSE(debugSet.getTemplate());
EXPECT_TRUE(debugSet.getDebug());
EXPECT_FALSE(debugSet.getQuiet());
EXPECT_EQ("", debugSet.getInputPath());

CmdLineOptions quietSet(false,false,false,true,"");
EXPECT_FALSE(quietSet.getHelp());
EXPECT_FALSE(quietSet.getTemplate());
EXPECT_FALSE(quietSet.getDebug());
EXPECT_TRUE(quietSet.getQuiet());
EXPECT_EQ("", quietSet.getInputPath());

CmdLineOptions pathSet(false,false,false,false,"ExplicitConstructorPath");
EXPECT_FALSE(pathSet.getHelp());
EXPECT_FALSE(pathSet.getTemplate());
EXPECT_FALSE(pathSet.getDebug());
EXPECT_FALSE(pathSet.getQuiet());
EXPECT_EQ("ExplicitConstructorPath", pathSet.getInputPath());
}

} // end namespace

int main(int argc, char * argv[]) {
::testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
} 
