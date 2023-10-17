/**
 * @file test_imu.c
 * @brief Verify SPI IMU communication.
 * @author Philipp Schilk 2023
 */

#include "test_imu.h"
#include "main.h"
#include <stdio.h>

bool test_imu(void) {

  uint8_t buf = 0;
  printf("Reading IMU Sensor WHOAMI Register...\r\n");

  // Read WHOAMI (0x0F) Reg:
  HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);

  buf = 0x8F; // 0x80 | 0x0F == READ 0x0F
  HAL_SPI_Transmit(&hspi2, &buf, 1, 100);

  HAL_SPI_Receive(&hspi2, &buf, 1, 100);

  HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, GPIO_PIN_SET);

  printf("Expected WHOAMI value: 0x71, Actual WHOAMI value: 0x%02X\r\n", buf);

  if (buf != 0x71) {
    printf("WHOAMI Value Wrong!\r\n");
    return false;
  }

  return true;
}
