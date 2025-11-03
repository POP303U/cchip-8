#include <stdint.h>
#define SIZE 64

#ifndef STACK_H_
#define STACK_H_

typedef struct Stack {
  uint8_t size;
  uint8_t SP;
  uint8_t arr[SIZE];

} Stack;

Stack *Chip8StackInit();
void Chip8StackPush(Stack *stack, uint8_t num);
uint8_t Chip8StackPop(Stack *stack);

#endif
