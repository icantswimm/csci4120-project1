#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        printf("Usage: %s input.pgm output.pgm threshold\n", argv[0]);
        return 1;
    }

    char *input_file=argv[1];
    char *output_file=argv[2];
    int threshold=atoi(argv[3]);

    /* 
    ToDO 1: Open input file in read-only binary mode i.e. "rb"
    Hint: google "man fopen()"
    */

    FILE* input_fileptr;
    input_fileptr = fopen(input_file, "rb");

    /*
    ToDo 2: Do error checking to ensure file was opened sucessfully before proceeding
    */

    if (input_fileptr == NULL) {
        printf("input file could not be opened (errno: %d): %s\n", errno, input_file);
        exit(1);
    }

    //======================Reading File Header=====================================
    //read the first line just to advance the cursor in file but dont use the line.
    char Line[256];

    fgets(Line, 256, input_fileptr);

    //read the width, and height of the file 
    
    int width, height, max_value;
    int return_val;
    
    fgets(Line, sizeof(Line), input_fileptr); // P5
    sscanf(Line, "%d %d", &width, &height);

    //read the maximum pixel value;
    sscanf(Line, "%d", &max_value);
    fgetc(input_fileptr); // consume the newline before pixel data

    //====================Done Reading File Header===================================
    /*
    ToDo 3: Allocate memory to store width x height number of pixels for black and white image. 
    Note that each pixel has max value of 255, so an 8-bit char is sufficent to store the pixel value.
    Hint: google "man malloc";
    */

    typedef unsigned char pixel_t;

    size_t size_of_pixels = sizeof(pixel_t) * width * height;
    pixel_t (*pixels)[width] = malloc(size_of_pixels);

    /*
    ToDo 4: Do Error checking to make sure memory was successfully allocated before proceeding.
    In case of failure, exit the program.
    */

    if (pixels == NULL) {
        printf("pixels array could not be allocated (errno: %d)\n", errno);
        exit(1);
    }

    /*
    ToDo 5: read all the content of input image file into allocated memory.
    Hint: Google "man fread(3p)"
    */

    size_t num_to_read = width * height;
    size_t number_of_elements_read = fread(pixels, sizeof(pixel_t), num_to_read, input_fileptr);


    /*ToDo 6: Do error checking to make sure that fread was sucessfull. If not, exit the program.
    Hoever, free the dynamically allocated memory before exiting.
    */

    if (number_of_elements_read == 0 || number_of_elements_read != num_to_read) {
        errno_t file_errno = ferror(input_fileptr);
        printf("input file could not be read (file errno: %d): %s\n", file_errno, input_file);
        clearerr(input_fileptr);
        exit(1);
    }


    //ToDo 7: close the input file

    return_val = fclose(input_fileptr);

    if (return_val != 0) {
        printf("input file could not close gracefully (errno: %d): %s\n", errno, input_file);
        exit(1);
    }
    

    /*
    ToDo 8: Iterate over all bytes of the dynmaically allocated memory (where each byte stores one pixel of the input file) 
    and update its avlue by doing following operation.

    if (value of the byte is greater than or equal to threshold)
        {
        set value of the byte to 255.
        }
    else
        {
        set value of the byte to 0.
        }
    */

    for (int h=0; h < height; h++) {
        for (int w=0; w < width; w++) {
            if (pixels[h][w] >= threshold) {
                pixels[h][w] = 255;
            }
            else {
                pixels[h][w] = 0;
            }
        }
    }

    /*
    ToDo 9: open the ouput file with "w+" permission
    Hint: google "man fopen"
    */

    FILE* output_fileptr;
    output_fileptr = fopen(output_file, "w+");


    /*
    ToDo 10: Do error checking to make sure that output file is opened sucessfully. If not, exit the program.
    However, free the dynamically allocated memory before exiting.
    */

    if (output_fileptr == NULL) {
        printf("input file could not be opened (errno: %d): %s\n", errno, output_file);
        exit(1);
    }
    

    //======================Write header of the output file====================
    fprintf(output_fileptr, "P5\n%d %d\n255\n", width, height);
    //===============Done writing header of the output file====================

    /*
    ToDo 11: Write output file with the content of dynamically allocated memory
    Hint: google "man fwrite(3p)"
    */

    size_t num_to_write = width * height;
    size_t number_of_elements_wrote = fwrite(pixels, sizeof(pixel_t), num_to_write, output_fileptr);
    

    /*
    ToDo 12: Do error checking to make sure that all bytes from allocated memory are written successfully to the output image file.
    If not, free the dynmaically allocated memory and exit the program.
    */

    if (number_of_elements_wrote == 0 || number_of_elements_wrote != num_to_write) {
        printf("output file could not be written (errno: %d): %s\n", errno, output_file);
        exit(1);
    }
    
    /*
    ToDo 13: Close output file.
    */

    return_val = fclose(output_fileptr);

    if (return_val != 0) {
        printf("input file could not close gracefully (errno: %d): %s\n", errno, output_file);
        exit(1);
    }
    
    /*
    ToDo 14: Free the memory that was allocated to store black and white image.
    Hint: Google "man free". 
    */

    free(pixels);

    return 0;
}
