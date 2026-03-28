/*
Questions to answer at top of server.c:
(You should not need to change client.c)

Understanding the Client:
1. How is the client sending data to the server? What protocol?
  It uses TCP(Transmission Control Protocol) to send data. It uses SOCK_STREAM
to create the socket.
2. What data is the client sending to the server?
  It is sending the strings that are saved in array which is "Hello", "Apple",
"Car", "Green", "Dog"

Understanding the Server:
1. Explain the argument that the `run_acceptor` thread is passed as an argument.
It passes the pointer of struct acceptor_args structure. This structure contains
a thread safe atomic_bool run flag to control the loop, a list_handle to store
received data, and a list_lock to make sure synchronized access is happening to
the shared list.
2. How are received messages stored?
It is stored in a Singly Linked List by dynamically allocating a struct
list_node. It copies the message into it, and appends it to the list using the
Notes().
3. What does `main()` do with the received messages?
After waiting for all expected messages, main() calls collect_all() which
traverses the linked list, prints each "Collected" message to the console, and
then frees the dynamically allocated memory for both the data and the nodes.
4. How are threads used in this sample code?
Acceptor Thread: Runs run_acceptor to keep listening for and accept new incoming
client connections without blocking. Worker Threads: A new thread is spawned to
run run_client which allows the server to handle multiple clients in parallel,
reading data from each socket separately.
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8001
#define BUF_SIZE 1024
#define ADDR "127.0.0.1"

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

#define NUM_MSG 5

static const char *messages[NUM_MSG] = {"Hello", "Apple", "Car", "Green",
                                        "Dog"};

int main() {
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
    handle_error("inet_pton");
  }

  if (connect(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    handle_error("connect");
  }

  char buf[BUF_SIZE];
  for (int i = 0; i < NUM_MSG; i++) {
    sleep(1);
    // prepare message
    // this pads the desination with NULL
    strncpy(buf, messages[i], BUF_SIZE);

    if (write(sfd, buf, BUF_SIZE) == -1) {
      handle_error("write");
    } else {
      printf("Sent: %s\n", messages[i]);
    }
  }

  exit(EXIT_SUCCESS);
}
