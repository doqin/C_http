#ifndef COROUTINE_H
#define COROUTINE_H

#include <setjmp.h>
#include <stdbool.h>

#define STACK_SIZE 1024 * 4 // 64KB stack

typedef struct coroutine {
  jmp_buf context;          // Saved state of this coroutine
  jmp_buf caller;           // State of whoever resumed this coroutine
  void (*function)(void *); // The actual function to run
  void *arg;                // Arguments to pass to the function
  unsigned char *stack;     // Dedicated stack (faking it)
  bool started;             // Has this coroutine started?
  bool finished;            // Has it returned?
} coroutine;

// Creating a coroutine.
// Allocate memory.
// Store function and arguments.
// Allocate a fake stack (manual control for realism).
coroutine *coro_create(void (*func)(void *), void *arg);

// Resuming a coroutine.
// Check if it's started:
// - If not -> switch to its stack.
// - If yes -> longjmp into the coroutine.
// Save the caller's state so it can yield back.
void coro_resume(coroutine *coro);

// Yielding control
// The coroutine decides when to pause and hand control back
void coro_yield();

// Free the allocation for the coroutine
void coro_destroy(coroutine *coro);

#endif
