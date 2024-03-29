/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
bool snake_read_left_button();
bool snake_read_right_button();
bool snake_read_enter_button();
void snake_write_display(uint32_t red, uint32_t green);
uint32_t snake_read_adc();

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN_ENTER_Pin         GPIO_PIN_0
#define BTN_ENTER_GPIO_Port   GPIOH
#define BTN_R_Pin             GPIO_PIN_1
#define BTN_R_GPIO_Port       GPIOC
#define MATRIX_PICO_Pin       GPIO_PIN_3
#define MATRIX_PICO_GPIO_Port GPIOC
#define POTI_Pin              GPIO_PIN_4
#define POTI_GPIO_Port        GPIOA
#define BTN_L_Pin             GPIO_PIN_0
#define BTN_L_GPIO_Port       GPIOB
#define MATRIX_RCK_Pin        GPIO_PIN_11
#define MATRIX_RCK_GPIO_Port  GPIOB
#define MATRIX_SCK_Pin        GPIO_PIN_13
#define MATRIX_SCK_GPIO_Port  GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
