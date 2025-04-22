#include <coroutine.h>
#include <setjmp.h>
#include <stdlib.h>

static coroutine *current = NULL;

static void trampoline();

coroutine *coro_create(void (*func)(void *), void *arg) {
  coroutine *coro = malloc(sizeof(coroutine));
  coro->function = func;
  coro->arg = arg;
  coro->started = false;
  coro->finished = false;
  coro->stack = malloc(STACK_SIZE);
  return coro;
}

void coro_resume(coroutine *coro) {
  if (coro->finished)
    return;

  if (!coro->started) {
    coro->started = true;

    if (setjmp(coro->caller) == 0) {
      current = coro;

      // Fake stack swap
      trampoline();
    }
  } else {
    if (setjmp(coro->caller) == 0) {
      current = coro;
      longjmp(coro->context, 1);
    }
  }
}

void coro_yield() {
  if (setjmp(current->context) == 0) {
    longjmp(current->caller, 1);
  }
}

static void trampoline() {
  current->function(current->arg);
  current->finished = true;
  coro_yield(); // back to caller
}

void coro_destroy(coroutine *coro) {
  free(coro->stack);
  free(coro);
}
