/**
 * @file application.c
 * @brief RTOS Thread competition/blocking exercise.
 * @author Philipp Schilk, 2023
 */

#include "application.h"
#include "main.h"
#include "tx_api.h"
#include "tx_user.h"

// ==== Definitions/Globals ====================================================

TX_THREAD thread_a;
#define THREAD_A_STACK_SIZE_BYTES 1024
ULONG thread_a_stack[(THREAD_A_STACK_SIZE_BYTES + sizeof(ULONG) - 1) / sizeof(ULONG)];

TX_THREAD thread_b;
#define THREAD_B_STACK_SIZE_BYTES 1024
ULONG thread_b_stack[(THREAD_B_STACK_SIZE_BYTES + sizeof(ULONG) - 1) / sizeof(ULONG)];

// ==== UTILS ==================================================================

static ULONG util_ms_to_ticks(ULONG ms) { return (ms * TX_TIMER_TICKS_PER_SECOND + 999) / 1000; }

// ==== THREAD 2 ===============================================================

static void thread_a_entry(ULONG parameter) {

  UNUSED(parameter);

  uint64_t count = 0;
  while (1) {
    count++;
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

    // TODO: Sleep for 100 ms (using the RTOS sleep function!)
    tx_thread_sleep(util_ms_to_ticks(100));

    // TODO: Start/resume thread b in the 10th loop iteration.
    if (count == 10 && tx_thread_resume(&thread_b) != TX_SUCCESS)
      while (1) {
      };
    // if ( != TX_SUCCESS) {
    //   //printf("ERROR resuming THREAD 2");
    //   // unfortuneately there is no way of notifying the user if this breaks
    // }
  }
}

// ==== THREAD B ===============================================================

static void thread_b_entry(ULONG parameter) {

  UNUSED(parameter);

  // TODO: Turn ON LED2, Delay for 5000ms using the HAL_Delay() function, Turn OFF LED2.
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
  for (int i = 0; i < 500; i++) {
    tx_thread_relinquish();
    HAL_Delay(10);
  }
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

  // After this, thread B finishes, as the entry function is allowed to return.
}

// ==== APPLICATION DEFINE =====================================================

UINT application_define(void) {

  UINT ret = TX_SUCCESS;

  ret = tx_thread_create(&thread_a,                 // Pointer to thread handle.
                         "Thread A",                // Thread name.
                         &thread_a_entry,           // Thread entry function.
                         0,                         // Thread entry function argument "parameter".
                         thread_a_stack,            // Thread stack.
                         THREAD_A_STACK_SIZE_BYTES, // Thread stack size (in bytes).
                         1,                         // Thread priority.
                         0,                         // Preemption threshold.
                         TX_NO_TIME_SLICE,          // Time-slicing.
                         TX_AUTO_START);            // Start thread immediately.
  if (ret != TX_SUCCESS) {
    return ret;
  }

  ret = tx_thread_create(&thread_b,                 // Pointer to thread handle.
                         "Thread B",                // Thread name.
                         &thread_b_entry,           // Thread entry function.
                         0,                         // Thread entry function argument "parameter".
                         thread_b_stack,            // Thread stack.
                         THREAD_B_STACK_SIZE_BYTES, // Thread stack size (in bytes).
                         2,                         // Thread priority.
                         0,                         // Preemption threshold.
                         TX_NO_TIME_SLICE,          // Time-slicing.
                         TX_DONT_START);            // Don't start thread immediately.
  if (ret != TX_SUCCESS) {
    return ret;
  }

  return TX_SUCCESS;
}
