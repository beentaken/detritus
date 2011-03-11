#include "wrappers.h"

#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>

/* Function to handle the death of children */
void child_reaper(int signum) {
	if (signum == SIGCHLD) {
		/* Wait for dead children */
		if (waitpid(-1, NULL, WNOHANG) == -1) {
			ERROR;
		} else {
			/* We have to register this function as the signal handler again. */
			SANITY_CHECK(signal(SIGCHLD, child_reaper) == SIG_ERR);
		}
	}
}

int rand_ordinate(void) {
	int	rtcode = (int)((float)GRID_SZ * rand() / (RAND_MAX));

	return rtcode;
}

/* Code to handle each client */
int handle_client(int fd) {
	int	buff;

	pos	server, p_torpedo, s_torpedo;

	srand(time(NULL));

	// Give the player a position
	buff = rand_ordinate();
	Write(fd, &buff, sizeof(int));
	buff = rand_ordinate();
	Write(fd, &buff, sizeof(int));

	// Give the server a position
	server.x = rand_ordinate();
	server.y = rand_ordinate();

	p_torpedo.x = p_torpedo.y = -1;
	s_torpedo.x = s_torpedo.y = -1;

	while(Read(fd, &buff, sizeof(int))) {
		switch (buff) {
			case	FIRE:	Read(fd, &p_torpedo, sizeof(pos));
					if (p_torpedo.x == server.x && p_torpedo.y == server.y) {
						buff = HIT;
					} else {
						buff = MISS;
					}

					break;

			case	SURRENDER:	return(0);

			case	MOVE:	buff = ACK;	
		}

		Write(fd, &buff, sizeof(int));

		if (buff == HIT) {
			// Move in a random direction
			switch ((int)(4.0 * rand() / RAND_MAX)) {
				case	0:	// Move up
						server.y = (server.y + GRID_SZ - 1) % GRID_SZ;
						break;

				case	1:	// Move down
						server.y = (server.y + 1) % GRID_SZ;
						break;

				case	2:	// Move left
						server.x = (server.x + GRID_SZ - 1) % GRID_SZ;
						break;

				case	3:	// Move right
						server.x = (server.x + 1) % GRID_SZ;
						break;
			}

			buff = MOVE;

			Write(fd, &buff, sizeof(int));
			Read(fd, &buff, sizeof(int));
		} else {
			// Fire at a random co-ordinate
			while ((s_torpedo.x = rand_ordinate()) == server.x);
			while ((s_torpedo.y = rand_ordinate()) == server.y);

			buff = FIRE;

			Write(fd, &buff, sizeof(int));
			Write(fd, &s_torpedo, sizeof(pos));

			Read(fd, &buff, sizeof(int));

			if (buff == HIT) {
				fprintf(stderr, "Yay, I got 'em!\n");
			}
		}
	}

	return(0);
}

int main(int argc, char **argv) {
	char	*buff;

	pid_t	pid;

	int	sock;
	int	fd;
	int	rtcode = 0;
	int	one = 1;

	unsigned int	addrlen = sizeof(struct sockaddr_in);
	unsigned int	port = 40642;

	sockaddr_in	addr;

	common_init(argc, argv, port, addr.sin_addr.s_addr);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	sock = Socket(PF_INET, SOCK_STREAM, 0);

	SANITY_CHECK(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)));

	Bind(sock, (struct sockaddr *)&addr, addrlen);
	Listen(sock, SOMAXCONN);

	/* Register the signal handler for SIGCHLD */
	SANITY_CHECK(signal(SIGCHLD, child_reaper) == SIG_ERR);

	while ((fd = Accept(sock, (struct sockaddr *)&addr, &addrlen))) {
		pid = fork();

		if (pid == -1) {
			ERROR;
		} else if (pid == 0) {

			handle_client(fd);

			if (shutdown(fd, 2) == -1) {
				ERROR;
			}

			close(fd);
			exit(0);
		} else {
			/* The file descriptor has to be closed in the parent as well */
			close(fd);
			buff = inet_ntoa(addr.sin_addr);
			printf("Spawned child %d to handle connection from %s\n", pid, buff);
		}
	}

	return(rtcode);
}
