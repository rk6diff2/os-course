//
// Created by starman on 26.09.2019.
//

#include "include/parser.h"
#include <stdio.h>

status parse_args(char **path, int argc, char **argv) {
  if (argc < 2) {
    *path = NULL;
    fprintf(stderr, "Usage: ./lab2 [path/to/dir]\n");
    return ERROR;
  }
  *path = argv[1];
  return OK;
}
