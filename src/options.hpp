#ifndef AWG2040_OPTIONS_HPP_20150815
#define AWG2040_OPTIONS_HPP_20150815

#include <string>
#include <iostream>

class CmdLineOptions {
public:
// Constructors
CmdLineOptions();
CmdLineOptions(bool helpFlag, bool templateFlag, bool debugFlag, bool quietFlag, std::string inputPath);

// Destructors

// Accessors etc.
void setHelp(bool value);
void setTemplate(bool value);
void setDebug(bool value);
void setQuiet(bool value);
void setInput(std::string newPath);

//int processCmdLine(int argc, char * argv[]);
void print(std::ostream& out) const;
private:
bool myHelpFlag, myTemplateFlag, myDebugFlag, myQuietFlag;
std::string myInputPath;	
};

// Inlined Constructors
inline CmdLineOptions::CmdLineOptions() {
 myHelpFlag = false;
myTemplateFlag = false;
myDebugFlag = false;
myQuietFlag = false;
myInputPath = "";
}

inline CmdLineOptions::CmdLineOptions(bool helpFlag, bool templateFlag, bool debugFlag, bool quietFlag, std::string inputPath){
myHelpFlag = helpFlag;
myTemplateFlag = templateFlag;
myDebugFlag = debugFlag;
myQuietFlag = quietFlag;
myInputPath = inputPath;
}

// Inlined Accessors
inline void CmdLineOptions::setHelp(bool value) {
	myHelpFlag = value;
}

inline void CmdLineOptions::setTemplate(bool value){
myTemplateFlag = value;
}

inline void CmdLineOptions::setDebug(bool value){
myDebugFlag = value;
}

inline void CmdLineOptions::setQuiet(bool value){
myQuietFlag = value;
}

inline void CmdLineOptions::setInput(std::string newPath) {
myInputPath = newPath;
}

inline void CmdLineOptions::print(std::ostream &out) const {
out << "Help:    \t" << myHelpFlag  << ";\tTemplate:\t" << myTemplateFlag << ";" << endl;
out << "Debug:   \t" << myDebugFlag << ";\tQuiet:   \t" << myQuietFlag    << ";" << endl;
out << "Input path:\t" << myInputPath << endl;
}

#endif // AWG2040_OPTIONS_HPP_20150815
