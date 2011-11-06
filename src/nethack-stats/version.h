#ifndef __VERSION_H
#define __VERSION_H

#include <iostream>

class Version {

    // iostream friends
    friend istream & operator >> (istream &input, Version &version);
    friend ostream & operator << (ostream &output, const Version &version);

    public:
        // Default constructor
        Version(int maj = 0, int min = 0, int pl = 0);
        void setversion(int maj, int min, int pl);

    private:
        int major;
        int minor;
        int patchlevel;
};

#endif
