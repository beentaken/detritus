#include "wrappers.h"

int Socket(int Domain, int Type, int Protocol) {
	int	Sockfd;

	SANITY_CHECK((Sockfd = socket(Domain, Type, Protocol)) < 0);

	return Sockfd;
}

void Bind(int Sockfd, sockaddr *MyAddr, int AddrLen) {
	SANITY_CHECK(bind(Sockfd, MyAddr, AddrLen) < 0);
}

void Listen(int Sockfd, int Backlog) {
	SANITY_CHECK(listen(Sockfd, Backlog) < 0);
}

void Connect(int Sockfd, sockaddr *ServAddr, int Size){
	SANITY_CHECK(connect(Sockfd, ServAddr, Size) < 0);
}

int Accept(int Sockfd, sockaddr *Addr, socklen_t *AddrLen) {
	int	Connfd;

	while ((Connfd = accept(Sockfd, Addr, AddrLen)) < 0) {
#ifdef EPROTO
		SANITY_CHECK((errno != EPROTO) && (errno != ECONNABORTED) && (errno != EINTR));
#else
		SANITY_CHECK((errno != ECONNABORTED) && (errno != EINTR));
#endif
	}

	return Connfd;
}

struct hostent *Gethostbyname(char *ServHost) {
	// get the address of the host
	hostent	*HostPtr;

	SANITY_CHECK((HostPtr = gethostbyname(ServHost)) == NULL);

	return HostPtr;
}

ssize_t Read(int fd, void *buf, size_t count) {
	assert(buf);

	SANITY_CHECK(read(fd, buf, count) != (ssize_t) count);

	return count;
}

ssize_t Write(int fd, const void *buf, size_t count) {
	assert(buf);

	SANITY_CHECK(write(fd, buf, count) != (ssize_t) count);

	return count;
}

void common_init(int argc, char **argv, unsigned int &port, u_int32_t &addr) {
	struct hostent *foo = NULL;

	if (argc > 1) {
		port = atoi(argv[1]);

		if (argc > 2) {
			foo = Gethostbyname(argv[2]);
			memcpy(&addr, foo->h_addr, foo->h_length);
		} else {
			addr = htonl(INADDR_ANY);
		}
	}
}
