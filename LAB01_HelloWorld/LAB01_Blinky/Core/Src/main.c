/**
 * @file   : main.c
 * @brief  : Exercise 1.2: Blinky
 * @author : Philipp Schilk, 2023
 *
 * Flash leds by configuring the relevant GPIO pins as outputs
 * and toggling them.
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void SystemClock_Config(void);
static void task2(void);
static void task5(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  // TODO (Task 2): All sections that require work are marked with such 'TODO' comment.
  // '______' indicate placeholders that have to be completed.

  // TODO (Task 2): Enable the GPIO ports to which the LEDs are connected by their clock:
  // Note: Replace 'x' with the correct port letters, the function is otherwise correct.
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  // TODO (Task 2): Create two GPIO_InitTypeDef structs which contain the configuration
  // settings we want to apply to the two GPIO pins:
  GPIO_InitTypeDef led1_config = {GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH};

  GPIO_InitTypeDef led2_config = {GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH};

  // TODO (Task 2): Apply these settings to the relevant pins/ports:
  HAL_GPIO_Init(GPIOB, &led1_config);
  HAL_GPIO_Init(GPIOC, &led2_config);
  /* Configure the system clock */
  SystemClock_Config();

  // TODO (Task 5): Select task 2 or task 5:
  // task2();
  task5();

  while (1) {
  }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
}

static void task2(void) {
  // Task 2: Toggle LEDs using the HAL functions.
  while (1) {

    // TODO (Task 2): Call the relevant HAL function to toggle the LED1 output pin:
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);

    // TODO (Task 2): Call the relevant HAL function to toggle the LED2 output pin:
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);

    // TODO (Task 2): Call the relevant HAL function to create a 500ms delay:
    HAL_Delay(500 /*ms*/);
  }
}

static void task5(void) {
  // Task 5: Toggle LEDs by directly modifying the relevant registers:

  // unnecessary to set mode of pin in MODER register because the HAL has taken care of that already
    volatile uint32_t *GPIOB_ODR = (volatile uint32_t *) 0x48000414;
    volatile uint32_t *GPIOC_ODR = (volatile uint32_t *) 0x48000814;
    *GPIOB_ODR &= ~(0b1 << 6);
    *GPIOC_ODR |= (0x1 << 7);
  while (1) {
    // TODO (Task 5): Create pointers that point at GPIOB's and GPIOC's ODR register:
    // ____ GPIOB_ODR = ____;
    // ____ GPIOC_ODR = ____;

    // TODO (Task 5): Turn LED1 ON, by modifying the relevant ODR register:
    // _____
    *GPIOB_ODR ^= (0b1 << 6);
    *GPIOC_ODR ^= (0b1 << 7);

    // Delay:
    HAL_Delay(500);

    // TODO (Task 5): Turn LED1 OFF again:
    // ____
    *GPIOB_ODR ^= (0b1 << 6); // waste of a copy/paste really, but it doenst matter
    *GPIOC_ODR ^= (0b1 << 7);

    // TODO (Task 5): Toggle LED2 using a single bitwise operation:
    // ____

    // Delay:
    HAL_Delay(500);
  }
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
  while (1) {
    ;
  }
}
#endif /* USE_FULL_ASSERT */
