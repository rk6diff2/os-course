//
// Created by starman on 22.09.2019.
// Variant: 14
// Text:
// Разработать программу транслитерации текста, которая заменяет нам все символы
// кириллицы созвучными символами латинского алфавита. Исхоный текст должен быть
// кодирован в КОИ-8. Программа должна получать исходный текст из потока
// стандартного ввода и помещать результат в поток стандартного вывода.
//

#include "include/utils.h"
#include "include/encoding.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  unsigned char* input_str = NULL;
  if (input(&input_str)) {
    return EXIT_FAILURE;
  }
  char* encoded_str = NULL;
  if (koi_to_cp1251(&input_str, &encoded_str)) {
    free(input_str);
    return EXIT_FAILURE;
  }
  printf("%s\n", encoded_str);
  free(input_str);
  free(encoded_str);
  return EXIT_SUCCESS;
}