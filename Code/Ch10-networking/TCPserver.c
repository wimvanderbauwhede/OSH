/*
** TCPserver.c -- a TCP socket server 
**  
** Invocation:
**   >./TCPserver&
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

 
#define MYPORT 3490    /* the port users will be connecting to */


int main(int argc, char *argv[])
{
  int sfd, cfd;                  /* listen on sfd, new connections on cfd */
  struct sockaddr_in my_addr;         /* my address information */
  struct sockaddr_in their_addr;      /* client address information */
  int sin_size;
  int yes=1;
  int n;
  char buf[1024];

/**** open the server (TCP) socket */
  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return(-1);
  }
  
/**** set the Reuse-Socket-Address option */  
  if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int))==-1) {
    perror("setsockopt");
    close(sfd);
    return(-1);
  }
  
/**** build server socket address */        
  bzero((char*) &my_addr, sizeof(struct sockaddr_in));
  my_addr.sin_family = AF_INET;         
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  my_addr.sin_port = htons(MYPORT);

/**** bind server socket to the local address */
  if (bind(sfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
    perror("bind");
    close(sfd);
    return(-1);
  }

/**** create queue (1 only) for client connection requests */
  if (listen(sfd, 1) == -1) {
    perror("listen");
    close(sfd);
    return(-1);
  }
   
/*
 * accept connection and read data until EOF, copying to standard output
 */
  sin_size = sizeof(struct sockaddr_in);
  if ((cfd = accept(sfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
    perror("accept");
    close(sfd);
    return(-1);
  }
  while ((n = recv(cfd, buf, sizeof(buf), 0)) > 0) {
    buf[n] = '\0';
    fputs(buf, stdout);
  }
  close(cfd);
  close(sfd);

  return 0;
} 
