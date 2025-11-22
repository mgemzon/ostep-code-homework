/**
 * Write a slight modification of the previous program, this time using
 * waitpid() instead of wait(). When would waitpid() be useful?
 */

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
  } else if (rc == 0) {
    // child path.
    fprintf(stdout, "[%d] child\n", getpid());
  } else {
    // parent path.
    int wstatus;
    pid_t pid = waitpid(rc, &wstatus, 0);

    fprintf(stdout, "[%d] parent: waitpid return: %d wstatus: %d\n", getpid(), pid, wstatus);
  }

  return 0;
}

// Thoughts
// I think that wait() is just a simplified form of waitpid(), waitpid() is a
// more general API for waiting for a particular process.
//
// I guess you would be using waitpid() if you want to be more specific, such as
// when you have many children, with waitpid() you could specify on which child
// you wait for. You could also specify more on what state changes you want for
// waitpid() to stop waiting.