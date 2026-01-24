#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *line_buff = NULL;
  size_t size = 0;
  ssize_t num_char;

  while (1) {
    printf("Please enter some text:");

    num_char = getline(&line_buff, &size, stdin);

    if (num_char == -1) {
      printf("Error");
      break;
    }

    printf("Tokens:\n");

    char *saveptr;
    char *token = strtok_r(line_buff, " ", &saveptr);

    while (token != NULL) {
      printf("%s\n", token);
      token = strtok_r(NULL, " ", &saveptr);
    }
  }

  free(line_buff);
  return 0;
}
