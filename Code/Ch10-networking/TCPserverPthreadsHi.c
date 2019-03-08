/*
 **
 ** Code skeleton for server with pthreads
 ** 
 */
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MYPORT 3490    /* the port users will be connecting to */
#define MAX_NCLIENTS 5
#define MAX_NCHARS 128 /* max number of characters to be read/written at once */
#define FALSE 0
/* ====================================================================== */

void *client_handler(void *);

int main(int argc, char * argv[]) {

    struct sockaddr_in my_addr;         /* my address information */
    struct sockaddr_in their_addr;      /* client address information */

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

    /**** open the server (TCP) socket */
    int s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd == -1) {
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

    unsigned int sin_size = sizeof(struct sockaddr_in);        

    while (1) {
        // accept on new client socket newfd
        int newfd = accept(s_fd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1) {
            perror("accept");
        } else {
            // Create new thread
            pthread_create(&tid, &attr, client_handler, (void*)newfd);
        }
    }  // while()
    return 0;
}

void * client_handler(void* fdp) {
    /*  handle client request */			    
    int c_fd = (int) fdp;
    char clientline[MAX_NCHARS]="";
    char tmpchar; 
    char newline = '\n';
    int eob = 0;

    while(eob==0 && strlen(clientline)<MAX_NCHARS) { 
        read(c_fd,&tmpchar,1);
        eob=(tmpchar==newline) ? 1 : 0;
        strncat(clientline,&tmpchar,1);
    }	   
    printf("%s",clientline);

    close(c_fd);
    pthread_exit(0);
}

