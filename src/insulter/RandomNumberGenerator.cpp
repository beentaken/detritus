
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>

#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator() {
	unsigned int	finetime;

	struct timeb    time;
	struct timespec	req;

	req.tv_sec = 0;
	req.tv_nsec = spawn_pause;

	// Pause to avoid seed collision
	nanosleep(&req, NULL);

	// Get the time
	ftime(&time);

	finetime = time.time * 1000 + time.millitm;

	// Peturb the RNG
	initstate(finetime, state, state_len);
}

RandomNumberGenerator::~RandomNumberGenerator() {
}

int RandomNumberGenerator::next(int max) {
	setstate(state);

	return(1 + (unsigned int)((float)max * random() / (RAND_MAX + 1.0)));
}
