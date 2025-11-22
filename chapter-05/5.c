/**
 * Now write a program that uses wait() to wait for the child process to finish
 * in the parent. What does wait() return? What happens if you use wait in the
 * child?
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
    fprintf(stdout, "[%d] in child process\n", getpid());
    int wait_return = wait(NULL);
    fprintf(stdout, "[%d] in child process. wait_return: %d\n", getpid(),
            wait_return);

  } else {
    // parent path.
    int wait_return = wait(NULL);
    fprintf(stdout, "[%d] in parent process. wait_return: %d\n", getpid(),
            wait_return);
  }

  return 0;
}

// Thoughts
// In the parent, wait retuns the PID of the process it waited for, in this case
// the child process it created.
//
// Calling wait on a child process seems to result in it failing since it
// returns the value of -1. I also experimented calling wait in the parent
// process without the call to fork (so technically it is not a parent), the
// call to wait also fails. I think the general answer is that wait will return
// -1 or a failure of there is nothing for it to wait for.

// Refined:
// In the parent process, wait() returns tha PID of the child that terminated.
//
// If the child calls wait(), the call fails and returns -1 because the child
// has no children of its own. The same happens if a process with no children
// calls wait(). In these cases, errno is set to ECHILD.