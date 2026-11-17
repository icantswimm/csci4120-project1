#ifndef PROJECT3MODA
#define PROJECT3MODA

#define _GNU_SOURCE /*For CPU_SET*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h> /*For CPU_SET*/
#include <pthread.h> /*for pthread library*/

#include <threads.h>


#define MAX_THRD_COUNT 512

int threshold;
_Thread_local char thread_name[16]; 

/*=============================ToDo 1: =======================
Complete body of threshold function.
Hint: google how to pass multiple parameters to a function that is executed by a thread created thorugh pthread_create().
NOTE: A thread running this function must exit (i.e., use pthread_exit()) after exceuting the function body.*/

/*Create a struct */
struct arguments {
    char* image;
    int start_row;
    int end_row;
    int width;
};

void* threshold_operation(void *arg){
    struct arguments *args = (struct arguments *)arg;

    char* my_image=args->image;
    int start_index=(args->start_row)*(args->width);
    int end_index=(args->end_row)*(args->width);

    
    for (int i=start_index;i<end_index;i++){
        my_image[i]=(my_image[i]>threshold)?255:0;
    }
    
}

#endif
