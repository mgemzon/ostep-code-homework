/**
 * Write a program that creates a child process, and then in the child closes
 * standard output (STDOUT_FILENO). What happens if the child calls printf() to
 * print some output after closing the descriptor?
 */

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // close(STDOUT_FILENO); // trying to close before creating child.
  int rc = fork();

  printf("[%d] hello!\n", getpid());

  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
  } else if (rc == 0) {
    // child path.
    printf("[%d] child about to close STDOUT_FILENO.\n", getpid());
    close(STDOUT_FILENO);
    fprintf(stdout, "[%d] child trying to print after closing.\n", getpid());
    perror("fprintf stdout error");
    fprintf(stderr, "[%d] child trying to print after closing in stderr.\n",
            getpid());
  } else {
    // parent path.
    wait(NULL);
    printf("[%d] parent says hello!\n", getpid());
  }

  return 0;
}

// Thoughts
// After calling close on standard output in the child, the child is not able to
// print using printf. However, the parent is not affected from the child
// closing STDOUT. I think this is because the parent and the child maintains
// their own file descriptor table so changing it in the child does not affect
// the parent.
//
// I also did an experiment where I call close on STDOUT before calling fork, in
// this case the parent and the child does not print anything when doing
// printf(). This makes sense because the child copies whatever the file
// descriptor table of the parent is at the point of its creation (at fork()).