/*
** TCPclient.c -- a TCP socket client 
**  
** Invocation:
**   >./TCPclient
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

 
#define MYPORT 3490    /* the port users will be connecting to */


int main(int argc, char *argv[])
{
  int sfd;                  /* connect on sfd */
  struct sockaddr_in sv_addr;         /* server address information */
  char buf[1024];
  int len;

/**** open the server (TCP) socket */
  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return(-1);
  }
  
/**** build server socket address */        
  bzero((char*) &sv_addr, sizeof(struct sockaddr_in));
  sv_addr.sin_family = AF_INET;         
  sv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  sv_addr.sin_port = htons(MYPORT);

/**** connect to server */
  if (connect(sfd, (struct sockaddr *)&sv_addr, sizeof(struct sockaddr)) == -1) {
    perror("connect");
    close(sfd);
    return(-1);
  }

  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    len = strlen(buf);
    if (send(sfd, buf, len, 0) != len) {
      perror("send");
      close(sfd);
      return(-1);
    }
  }
  close(sfd);

  return 0;
} 
