#include "Wordy.h"

int main(int argc, char **argv) {
    Wordy   prefix("prefix.txt");
    Wordy   fix("fix.txt");
    Wordy   suffix("suffix.txt");

    cout << "Thou " << prefix.get() << ' ' << fix.get() << ' ' << suffix.get() << '.' << endl;

    return(0);
}
