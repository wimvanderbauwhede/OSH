/*
 * udp_ack.h - defines and typedefs used by UDPACKclient and UDPACKserver
 */

#ifndef DEFINED_UDP_ACK
#define DEFINED_UDP_ACK

#define DATA 1
#define ACK 2
#define PAYLOAD_SIZE 256
#define MSEC_TMO 50	/* sender waits 50 msec for ACK before retry */
#define NRETRIES 10	/* number of retransmission before giving up */

typedef struct {
  unsigned short mtype;		/* DATA in network order, 2 bytes */
  unsigned long mseqn;		/* sequence number in network order, 4 bytes */
  char payload[PAYLOAD_SIZE];	/* the actual message */
} data_frame;
typedef struct {
  unsigned short mtype;		/* ACK in network order */
  unsigned long mseqn;		/* sequence number in network order */
} ack_frame;

#define ACK_SIZE (sizeof(data_frame)-PAYLOAD_SIZE)

#endif /* DEFINED_UDP_ACK */
