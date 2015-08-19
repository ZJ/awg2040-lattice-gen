#include <string>
#include <sstream>
using namespace std;

#include <gtest/gtest.h>
#include "../src/options.hpp"

namespace {

class CmdLineOptionsOutputTest : public ::testing::Test {
protected:
  CmdLineOptions opts;
  stringstream testStream;
  string expectedResponse;
};

TEST_F(CmdLineOptionsOutputTest, Print) {
expectedResponse="Help:    \t0;\tTemplate:\t0;\nDebug:   \t0;\tQuiet:   \t0;\nInput path:\tPrintedPath\n";

opts.setInputPath("PrintedPath");
opts.print(testStream);

EXPECT_EQ(expectedResponse, testStream.str());
}

TEST_F(CmdLineOptionsOutputTest, InsertionOperator) {
expectedResponse = "NoAlpha10 Help Flag: true, Template Flag: true, Debug Flag: false, Quiet Flag: false, Input Path: TheDumbestPath NoAlpha10";
opts=CmdLineOptions(true,true,false,false,"TheDumbestPath");
testStream << noboolalpha << "NoAlpha" << true << false << " " << opts << " NoAlpha" << true << false;

EXPECT_EQ(expectedResponse, testStream.str());

expectedResponse="Help Flag: true, Template Flag: true, Debug Flag: false, Quiet Flag: false, Input Path: <unset>";
opts=CmdLineOptions(true,true,false,false,"");
testStream.clear();
testStream.str(string());
testStream << opts;

EXPECT_EQ(expectedResponse, testStream.str());
}
} // end namespace

int main(int argc, char * argv[]) {
::testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
} 
