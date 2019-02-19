#include "driver.h"
#include <stdio.h>
//#include <sys/types.h>
#include <stdlib.h>

#include <pthread.h>
//#include <string.h>
//#include <unistd.h>
#include <errno.h>
//#include <ctype.h>

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct thread_info {    /* Used as argument to thread_start() */
    pthread_t thread_id;        /* ID returned by pthread_create() */
    long long unsigned int thread_num;       /* Application-defined thread # */
        CLObject* ocl;
};

/* Thread start function: display address near top of our stack,
          and return upper-cased copy of argv_string */

void* thread_start(void *vtinfo) {
    struct thread_info *tinfo = vtinfo;
#if VERBOSE_MT>3    
    printf("Thread %llu started\n", tinfo->thread_num);
#endif
    const unsigned int mSize = BUF_SZ;
    const int w1 = W1;
    const int w2 = W2;
    
    // Create the data sets   
    int* mA=(int*)malloc(sizeof(int)*mSize);
    int* mB=(int*)malloc(sizeof(int)*mSize);

    for(unsigned int i = 0; i < mSize; i++) {
        mA[i] = (i+1)*(tinfo->thread_num+1);
        mB[i] = (i+1)*(tinfo->thread_num+2);
    }
    // Create the reference
    int* mCref=(int*)malloc(sizeof(int)*mSize);
    for (unsigned int i = 0; i<mSize; i++) {
            mCref[i]=w1*mA[i]*mA[i]+w2*mB[i]*mB[i];        
    }

    int* mC=(int*)malloc(sizeof(int)*mSize);
    tinfo->ocl->thread_num = tinfo->thread_num;
    int status = run_driver(tinfo->ocl,mSize,mA,mB,w1,w2,mC);

    // Check the returned result
    unsigned int correct=0;               // number of correct results returned
    for (unsigned int i = 0; i < mSize; i++){
        //std::cerr << mC[i] << " : " << mCref[i] << std::endl;
    	int reldiff = mC[i] - mCref[i];
        if(reldiff==0)
            correct++;
    }
    free(mCref);
    printf("Computed '%d/%d' correct values in thread %llu, %s!\n",correct,mSize,tinfo->thread_num, (correct == mSize) ? "SUCCESS" : "FAILURE");
    free(mA);
    free(mB);
    free(mC);
    return  (void*)(tinfo->thread_num);
}    

int main(int argc, char *argv[]) {
    // Initialise OpenCL
    CLObject* ocl = init_driver();

    int st;
    struct thread_info *tinfo;
//    pthread_attr_t attr;
    unsigned int num_threads = NTH;

    /* Initialize thread creation attributes */
//    st = pthread_attr_init(&attr);
//    if (st != 0)
//        handle_error_en(st, "pthread_attr_init");

    /* Allocate memory for pthread_create() arguments */
    tinfo = calloc(num_threads, sizeof(struct thread_info));
    if (tinfo == NULL)
        handle_error("calloc");

    /* Create threads */
    for (unsigned int tnum = 0; tnum < num_threads; tnum++) {
        tinfo[tnum].thread_num = tnum + 1;
        tinfo[tnum].ocl = ocl;
        /* The pthread_create() call stores the thread ID into
           corresponding element of tinfo[] */
        st = pthread_create(&tinfo[tnum].thread_id, NULL,
                &thread_start, &tinfo[tnum]);
        if (st != 0)
            handle_error_en(st, "pthread_create");
    }

    /* Destroy the thread attributes object, since it is no
       longer needed */
//    st = pthread_attr_destroy(&attr);
//    if (st != 0)
//        handle_error_en(st, "pthread_attr_destroy");

    /* Now join with each thread */
    for (unsigned int tnum = 0; tnum < num_threads; tnum++) {
        st = pthread_join(tinfo[tnum].thread_id, NULL);
        if (st != 0)
            handle_error_en(st, "pthread_join");
#if VERBOSE_MT>3
        printf("Joined with thread %llu\n", tinfo[tnum].thread_num);
#endif        
    }

    free(tinfo);

    shutdown_driver(ocl);
    
    exit(EXIT_SUCCESS);
}

