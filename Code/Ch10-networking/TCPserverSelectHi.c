/*
 **
 ** Code skeleton for server with select() 
 ** 
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/time.h>
#include <sys/select.h> 

/* for non-blocking I/O */
//#include <fcntl.h> 
//fcntl(fd,FL_SET,O_NONBLOCK) 

#define MYPORT 3490    /* the port users will be connecting to */
#define MAX_NCLIENTS 5
#define MAX_NCHARS 128 /* max number of characters to be read/written at once */
#define FALSE 0
/* ====================================================================== */

int main(int argc, char * argv[]) {
    fd_set master; /* master set of file descriptors  */
    fd_set read_fds; /* set of file descriptors to read from */
    int fdmax; /* highest fd in the set */        
    int s_fd;     

    FD_ZERO(&read_fds);	        
    FD_ZERO(&master);	
    /* get the current size of file descriptors table */
    fdmax = getdtablesize();

    struct sockaddr_in my_addr;         /* my address information */
    struct sockaddr_in their_addr;      /* client address information */

    /**** open the server (TCP) socket */
    if ((s_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return(-1);
    }

    /**** set the Reuse-Socket-Address option */  
    const int yes=1;
    if (setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int))==-1) {
        perror("setsockopt");
        close(s_fd);
        return(-1);
    }

    /**** build server socket address */        
    bzero((char*) &my_addr, sizeof(struct sockaddr_in));
    my_addr.sin_family = AF_INET;         
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    my_addr.sin_port = htons(MYPORT);

    /**** bind server socket to the local address */
    if (bind(s_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        close(s_fd);
        return(-1);
    }

    listen(s_fd, MAX_NCLIENTS);

    FD_SET(s_fd, &master); // add s_fd to the master set

    fdmax = s_fd; 

    while (1) {
        read_fds=master;
        select(fdmax+1, &read_fds, NULL, NULL, (struct timeval *)NULL); // never time out
        /* run through the existing connections looking for data to read */
        for(int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // if i belongs to the set read_fds
                if (i == s_fd) { // fd of server socket
                    // accept on new client socket newfd
                    int sin_size = sizeof(struct sockaddr_in);
                    int newfd = accept(s_fd, (struct sockaddr *)&their_addr, &sin_size);
                    if (newfd == -1) {
                        perror("accept");
                    }  						
                    FD_SET(newfd, &master); // add newfd to the master set
                    if (newfd > fdmax) {    
                        fdmax = newfd;
                    }						
                } else { // i is a client socket
                    printf("Hi, client\n");
                    /*  handle client request */			    
                    char clientline[MAX_NCHARS]="";
                    char tmpchar; 
                    char newline = '\n';
                    int eob = 0;
                    while(eob==0 && strlen(clientline)<MAX_NCHARS) {
                        read(i,&tmpchar,1);
                        eob=(tmpchar==newline) ? 1 : 0;
                        strncat(clientline,&tmpchar,1);
                    }	   
                    printf("%s",clientline);

                    /* clean up: close fd, remove from master set, decrement fdmax */
                    close(i); 
                    FD_CLR(i, &master);		
                    if (i == fdmax) {
                        while (FD_ISSET(fdmax, &master) == FALSE) {
                            fdmax -= 1;
                        }
                    }      			      
                } // i?=s_fd			  
            } // FD_ISSET 				
        } // for i 
    } // while()        
    return 0;
} 

