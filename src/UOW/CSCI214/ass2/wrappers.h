#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/signal.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#ifdef __ASSERT_FUNCTION
	#define ERROR	perror(__ASSERT_FUNCTION)
#else
	#define ERROR	perror("")
#endif

#define FERROR	ERROR; exit(1)

#define SANITY_CHECK(x)	if(x) { FERROR; } 

// Handle inconsistancies between Unix flavours
typedef uint32_t u_int32_t;

const int SERV_TCP_PORT = 8000; // server's default port number
const int MAX_SIZE = 80;        // send/receive buffer size
const int GRID_SZ = 7;		// size of the playing grid

int Socket(int Domain, int Type, int Protocol);

void Bind(int Sockfd, sockaddr *MyAddr, int AddrLen);
void Listen(int Sockfd, int Backlog);
void Connect(int Sockfd, sockaddr *ServAddr, int Size);

int Accept(int Sockfd, sockaddr *Addr, socklen_t *AddrLen);
hostent *Gethostbyname(char *ServHost);

ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);

void common_init(int argc, char **argv, unsigned int &port, u_int32_t &addr);

enum	COMMANDS {
	MOVE,
	FIRE,
	SURRENDER
};

enum	RESPONSES {
	ACK,
	HIT,
	MISS
};

typedef struct _pos {
	int     x;
	int     y;
} pos;

#endif
