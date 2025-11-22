/**
 * Write a program that calls fork(). Before calling fork(), have the main
 * access a variable (e.g. x) and set its value to something (e.g. 100).
 * What value is the variable in the child process? What happens to the
 * variable when both the child and parent change the value of x?
 */

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int x = 100;

  // Calling form, the OS creates a new process that is an almost exact copy of
  // the parent.
  int rc = fork();

  // At this point both the parent and the child have its own 'x' variable
  // with a value of 100.

  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    return -1;
  } else if (rc == 0) {
    // Child code path.
    fprintf(stdout, "[child]: x is %d\n", x);
    fprintf(stdout, "[child]: changing x to 69\n");
    x = 69; // This only changes the 'x' of the child, not the parent.
    fprintf(stdout, "[child]: x is %d\n", x);
  } else {
    // Parent code path.
    wait(NULL);

    // This would remain as 100.
    fprintf(stdout, "[parent]: x is %d\n", x);
  }

  return 0;
}

// Thoughts
// Observing the code behavior, it seems like that the parent and the child has
// its own copy of the variable. This means that whatever the child does to the
// variable, it happens independently and does not affect the variable in the
// parent (this goes vice versa).
//
// Another key point I also observed is the concept of  "common code" if we do
// not use if-else branching for checking the return value of fork, then the
// instructions/code will run on both the child and the parent.