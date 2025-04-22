#include <httpBuilder.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utils.h>

// For convenience
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

// The socket's port
int port = 8080;

int main() {
  // server_fd: server file descriptor -> for listening socket
  // client_fd: client file descriptor -> for a connected client
  int server_fd, client_fd;

  // server_addr: holds the information about the server's IP address
  // (obviously)
  // sockaddr_in: IPv4 address. It's the internet socket address
  // structure
  sockaddr_in server_addr;

  // buffer: A place to store the the incoming HTTP request data from the client
  char buffer[4096];

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
  server_addr.sin_port = htons(port);

  // Bind socket to the address
  // bind() tells the OS to use this port to accept incoming connections
  if (bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
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

  printf("HTTP server listening on http://localhost:%d\n", port);

  while (1) {
    // Accept incoming connection
    // Return a file descriptor for a new socket
    // NULL means we don't care about the client's address info
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
      perror("accept failed");
      continue;
    }
    // Read the request
    // Read incoming data from the client into buffer
    // sizeof(buffer) - 1 to leave space for null-terminator '\0'
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
      perror("read failed");
      close(client_fd);
      continue;
    }

    // Null-terminate the buffer so it behaves like a C string
    buffer[bytes_read] = '\0';
    printf("Received request:\n%s\n", buffer);

    // Read html file into buffer
    char *content = NULL;
    long size = 0;
    readFile(&content, &size, "./resources/templates/home.html");

    // Build HTTP response header
    char header[512];
    htmlBuilder(header, size);

    // Send header + html content
    send(client_fd, header, strlen(header), 0);
    send(client_fd, content, size, 0);

    /*
    // Raw http response
    const char *response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/plain\r\n"
                           "Content-Length: 13\r\n"
                           "\r\n"
                           "Hello, world!";

    // Send the response to the client
    write(client_fd, response, strlen(response));
    */

    // Close connection after responding
    close(client_fd); // Close connection after responsding
    free(content);
    content = NULL;
  }

  close(server_fd);
  return EXIT_SUCCESS;
}
