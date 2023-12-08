#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>

// Function to get the current timestamp for performance measurement
long now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

// Function to generate a random string
void get_random_str(char* random_str, size_t len) {
    const char seed_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t seed_chars_len = strlen(seed_chars);
    for (size_t i = 0; i < len; ++i) {
        random_str[i] = seed_chars[rand() % seed_chars_len];
    }
    random_str[len] = '\0'; // Null-terminate the string
}

int main(int argc, char* argv[]) {
    clock_t start, end;
    double total_time;
    //unsigned long long int size_of_block=5000000000000000000000000ULL;
    //unsigned long long int size_of_block = 5000000000000000000ULL;
    
    //int x=i;
    //while(size_of_block<100){
       //size_of_block=size_of_block*2;
    //}
    const char* filename = argv[1]; //filename
    int block_size = atoi(argv[2]); //blocksize 
  

    //if (argc != 3 ) {// && !(strcmp(mode, "-r") == 0) //fix later//seg fault// we want to allow the user to read from the file even if they don't know the file size
        //fprintf(stderr, "Usage: %s <filename> [-r|-w] <block_size> \n", argv[0]);
        //return 1;
    //}

    srand(time(NULL)); // Seed the random number generator

    
    int fd;
    //long start, finish;
    ssize_t result;
 

        //if (argc <=4) { //if the user doesn't know the size of the file they can still read from it
            //fprintf(stderr, "Usage: %s <filename> [-r|-w] <block_size> <block_count> OR Usage: %s <filename> [-r|-w] <block_size> \n", argv[0],argv[0]);
            //return 1;
        //}
        // Read mode
         //posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);
        fd = open(filename, O_RDONLY |O_DIRECT);
        if (fd == -1) {
            perror("Error opening file");
            return 1;
        }
        
        off_t size_of_block = 907374182456799; //90000000000000000
        off_t  block_count = size_of_block;
        char* buffer = (char*)malloc(block_count + 1); // +1 for null terminator
       // while (1) {
            // Read a portion of file and measure the time taken
            start = clock();
            result = read(fd, buffer, size_of_block);
            end = clock();

            total_time = (double)(end - start) / CLOCKS_PER_SEC;
            printf("total_time is %f\n", total_time);
            if (total_time > 5 && total_time < 15) {
                // The read time is within the desired range, print the file size and exit
                printf("File size for reasonable reading time: %lld\n", (long long)size_of_block);
                //break;
            }

            // Double the size of the portion to be read and try again
            //size_of_block *= 2;
        //}
        /*

        start = clock();
        //start = now();
        for (int i = 0; i < block_count; ++i) {
            result = read(fd, buffer, block_size);
            if (result == -1) {
                perror("Error reading from file");
                close(fd);
                free(buffer);
                return 1;
            }
            if (result == 0) {
                // EOF reached
                break;
            }
            // Optionally process the buffer here
        }
        //finish = now();
        end = clock();
        total_time = (double)(end - start) / CLOCKS_PER_SEC;

        //printf("Read performance: %f MB/s\n", block_size * block_count / ((finish - start) / 1000000.0) / (1024 * 1024));
        printf("block count is  %lld\n", (long long)block_count);
        printf("Time taken to read the file: %f seconds\n", total_time);
        close(fd);
        free(buffer);*/
        return 0;
}
