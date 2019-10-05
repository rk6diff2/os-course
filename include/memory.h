//
// Created by starman on 05.10.2019.
//

#ifndef LAB2_MEMORY_H
#define LAB2_MEMORY_H
#include <stddef.h>
typedef char ALIGN[16];


union header {
  struct {
    size_t size;
    unsigned is_free;
    union header *next;
  } s;
  ALIGN stub;
};
typedef union header header_t;

header_t *head, *tail;

header_t *get_free_block(size_t size);
void *new(size_t size);
void delete(void *block);

#endif // LAB2_MEMORY_H
