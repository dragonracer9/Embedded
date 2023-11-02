/**
 * @file stts22h_driver.h
 * @brief STTS22H Temperature Sensor Driver
 * @author Philipp Schilk, 2023
 */
#ifndef STTS22H_DRIVER_H_
#define STTS22H_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>
#define TODO 0 // This stops from 'TODO's in the code from throwing compiler errors.

// ===== General ===============================================================

// TODO: Determine the I2C address of the STTS22H sensors as mounted
//       on the PBL shield. Give the address in the *7-bit* format.
// HINT: The address depends on how the "ADDR" pin is connected. See sensor
//       datasheet and PBL shield schematic.
#define STTS22H_I2C_ADDR_7B 0x3F /** @brief I2C Address (7-bit format). */ //$ SOL
// #define STTS22H_I2C_ADDR_7B TODO /** @brief I2C Address (7-bit format). */ //$ EX

// ===== Registers =============================================================

// TODO: Complete the addresses of the registers inside the STTS22H:
//$ START SOL
#define STTS22H_REG_WHOAMI       0x01 /** @brief WHO-AM-I Register Address. */
#define STTS22H_REG_TEMP_H_LIMIT 0x02 /** @brief TEMP_H_LIMIT Register Address. */
#define STTS22H_REG_TEMP_L_LIMIT 0x03 /** @brief TEMP_L_LIMIT Register Address. */
#define STTS22H_REG_CTRL         0x04 /** @brief CTRL Register Address. */
#define STTS22H_REG_STATUS       0x05 /** @brief STATUS Register Address. */
#define STTS22H_REG_TEMP_L_OUT   0x06 /** @brief TEMP_L_OUT Register Address. */
#define STTS22H_REG_TEMP_H_OUT   0x07 /** @brief TEMP_H_OUT Register Address. */
//$ START EX
// #define STTS22H_REG_WHOAMI       0x01 /** @brief WHO-AM-I Register Address. */
// #define STTS22H_REG_TEMP_H_LIMIT TODO /** @brief TEMP_H_LIMIT Register Address. */
// #define STTS22H_REG_TEMP_L_LIMIT TODO /** @brief TEMP_L_LIMIT Register Address. */
// #define STTS22H_REG_CTRL         TODO /** @brief CTRL Register Address. */
// #define STTS22H_REG_STATUS       TODO /** @brief STATUS Register Address. */
// #define STTS22H_REG_TEMP_L_OUT   TODO /** @brief TEMP_L_OUT Register Address. */
// #define STTS22H_REG_TEMP_H_OUT   TODO /** @brief TEMP_H_OUT Register Address. */
//$ END

// TODO: Complete the expected value of the WHOAMI register:
//$ START SOL
#define STTS22H_REG_WHOAMI_VALUE 0xA0 /** @brief WHO-AM-I Register Value. */
//$ START EX
// #define STTS22H_REG_WHOAMI_VALUE TODO /** @brief WHO-AM-I Register Value. */
//$ END

// ===== Types =================================================================

typedef enum {
  STTS22H_OK = 0,
  STTS22H_ERROR = 1
} STTS22H_Status;

typedef enum {
  STTS22H_MODE_ONE_SHOT = 0,
  STTS22H_MODE_LOW_ODR_1HZ,
  STTS22H_MODE_FREERUN_25HZ,
  STTS22H_MODE_FREERUN_50HZ,
  STTS22H_MODE_FREERUN_100HZ,
  STTS22H_MODE_FREERUN_200HZ,
} STTS22H_Mode;

// ===== Functions =============================================================

/**
 * @brief Verify that communication with the stts22h is functional.
 *
 * This is done by reading the sensor's WHOAMI register and verifying that it
 * the expected value is present.
 *
 * @return STTS22H_OK if the communication is functional, STTS22H_ERROR otherwise.
 */
STTS22H_Status stts22h_check_communication(void);

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
STTS22H_Status stts22h_configure(STTS22H_Mode mode);

/**
 * @brief Retrieve the latest temperature measurement.
 *
 * @note this is only valid if at least one measurement has been completed.
 * @param temp_cenitdegree the latest temperature measurement in centi-degrees (in hundredths of a degree)
 * @return
 */
STTS22H_Status stts22h_read_temp(int16_t *temp_cenitdegree);

/**
 * @brief Trigger a single temperature measurement.
 *
 * @note this is only possible when the sensor is in one-shot mode.
 * @return STTS22H_OK if a measurement was be triggered, STTS22H_ERROR otherwise.
 */
STTS22H_Status stts22h_trigger_oneshot(void);

/**
 * @brief Check if a one-shot measurement has completed or is still in progress.
 *
 * @note this is only valid when the sensor is in one-shot mode.
 * @param is_busy
 * @return
 */
STTS22H_Status stts22h_check_is_busy(bool *is_busy);

#endif /* STTS22H_DRIVER_H_ */
