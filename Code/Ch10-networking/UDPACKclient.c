/*
 * UDPACKclient.c -- a UDP socket client that expects Acks for sent messages
 *
 * Invocation:
 *   >./UDPclient
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#define MYPORT 3490		/* the port to which the server is bound */

#include "udp_ack.h"

int main(int argc, char *argv[])
{
  int s_fd;			/* the socket for communication */
  struct sockaddr_in sv_addr, m_addr;	/* s(erver) and m(y) addr data */
  data_frame msg;
  ack_frame ack;
  int n;
  unsigned long seqno = 0;
  int remaining;
  struct timeval tmo;
  fd_set rset;
  unsigned short plen;

  memset(&m_addr, 0, sizeof(m_addr));	/* my address information */
  m_addr.sin_family = AF_INET;
  m_addr.sin_port = 0;			/* 0 ==> assign me a port */
  m_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  memset(&sv_addr, 0, sizeof(sv_addr));	/* server addr info */
  sv_addr.sin_family = AF_INET;
  sv_addr.sin_port = htons(MYPORT);
  sv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

/**** open the UDP socket */
  if ((s_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return(-1);
  }

/**** bind to local UDP port (randomly assigned) */
  if (bind(s_fd, (struct sockaddr *)&m_addr, sizeof(m_addr)) < 0) {
    perror("bind");
    return(-1);
  }

/**** send each line from stdin as a separate message to server */
  while (fgets(msg.payload, PAYLOAD_SIZE, stdin) != NULL) {
    n = strlen(msg.payload) + 1 + ACK_SIZE;	/* count the EOS! */
    msg.mtype = htons(DATA);
    msg.mseqn = htonl(++seqno);
    sendto(s_fd, (char *)&msg, n, 0, (struct sockaddr *)&sv_addr, sizeof(sv_addr));
    for (remaining = NRETRIES; remaining > 0; remaining--) { // attempt to receive ACK & resend msg
      tmo.tv_sec = 0;
      tmo.tv_usec = MSEC_TMO * 1000;
      FD_ZERO(&rset);
      FD_SET(s_fd, &rset); 
      if (select(s_fd+1, &rset, NULL, NULL, &tmo) > 0)
	break;
      sendto(s_fd, (char *)&msg, n, 0,
	     (struct sockaddr *)&sv_addr, sizeof(sv_addr));
    }
    if (remaining <= 0)
      return (-1);
    recv(s_fd, (char *)&ack, sizeof(ack), 0);
  }

/**** close the socket */
  close(s_fd);

}
