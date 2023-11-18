/**
 * @file application.c
 * @brief RTOS Threads exercise.
 * @author Philipp Schilk, 2023
 */

#include "application.h"
#include "main.h"
#include "tx_api.h"
#include "tx_user.h"

// ==== Definitions/Globals ====================================================

TX_THREAD thread1;
#define THREAD1_STACK_SIZE_BYTES 1024
ULONG thread1_stack[(THREAD1_STACK_SIZE_BYTES + sizeof(ULONG) - 1) / sizeof(ULONG)];

// TODO: Create a thread control block and 1024-byte stack for thread 2:
TX_THREAD thread2;
#define THREAD2_STACK_SIZE_BYTES 1024
ULONG thread2_stack[(THREAD2_STACK_SIZE_BYTES + sizeof(ULONG) - 1) / sizeof(ULONG)];

// ==== UTILS ==================================================================

static ULONG util_ms_to_ticks(ULONG ms) {
  // TODO: Convert a duration in milliseconds to a number of RTOS ticks.
  // HINT: Use TX_TIMER_TICKS_PER_SECOND.
  return ms * ((TX_TIMER_TICKS_PER_SECOND + 999) / 1000);
}

// ==== THREAD 1 ===============================================================

static void thread1_entry(ULONG parameter) {

  UNUSED(parameter);

  uint64_t count = 0;
  while (1) {
    count++;
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

    // TODO: Put the current thread to sleep for 100ms:
    tx_thread_sleep(util_ms_to_ticks(100));

    // TODO: In the 10th loop iteration, start/resume thread 2:
    if (count == 10 && tx_thread_resume(&thread2) != TX_SUCCESS) {
      while (1) {
      }
    }
    // TODO: In the 20th loop iteration, terminate thread 2:
    if (count == 20 && tx_thread_terminate(&thread2) != TX_SUCCESS) {
      while (1) {
      }
    }
  }
}

// ==== THREAD 2 ===============================================================

// TODO: Implement the entry function for thread 2.
//       Thread 2 should do the following in an infinite loop:
//          - Sleep for 50ms
//          - Toggle LED2
static void thread2_entry(ULONG parameter) {

  UNUSED(parameter);

  while (1) {
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

    // TODO: Put the current thread to sleep for 50ms:
    tx_thread_sleep(util_ms_to_ticks(50));
  }
}

// ==== APPLICATION DEFINE =====================================================

UINT application_define(void) {

  UINT ret = TX_SUCCESS;
  UINT ret2 = TX_SUCCESS;

  ret = tx_thread_create(&thread1,                 // Pointer to thread handle.
                         "MyThread 1",             // Thread name.
                         &thread1_entry,           // Thread entry function.
                         0,                        // Thread entry function argument "parameter".
                         thread1_stack,            // Thread stack.
                         THREAD1_STACK_SIZE_BYTES, // Thread stack size (in bytes).
                         0,                        // Thread priority.
                         0,                        // Preemption threshold (Disabled).
                         TX_NO_TIME_SLICE,         // Time-slicing (Disabled).
                         TX_AUTO_START);           // Start thread immediately.
  if (ret != TX_SUCCESS) {
    return ret;
  }

  // TODO: Create thread 2. Use the same settings as thread 1, but do NOT
  //       have it start automatially. Make sure to use the correct handle,
  //       stack, and entry funciton.
  ret2 = tx_thread_create(&thread2,                 // Pointer to thread handle.
                          "MyThread 2",             // Thread name.
                          &thread2_entry,           // Thread entry function.
                          0,                        // Thread entry function argument "parameter".
                          thread2_stack,            // Thread stack.
                          THREAD2_STACK_SIZE_BYTES, // Thread stack size (in bytes).
                          0,                        // Thread priority.
                          0,                        // Preemption threshold (Disabled).
                          TX_NO_TIME_SLICE,         // Time-slicing (Disabled).
                          TX_DONT_START);           // Start thread manually.

  if (ret2 != TX_SUCCESS) {
    return ret2;
  }

  return TX_SUCCESS;
}
