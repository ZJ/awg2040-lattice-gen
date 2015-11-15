#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#include "options.hpp"
#include "version.h"
#include "latticePair.hpp"

int main(int argc, char * argv[]) {
	ifstream theFile;
	CmdLineOptions parsedOptions(argc, argv);

	if ( !parsedOptions.getQuiet() ) cout << "awg20400-lattice-gen " << GIT_VERSION << endl;

	if ( parsedOptions.getDebug() ) {
		cout << "Command invoked as: " << argv[0];
		for(int arg=1; arg<argc; arg++) {
			cout << ' ' << argv[arg];
		}
		cout << "\n";
	}

	if ( parsedOptions.getHelp() ) {
		parsedOptions.printUsage();
		return -1;
	}
	
	string inputFilePath("AWGSpec.csv");
	if ( parsedOptions.getInputPath().size() > 0 ) inputFilePath = parsedOptions.getInputPath();

	if ( !parsedOptions.getQuiet() ) cout << "\nReading file from: \"" << inputFilePath << "\"\n";
	theFile.open(inputFilePath.c_str());
	if (!theFile.is_open() || !theFile.good()) {
		cerr << "Problem opening data file!" << endl;
		theFile.close();
		return -1;
	}
	
	string theLine;
	unsigned int linesRead(0);
	latticePair ourPair;	
	bool syntaxOK(true);
	while( theFile.good() ) {
		std::getline(theFile, theLine);
		theFile.peek();
		linesRead++;
		if ( parsedOptions.getDebug() ) cout << setw(4) << linesRead << " | " << theLine << "\n";
		if ( !ourPair.processLine(theLine) ) {
			syntaxOK = false;
			cerr << "\tFile syntax error on line" << linesRead << endl;
		}
	}
	if ( !syntaxOK ) {
		cerr << "Problems reading file, correct it and try again later" << endl;
		return -1;
	}
	if ( !parsedOptions.getQuiet() ) cout << "Done reading file." << endl;
	
	return 0;
}

