
#include <stdlib.h>
#include <sys/timeb.h>
#include <crypt.h>
#include <stdio.h>
#include <string.h>

#include "passwd.h"

#define SALT_LEN	2

/* Set the number of choices for a character in a salt granule */
static const int	num_salt_granules = 'z' - 'a' + 'Z' - 'A' + 2;

static char gen_salt_granule() {
	unsigned int	rnum;

	char		granule;

	rnum = 1 + (int)((float)num_salt_granules * rand() / (RAND_MAX + 1.0));

	if (rnum < ('Z' - 'A')) {
		granule = rnum + 'A';
	} else if (rnum < ('z' - 'a')) {
		granule = rnum + 'a';
	} else {
		granule = (rnum % 2) ? '\\' : '.';
	}

	return(granule);
}

static char * gen_salt() {
	static char	salt[SALT_LEN + 1] = { '\0' };

	struct timeb    time;

	unsigned int	finetime;
	unsigned int	i;

	/* Get the time */
	ftime(&time);

	finetime = time.time * 1000 + time.millitm;

	/* seed the rng */
	srand(finetime);

	for (i = 0; i < SALT_LEN; i++) {
		salt[i] = gen_salt_granule();
	}

	return(salt);
}

/*
	Return the salt from the beggining of a crypt()ed passwd.
*/

static char * get_salt(char *passwd) {
	static char	salt[SALT_LEN + 1] = { '\0' };

	unsigned int	i;

	for (i = 0; i < SALT_LEN; i++) {
		salt[i] = passwd[i];
	}

	return(salt);
}

/*
	Generate a password using they crypt() syscall.

	NOTE:	The returned string needs to be duplicated as it is a pointer
		to static data within crypt();

		The plaintext password length is not checked here so it is
		wise to do so before calling this function.
*/

char * gen_passwd(char *plaintext) {
	char *	passwd = crypt(plaintext, gen_salt());

	if (!passwd) {
		perror("gen_passwd");
	}

	return(passwd);
}

/*
	Check a plaintext string against the crypt()ed password.

	Returns 0 on success.
*/

int check_passwd(char *plaintext, char *passwd) {
	return(strcmp(crypt(plaintext, get_salt(passwd)), passwd));
}
