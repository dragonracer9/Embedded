/* USER CODE BEGIN Header */
/**
 * @file main.c
 * @brief STTS22H Driver Testbench
 * @author Philipp Schilk, 2023
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stts22h_driver.h"
#include <inttypes.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void test_driver(void);
void wait_for_button_press(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  test_driver();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10909CEC;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
    Error_Handler();
  }

  /** Configure Analogue filter
   */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
    Error_Handler();
  }

  /** Configure Digital filter
   */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BTN_ENTER_Pin */
  GPIO_InitStruct.Pin = BTN_ENTER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BTN_ENTER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED2_Pin */
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void test_driver(void) {

  printf("Press enter button to test sensor communication...\r\n");
  wait_for_button_press();

  // ==== 1: Check if communication is OK ====

  // TODO: Call your completed stts22h_check_communication() function.
  //       If it returns an error, print an error message using printf and
  //       call the Error_Handler() function. Otherwise print a confirmation
  //       message that the sensor communication is OK.
  if (stts22h_check_communication() == STTS22H_OK) {
    printf("Success reading from WHOAMI\r\n");
  } else {
    printf("Error reading from WHOAMI\r\n");
    Error_Handler();
  }

  printf("Press enter button to test free-run mode...\r\n");
  wait_for_button_press();

  // ==== 2: Configure in free-run mode ====
  // TODO: Call your completed stts22h_configure() function to put the
  //       temperature sensor into free-run mode at an output data rate of 25Hz.
  //       If it returns an error, print an error message using printf and
  //       call the Error_Handler() function. Otherwise print a confirmation
  //       message that the sensor was configured correctly.
  if (stts22h_configure(STTS22H_MODE_FREERUN_25HZ) != STTS22H_OK) {
    printf("Sensor config ERROR!\r\n");
    Error_Handler();
  }

  HAL_Delay(100);

  // ==== 3: Read temperature 20 times ====
  for (unsigned int i = 0; i < 20; i++) {
    HAL_Delay(500);

    int16_t temp_decidegree = 0;
    // TODO: Use your stts22h_read_temp() functiont to retrieve the
    //       latest temperature measurement from the stts22h. If it returns
    //       an error, use printf to print an error message and call
    //       the Error_Handler() function.
    if (stts22h_read_temp(&temp_decidegree) != STTS22H_OK) {
      printf("ERROR reading temperature");
      Error_Handler();
    }
    // TODO: Convert temp_decidegree value to actual degrees:
    float temp_degrees = temp_decidegree / 100.0f;

    printf("[%u/20] Current temperature: %.2f\r\n", i + 1, temp_degrees);
  }

  printf("Press enter button to test one-shot mode...\r\n");
  wait_for_button_press();

  // ==== 4: Configure in one-shot mode ====
  // TODO: Call your completed stts22h_configure() function to put the
  //       temperature sensor into one-shot mode.
  //       If it returns an error, print an error message using printf and
  //       call the Error_Handler() function. Otherwise print a confirmation
  //       message that the sensor was configured correctly.
  if (stts22h_configure(STTS22H_MODE_ONE_SHOT) == HAL_OK) {
    printf("Successfully configured to ONE_SHOT\r\n");
  } else {
    printf("ERROR configuring to ONE_SHOT\r\n");
    Error_Handler();
  }

  // ==== 5: On button press: trigger measurement, wait for completion, print result ====
  while (1) {

    // Trigger measurement:
    // TODO: Call your completed stts22h_trigger_oneshot() function to
    //       trigger a single measurement.
    //       If it returns an error, print an error message using printf and
    //       call the Error_Handler() function. Otherwise print a confirmation
    //       message that a measurement was triggered.
    if (stts22h_trigger_oneshot() == HAL_OK) {
      printf("Successfully tiggered one-shot measurement\r\n");
    } else {
      printf("ERROR triggering one-shot measurement\r\n");
      Error_Handler();
    }

    // Wait for measurement to be completed:
    uint32_t cnt = 0;
    while (1) {
      HAL_Delay(10);

      bool is_busy = false;
      // TODO: Call your stts22h_check_is_busy function to determine if the
      //       measurement you triggered is completed. If it returns an error,
      //       use printf to print an error message.
      stts22h_check_is_busy(&is_busy);

      if (!is_busy) {
        break; // Measurement done.
      }

      // Timeout:
      cnt++;
      if (cnt > 20) {
        printf("ERROR: One-shot measurement took too long to complete!\r\n");
        Error_Handler();
      }
    }

    // Retrieve & print measurement:
    int16_t temp_decidegree = 0;
    // TODO Retrieve the current temperature, convert it to degrees, and
    //      print it to the console (just like you did before).
    if (stts22h_read_temp(&temp_decidegree) != STTS22H_OK) {
      printf("ERROR reading temperature");
      Error_Handler();
    }
    // TODO: Convert temp_decidegree value to actual degrees:
    float temp_degrees = temp_decidegree / 100.0f;

    printf("Current temperature: %.2f\r\n", temp_degrees);

    // Wait for button to be pressed before doing another one-shot measurement:
    printf("Press enter to test one-shot measurement...\r\n");
    wait_for_button_press();
  }
}

void wait_for_button_press(void) {
  // If the button is already pressed, wait for button to be released:
  while (HAL_GPIO_ReadPin(BTN_ENTER_GPIO_Port, BTN_ENTER_Pin) == GPIO_PIN_RESET) {
  }
  // Wait for the button to be released:
  while (HAL_GPIO_ReadPin(BTN_ENTER_GPIO_Port, BTN_ENTER_Pin) == GPIO_PIN_SET) {
  }
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
  while (1) {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    HAL_Delay(1000);

    printf("Stuck in error handler :(...\r\n");
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
