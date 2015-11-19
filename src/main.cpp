#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#include "options.hpp"
#include "latticePair.hpp"

// Auto-generated sources
#include "version.h"
#include "sampleFile.h"

#define MASTER_FILE	"Master.AWG.txt"
#define SLAVE_FILE	"Slave.AWG.txt"

int main(int argc, char * argv[]) {
	ifstream theFile;
	ofstream outputFiles;

	CmdLineOptions parsedOptions(argc, argv);

	if ( !parsedOptions.getQuiet() ) cout << "awg20400-lattice-gen " << GIT_VERSION << endl;

	if ( parsedOptions.getDebug() ) {
		cout << "Command invoked as: " << argv[0];
		for(int arg=1; arg<argc; arg++) {
			cout << ' ' << argv[arg];
		}
		cout << "\n";
		cout << "with options parsed as " << parsedOptions << endl;
	}

	if ( parsedOptions.getHelp() ) {
		parsedOptions.printUsage();
		return -1;
	}

	if ( parsedOptions.getTemplate() ) {
		outputFiles.open(SAMPLE_FILE_NAME, ios::out | ios::trunc);
		if ( outputFiles.is_open() && outputFiles.good() ) {
			outputFiles << sampleFile;
			outputFiles.close();
		} else {
			cerr << "Error saving template file at " << SAMPLE_FILE_NAME << endl;
			return -1;
		}
		if (! parsedOptions.getQuiet()) cout << "Template saved to " << SAMPLE_FILE_NAME << endl;
		return 0;
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
		if ( !ourPair.processLine(theLine, true) ) {
			syntaxOK = false;
			cerr << "\tSyntax error on line " << linesRead << endl;
		}
	}
	if ( !syntaxOK ) {
		cerr << "Problems reading file, correct it and try again later" << endl;
		return -1;
	}
	if ( !parsedOptions.getQuiet() ) cout << "Done reading file." << endl;

	outputFiles.open(MASTER_FILE, ios::out | ios::binary | ios::trunc);
	if ( outputFiles.is_open() && outputFiles.good() ) {
		outputFiles << ourPair.masterProgrammingString();
		outputFiles.close();
		if (! parsedOptions.getQuiet()) cout << "Commands saved to " << MASTER_FILE << endl;
	}
	outputFiles.open("Slave.AWG.txt", ios::out | ios::binary | ios::trunc);
	if ( outputFiles.is_open() && outputFiles.good() ) {
		outputFiles << ourPair.slaveProgrammingString();
		outputFiles.close();
		if (! parsedOptions.getQuiet()) cout << "Commands saved to " << SLAVE_FILE << endl;
	}

	return 0;
}

