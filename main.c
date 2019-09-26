//
// Created by starman on 23.09.2019.
// Variant: 14
// Text:
// Разработать программу копирования всех регулярных файлов текущего каталога
// в другой заданный каталог. Если указанный каталог копирования не существует,
// он должен создаваться автоматически. Копирование не должно производиться,
// если текущий и результирующий каталоги совпадают.
//

#include "include/utils.h"
#include "include/parser.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
  char* result_dir = NULL;
  if (parse_args(&result_dir, argc, argv)) {
    return ERROR;
  }

  if (check_or_create_path(result_dir)) {
    return ERROR;
  }

  char* cur_dir = ".";
  status result = copy_files(cur_dir, result_dir);
  if (result != OK) {
    fprintf(stderr, "An error has occurred. Return code %d\n"
                    "ERROR: %s\n", result, strerror(errno));
  }

  return result;
}
