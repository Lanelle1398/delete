#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>

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
    //if (argc != 5) {
        //fprintf(stderr, "Usage: %s <filename> [-r|-w] <block_size> <block_count>\n", argv[0]);
        //return 1;
    //}

    srand(time(NULL)); // Seed the random number generator

    const char* filename = argv[1];
    const char* mode = argv[2];
    //int block_size = atoi(argv[3]);
    float block_size=10;
    int block_count = atoi(argv[3]);

    int fd;
    long start, finish;
    ssize_t result;
    struct stat fileStat;
    char* buffer = (char*)malloc(block_size + 1); // +1 for null terminator
    
    //if(stat(filename, &fileStat)==0){
    	//printf("File size: %ld bytes\n",fileStat.st_size);
    //}

    if (strcmp(mode, "-w") == 0) {
        // Write mode
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd == -1) {
            perror("Error opening file");
            return 1;
        }

        get_random_str(buffer, block_size); // Fill the buffer with random data

        start = now();
        for (int i = 0; i < block_count; ++i) {
            result = write(fd, buffer, block_size);
            if (result == -1) {
                perror("Error writing to file");
                close(fd);
                free(buffer);
                return 1;
            }
        }
        finish = now();

        printf("Write performance: %f MB/s\n", block_size * block_count / ((finish - start) / 1000000.0) / (1024 * 1024));
    } else if (strcmp(mode, "-r") == 0) {
        // Read mode
        fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("Error opening file");
            return 1;
        }
       
        char* buffer = (char*)malloc(block_size + 1); 
        if (buffer == NULL) {
	    perror("Error allocating memory for buffer");
	    close(fd);
	    return 1;
}
       
        for(int x=0; x<1000; x++){
        	start = now();
       
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
        finish = now();
        if(block_size> (30000000000/block_count)){//or just 30000000 ////the max block size should be 30 megabytes if our block count is 1000 and our file size is 30gb //1099000000 
        	break;
        }
        
       
        printf("Block size is: %f" , block_size);
        printf(" Read performance: %f s", (finish - start) / 1000000.0);
        printf(" Read performance: %f MB/s \n", block_size * block_count/((finish-start)/1000000.0)/(10241024));
       
        block_size*=2;
        }
        free(buffer);
  

    }
    
     else {
        fprintf(stderr, "Invalid mode. Use -r for read or -w for write.\n");
        free(buffer);
        return 1;
    }

    close(fd);
    //free(buffer);
    return 0;
}
