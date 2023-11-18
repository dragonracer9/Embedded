/**
 * @file application.c
 * @brief RTOS stack overflow exercise.
 * @author Philipp Schilk, 2023
 */

#include "application.h"
#include "main.h"
#include "tx_api.h"
#include "tx_user.h"
#include <inttypes.h>
#include <math.h>
#include <stdio.h>

// ==== Definitions/Globals ====================================================

TX_THREAD thread1;
#define THREAD1_STACK_SIZE_BYTES 1024
ULONG thread1_stack[(THREAD1_STACK_SIZE_BYTES + sizeof(ULONG) - 1) / sizeof(ULONG)];

// ==== THREAD 1 ===============================================================

uint32_t fib(uint32_t n) {
  // Sleep for 1 tick.
  // This is required for the stack-overflow detection to trigger, since the RTOS
  // only checks the stack height when suspending or resuming a task.
  tx_thread_sleep(1);

  // TODO: Implement the fibonacci function *recursiely*:
  //           fib(0) = 0
  //           fib(1) = 1
  //           fib(n) = fib(n-1) + fib(n-2), n >= 2
  if (n == 0) {
    return 0;
  } else if (n == 1) {
    return 1;
  } else {
    return fib(n - 1) + fib(n - 2);
  }
}
static uint32_t n = 0;
static void thread1_entry(ULONG parameter) {
  UNUSED(parameter);
  for (; n <= 20; n++) {
    printf("fib(%02" PRIu32 ") = %" PRIu32 "\r\n", n, fib(n));
  }
}

// ==== Stack Overflow Handling ================================================

// TODO Implement a stack-error handler that prints an error message and then
//      gets stuck in an infinite loop.
// HINT: The function must return void and take a TX_THREAD pointer as an argument.
__NO_RETURN
void stack_error_handler(TX_THREAD *ptr) {
  UNUSED(ptr);
  float phi = (1 + sqrt(5)) / 2;
  float fibonacci = round(pow(phi, n) / sqrt(5));
  printf("LMAO, the stack did overflow xP\r\n(btw, at n = %" PRIu32 ", where fib would've been: %g)\r\nbreh\r\n", n, fibonacci);
  while (1) {
  }
}
// ==== APPLICATION DEFINE =====================================================

UINT application_define(void) {

  UINT ret = TX_SUCCESS;

  // TODO: Register your stack error handler with ThreadX.
  tx_thread_stack_error_notify(&stack_error_handler);

  ret = tx_thread_create(&thread1,                 // Pointer to thread handle.
                         "StackSmasher",           // Thread name.
                         &thread1_entry,           // Thread entry function.
                         0,                        // Thread entry function argument "parameter".
                         thread1_stack,            // Thread stack.
                         THREAD1_STACK_SIZE_BYTES, // Thread stack size (in bytes).
                         0,                        // Thread priority.
                         0,                        // Preemption threshold.
                         TX_NO_TIME_SLICE,         // Time-slicing.
                         TX_AUTO_START);           // Start thread immediately.
  if (ret != TX_SUCCESS) {
    return ret;
  }

  return TX_SUCCESS;
}
