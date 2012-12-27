#ifndef _RandomNumberGenerator_h
#define _RandomNumberGenerator_h

#include <stdlib.h>

class RandomNumberGenerator {
  public:
    RandomNumberGenerator();
    ~RandomNumberGenerator();

    int next(int max);

  private:
    static const int state_len = 256;
    static const int spawn_pause = 1000000;

    char state[state_len];
};

#endif
