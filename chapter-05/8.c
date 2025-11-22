/**
 * Write a program that creates two children, and connects the standard output
 * of one to the standard input of the other, using the pipe() system call.
 */

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fds[2];

  // A pipe is a unidirectional data channel that can be used for interprocess
  // communication.
  if (pipe(fds) != 0) {
    perror("pipe creation failed");
    return -1;
  }

  // fds[0] refers to the read end of the pipe.
  printf("fds[0]: %d\n", fds[0]);

  // fds[1] refers to the write end of the pipe.
  printf("fds[1]: %d\n", fds[1]);
  fprintf(stdout, "[%d] da parent\n", getpid());
  int rc_first_fork = fork();

  if (rc_first_fork < 0) {
    fprintf(stderr, "fork failed\n");
    return -1;
  } else if (rc_first_fork == 0) {
    // first child path.
    fprintf(stdout, "[%d] first child. a read child.\n", getpid());
    close(fds[1]);
    dup2(fds[0], STDIN_FILENO);

    char buf[100];
    read(STDIN_FILENO, buf, sizeof(buf));
    fprintf(stdout,
            "[%d] read child: message received from write child \"%s\"\n",
            getpid(), buf);
  } else {
    int rc_second_fork = fork();

    if (rc_second_fork < 0) {
      fprintf(stderr, "fork failed\n");
      return -1;
    } else if (rc_second_fork == 0) {
      // second child path.
      fprintf(stdout, "[%d] second child. a write child\n", getpid());
      close(fds[0]);
      dup2(fds[1], STDOUT_FILENO);
      close(fds[1]);
      fprintf(stdout, "[%d] Hello from write child.", getpid());
    } else {
      waitpid(rc_first_fork, NULL, 0);
      waitpid(rc_second_fork, NULL, 0);
      fprintf(stdout, "[%d] da parent. closing da pipe.\n", getpid());
      close(fds[0]);
      close(fds[1]);
    }
  }

  return 0;
}

// What dup2(oldfd, newfd) actually does:
// It redirects writes to newfd so that they go to the same underlying thing as
// oldfd. dup2(3, STDOUT_FILENO) makes STDOUT point to the same kernel object as
// file descriptor 3.
//
// UNIX philosophy: Forcing all programs to always read from STDIN (FD 0), write
// to STDOUT (FD 1), and write errors to STDERR (FD 2) is what makes UNIX
// pipelines and redirection work UNIVERSALLY -- without modifying individual
// programs.
//
// The 'shell' handles the redirection, not the program. Program does not decide
// where their input/output goes. The shell or parent process decides wiring FDs
// before exec.
//
// UNIX philosophy: Do one thing well. Programs are components that can be
// chained together.
//
// cat file | grep foo | sort | wc -l
// This is possible only because they all read from fd 0 and write to fd 1.
