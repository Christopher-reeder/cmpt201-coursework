#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_HISTORY 5

void print_history(char *history[], int count) {
  for (int i = 0; i < count; i++) {
    if (history[i] != NULL) {
      printf("%s", history[i]);
    }
  }
}

void update_history(char *history[], int *count, char *new_input) {
  if (*count < MAX_HISTORY) {
    history[*count] = new_input;
    (*count)++;
  } else {
    free(history[0]);
    for (int i = 0; i < MAX_HISTORY - 1; i++) {
      history[i] = history[i + 1];
    }
    history[MAX_HISTORY - 1] = new_input;
  }
}

int main() {
  char *history[MAX_HISTORY] = {NULL};
  int count = 0;

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while (1) {
    printf("Enter input: ");
    nread = getline(&line, &len, stdin);

    if (nread == -1) {
      break;
    }

    char *entry = strdup(line);

    if (strcmp(entry, "print\n") == 0) {
      print_history(history, count);
    } else {
      update_history(history, &count, entry);
    }
  }

  for (int i = 0; i < count; i++) {
    free(history[i]);
  }
  free(line);

  return 0;
}
