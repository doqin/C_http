#include <handleClient.h>
#include <httpBuilder.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utils.h>

void *handleClient(void *arg) {
  int client_fd = *(int *)arg;
  free(arg);

  char buffer[1024] = {0};

  // Read the request
  // Read incoming data from the client into buffer
  // sizeof(buffer) - 1 to leave space for null-terminator '\0'
  ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
  if (bytes_read < 0) {
    perror("read failed");
    close(client_fd);
    return NULL;
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

  // Close connection after responding
  close(client_fd); // Close connection after responsding
  free(content);
  content = NULL;

  return NULL;
}
