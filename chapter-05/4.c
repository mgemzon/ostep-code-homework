/**
 * Write a program that calls fork() and then calls some form of exec to run the
 * program /bin/ls. See if you can try all of the variants of exec, including
 * (on Linux) execl(), execle(), execlp(), execv(), execvp(), and execvpe(). Why
 * do you think there are many variants of the same basic call?
 */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
  } else if (rc == 0) {
    // child path.
    // execl("/bin/ls", "/bin/ls", NULL);

    // execl(const char *path, const char *arg, ...)
    // execlp(const char *file, const char *arg, ...)
    // execle(const char *path, const char *arg, ...)

    // execv(const char *path, char *const *argv);
    // execvp(const char *file, char *const *argv)
    // execve(const char *path, char *const *argv, char *const *envp);
  } else {
    // parent path.
  }

  return 0;
}

// Thoughts
// execv:
// execl:
// execvp:
// execlp:
// I think there are many variants exec (which ends up doing the same thing,
// replacing current program of the process) due to application developer needs.
// For example, they might not have exact information about the file path of the
// executable they want to run, so they could rely on the 'p' flag to leverage
// the path environment variable.
//
// Maybe they really need to be specific on which executable to run, for example
// there are many versions of the executable in the system, they could provide
// the exact one by providing the full path.
//
// For 'l' and 'v', I really think this is for preference of the developer
// either he finds it more convenient. For example, the array one to be passed
// ('v' option) seems like a more flexible option when you do not know yet the
// full arguments to be passed to exec.
//
// Another POV. I think the reason why there are many variants is that the UNIX
// developers created new exec variants, they do not want to break other
// peoples' code so instead of modifying the API, they just extended it.