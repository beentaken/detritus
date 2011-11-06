#include "version.h"

Version::Version(int maj, int min, int pl) {
    setversion(maj, min, pl);
}

void Version::setversion(int maj, int min, int pl) {
    major = maj;
    minor = min;
    patchlevel = pl;
}

istream & operator >> (istream &input, Version &version) {
    int maj, min, pl;
    char temp;

    input >> maj >> temp >> min >> temp >> pl;
    version.setversion(maj, min, pl);

    return(input);
}

ostream & operator << (ostream &output, const Version &version) {
    const char  dot = '.';

    output << version.major << dot << version.minor << dot << version.patchlevel;

    return(output);
}
