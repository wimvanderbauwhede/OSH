CC=gcc
LIBS=

all: TCPclient TCPserver UDPserver UDPclient UDPconnclient UDPACKserver UDPACKclient 

clean:
	rm -f *.o  TCPclient UDPclient TCPserver TCPserverSelectHi TCPserverForkHi TCPserverPthreadsHi UDPserver UDPACKserver UDPACKclient *~

UDPACKserver.o: udp_ack.h UDPACKserver.c

UDPACKclient.o: udp_ack.h UDPACKclient.c

TCPclient: TCPclient.o
	${CC} ${LIBS} -o TCPclient TCPclient.o

TCPserver: TCPserver.o
	${CC} ${LIBS} -o TCPserver TCPserver.o

TCPserverSelectHi: TCPserverSelectHi.o
	${CC} ${LIBS} -o TCPserverSelectHi TCPserverSelectHi.o

TCPserverForkHi: TCPserverForkHi.o
	${CC} ${LIBS} -o TCPserverForkHi TCPserverForkHi.o

TCPserverPthreadsHi: TCPserverPthreadsHi.o
	${CC} ${LIBS} -o TCPserverPthreadsHi TCPserverPthreadsHi.o

UDPserver: UDPserver.o
	${CC} ${LIBS} -o UDPserver UDPserver.o

UDPclient: UDPclient.o
	${CC} ${LIBS} -o UDPclient UDPclient.o

UDPACKserver: UDPACKserver.o
	${CC} ${LIBS} -o UDPACKserver UDPACKserver.o

UDPACKclient: UDPACKclient.o
	${CC} ${LIBS} -o UDPACKclient UDPACKclient.o
