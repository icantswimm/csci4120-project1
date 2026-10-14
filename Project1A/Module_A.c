// Gregory Olmstead, John Renfro, Quinn Sansom

#include "Module_A.h"

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

    /* ToDo 1:
     * Dynamically allocate memory of size ROWS_COUNT x COLUMN_COUNT x size_of(int).
     * run "man malloc" on ubuntu/linux terminal or see online help about malloc function
     */
    
     size_t size_of_2d_array = ROWS_COUNT * COLUMN_COUNT * sizeof(int);
     int (*matrix)[COLUMN_COUNT] = malloc(size_of_2d_array); // pointer to an array of ints the size of COLUMN_COUNT.

    /* ToDo 2:
     * Initialize the allocated memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
    */

    for (int row_offset = 0; row_offset < ROWS_COUNT; row_offset++) {
        for (int column_offset = 0; column_offset < COLUMN_COUNT; column_offset++) {
            int *pointer_to_element = *(matrix + row_offset) + column_offset; // find offset of int * pointers, find offset of index of int *
            *pointer_to_element = row_offset + column_offset;
        }
    }

    printf("Matrix initialized\n");

    int rows_per_process=ROWS_COUNT/num_processes;

    clock_gettime(CLOCK_REALTIME, &start);

    /*
    ToDo 3: Create num_processes count of children processes. (using fork)
    ToDo 4: Ask each child process to print "row_per_process" number of rows by calling function print_matrix_rows(). See header file.
    ToDo 5: Each child process MUST free the dynamically allocated memory for matrix from its own adress space after printing the designated rows.
    ToDo 6: All children processes MUST exit after freeing dyamically allocated memory of matrix.
    ToDo 7: Parent process must wait for all children processes to complete.
    */

    for (int process_count = 0; process_count < num_processes; process_count++) {
        pid_t pid = fork();

        if (pid == 0) {
            int start_row_for_child = process_count * rows_per_process; // start of subdivision
            int end_row_for_child = start_row_for_child + rows_per_process - 1; // end of subdivision, using subdivision size + start of subdivision

            if (process_count == num_processes - 1) { // an odd subdivision split could leave out rows at the end.
                end_row_for_child = ROWS_COUNT - 1;
            }

            print_matrix_rows(*matrix, start_row_for_child, end_row_for_child);
            free(matrix); // free childs copy of matrix
            exit(0);
        }
    }

    for (int process_count = 0; process_count < num_processes; process_count++) {
        wait(NULL);
    }

    printf("all child processes completed their execution\n");

    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);

    free(matrix);
    return 0;
}
