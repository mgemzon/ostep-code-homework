#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    pid_t pid = getpid();

    printf("[pid %d]\n", pid);

    if (argc < 2 || argc > 3) {
        printf("Usage: %s <size in megabytes> [duration in seconds]\n", argv[0]);
        return -1;
    }

    char *endptr;
    long size_in_mb = strtol(argv[1], &endptr, 10);

    if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "Invalid number: %s\n", argv[1]);
        return -1;
    }

    time_t duration_in_seconds = -1;

    if (argc == 3) {
        duration_in_seconds = (time_t)strtol(argv[2], &endptr, 10);

        if (endptr == argv[2] || *endptr != '\0') {
            fprintf(stderr, "Invalid duration: %s\n", argv[2]);
            return -1;
        }
        
        if (duration_in_seconds < 0) {
            fprintf(stderr, "Failed to get duration!\n");
            return -1;
        }
    }
    
    size_t bytes_to_occupy = size_in_mb * 1024 * 1024;

    printf("Bytes to occupy: %lu\n", bytes_to_occupy);
    
    char *occupy_memory = (char*)malloc(bytes_to_occupy);

    if (occupy_memory == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return -1;
    }

    int loop_num = 0;

    time_t start_time = time(NULL);

    if (start_time < 0) {
        fprintf(stderr, "Failed to start time!\n");
        return -1;
    }

    while (1) {
        
        for (size_t i = 0; i < bytes_to_occupy; i++) {
            occupy_memory[i] = i;
        }

        printf("[pid: %d] loop %d done!\n", pid, loop_num++);

        if (duration_in_seconds != -1) {
            // Only activate when duration is set.
            if (time(NULL) - start_time >= duration_in_seconds) {
                break;
            }
        }
    }

    free(occupy_memory);
    return 0;
}