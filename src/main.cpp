#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "options.hpp"
#include "version.h"
#include "latticePair.hpp"

int main(int argc, char * argv[]) {
	ifstream theFile;
	CmdLineOptions parsedOptions(argc, argv);

	cout << "awg20400-lattice-gen " << GIT_VERSION << endl;

	if ( parsedOptions.getDebug() ) {
		cout << "Command invoked as: `" << argv[0];
		for(int arg=1; arg<argc; arg++) {
			cout << ' ' << argv[arg];
		}
		cout << "'\n";
	}

	if ( parsedOptions.getHelp() ) {
		parsedOptions.printUsage();
		return -1;
	}
	
	string inputFilePath("AWGSpec.csv");
	if ( parsedOptions.getInputPath().size() > 0 ) inputFilePath = parsedOptions.getInputPath();

	cout << "\nReading file from: " << inputFilePath << endl;
	theFile.open(inputFilePath.c_str());
	if (!theFile.is_open() || !theFile.good()) {
		cerr << "Problem opening data file!" << endl;
		theFile.close();
		return -1;
	}
	
	string theLine;
	unsigned int linesRead=0;
	latticePair ourPair;	
	while( theFile.good() ) {
		std::getline(theFile, theLine);
		theFile.peek();
		linesRead++;
		ourPair.processLine(theLine);
		if ( parsedOptions.getDebug() ) cout << linesRead << "\t| " << theLine << endl;
	
	}
	cout << "Done!" << endl;	
	
	string master, slave;
	cout << "Getting master string" << endl;
	master = ourPair.masterProgrammingString();
	cout << "Getting slave string" << endl;
	slave  = ourPair.slaveProgrammingString();
		
	cout << "Master Programming String:" << endl;
	cout << master; 
	cout << endl << endl;

	cout << "Slave Programming String:" << endl;
	cout << slave;
		
	return 0;
}

