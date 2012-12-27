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
    list <string> words;
    RandomNumberGenerator rng;
};
