/**
 * Write a program that opens a file (with the open() system call) and then
 * calls fork() to create a new process. Can both the child and the parent
 * access the file descriptor returned by open()? What happens when they are
 * writing to the file concurrently, i.e. at the same time?
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Open a file.
  // O_CREAT: If path does not exist, create it as a regular file.
  // O_TRUNC: If the file aready exists and is a regular file and the access
  // mode allows writing (i.e. is O_RDWR or O_WRONLY) it will be truncated to
  // length 0.
  // O_WRONLY: Write only.
  // S_IRWXU: User has read, write, and, execute permission.
  int fd = open("./2.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

  char buf[64];
  int length = snprintf(buf, sizeof(buf), "[%d] PARENT WRITE DIS.\n", getpid());
  size_t bytes_written = write(fd, buf, length);

  if (bytes_written < 0 || bytes_written != length) {
    fprintf(stderr, "write() failed\n");
    return -1;
  }

  // Create new process.
  int rc = fork();

  // Parent and child runs this code.
  length =
      snprintf(buf, sizeof(buf), "[%d] MUST BE WRITTEN TWICE.\n", getpid());
  bytes_written = write(fd, buf, length);

  if (bytes_written < 0 || bytes_written != length) {
    fprintf(stderr, "write() failed\n");
    return -1;
  }

  if (rc < 0) {
    fprintf(stderr, "fork() failed\n");
    return -1;
  } else if (rc == 0) {
    // Child path.
    length = snprintf(buf, sizeof(buf), "[%d] CHILD WRITE DIS.\n", getpid());
    bytes_written = write(fd, buf, length);

    if (bytes_written < 0 || bytes_written != length) {
      fprintf(stderr, "write() failed\n");
      return -1;
    }
  } else {
    // Parent path.
    length =
        snprintf(buf, sizeof(buf), "[%d] PARENT WRITE DIS AGAIN.\n", getpid());
    bytes_written = write(fd, buf, length);

    if (bytes_written < 0 || bytes_written != length) {
      fprintf(stderr, "write() failed\n");
      return -1;
    }

    wait(NULL);
  }

  return 0;
}

// Thoughts
// Both parent and the child can access the file descriptor returned by open().
// I guess we can say that the child also inherited the descriptors that the
// parent owned before fork().
//
// I don't think we can write at it at the same time per se, the OS still
// handles the write sequence. We don't have much power to control it unless we
// use some synchronization primitives... i think.