/**
 * @file stts22h_driver.c
 * @brief STTS22H Temperature Sensor Driver
 * @author Philipp Schilk, 2023
 */

#include "stts22h_driver.h"
#include "main.h"

/**
 * @brief Verify that communication with the stts22h is functional.
 *
 * This is done by reading the sensor's WHOAMI register and verifying that it
 * the expected value is present.
 *
 * @return STTS22H_OK if the communication is functional, STTS22H_ERROR otherwise.
 */
STTS22H_Status stts22h_check_communication(void) {
  uint8_t whoami = 0;

  // TODO 1: Read "WHOAMI" register:
  // HINT : The STM32 HAL I2C functions expect the I2C address in 8-bit format. (addr_8bit = addr_7bit << 1)
  //$ START SOL
  if (HAL_I2C_Mem_Read(&hi2c1, STTS22H_I2C_ADDR_7B << 1, STTS22H_REG_WHOAMI, I2C_MEMADD_SIZE_8BIT, &whoami, 1, 20) !=
      HAL_OK) {
    return STTS22H_ERROR;
  }
  //$ START EX
  // if (HAL_I2C_Mem_Read(&hi2c1, TODO, TODO, I2C_MEMADD_SIZE_8BIT, &whoami, TODO, 20) != HAL_OK) {
  //   return STTS22H_ERROR;
  // }
  //$ END

  // TODO 2: Check if whoami contains the expected value:
  //$ START SOL
  if (whoami == STTS22H_REG_WHOAMI_VALUE) {
    return STTS22H_OK;
  } else {
    return STTS22H_ERROR;
  }
  //$ END
}

/**
 * @brief Set the sensor to a specific operating mode.
 *
 * The sensor can run in one of the following modes:
 *    - FREERUN: The temperature is measured continuously at 25, 50, 100 or 200Hz.
 *    - LOW_ODR: The temperature is measured continuously at 1Hz.
 *    - ONESHOT: The sensor is in sleep mode, and an individual measurement can be triggered.
 *
 * @param mode the mode to configure the sensor to.
 * @return STTS22H_OK if it was configured successfully, STTS22H_ERROR otherwise.
 */
STTS22H_Status stts22h_configure(STTS22H_Mode mode) {
  // HINT: Anytime we have to modify certain bits in a sensors register we will use
  //       the following procedure:
  //         1) Read the current register value via I2C into a variable.
  //         2) Make the desired modifications to that variable.
  //         3) Write the new register value back to the register via I2C.

  // TODO 1: Read current "CTRL" register:
  uint8_t reg_ctrl = 0;
  //$ START SOL
  if (HAL_I2C_Mem_Read(&hi2c1, STTS22H_I2C_ADDR_7B << 1, STTS22H_REG_CTRL, I2C_MEMADD_SIZE_8BIT, &reg_ctrl, 1, 20) !=
      HAL_OK) {
    return STTS22H_ERROR;
  }
  //$ END

  // === Step 1: Power-down the sensor ===

  // The datasheet reads:
  //   Before changing the operating mode or ODR frequency, the user has to power down the device by writing 0 to
  //   both the FREERUN and LOW_ODR_START bits.

  // TODO 2: Set FREERUN to 0:
  reg_ctrl &= ~(0x1 << 2); //$ SOL
  // reg_ctrl &= TODO;     //$ END

  // TODO 3: Set LOW_ODR_START to 0:
  reg_ctrl &= ~(0x1 << 7); //$ SOL
  // reg_ctrl &= TODO;     //$ END

  // TODO 4: Write the new CTRL register value to sensor to apply it:
  // HINT : The STM32 HAL I2C functions expect the I2C address in 8-bit format. (addr_8bit = addr_7bit << 1)
  //$ START SOL
  if (HAL_I2C_Mem_Write(&hi2c1, STTS22H_I2C_ADDR_7B << 1, STTS22H_REG_CTRL, I2C_MEMADD_SIZE_8BIT, &reg_ctrl, 1, 20) !=
      HAL_OK) {
    return STTS22H_ERROR;
  }
  //$ START EX
  // if (HAL_I2C_Mem_Write(&hi2c1, TODO, TODO, I2C_MEMADD_SIZE_8BIT, &reg_ctrl, TODO, 20) != HAL_OK) {
  //   return STTS22H_ERROR;
  // }
  //$ END

  // === Step 2: Configure the Sensor ===

  // Depending on which mode was requested, we must correctly configure the
  // FREERUN, LOW_ODR_START, and AVG bitfields in the ctrl register.

  // LOW_ODR_START: If the low-odr (1Hz) mode was requested, the LOW_ODR_START bit
  //                must be set to 1. Otherwise it should be set to 0.
  if (mode == STTS22H_MODE_LOW_ODR_1HZ) {
    // TODO 5: Set LOW_ODR_START bit to 1:
    reg_ctrl |= (0x1 << 7); //$ SOL
    // reg_ctrl |= TODO;    //$ EX
  } else {
    // TODO 6: Set LOW_ODR_START bit to 0:
    reg_ctrl &= ~(0x1 << 7); //$ SOL
    // reg_ctrl &= TODO;     //$ EX
  }

  // TODO 7:
  // FREERUN: If the freerun mode was requested (no matter what ouput rate),
  //          the FREERUN bit should be set to 1. Otherwise it should be set to 0.
  // HINT:    The freerun mode was requested if mode is set to any of the following:
  //            - STTS22H_MODE_FREERUN_25HZ
  //            - STTS22H_MODE_FREERUN_50H
  //            - STTS22H_MODE_FREERUN_100HZ
  //            - STTS22H_MODE_FREERUN_200HZ
  //$ START SOL
  if (mode >= STTS22H_MODE_FREERUN_25HZ && mode <= STTS22H_MODE_FREERUN_200HZ) {
    // Set FREERUN bit to 1:
    reg_ctrl |= (1 << 2);
  } else {
    // Set FREERUN bit to 0:
    reg_ctrl &= ~(1 << 2);
  }
  //$ END

  // TODO 8:
  // AVG:  If the freerunn mode was requested, we must set the AVG field to
  //       configure the correct output data rate. In all other modes the
  //       AVG field's value does not matter.
  // HINT: Look at the sensor datasheet to see what value you have to write to the
  //       AVG field to achieve a certain output data rate.
  //$ START SOL
  switch (mode) {
    case STTS22H_MODE_FREERUN_25HZ:
      // Set AVG to 0
      reg_ctrl &= ~(0x3 << 4);
      break;
    case STTS22H_MODE_FREERUN_50HZ:
      // Set AVG to 1
      reg_ctrl &= ~(0x3 << 4);
      reg_ctrl |= (0x1 << 4);
      break;
    case STTS22H_MODE_FREERUN_100HZ:
      // Set AVG to 2
      reg_ctrl &= ~(0x3 << 4);
      reg_ctrl |= (0x2 << 4);
      break;
    case STTS22H_MODE_FREERUN_200HZ:
      // Set AVG to 3
      reg_ctrl |= (0x3 << 4);
      break;
    default:
      // Do nothing - AVG field does not matter in non-freerun mode.
      break;
  }
  //$ END

  // No matter the mode, always set BDU to 1 and IF_ADD_INC to 1:
  // These are settings that control how the communication interface works.
  // Have a look at the datasheet if you are interested.
  reg_ctrl |= (0x1 << 6);
  reg_ctrl |= (0x1 << 3);

  // TODO 9: Write to sensor to apply:
  //$ START SOL
  if (HAL_I2C_Mem_Write(&hi2c1, STTS22H_I2C_ADDR_7B << 1, STTS22H_REG_CTRL, I2C_MEMADD_SIZE_8BIT, &reg_ctrl, 1, 20) !=
      HAL_OK) {
    return STTS22H_ERROR;
  }
  //$ END

  return STTS22H_OK;
}

/**
 * @brief Retrieve the latest temperature measurement.
 *
 * @note this is only valid if at least one measurement has been completed.
 * @param temp_cenitdegree the latest temperature measurement in centi-degrees (in hundredths of a degree)
 * @return
 */
STTS22H_Status stts22h_read_temp(int16_t *temp_cenitdegree) {
  // TODO 1: Read "TEMP_H_OUT and TEMP_L_OUT" register:
  // HINT:   You can read sequential register with a single I2C read.
  uint8_t temp_out[2] = {0};
  //$ START SOL
  if (HAL_I2C_Mem_Read(&hi2c1, STTS22H_I2C_ADDR_7B << 1, STTS22H_REG_TEMP_L_OUT, I2C_MEMADD_SIZE_8BIT, temp_out, 2,
                       20) != HAL_OK) {
    return STTS22H_ERROR;
  }
  //$ END

  // TODO 2: Construct temp_cenitdegree from TEMP_H_OUT and TEMP_L_OUT registers:
  *temp_cenitdegree = 0;
  //$ START SOL
  *temp_cenitdegree |= (temp_out[0] << 0);
  *temp_cenitdegree |= (temp_out[1] << 8);
  //$ END

  return STTS22H_OK;
}

/**
 * @brief Trigger a single temperature measurement.
 *
 * @note this is only possible when the sensor is in one-shot mode.
 * @return STTS22H_OK if a measurement was be triggered, STTS22H_ERROR otherwise.
 */
STTS22H_Status stts22h_trigger_oneshot(void) {
  // TODO 1: Read current "CTRL" register:
  uint8_t reg_ctrl = 0;
  //$ START SOL
  if (HAL_I2C_Mem_Read(&hi2c1, STTS22H_I2C_ADDR_7B << 1, STTS22H_REG_CTRL, I2C_MEMADD_SIZE_8BIT, &reg_ctrl, 1, 20) !=
      HAL_OK) {
    return STTS22H_ERROR;
  }
  //$ END

  // TODO 2: Check if we are in one-shot mode (LOW_ODR_START == 0 and FREERUN == 0)-
  //         If not, return an error.
  //$ START SOL
  uint8_t low_odr_start = (reg_ctrl >> 7) & 0x1;
  uint8_t freerun = (reg_ctrl >> 2) & 0x1;
  if (low_odr_start != 0 || freerun != 0) {
    // We are not in one-shot mode. Cannot trigger one-shot measurement.
    return STTS22H_ERROR;
  }
  //$ START EX
  // uint8_t low_odr_start = (reg_ctrl >> TODO) & TODO;
  // uint8_t freerun = (reg_ctrl >> TODO) & TODO;
  //$ END

  // TODO 3: Set "ONESHOT" bit to 1:
  reg_ctrl |= (1 << 0); //$ SOL
  // reg_ctrl |= TODO;  //$ EX

  // TODO 4: Write new CTRL register value to sensor to apply it:
  //$ START SOL
  if (HAL_I2C_Mem_Write(&hi2c1, STTS22H_I2C_ADDR_7B << 1, STTS22H_REG_CTRL, I2C_MEMADD_SIZE_8BIT, &reg_ctrl, 1, 20) !=
      HAL_OK) {
    return STTS22H_ERROR;
  }
  //$ END

  return STTS22H_OK;
}

/**
 * @brief Check if a one-shot measurement has completed or is still in progress.
 *
 * @note this is only valid when the sensor is in one-shot mode.
 * @param is_busy
 * @return
 */
STTS22H_Status stts22h_check_is_busy(bool *is_busy) {
  // TODO 1: Read current "STATUS" register:
  uint8_t reg_status = 0;
  //$ START SOL
  if (HAL_I2C_Mem_Read(&hi2c1, STTS22H_I2C_ADDR_7B << 1, STTS22H_REG_STATUS, I2C_MEMADD_SIZE_8BIT, &reg_status, 1,
                       20) != HAL_OK) {
    return STTS22H_ERROR;
  }
  //$ END

  // TODO 2: Extract "BUSY" bit into is_busy:
  *is_busy = reg_status & 0x1; //$ SOL

  return STTS22H_OK;
}
