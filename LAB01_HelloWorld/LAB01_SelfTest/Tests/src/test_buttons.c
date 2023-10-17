/**
 * @file test_buttons.c
 * @brief Verify buttons.
 * @author Philipp Schilk 2023
 */

#include "test_buttons.h"
#include "main.h"
#include <stdio.h>

// 8 bit binary printf formatting:
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                                                                           \
  ((byte)&0x80 ? '1' : '0'), ((byte)&0x40 ? '1' : '0'), ((byte)&0x20 ? '1' : '0'), ((byte)&0x10 ? '1' : '0'),          \
      ((byte)&0x08 ? '1' : '0'), ((byte)&0x04 ? '1' : '0'), ((byte)&0x02 ? '1' : '0'), ((byte)&0x01 ? '1' : '0')

// Read all buttons, pack into an integer:
unsigned int read_buttons(void) {
  unsigned int result = 0;
  result |= (!HAL_GPIO_ReadPin(BTN_UP_GPIO_Port, BTN_UP_Pin) << 0);
  result |= (!HAL_GPIO_ReadPin(BTN_DOWN_GPIO_Port, BTN_DOWN_Pin) << 1);
  result |= (!HAL_GPIO_ReadPin(BTN_L_GPIO_Port, BTN_L_Pin) << 2);
  result |= (!HAL_GPIO_ReadPin(BTN_R_GPIO_Port, BTN_R_Pin) << 3);
  result |= (!HAL_GPIO_ReadPin(BTN_ENTER_GPIO_Port, BTN_ENTER_Pin) << 4);
  return result;
}

// Verification routine for an individual button:
bool verify_buttons(unsigned int mask) {

  // Wait for buttons to be pressed:
  while (!read_buttons()) {
  }

  // Check correct button pressed:
  HAL_Delay(30);
  unsigned int pressed = read_buttons();

  if (pressed != mask) {
    printf("Unexpected buttons were pressed! Expected " BYTE_TO_BINARY_PATTERN ", Got " BYTE_TO_BINARY_PATTERN "\r\n",
           BYTE_TO_BINARY(mask), BYTE_TO_BINARY(pressed));
    return false; // Test fail.
  }

  // Wait for buttons to be released:
  printf("Please release...\r\n");
  while (read_buttons()) {
  }

  HAL_Delay(30);

  return true;
}

bool test_buttons(void) {

  // Verify that no buttons are pressed:
  for (int i = 0; i < 10; i++) {
    unsigned int pressed = read_buttons();
    if (pressed != 0) {
      printf("Unexpected buttons were pressed: " BYTE_TO_BINARY_PATTERN "\r\n", BYTE_TO_BINARY(pressed));
      return false;
    }
  }

  // Verify button presses:
  printf("Press UP Button...\r\n");
  if (!verify_buttons(0x1 << 0)) {
    return false;
  }
  printf("Press DOWN Button...\r\n");
  if (!verify_buttons(0x1 << 1)) {
    return false;
  }
  printf("Press LEFT Button...\r\n");
  if (!verify_buttons(0x1 << 2)) {
    return false;
  }
  printf("Press RIGHT Button...\r\n");
  if (!verify_buttons(0x1 << 3)) {
    return false;
  }
  printf("Press ENTER Button...\r\n");
  if (!verify_buttons(0x1 << 4)) {
    return false;
  }

  return true;
}
