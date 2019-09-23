//
// Created by starman on 22.09.2019.
//
#include "include/encoding.h"
#include <stdlib.h>
#include <string.h>

int koi_to_cp1251(unsigned char *const *string, char **res_str) {
  int i = 0;
  char* str_ptr = (char*) malloc(strlen((char*)*string) + 1);
  if (!str_ptr) {
    return EXIT_FAILURE;
  }

  do {
    if (193 <= (int)(*string)[i] && (int)(*string)[i] <= 223) {
      str_ptr[i] = (char) ((*string)[i] - 96);
    } else if (225 <= (int)(*string)[i] && (int)(*string)[i] <= 255) {
      str_ptr[i] = (char) ((*string)[i] - 160);
    } else {
      str_ptr[i] = (char)(*string)[i];
    }
    ++i;
  } while ((*string)[i] != '\0');
  *res_str = str_ptr;
  return EXIT_SUCCESS;
}
