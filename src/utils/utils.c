#include <stdio.h>
#include <stdlib.h>
#include <utils.h>

void readFile(char **content, long *size, char *directory) {
  FILE *file = fopen(directory, "r");
  if (!file) {
    perror("Failed to open HTML file");
    *content = NULL;
    *size = 0;
    return;
  }

  fseek(file, 0, SEEK_END);
  *size = ftell(file);
  rewind(file);

  *content = malloc(*size);
  if (!*content) {
    perror("Memory allocation failed");
    fclose(file);
    return;
  }

  fread(*content, 1, *size, file);
  fclose(file);
}
