#include <httpBuilder.h>
#include <stdio.h>

void htmlBuilder(char *header, long size) {
  sprintf(header,
          "HTTP/1.1 200 OK\r\n"
          "Content-Type: text/html\r\n"
          "Content-Length: %ld\r\n"
          "\r\n",
          size);
}
