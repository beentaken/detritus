#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "resolve.h"

/* Function to handle the death of children */
void child_reaper(int signum) {
    if (signum == SIGCHLD) {
        /* Wait for dead children */
        if (waitpid(-1, NULL, WNOHANG) == -1) {
            perror("child_reaper:waitpid");
        } else {
            /* We have to register this function as the signal handler again. */
            if (signal(SIGCHLD, child_reaper) == SIG_ERR) {
                perror("main:signal:");
                exit(1);
            }
        }
    }
}

int handle_client(int fd) {
    char    buff;

    while (read(fd, &buff, 1)) {
        if(!write(fd, &buff, 1)) {
            perror("handle_client:write");
            return(-1);
        }
    }

    return(0);
}

int main(int argc, char **argv) {
    char    *buff;

    pid_t   pid;

    int sock;
    int fd;
    int rtcode = 1;

    unsigned int    addrlen = sizeof(struct sockaddr_in);
    unsigned int    port = 31337;

    struct sockaddr_in *addr = NULL;

    /* The port to listen on can be set on the command line */
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    addr = malloc(sizeof(struct sockaddr_in));

    if (!addr) {
        perror("main:malloc");
    } else {
        addr->sin_family = AF_INET;
        addr->sin_port = htons(port);

        if (!inet_aton("0.0.0.0", &addr->sin_addr.s_addr)) {
            perror("main:inet_aton");
        } else {

            sock = socket(PF_INET, SOCK_STREAM, 0);

            if (sock == -1) {
                perror("main:socket");
            } else {
                if (bind(sock, (struct sockaddr *)addr, addrlen)) {
                    perror("main:bind");
                } else {
                    if (listen(sock, SOMAXCONN)) {
                        perror("main:listen");
                    } else {
                        /* Register the signal handler for SIGCHLD */
                        if (signal(SIGCHLD, child_reaper) == SIG_ERR) {
                            perror("main:signal:");
                            exit(1);
                        }

                        while ((fd = accept(sock, (struct sockaddr *)addr, &addrlen))) {
                            if (fd == -1) {
                                perror("main:accept");
                            } else {
                                pid = fork();

                                if (pid == -1) {
                                    perror("main:fork");
                                } else if (pid == 0) {

                                    handle_client(fd);

                                    if (shutdown(fd, 2) == -1) {
                                        perror("main:shutdown");
                                    }

                                    close(fd);
                                    exit(0);
                                } else {
                                    /* The file descriptor has to be closed in the parent as well */
                                    close(fd);
                                    buff = inet_ntoa(addr->sin_addr);
                                    printf("Spawned child %d to handle connection from %s (%s).\n", pid, resolve(buff), buff);
                                }
                            }
                        }

                        rtcode = 0;
                    }
                }
            }
        }
    }

    return(rtcode);
}
