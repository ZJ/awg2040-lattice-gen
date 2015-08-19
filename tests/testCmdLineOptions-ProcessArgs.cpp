#include <string>
#include <cstring>
#include <tr1/tuple>
using namespace std;

#include <gtest/gtest.h>
#include "../src/options.hpp"

namespace {
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
