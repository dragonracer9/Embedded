/**
 * @file test_temp.c
 * @brief Verify I2C temperature sensor communication.
 * @author Philipp Schilk 2023
 */
#include "test_temp.h"
#include "main.h"
#include <stdio.h>

bool test_temp(void) {

  HAL_StatusTypeDef err;

  uint8_t buf = 0;

  printf("Reading Temperature Sensor WHOAMI Register...\r\n");

  // Read WHOAMI (0x01) Reg:
  err = HAL_I2C_Mem_Read(&hi2c1, 0x7E, 0x1, 1, &buf, 1, 100);

  if (err != HAL_OK) {
    printf("HAL I2C Error!\r\n");
    return false;
  }

  printf("Expected WHOAMI value: 0xA0, Actual WHOAMI value: 0x%02X\r\n", buf);

  if (buf != 0xA0) {
    printf("WHOAMI Value Wrong!\r\n");
    return false;
  }

  return true;
}
