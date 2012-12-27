#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include "resolve.h"

#define LINELEN 4096

int main(int argc, char *argv[]) {
  char ip[16] = "";
  char stuff[LINELEN] = "";
  char *host = NULL;

  int   rtcode = 0;

  while (scanf("%s%[^\n]", ip, stuff) != EOF) {
    host = resolve(ip);

    if (host) {
      printf("%s%s\n", host, stuff);
    }
  }

  return(rtcode);
}
