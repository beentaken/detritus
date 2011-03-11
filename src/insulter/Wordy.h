// $Id: Wordy.h 2 2007-07-19 13:00:48Z pdezwart $

#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include "RandomNumberGenerator.h"

using namespace std;

class Wordy {
	public:
		Wordy(char *file_name);
		~Wordy();

		string get();

	private:
		list <string>	words;
		RandomNumberGenerator	rng;
};
