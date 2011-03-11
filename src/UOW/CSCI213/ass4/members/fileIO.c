#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "member.h"

static const char Filename[] = "members.dat";
static const char Lockfile[] = ".members.dat.lock";

/*
	Lock the data file and open it.

	Returns the file descriptor as an integer. Or a negative number
	depending on the error:

	-1	open() call failed.
	-2	lock failed.
*/

static int locknload() {
	const	unsigned long	sleep_time = 50;
	const	unsigned long	max_sleep_time = sleep_time * 100;

	unsigned long	cumulative_sleep_time = 0;

	int	fd;

	/* spinlock */
	do {
		fd = link(Filename, Lockfile);

		if (fd) {
			usleep(sleep_time);
			cumulative_sleep_time += sleep_time;
		}
	} while (fd && cumulative_sleep_time < max_sleep_time);

	/*
		To avoid a race condition, assume that the file will
		always exist.
	*/
	if (cumulative_sleep_time < max_sleep_time) {
		fd = open(Filename, O_RDWR | O_SYNC);

		if (fd == -1) {
			perror("locknload()");
		}
	} else {
		fprintf(stderr, "locknload(): Couldn't obtain exclusive lock."
			" Check for stale lockfile.\n");

		fd = -2;
	}

	return(fd);
}

/*
	Close the file descriptor and remove the lock.

	Returns zero on success.
*/

static int unload(int fd) {
	int	rtcode = 0;

	rtcode = close(fd);

	if (rtcode) {
		perror("unload()");
	} else {
		/* Remove the lock file */
		rtcode = unlink(Lockfile);

		if (rtcode) {
			perror("unload()");
		}
	}

	return (rtcode);
}

enum io_operation { retrieve, store };

/*
	Allocate a member number.

	Returns the member number on success.

	Returns a negative error code depending on the error:

	-1	locknload() failed.
*/

int allocate_number(void) {
	int	fd = -1;
	int	rtcode = 0;

	Member	member;

	fd = locknload();

	if (fd >= 0) {
		lseek(fd, 0, 0);

		while(read(fd, (char *)&member, sizeof(Member)) == sizeof(Member)) {
			if (member.number < 0) {
				break;
			} else {
				rtcode++;
			}
		}

		unload(fd);
	} else {
		rtcode = -1;
	}

	return(rtcode);
}

/*
	The real brains behind the I/O.

	The resulting member is stored or retrieved using the Member pointer.

	Returns zero on success.

	Returns a negative value depending on the error:

	-1	lseek() failed.
	-2	read()/write() failed.
	-3	Requested member doesn't exist.
	-4	unload() call failed after a successful operation.
	-5	lockandload() call failed.
	-6	Unknown I/O Operation.
*/

static int io(Member *member, int member_number, int operation) {
	int	fd = -1;
	int	temp = 0;
	int	rtcode = 0;

	fd = locknload();

	/* Sanity check */
	if (fd >= 0) {
		temp = lseek(fd, 0, SEEK_END);

		/* Check to see if the member is in the file */
		if (operation == store || member_number < (temp / sizeof(Member))) {
			temp = lseek(fd, member_number * sizeof(Member),
				SEEK_SET);

			if (temp == -1) {
				perror("io()");
				rtcode = -1;
			} else { 
				if (operation == retrieve) {
					temp = read(fd, (char *)member, sizeof(Member));
				} else if (operation == store) {
					temp = write(fd, (char *)member, sizeof(Member));
				} else {
					fprintf(stderr, "Unknown I/O Operation.\n");
					rtcode = -6;
					temp = -1;
				}

				if (temp != -1 && temp != sizeof(Member)) {
					perror("io()");
					rtcode = -2;
				}
			}
		} else {
			fprintf(stderr, "io(): Member #%d doesn't exist.\n",
				member->number);
			rtcode = -3;
		}

		fd = unload(fd);

		/* If there were no previous errors and unload() failed */
		if (fd && !rtcode) {
			rtcode = -4;
		}
	} else {
		rtcode = -5;
	}

	return(rtcode);
}

/*
	Gets a Member from the data file. Uses the member number stored in the
	Member structure pointed to by the argument.
*/

int get(Member *member, int member_number) {
	return(io(member, member_number, retrieve));
}

/*
	Write the Member to the file.
*/

int put(Member *member, int member_number) {
	return(io(member, member_number, store));
}
