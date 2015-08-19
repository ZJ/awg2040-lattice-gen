#include <string>
#include <cstring>
#include <tr1/tuple>
#include <algorithm>
using namespace std;

#include <gtest/gtest.h>
#include "../src/options.hpp"

namespace {
#ifdef GTEST_HAS_COMBINE
// Setup the command line processing
// Do it with tuples so we get all possible combos easily

using ::testing::ValuesIn;
using ::testing::Values;

#define N_HELP 3
string helpStrings[N_HELP] = {"-h", "--help", ""};
#define N_TEMP 3
string templateStrings[N_TEMP] = {"-t", "--template", ""};
#define N_DEBUG 3
string debugStrings[N_DEBUG] = {"-d", "--debug", ""};
#define N_QUIET 3
string quietStrings[N_QUIET] = {"-q", "--quiet", ""};
#define N_INPUT 3
string inputPathStrings[N_INPUT] = {"-i", "--input-path", ""};
char dummyProg[] = "PretendProg";
string dummyPath = "PretendInputFile";

class ProcessCmdLineTest : public ::testing::TestWithParam< tr1::tuple<string, string, string, string, string> > {
protected:
CmdLineOptions processedOptions;
CmdLineOptions* expectedOptions;
int argc;
char * argv[8];
bool inputsValid;

virtual void SetUp() {
  string helpIn, tempIn, debugIn, quietIn, inputIn;
  tr1::tie(helpIn,tempIn,debugIn,quietIn,inputIn) = GetParam();
  expectedOptions = new CmdLineOptions(!helpIn.empty(), !tempIn.empty(), !debugIn.empty(), !quietIn.empty(), inputIn.empty() ? "" : "PretendInputFile");
  // Int args
  inputsValid=true;
  argv[0] = dummyProg;
  argc=1; 
  // Fill argv appropriately
  if (!helpIn.empty()) {
	inputsValid = inputsValid && ( find(helpStrings, helpStrings+N_HELP, helpIn) != (helpStrings+N_HELP));
        argv[argc] = new char [helpIn.length()+1];
	strcpy(argv[argc], helpIn.c_str());
        argc++;
  } 
  if (!tempIn.empty()) {
	inputsValid = inputsValid && ( find(templateStrings, templateStrings+N_TEMP, tempIn) != (templateStrings+N_TEMP));
        argv[argc] = new char [tempIn.length()+1];
	strcpy(argv[argc], tempIn.c_str());
        argc++;
  } 
  if (!debugIn.empty()) {
	inputsValid = inputsValid && ( find(debugStrings, debugStrings+N_DEBUG, debugIn) != (debugStrings+N_DEBUG));
        argv[argc] = new char [debugIn.length()+1];
	strcpy(argv[argc], debugIn.c_str());
        argc++;
  } 
  if (!quietIn.empty()) {
	inputsValid = inputsValid && ( find(quietStrings, quietStrings+N_QUIET, quietIn) != (quietStrings+N_QUIET));
        argv[argc] = new char [quietIn.length()+1];
	strcpy(argv[argc], quietIn.c_str());
        argc++;
  } 
  if (!inputIn.empty()) {
	inputsValid = inputsValid && ( find(inputPathStrings, inputPathStrings+N_INPUT, inputIn) != (inputPathStrings+N_INPUT));
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
if (inputsValid) {
  ASSERT_EQ(OPT_PARSE_OK, processedOptions.processCmdLine(argc, argv));
  EXPECT_EQ(*expectedOptions, processedOptions);
} else {
  ASSERT_EQ(OPT_PARSE_BAD_OPTS, processedOptions.processCmdLine(argc, argv));
}
}

INSTANTIATE_TEST_CASE_P(AllValidCombinations, ProcessCmdLineTest, ::testing::Combine( ValuesIn(helpStrings), ValuesIn(templateStrings), ValuesIn(debugStrings), ValuesIn(quietStrings), ValuesIn(inputPathStrings) ) );

string badArgs[2]={"-x","NotAnOption"};
string smallHelp[2]={"","-h"};
INSTANTIATE_TEST_CASE_P(InvalidEntries, ProcessCmdLineTest, ::testing::Combine( ValuesIn(smallHelp), ValuesIn(badArgs), Values(string("")), Values(string("")), Values(string("")) ) );

#else
TEST(DummyTest, CombineIsNoteSupportedOnThisPlatform) {ASSERT_TRUE(false);}
#endif // GTEST_HAS_COMBINE

} // end namespace

int main(int argc, char * argv[]) {
::testing::InitGoogleTest(&argc, argv);

return RUN_ALL_TESTS();
} 
