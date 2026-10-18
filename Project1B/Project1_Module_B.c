#include "Project1_Module_B.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        printf("Invalid number of processes: %d\n", num_processes);
        return 1;
    }


    struct timespec start, end;
    long long seconds, nano_seconds;


    /* ToDo1
     * Create and attach shared memory region 1 to store the matrix
     */
    size_t matrix_size = ROWS_COUNT * COLUMN_COUNT * sizeof(int);
    int shmid_matrix = shmget(IPC_PRIVATE, matrix_size, IPC_CREAT | 0666);

    if (shmid_matrix == -1) {
        printf("Error with shmget matrix");
        return 1;
    }

    int *matrix = (int *)shmat(shmid_matrix, NULL, 0);

    if (matrix == (void *)-1) {
        printf("Error with shmat matrix");
        return 1;
    }


    /* ToDo2
     * create and attach shared memory region 2 to store the partial sums that
     * are returned by children processes.
     */

    size_t sums_size = num_processes * sizeof(long);
    int shmid_sums = shmget(IPC_PRIVATE, sums_size, IPC_CREAT | 0666);
    long *partial_sums = (long *)shmat(shmid_sums, NULL, 0);


    /* ToDo3
     * Initialize the shared memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
     *
     * Hint: Treat starting address of shared memory region as first
     * elemnt of a 2D matrix i.e.,  matrix[0][0]
    */

    for (int i = 0; i < ROWS_COUNT; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            matrix[i * COLUMN_COUNT + j] = i + j;
        }
    }


    clock_gettime(CLOCK_REALTIME, &start);


    /* ToDo4:
     * 1) Create num_processes count of children processes. (using fork). Note >
     * (i.e., binary of child_proc.c) using execlp()
     * 2) Specify for each process the start_row, end_row, and
     * index in shared memory region 2 where a child process is supposed to sto>
     * calculated partial sum.
    */

    int child_rows = ROWS_COUNT / num_processes;

    for (int i = 0; i < num_processes; i++){
        pid_t pid = fork();
        if (pid == 0){
            int start = i * child_rows;
            int end = (i == num_processes -1) 
                    ? ROWS_COUNT -1 
                    : start + child_rows -1;
            char s[16], e[16], m[16], p[16],idx[16];
            sprintf(s, "%d", start);
            sprintf(e, "%d", end);
            sprintf(m, "%d", shmid_matrix);
            sprintf(p, "%d", shmid_sums);
            sprintf(idx, "%d", i);

            execlp("./child", "child", s, e, m, p, idx, (char *)NULL);
            perror("execlp");
            exit(1);
        }
    }


    /* ToDo5
     * Wait for all children processes to complete
    */
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    printf("Usage: %s <number_of_processes>\n", argv[0]);

    /* ToDo6:
     * Sum all the partial-sums stored in sharred memory region 2 (by children >
     * and print the sum.
    */
   long total = 0;
   for (int i = 0; i< num_processes; i++){
    total += partial_sums[i];
   }

   printf("Total sum %ld\n", total);


    clock_gettime(CLOCK_REALTIME, &end);
    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1>
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_t>


    /* ToDo7:
     * Detach and destroy both shared memory regions.
     * Hint:
     * Google "man shmdt" and "man shmctl"
    */

    shmdt(matrix);
    shmdt(partial_sums);

    shmctl(shmid_matrix, IPC_RMID, NULL);
    shmctl(shmid_sums, IPC_RMID, NULL);



    return 0;
}

