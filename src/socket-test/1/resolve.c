#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resolve.h"

#define MAX_IP_LEN  16
#define MAX_IP_ADDR_LEN 4
#define MAX_HOST_LEN  1024

char *
resolve(char *ip) {
  static char previous_ip[MAX_IP_LEN] = "";
  static char previous_answer[MAX_HOST_LEN] = "";

  struct hostent *host = NULL;

  char *resolution = NULL;
  char ip_addr[MAX_IP_ADDR_LEN];

  int ip_addr_int[MAX_IP_ADDR_LEN];
  int i;

  if (strcmp(ip, previous_ip) == 0) {
    resolution = previous_answer;
  } else {
    sscanf(ip, "%d.%d.%d.%d", &ip_addr_int[0], &ip_addr_int[1],
      &ip_addr_int[2], &ip_addr_int[3]);

    for (i = 0; i < MAX_IP_ADDR_LEN; i++) {
      ip_addr[i] = (char)ip_addr_int[i];
    }

    host = gethostbyaddr(ip_addr, sizeof(ip_addr), AF_INET);

    if (host) {
      resolution = host->h_name;
    } else {
      resolution = ip;
    }

    strcpy(previous_answer, resolution);
    strcpy(previous_ip, ip);
  }

  return(resolution);
}
