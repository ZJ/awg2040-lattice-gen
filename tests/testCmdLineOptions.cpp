#include <string>
#include <cstring>
#include <sstream>
#include <tr1/tuple>
using namespace std;

#include <gtest/gtest.h>
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

TEST_F(CmdLineOptionsTest, Print) {
stringstream testStream;
string expectedResponse="Help:    \t0;\tTemplate:\t0;\nDebug:   \t0;\tQuiet:   \t0;\nInput path:\tPrintedPath\n";

opts.setInputPath("PrintedPath");

opts.print(testStream);
EXPECT_EQ(expectedResponse, testStream.str());
}

#ifdef GTEST_HAS_COMBINE
// Setup the command line processing
// Do it with tuples so we get all possible combos easily

using ::testing::ValuesIn;

string helpStrings[3] = {"-h", "--help", ""};
string templateStrings[3] = {"-t", "--template", ""};
string debugStrings[3] = {"-d", "--debug", ""};
string quietStrings[3] = {"-q", "--quiet", ""};
string inputPathStrings[3] = {"-i", "--input-path", ""};
char dummyProg[] = "PretendProg";
string dummyPath = "PretendInputFile";

class ProcessCmdLineTest : public ::testing::TestWithParam< tr1::tuple<string, string, string, string, string> > {
protected:
CmdLineOptions processedOptions;
CmdLineOptions* expectedOptions;
int argc;
char * argv[8];
virtual void SetUp() {
  string helpIn, tempIn, debugIn, quietIn, inputIn;
  tr1::tie(helpIn,tempIn,debugIn,quietIn,inputIn) = GetParam();
  expectedOptions = new CmdLineOptions(!helpIn.empty(), !tempIn.empty(), !debugIn.empty(), !quietIn.empty(), inputIn.empty() ? "" : "PretendInputFile");
  // Int args
  argv[0] = dummyProg;
  argc=1; 
  // Fill argv appropriately
  if (!helpIn.empty()) {
        argv[argc] = new char [helpIn.length()+1];
	strcpy(argv[argc], helpIn.c_str());
        argc++;
  } 
  if (!tempIn.empty()) {
        argv[argc] = new char [tempIn.length()+1];
	strcpy(argv[argc], tempIn.c_str());
        argc++;
  } 
  if (!debugIn.empty()) {
        argv[argc] = new char [debugIn.length()+1];
	strcpy(argv[argc], debugIn.c_str());
        argc++;
  } 
  if (!quietIn.empty()) {
        argv[argc] = new char [quietIn.length()+1];
	strcpy(argv[argc], quietIn.c_str());
        argc++;
  } 
  if (!inputIn.empty()) {
	inputIn += "=" + dummyPath;
        argv[argc] = new char [inputIn.length()+1];
	strcpy(argv[argc], inputIn.c_str());
        argc++;
  }
  for (int i = argc; i<8; i++) argv[i]=NULL;
}

virtual void TearDown() {
delete expectedOptions;
expectedOptions = NULL;
for(int i=1; i<argc; i++) {
  if(NULL != argv[i]) {
	delete[] argv[i];
        argv[i]=NULL;
}
}
}

};

TEST_P(ProcessCmdLineTest, ProcessesCorrectly) {
ASSERT_EQ(OPT_PARSE_OK, processedOptions.processCmdLine(argc, argv));
EXPECT_EQ(*expectedOptions, processedOptions);
}

INSTANTIATE_TEST_CASE_P(AllValidCombinations, ProcessCmdLineTest, ::testing::Combine( ValuesIn(helpStrings), ValuesIn(templateStrings), ValuesIn(debugStrings), ValuesIn(quietStrings), ValuesIn(inputPathStrings) ) );
#else
TEST(DummyTest, CombineIsNoteSupportedOnThisPlatform) {ASSERT_TRUE(false);}
#endif // GTEST_HAS_COMBINE

} // end namespace

int main(int argc, char * argv[]) {
::testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
} 
