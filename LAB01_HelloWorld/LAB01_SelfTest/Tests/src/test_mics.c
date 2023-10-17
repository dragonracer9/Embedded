/**
 * @file test_mics.c
 * @brief Verify microphones.
 * @author Philipp Schilk 2023
 */
#include "test_mics.h"
#include "barchart.h"
#include "main.h"
#include <inttypes.h>
#include <stdio.h>

#define MIC_BUFFER_SIZE 256

int32_t buffer_l[MIC_BUFFER_SIZE] = {0};
int32_t buffer_r[MIC_BUFFER_SIZE] = {0};

volatile bool l_done = false;
volatile bool r_done = false;

void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter) {
  if (hdfsdm_filter == &hdfsdm1_filter0) {
    l_done = true;
  } else {
    r_done = true;
  }
}

bool test_mics(void) {

  printf("Showing Microphone Levels. Hold UP to continue or DOWN to indicate error.\r\n");

  HAL_Delay(1500);

  // Level to display:
  uint32_t level_l = 0;
  uint32_t level_r = 0;

  // Maximum to scale display:
  const uint64_t max = 8340544;
  uint32_t max_l = 0;
  uint32_t max_r = 0;

  // Start microphone acquisition:
  l_done = false;
  r_done = false;
  if (HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, buffer_r, MIC_BUFFER_SIZE) != HAL_OK) {
    printf("Failed to enable filter 0\r\n");
    return false;
  }
  if (HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter1, buffer_l, MIC_BUFFER_SIZE) != HAL_OK) {
    printf("Failed to enable filter 1\r\n");
    return false;
  }

  // Track time between printing microphone levels to the console:
  uint32_t last_print = HAL_GetTick();

  while (1) {

    // Update levels if new data is available:
    if (l_done) {
      l_done = false;
      uint64_t sum = 0;
      for (int i = 0; i < MIC_BUFFER_SIZE; i++) {
        uint32_t val = buffer_l[i] > 0 ? buffer_l[i] : -buffer_l[i];
        sum += val;

        if (val > max_l) {
          max_l = val;
        }
      }
      uint64_t avg = sum / MIC_BUFFER_SIZE;
      level_l = avg * 20 / max;
      level_l = level_l > 20 ? 20 : level_l;
    }
    if (r_done) {
      r_done = false;
      uint64_t sum = 0;
      for (int i = 0; i < MIC_BUFFER_SIZE; i++) {
        uint32_t val = buffer_r[i] > 0 ? buffer_r[i] : -buffer_r[i];
        sum += val;

        if (val > max_r) {
          max_r = val;
        }
      }
      uint64_t avg = sum / MIC_BUFFER_SIZE;
      level_r = avg * 20 / max;
      level_r = level_r > 20 ? 20 : level_r;
    }

    // Print meters every 150:
    if (HAL_GetTick() - last_print > 150) {
      last_print = HAL_GetTick();
      printf("L: ");
      print_bar_char(level_l, 20);
      printf(" R: ");
      print_bar_char(level_r, 20);
      printf(" %"PRIu32", %"PRIu32"", max_l, max_r);
      printf("\r\n");
    }

    // Exit on button:
    if (!HAL_GPIO_ReadPin(BTN_DOWN_GPIO_Port, BTN_DOWN_Pin)) {
      HAL_DFSDM_FilterRegularStop(&hdfsdm1_filter0);
      HAL_DFSDM_FilterRegularStop(&hdfsdm1_filter1);
      return false;
    } else if (!HAL_GPIO_ReadPin(BTN_UP_GPIO_Port, BTN_UP_Pin)) {
      HAL_DFSDM_FilterRegularStop_DMA(&hdfsdm1_filter0);
      HAL_DFSDM_FilterRegularStop_DMA(&hdfsdm1_filter1);
      return true;
    }
  }

  return true;
}
