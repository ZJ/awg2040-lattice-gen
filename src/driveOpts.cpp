#include <iostream>
using namespace std;

#include "../Config.h"
#include "options.hpp"

int main (int argc, char * argv[]) {
	cout << "In main!" << endl;
	cout << "This is awg2040-lattice-gen v" << awgLS_VERSION_MAJOR << "." << awgLS_VERSION_MINOR << endl;
	CmdLineOptions opts;
	opts.print(cout);

	return 0;
}
