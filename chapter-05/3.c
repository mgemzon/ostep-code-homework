/**
 * Write another program using fork(). The child process should print "hello";
 * the parent process should print "goodbye". You should try to ensure that the
 * child process always prints first; can you do it without calling wait() in
 * parent?
 */

#include <stdio.h>
// #include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    return -1;
  } else if (rc == 0) {
    // Child code path.
    printf("[%d] hello\n", getpid());
  } else {
    // Parent code path.
    for (int i = 0; i < 1000000; i++) {
      ;
    }
    printf("[%d] goodbye\n", getpid());
  }

  return 0;
}

// Thoughts
// I achieved the child printing first by creating a busy loop inside the parent
// code path. Without that, it seems that it is always the parent printing
// "goodbye" first.
//
// I do realize that this is sort of resource intensive since we are keeping the
// process doing a for loop and doing nothing. I wonder how the wait() system
// call is implemented. Does it also implement a busy checking inside it like
// 'while(is_child_still_executing);'?