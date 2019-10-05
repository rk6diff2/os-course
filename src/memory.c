//
// Created by starman on 05.10.2019.
//

#include <include/memory.h>
#include <unistd.h>

header_t *get_free_block(size_t size) {
  header_t *curr = head;
  while (curr) {
    if (curr->s.is_free && curr->s.size >= size) {
      return curr;
    }
    curr = curr->s.next;
  }
  return NULL;
}

void *new (size_t size) {
  size_t total_size;
  void *block;
  header_t *header;
  if (!size) {
    return NULL;
  }
  header = get_free_block(size);
  if (header) {
    header->s.is_free = 0;
    return (void *)(header + 1);
  }
  total_size = sizeof(header_t) + size;
  block = sbrk(total_size);
  if (block == (void *)-1) {
    return NULL;
  }
  header = block;
  header->s.size = size;
  header->s.is_free = 0;
  header->s.next = NULL;
  if (!head) {
    head = header;
  }
  if (tail) {
    tail->s.next = header;
  }
  tail = header;
  return (void *)(header + 1);
}

void delete (void *block) {
  header_t *header, *tmp;

  if (!block) {
    return;
  }

  header = (header_t *)block - 1;

  void *program_break = sbrk(0);
  if ((char *)block + header->s.size == program_break) {
    if (head == tail) {
      head = tail = NULL;
    } else {
      tmp = head;
      while (tmp) {
        if (tmp->s.next == tail) {
          tmp->s.next = NULL;
          tail = tmp;
        }
        tmp = tmp->s.next;
      }
    }
    sbrk(0 - sizeof(header_t) - header->s.size);
    return;
  }
  header->s.is_free = 1;
}
