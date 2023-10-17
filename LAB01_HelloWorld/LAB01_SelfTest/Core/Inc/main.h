/* USER CODE BEGIN Header */
/**
 * @file main.h
 * @brief PBLShield Selftest: Hardware interfacing.
 * @author Philipp Schilk 2023
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
#include <stdio.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN_UP_Pin            GPIO_PIN_13
#define BTN_UP_GPIO_Port      GPIOC
#define BTN_ENTER_Pin         GPIO_PIN_0
#define BTN_ENTER_GPIO_Port   GPIOH
#define BTN_R_Pin             GPIO_PIN_1
#define BTN_R_GPIO_Port       GPIOC
#define OB_SPI_MOSI_Pin       GPIO_PIN_3
#define OB_SPI_MOSI_GPIO_Port GPIOC
#define STLINK_TX_Pin         GPIO_PIN_2
#define STLINK_TX_GPIO_Port   GPIOA
#define STLINK_RX_Pin         GPIO_PIN_3
#define STLINK_RX_GPIO_Port   GPIOA
#define POTI_Pin              GPIO_PIN_4
#define POTI_GPIO_Port        GPIOA
#define BTN_L_Pin             GPIO_PIN_0
#define BTN_L_GPIO_Port       GPIOB
#define MATRIX_RCK_Pin        GPIO_PIN_11
#define MATRIX_RCK_GPIO_Port  GPIOB
#define IMU_CS_Pin            GPIO_PIN_12
#define IMU_CS_GPIO_Port      GPIOB
#define OB_SPI_SCK_Pin        GPIO_PIN_13
#define OB_SPI_SCK_GPIO_Port  GPIOB
#define OB_SPI_MISO_Pin       GPIO_PIN_14
#define OB_SPI_MISO_GPIO_Port GPIOB
#define WIFI_nRST_Pin         GPIO_PIN_15
#define WIFI_nRST_GPIO_Port   GPIOB
#define WIFI_AM_1_Pin         GPIO_PIN_6
#define WIFI_AM_1_GPIO_Port   GPIOC
#define LED_2_Pin             GPIO_PIN_7
#define LED_2_GPIO_Port       GPIOC
#define WIFI_AM_0_Pin         GPIO_PIN_8
#define WIFI_AM_0_GPIO_Port   GPIOC
#define BTN_DOWN_Pin          GPIO_PIN_9
#define BTN_DOWN_GPIO_Port    GPIOC
#define WIFI_TX_Pin           GPIO_PIN_10
#define WIFI_TX_GPIO_Port     GPIOC
#define WIFI_RX_Pin           GPIO_PIN_11
#define WIFI_RX_GPIO_Port     GPIOC
#define LED_1_Pin             GPIO_PIN_6
#define LED_1_GPIO_Port       GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
