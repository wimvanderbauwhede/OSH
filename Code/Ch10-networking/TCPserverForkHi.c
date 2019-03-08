/*
 **
 ** Code skeleton for server with fork() 
 ** 
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/wait.h>       /* for waitpid() */
#include <signal.h>         /* for sigaction() */

#define MYPORT 3490    /* the port users will be connecting to */
#define MAX_NCLIENTS 5
#define MAX_NCHARS 128 /* max number of characters to be read/written at once */
#define FALSE 0
/* ====================================================================== */
void sigchld_handler();     /* Function to clean up zombie child processes */

int main(int argc, char * argv[]) {

    struct sockaddr_in my_addr;         /* my address information */
    struct sockaddr_in their_addr;      /* client address information */

    pid_t pid;

    struct sigaction sigchld_action;       /* Signal handler specification structure */
    /* Set sigchld_handler() as handler function */
    sigchld_action.sa_handler =  sigchld_handler;
    if (sigfillset(&sigchld_action.sa_mask) < 0) {  /* mask all signals */
        perror("sigfillset() failed");
        return(-1);
    }
    /* SA_RESTART causes interrupted system calls to be restarted */
    sigchld_action.sa_flags = SA_RESTART;

    /* Set signal disposition for child-termination signals */
    if (sigaction(SIGCHLD, &sigchld_action, 0) < 0) {
        perror("sigfillset() failed");
        return(-1);
    }

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
        if (newfd  == -1) {
            perror("accept");
        } else {
            /*** error handling ***/
            pid = fork();
            if (pid < 0 ) {
                perror ("fork failed \n");
            }

            if (pid ==(pid_t)0) { //It's the child
                /*  handle client request */			    
                char clientline[MAX_NCHARS]="";
                char tmpchar; 
                char newline = '\n';
                int eob = 0;
                while(eob==0 && strlen(clientline)<MAX_NCHARS) {
                    read(newfd,&tmpchar,1);
                    eob=(tmpchar==newline) ? 1 : 0;
                    strncat(clientline,&tmpchar,1);
                }	   
                printf("%s",clientline);
                shutdown(newfd,2);
                exit(0);
            } 
        }
    }  // while()
    return 0;
}

void sigchld_handler() {
    pid_t cpid;
    cpid=wait(NULL);
}
