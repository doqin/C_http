#ifndef UTILS_H
#define UTILS_H

// File reader wrapper for convenience.
// char *content: Pass your content variable to get the content from the file.
// long *size: Address to your size variable to get the size of the content.
// char *directory: Directory to your file.
// --MAKE SURE TO FREE YOUR CONTENT VARIABLE--
void readFile(char **content, long *size, char *directory);

#endif
