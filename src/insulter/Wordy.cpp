#include "Wordy.h"

Wordy::Wordy(char *file_name) {
    ifstream    ifs;
    string      word;

    ifs.open(file_name);

    while(ifs.good()) {
        ifs >> word;

        words.insert(words.begin(), word);
    }
}

Wordy::~Wordy() {
}

string Wordy::get() {
    list <string> ::iterator    i = words.begin();

    int next = rng.next(words.size() - 1);

    while (next--) {
        i++;
    }

    return(*i);
}
