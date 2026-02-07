#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define BLOCK_SIZE 128
#define TOTAL_ALLOC 256

struct header {
  uint64_t size;
  struct header *next;
};

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                                       : *(void **)data);
  if (len < 0) {
    perror("snprintf");
    exit(1);
  }
  write(STDOUT_FILENO, buf, len);
}

int main() {
  void *start_ptr = sbrk(TOTAL_ALLOC);
  if (start_ptr == (void *)-1) {
    perror("sbrk");
    return 1;
  }

  struct header *first_block = (struct header *)start_ptr;
  struct header *second_block =
      (struct header *)((char *)start_ptr + BLOCK_SIZE);

  first_block->size = BLOCK_SIZE;
  first_block->next = NULL;

  second_block->size = BLOCK_SIZE;
  second_block->next = first_block;

  void *first_data = (char *)first_block + sizeof(struct header);
  void *second_data = (char *)second_block + sizeof(struct header);

  memset(first_data, 0, BLOCK_SIZE - sizeof(struct header));
  memset(second_data, 1, BLOCK_SIZE - sizeof(struct header));

  print_out("first block:       %p\n", &first_block, sizeof(struct header *));
  print_out("second block:      %p\n", &second_block, sizeof(struct header *));

  print_out("first block size:  %lu\n", &first_block->size, sizeof(uint64_t));
  print_out("first block next:  %p\n", &first_block->next,
            sizeof(struct header *));

  print_out("second block size: %lu\n", &second_block->size, sizeof(uint64_t));
  print_out("second block next: %p\n", &second_block->next,
            sizeof(struct header *));
}
