#ifndef PROJECT2MODB
#define PROJECT2MODB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> /*for pthread library*/

int threshold;

/*=============================ToDo 1: =======================
Complete body of threshold function.
Hint: google how to pass multiple parameters to a function that is executed by a thread created thorugh pthread_create().
NOTE: A thread running this function must exit (i.e., use pthread_exit()) after exceuting the function body.*/

struct arguments {
    unsigned char *image;
    int width;
    int start_row;
    int end_row;
};

void threshold_operation(struct arguments *args){
    unsigned char *img = args->image;
    int width = args->width;

    for (int r = args->start_row; r < args->end_row; r++) {
        for (int c = 0; c < width; c++) {
            int idx = r * width + c;
            if (img[idx] >= threshold) {
                img[idx] = 255;
            } else {
                img[idx] = 0;
            }
        }
    }
    pthread_exit(NULL);
}

#endif