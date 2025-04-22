#include <handleClient.h>
#include <httpBuilder.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utils.h>

// The socket's port
#define PORT 1504

int main() {
  // server_fd: server file descriptor -> for listening socket
  // client_fd: client file descriptor -> for a connected client
  int server_fd;

  // server_addr: holds the information about the server's IP address
  // (obviously)
  // sockaddr_in: IPv4 address. It's the internet socket address
  // structure
  struct sockaddr_in server_addr;
  socklen_t addrlen;

  // Create socket
  // AF_INIT: address family -> IPv4
  // SOCK_STREAM: stream socket (TCP)
  // 0: Use the default protocol (TCP for stream)
  // socket(int, int, int) -> returns a file descriptor or -1 if failed
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set address and port
  // .sin_family = AF_INET -> Using IPv4
  // .sin_addr.s_addr = INADDR_ANY -> bind to all available interfaces
  // (localhost, local IPs, etc.)
  // .sin_port = htons(port) -> set port number, use htons() to convert to
  // network byte order
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  server_addr.sin_port = htons(PORT);

  // Bind socket to the address
  // bind() tells the OS to use this port to accept incoming connections
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("bind failed");
    close(server_fd);
  }

  // Listen for incoming connections
  // 10: Max number of queued connections (backlog)
  if (listen(server_fd, 10) < 0) {
    perror("listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("HTTP server listening on http://localhost:%d\n", PORT);

  while (1) {
    // Accept incoming connection
    // Return a file descriptor for a new socket
    // NULL means we don't care about the client's address info
    int socket = accept(server_fd, (struct sockaddr *)&server_addr, &addrlen);
    if (socket < 0) {
      perror("accept failed");
      continue;
    }

    int *client_fd = malloc(sizeof(int));
    *client_fd = socket;

    pthread_t tid;
    pthread_create(&tid, NULL, handleClient, client_fd);
    pthread_detach(tid);
  }

  close(server_fd);
  return EXIT_SUCCESS;
}
