/*
 * @file self_test.c
 * @brief PBLShield Selftest.
 * @author Philipp Schilk 2023
 */

#include "main.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "test_buttons.h"
#include "test_imu.h"
#include "test_leds.h"
#include "test_matrix.h"
#include "test_mics.h"
#include "test_poti.h"
#include "test_temp.h"

// ==== Prototypes/Defines =====================================================

// Tests:
const struct test tests[] = {
    // Tests:
    {.name = "Buttons", .run_test = &test_buttons},         // Test all buttons
    {.name = "Poti", .run_test = &test_poti},               // Test potentiometer
    {.name = "LEDs", .run_test = &test_leds},               // Test basic LEDs
    {.name = "Matrix", .run_test = &test_matrix},           // Test LED Matrix
    {.name = "Temperature Sensor", .run_test = &test_temp}, // Test Temp Sensor
    {.name = "IMU", .run_test = &test_imu},                 // Test IMU
    {.name = "Mics", .run_test = &test_mics},               // Test Microphones
};

#define TEST_COUNT (sizeof(tests) / sizeof(struct test))

// ==== Main Test Routine ======================================================

void run_self_test(void) {
  bool results[TEST_COUNT] = {0};

  // Print header:
  printf("\r\n\r\n\r\n\r\n\r\n");
  printf("PBL Shield Self Test\r\n");
  printf("    Hardware rev: 1.3\r\n");
  printf("    schilkp, 2023\r\n");
  printf("\r\n\r\n");

  // Run all tests:
  for (size_t i = 0; i < TEST_COUNT; i++) {
    int test_num = i + 1;
    printf("==== Test %i/%i: %s =====\r\n", test_num, TEST_COUNT, tests[i].name);
    results[i] = tests[i].run_test();

    if (results[i]) {
      printf("OK!\r\n");
    } else {
      printf("ERROR!\r\n");
    }
  }

  // Print Summary:
  printf("==== SUMMARY ====\r\n");
  size_t error_count = 0;
  for (size_t i = 0; i < TEST_COUNT; i++) {
    if (results[i]) {
      printf("%02i %s:OK\r\n", i + 1, tests[i].name);
    } else {
      printf("%02i %s:ERROR!\r\n", i + 1, tests[i].name);
      error_count++;
    }
  }
  printf("\r\n");
  printf("OK:    %*i\r\n", 4, TEST_COUNT - error_count);
  printf("ERROR: %*i\r\n", 4, error_count);
  printf("TOTAL: %*i\r\n", 4, TEST_COUNT);

  // Hold:
  printf("Press ENTER button to restart..\r\n");
  while (HAL_GPIO_ReadPin(BTN_ENTER_GPIO_Port, BTN_ENTER_Pin)) {
    HAL_Delay(100);
  }
  while (!HAL_GPIO_ReadPin(BTN_ENTER_GPIO_Port, BTN_ENTER_Pin)) {
    HAL_Delay(100);
  }
  HAL_Delay(100);
}
