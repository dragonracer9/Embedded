/**
 * @file test_leds.c
 * @brief Verify LED1/LED2.
 * @author Philipp Schilk 2023
 */

#include "test_leds.h"
#include "main.h"
#include <stdio.h>

bool test_leds(void) {
  printf("Playing LED pattern:\r\n");
  printf("LED1  LED2\r\n");
  printf(" X        \r\n");
  printf("        X \r\n");
  printf(" X      X \r\n");
  printf("          \r\n");
  printf("Hold UP to continue or DOWN to indicate error.\r\n");
  HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);

  size_t pattern_step = 0;

  while (1) {
    switch (pattern_step) {
      case 0:
        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
        HAL_Delay(300);
        break;
      case 1:
        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
        HAL_Delay(300);
        break;
      case 2:
        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
        HAL_Delay(300);
        break;
      case 3:
        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
        HAL_Delay(300);
        break;
    }

    pattern_step = pattern_step == 3 ? 0 : pattern_step + 1;

    if (!HAL_GPIO_ReadPin(BTN_DOWN_GPIO_Port, BTN_DOWN_Pin)) {
      HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
      return false;
    } else if (!HAL_GPIO_ReadPin(BTN_UP_GPIO_Port, BTN_UP_Pin)) {
      HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
      return true;
    }
  }
}
