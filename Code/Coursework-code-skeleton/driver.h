#include <pthread.h>

#ifndef OSX
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#include <CL/cl.h>
#else
#include <OpenCL/opencl.h>
#endif

typedef struct ocl_objects {
    cl_device_id device_id;
    cl_context context;                 // compute context
    cl_command_queue command_queue;          // compute command queue
    cl_program program;   
    cl_kernel kernel;   
    int status;
    long long unsigned int thread_num;
    pthread_mutex_t device_lock; 
} CLObject;
// the driver contains a function with following signature:
CLObject* init_driver();
int run_driver(CLObject* ocl,unsigned int buffer_size,  int* input_buffer_1, int* input_buffer_2, int w1, int w2, int* output_buffer);
int shutdown_driver(CLObject*);
int driver(unsigned int buffer_size,  int* input_buffer_1,   int* input_buffer_2, int w1, int w2, int* output_buffer);


