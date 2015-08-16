#include <iostream>
using namespace std;

#include "options.hpp"

int CmdLineOptions::processCmdLine(int argc, char * argv[]) {

return 0;
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
