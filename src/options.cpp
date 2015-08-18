#include <iostream>
using namespace std;

#include "options.hpp"
#include "../optionparser/optionparser.h"


int CmdLineOptions::processCmdLine(int argc, char * argv[]) {
enum OptionIndex {UNKNOWN,HELP, TEMPLATE, DEBUG, QUIET, INPUTFILE};
enum OptionType {DISABLE, ENABLE, OTHER};
const option::Descriptor usage[] = {
// {index, type, short, long, check_arg, helptext}
{ UNKNOWN, OTHER, "", "", option::Arg::None, "<PROGRAM>\n\nUsage: <PROGRAM> words/options" },
{0,0,"","",option::Arg::None,0},
{ HELP, OTHER, "h", "help", option::Arg::None, "-h\v--help\tPrints out this help/usage message" },
{ TEMPLATE, OTHER, "t", "template", option::Arg::None, "-t\v--template\tSaves a template input file to \'template.txt\'" },
{ DEBUG, OTHER, "d", "debug", option::Arg::None, "-d\v--debug\tPrint debugging information to the screen"},
{ QUIET, OTHER, "q", "quiet", option::Arg::None, "-q\v-quiet\tSuppresses normal output.  Does not affect --debug" },
{ INPUTFILE, OTHER, "", "input-path", option::Arg::Optional, "-i,\v--input-file  \tProvides an alternate path for the frequency specification file." },
{ INPUTFILE, OTHER, "", "i", option::Arg::Optional, ""},
{0,0,0,0,0,0}
};

argc -= (argc>0); argv+=(argc>0); // skip argv[0] if present, it is the program name
option::Stats stats(usage, argc, argv);
option::Option options[stats.options_max], buffer[stats.buffer_max];
option::Parser parse(usage, argc, argv, options, buffer, 0, true);

if (parse.error()) return OPT_PARSE_ERR;

if (options[HELP]) myHelpFlag = true;
if (options[TEMPLATE]) myTemplateFlag=true;
if (options[DEBUG]) myDebugFlag=true;
if (options[QUIET]) myQuietFlag=true;
if (options[INPUTFILE]) {
const char * fileStr = options[INPUTFILE].last()->arg;
if (NULL != fileStr) {
myInputPath = string(fileStr);
} else {
myInputPath = "";
}
}

if( ( parse.nonOptionsCount() > 0) || options[UNKNOWN]) {
if( options[UNKNOWN] ) {
cerr << "Encountered unknown options:\n";
for(option::Option* opt = options[UNKNOWN]; opt; opt = opt->next()) cerr << "\t" << opt->name << "\n";
}
if (parse.nonOptionsCount() > 0) {
cerr << "Encountered non-options:\n";
for(int i=0; i<parse.nonOptionsCount(); i++) cerr << "\t\"" << parse.nonOption(i) << "\"\n";
}
return OPT_PARSE_BAD_OPTS;
}
return OPT_PARSE_OK;
}

bool CmdLineOptions::operator ==(const CmdLineOptions& d) const {
return (myHelpFlag == d.myHelpFlag) && (myTemplateFlag == d.myTemplateFlag) && (myDebugFlag == d.myDebugFlag) && (myQuietFlag == d.myQuietFlag) && (myInputPath == d.myInputPath);
}

ostream& operator<<(ostream& os, const CmdLineOptions& opts) {
ios_base::fmtflags oldFlags = os.flags();
os << boolalpha << "Help Flag: " << opts.myHelpFlag << ", Template Flag: " << opts.myTemplateFlag;
os << ", Debug Flag: " << opts.myDebugFlag << ", Quiet Flag: " << opts.myQuietFlag;
os << ", Input Path: " << ((opts.myInputPath.size() > 0) ? opts.myInputPath : "<unset>");
os.flags(oldFlags);

return os;
}
