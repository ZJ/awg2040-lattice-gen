#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "options.hpp"
#include "version.h"

int main(int argc, char * argv[]) {
	ifstream theFile;
	CmdLineOptions parsedOptions(argc, argv);

	cout << "awg20400-lattice-gen " << GIT_VERSION << endl;

	cout << "\nRunning via command: `" << argv[0] << "'" << endl;

	if ( parsedOptions.getHelp() ) {
		parsedOptions.printUsage();
		return -1;
	}
	return 0;
}
