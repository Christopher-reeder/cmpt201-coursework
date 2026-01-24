#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *line;
  size_t buff_size;
  ssize_t reader;

  while (1) {
    printf("Enter programs to run.\n>");

    reader = getline(&line, &buff_size, stdin);
    if (reader == -1) {
      break;
    }

    if (line[reader - 1] == '\n') {
      line[reader - 1] = '\0';
    }

    pid_t pid = fork();

    if (pid < -1) {
      // error
      perror("fork failed");
    } else if (pid == 0) {
      // child
      if (execl(line, line, NULL) == -1) {
        printf("Exec failure\n");
        exit(1);
      }
    } else {
      // parent
      int status;
      waitpid(pid, &status, 0);
    }
  }
  free(line);
  return 0;
}
