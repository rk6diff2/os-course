//
// Created by starman on 22.09.2019.
//

#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t BUF_SIZE = 256;

int input(char** string) {
  char *str = (char *)calloc(1, 1);
  if (!str) {
    free(str);
    return EXIT_FAILURE;
  }

  char buffer[BUF_SIZE];
  while (fgets(buffer, BUF_SIZE, stdin)) {
    char *str_ptr = (char *)realloc(str, strlen(str) + 1 + strlen(buffer));
    if (!str_ptr) {
      free(str);
      return EXIT_FAILURE;
    } else {
      str = str_ptr;
    }
    strncat(str, buffer, BUF_SIZE);
  }
  *string = str;
  printf("%s", *string);
  return EXIT_SUCCESS;
}
