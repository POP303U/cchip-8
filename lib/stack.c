#include "stack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Minimal implementation of a stack in C for the Chip8 architecture
Stack *Chip8StackInit() {
  Stack *stack = malloc(sizeof(Stack));

  if (stack == NULL) {
    fprintf(stderr, "Chip8StackInit: could not alloc");
  }

  memset(stack, 0, sizeof(Stack));

  stack->SP = 0;
  stack->size = SIZE;

  return stack;
}

// Push a element on top of the stack
void Chip8StackPush(Stack *stack, uint8_t num) {
  // If this actually happens the rom should be at fault
  if (stack->SP == stack->size - 1) {
    fprintf(stderr, "Chip8StackPush: Stack Overflow");
  }

  stack->SP++;
  stack->arr[stack->SP] = num;
}

// Pop the topmost element from the stack
uint8_t Chip8StackPop(Stack *stack) {
  // If this actually happens the rom should be at fault
  if (stack->SP == 0) {
    fprintf(stderr, "Chip8StackPush: Stack Underflow");
  }

  // Take the top element out and shift top down
  uint8_t pop = stack->arr[stack->SP];
  stack->SP -= 1;

  return pop;
}
