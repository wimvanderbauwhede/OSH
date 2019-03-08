/*
 * UDPserver.c -- a UDP socket server
 *
 * Invocation:
 *   >./UDPserver >out.txt&
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#define MYPORT 3490		/* the port to which the server is bound */

int main(int argc, char *argv[])
{
  int sfd;			/* the socket for communication */
  int len, n;
  struct sockaddr_in sv_addr;	/* my s(erver) address data */
  struct sockaddr_in c_addr;	/* c(lient) address data */
  char buf[1024];

  memset(&sv_addr, 0, sizeof(sv_addr));	/* my address info */
  sv_addr.sin_family = AF_INET;
  sv_addr.sin_port = htons(MYPORT);
  sv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

/**** open the UDP socket */
  if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return(-1);
  }

/**** bind to my local port number */
  if ((bind(sfd, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) < 0)) {
    perror("bind");
    return(-1);
  }

/**** receive each message on the socket, printing on stdout */
  while (1) {
    memset(&c_addr, 0, sizeof(c_addr));
    len = sizeof (c_addr);
    n = recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr *)&c_addr, &len);
    if (n < 0) {
      perror("recvfrom");
      return(-1);
    }
    fputs(buf, stdout);
    fflush(stdout);
  }
}
