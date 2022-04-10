#include "tape.h"

#include <stdio.h>
#include <stdlib.h>

tape_t *tape_create(symbol_t symbol, tape_t *left, tape_t *right) {
  tape_t *tape = malloc(sizeof(tape_t));
  tape->symbol = symbol;

  if (left != NULL) {
    tape->left = left;
    left->right = tape;
  }

  if (right != NULL) {
    tape->right = right;
    right->left = tape;
  }

  return tape;
}

void tape_destroy(tape_t *tape) {
  tape_t *current;

  // destroy nodes to the right
  current = tape;
  while (current != NULL) {
    tape_t *next = current->right;
    free(current);
    current = next;
  }

  // destroy nodes to the left
  current = tape;
  while (current != NULL) {
    tape_t *next = current->left;
    free(current);
    current = next;
  }
}

tape_t *tape_from_input(symbol_t *input, size_t size) {
  if (size == 0)
    return NULL;

  tape_t *head = tape_create(input[0], NULL, NULL);

  tape_t *current = head;
  for (size_t i = 1; i < size; i++) {
    current = tape_create(input[i], current, NULL);
  }

  return head;
}

void tape_print(tape_t *tape) {
  if (tape == NULL)
    return;

  tape_t *current = tape;

  // find start of the tape
  while (current->left != NULL)
    current = current->left;

  // print tape
  while (current != NULL) {
#ifdef UNICODE
    printf("S\xe2\x82%c", (char)(0x80 + current->symbol));
#else
    printf("S%u", current->symbol);
#endif // UNICODE

    current = current->right;
  }

  printf("\n");
}
