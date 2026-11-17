#include "project3ModA.h"

int main(int argc, char *argv[]) {
    
    
    if (argc != 6) {
        printf("Usage: %s input.pgm output.pgm threshold worker_thread_cnt scheduling_policy\n", argv[0]);
        return 1;
    }

    //===================Reading input parameters
    char *input_file=argv[1];
    char *output_file=argv[2];
    threshold=atoi(argv[3]);
    int worker_thread_cnt=atoi(argv[4]);
    int policy=atoi(argv[5]);

    if (policy==0){
        policy=SCHED_RR;
    }
    else if (policy==1){
        policy=SCHED_FIFO;
    }
    else {
        printf("Unsupported scheduling policy\n");
        return 1;
    }

    /* 
    Open input file in read-only binary mode i.e. "rb"
    Hint: google "man fopen()"
    */

    FILE *Fin=fopen(input_file, "rb");

    /*
    Do error checking to ensure file was opened sucessfully before proceeding
    */
    if(Fin==NULL)
    {
        printf("Failed to open the input file\n");
        exit(0);
    }

    //read the first line just to advance the cursor in file but dont use the line.
    char Line[256];

    fgets(Line,256,Fin);

    //read the width, and height of the file 
    
    int width, height, max_value;
    int return_val;
    
    fgets(Line, sizeof(Line), Fin); // P5
    sscanf(Line, "%d %d", &width, &height);

    //read the maximum pixel value;
    sscanf(Line, "%d", &max_value);
    fgetc(Fin); // consume the newline before pixel data

    if (height%worker_thread_cnt!=0){
        printf("Number of rows (%d) is not a multiple of worker_thread_cnt (%d)\n",height,worker_thread_cnt);
        fclose(Fin);
        exit(0);
    }

    /*
    Allocate memory to store width x height number of pixels for black and white image. 
    Note that each pixel has max value of 255, so an 8-bit char is sufficent to store the pixel value.
    Hint: google "man malloc";
    */

    char *image=malloc(width*height*1);
    

    if(image==NULL)
    {
        printf("Failure in dynamically allocating the required memory size\n");
        exit(0);
    }


    /*
    Read all the content of input image file into allocated memory.
    Hint: Google "man fread(3p)"
    */
    return_val=fread(
                    image,
                    1, 
                    width*height,
                    Fin);

    if(return_val!=(width*height))
    {
        printf("Failure in reading input file into dymaically allocated memory\n");
        free(image);
        exit(0);
    }

    //close the input file
    fclose(Fin);

    /*
    Create worker_thrd_cnt number of threads. 
    Each thread is responsible for doing the threshold operation on pixles of height/worker_thread_cnt number of rows. 
    */

    pthread_t tid[MAX_THRD_COUNT]; //make an array for the extreme case when 512 threads are created.
    pthread_attr_t attr;  /* set of thread attributes */    
	pthread_attr_init (&attr);  /*initialize the attribute structre with default values*/
    struct arguments arg_array[MAX_THRD_COUNT];
      
    void* (*func_ptr)(void *arg);
    func_ptr = threshold_operation;

    int start_row=0; 
    int end_row;

    /*
    First give higher priority to parent thread so that newly created thread run only after 
    parent is done with creation of all threads.
    */
    pthread_t parent_tid = pthread_self();  // or another thread ID
    struct sched_param param;

    param.sched_priority = 80;  // Choose a value between sched_get_priority_min() and max()

    pthread_setschedparam(parent_tid, SCHED_RR, &param);

    

    /* 
    Parent thread creats children thread
    */

    for (int i=0;i<worker_thread_cnt;i++){ 
        arg_array[i].image=image;
        arg_array[i].width=width;
        end_row=start_row+(height/worker_thread_cnt);
        arg_array[i].start_row=start_row;
        arg_array[i].end_row=end_row;
        start_row=end_row;

        // printf("arg_array[%d]: width=%d start_row=%d, end_row=%d\n", i,arg_array[i].width,arg_array[i].start_row,arg_array[i].end_row);
        
		if(pthread_create (
            &(tid[i]) /*address of the thread id*/,                        				
            &attr /*start the thread with specified attributes */,                        				
            func_ptr /*function executed by the thread*/,                        				
            &(arg_array[i]) /*argument for the function*/)!=0)
            {
                printf("Failure in creating thrd_%d\n",i);
                exit(0);
            }  

        // ToDo 1: Set affinity of (i.e., pin it) each newly created thread to core 1.

       

        //ToDo 2: Set the user-specified policy for all newly created threads

	}

    /*
    Make the parent thread (the one that is generating new threads) wait for all threads to complete before proceeding.
    */
    for (int i=0;i<worker_thread_cnt;i++){   
        pthread_join(tid[i],NULL/*exit status is not copied*/);
	}


    /*
    open the ouput file
    */

    FILE *Fout=fopen(output_file, "w+");
    if(Fout==NULL)
    {
        printf("Failure in opening the output file\n");
        free(image);
        exit(0);
    }


    fprintf(Fout, "P5\n%d %d\n255\n", width, height);
    /*
    Write output file with the content of dynamically allocated memory
    */
    return_val=fwrite(image,1,width*height,Fout);

    if(return_val!=width*height)
    {
        printf("Failure in writing black and white image to output file\n");
        free(image);
        exit(0);
    }

    /*
    Close output file.
    */
    fclose(Fout);
    
    /*
    Free the memory that was allocated to store black and white image.
    */
    free(image);


    return 0;
}
