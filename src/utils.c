//
// Created by starman on 22.09.2019.
//

#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t BUF_SIZE = 256;

int input(unsigned char** string) {
  unsigned char *str = (unsigned char *)calloc(1, 1);
  if (!str) {
    free(str);
    return EXIT_FAILURE;
  }

  char buffer[BUF_SIZE];
  while (fgets(buffer, BUF_SIZE, stdin)) {
    unsigned char *str_ptr = (unsigned char *)realloc(str, strlen(str) + 1 + strlen(buffer));
    if (!str_ptr) {
      free(str);
      return EXIT_FAILURE;
    } else {
      str = str_ptr;
    }
    strncat(str, buffer, BUF_SIZE);
  }
  *string = str;
  return EXIT_SUCCESS;
}
