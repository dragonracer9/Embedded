/**
 * @file test_poti.c
 * @brief Verify potentiometer.
 * @author Philipp Schilk 2023
 */

#include "test_poti.h"
#include "barchart.h"
#include "main.h"
#include <inttypes.h>
#include <stdio.h>

bool validate_position(uint32_t min, uint32_t max, char *label) {
  unsigned int count = 0;
  while (1) {
    HAL_Delay(200);
    uint32_t i = HAL_ADC_GetValue(&hadc1);
    print_bar_char(i * 20 / 4095, 20);
    printf(" %" PRIu32 "/4095\r\n", i);
    printf(" %s\r\n", label);

    if (i < max && i >= min) {
      return true;
    }
    if (count > 60) {
      printf("Timeout!\r\n");
      return false;
    }
    count++;
  }
}

bool test_poti(void) {

  // Start ADC:
  if (HAL_ADC_Start(&hadc1) != HAL_OK) {
    printf("Failed to start ADC.\r\n");
    return false;
  }

  // Test minimum:
  printf("Turn poti to minimum position...\r\n");
  if (!validate_position(0, 50, "^")) {
    HAL_ADC_Stop(&hadc1);
    return false;
  }

  // Test middle:
  printf("Turn poti to middle position...\r\n");
  if (!validate_position(1800, 2200, "        ^^^")) {
    HAL_ADC_Stop(&hadc1);
    return false;
  }

  // Test maximum:
  printf("Turn poti to maximum position...\r\n");
  if (!validate_position(4000, 4096, "                   ^")) {
    HAL_ADC_Stop(&hadc1);
    return false;
  }

  HAL_ADC_Stop(&hadc1);

  return true;
}
