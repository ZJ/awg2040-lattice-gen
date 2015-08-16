#include <gtest/gtest.h>
#include <string>
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
EXPECT_EQ("", opts.getInputPath());
}

TEST_F(CmdLineOptionsTest, ExplicitConstructor) {
CmdLineOptions helpSet(true,false,false,false,"");
EXPECT_EQ(true, helpSet.getHelp());
EXPECT_EQ(false, helpSet.getTemplate());
EXPECT_EQ(false, helpSet.getDebug());
EXPECT_EQ(false, helpSet.getQuiet());
EXPECT_EQ("", helpSet.getInputPath());

CmdLineOptions tempSet(false,true,false,false,"");
EXPECT_EQ(false, tempSet.getHelp());
EXPECT_EQ(true, tempSet.getTemplate());
EXPECT_EQ(false, tempSet.getDebug());
EXPECT_EQ(false, tempSet.getQuiet());
EXPECT_EQ("", tempSet.getInputPath());

CmdLineOptions debugSet(false,false,true,false,"");
EXPECT_EQ(false, debugSet.getHelp());
EXPECT_EQ(false, debugSet.getTemplate());
EXPECT_EQ(true, debugSet.getDebug());
EXPECT_EQ(false, debugSet.getQuiet());
EXPECT_EQ("", debugSet.getInputPath());

CmdLineOptions quietSet(false,false,false,true,"");
EXPECT_EQ(false, quietSet.getHelp());
EXPECT_EQ(false, quietSet.getTemplate());
EXPECT_EQ(false, quietSet.getDebug());
EXPECT_EQ(true, quietSet.getQuiet());
EXPECT_EQ("", quietSet.getInputPath());

CmdLineOptions pathSet(false,false,false,false,"ExplicitConstructorPath");
EXPECT_EQ(false, pathSet.getHelp());
EXPECT_EQ(false, pathSet.getTemplate());
EXPECT_EQ(false, pathSet.getDebug());
EXPECT_EQ(false, pathSet.getQuiet());
EXPECT_EQ("ExplicitConstructorPath", pathSet.getInputPath());
}
} // end namespace

int main(int argc, char * argv[]) {
::testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
} 
