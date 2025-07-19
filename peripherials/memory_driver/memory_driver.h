#ifndef __MEMORY_DRIVER_H__
#define __MEMORY_DRIVER_H__

#include "hardware/i2c.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @file memory_driver.h
 * @brief Header file for EEPROM memory driver functions.
 * This file contains the function declarations and constants for interfacing with the EEPROM memory.
 * This driver uses I2C communication to read and write data to the EEPROM.
 */

#define EEPROM_PAGE_SIZE 16 ///< Size of a page in EEPROM
#define EEPROM_BLOCK_SIZE 256 ///< Size of a block in EEPROM
#define EEPROM_TOTAL_SIZE 2048 ///< Total size of EEPROM in bytes

#define SLAVE_ADD 0x0A ///< I2C slave address for EEPROM 
#define OFFSET_DATOS 4 ///< Offset for data in EEPROM
#define BLOQUE_NOISE 0x01 ///< Block for noise data
#define BLOQUE_LAT 0x02 ///< Block for latitude data
#define BLOQUE_LON 0x03 ///< Block for longitude data
#define MAX_CAPTURAS 63  ///< Maximum number of captures
#define DIR_INDEX 0XFFF1 ///< Address for index in EEPROM
#define DIR_BASE 0xFF ///< Base address for data storage in EEPROM

/**
 * @brief Initializes the EEPROM memory driver.
 * This function sets up the I2C communication for the EEPROM.
 * It should be called before any read/write operations.
 */
void eeprom_init();

/**
 * @brief Saves a measurement to EEPROM.
 * This function writes a string of data to the EEPROM at a specified address.
 * @param datos Pointer to the data to be saved.
 */
void save_measurement(char * datos);

/**
 * @brief Writes a byte to the EEPROM at a specified address.
 * This function writes a single byte of data to the EEPROM.
 * @param mem_addr The memory address to write to.
 * @param data The byte of data to write.
 * @return true if the write operation was successful, false otherwise.
 */
bool eeprom_write_byte(uint16_t mem_addr, uint8_t data);

/**
 * @brief Reads a byte from the EEPROM at a specified address.
 * This function reads a single byte of data from the EEPROM.
 * @param mem_addr The memory address to read from.
 * @param data Pointer to store the read byte.
 * @return true if the read operation was successful, false otherwise.
 */
bool eeprom_write_page(uint16_t mem_addr, const uint8_t *data, uint8_t len);

/**
 * @brief Reads a byte from the EEPROM at a specified address.
 * This function reads a single byte of data from the EEPROM.
 * @param mem_addr The memory address to read from.
 * @param data Pointer to store the read byte.
 * @return true if the read operation was successful, false otherwise.
 */
bool eeprom_read_byte(uint16_t mem_addr, uint8_t *data);

/**
 * @brief Reads a block of data from the EEPROM.
 * This function reads a specified number of bytes from the EEPROM.
 * @param mem_addr The memory address to read from.
 * @param buffer Pointer to the buffer to store the read data.
 * @param len The number of bytes to read.
 * @return true if the read operation was successful, false otherwise.
 */
bool eeprom_read_data(uint16_t mem_addr, uint8_t *buffer, uint8_t len);

/**
 * @brief Writes a block of data to the EEPROM.
 * This function writes a specified number of bytes to the EEPROM.
 * @param mem_addr The memory address to write to.
 * @param data Pointer to the data to write.
 * @param len The number of bytes to write.
 * @return true if the write operation was successful, false otherwise.
 */
bool eeprom_read_measurement(uint16_t mem_addr, float *noise, float *lat, float *lon);

/**
 * @brief Writes a measurement to the EEPROM.
 * This function writes three float values (noise, latitude, longitude) to the EEPROM at a specified address.
 * @param mem_addr The memory address to write to.
 * @param noise The noise value to write.
 * @param lat The latitude value to write.
 * @param lon The longitude value to write.
 * @return true if the write operation was successful, false otherwise.
 */
bool eeprom_write_measurement(uint16_t mem_addr, float noise, float lat, float lon);

/**
 * @brief Sets the index in the EEPROM.
 * This function writes the current index to the EEPROM at a predefined address.
 * @param index The index to set.
 */
void eeprom_set_index(uint8_t index);

/**
 * @brief Gets the current index from the EEPROM.
 * This function reads the current index from the EEPROM at a predefined address.
 * @param data Pointer to store the read index.
 * @return true if the read operation was successful, false otherwise.
 */
bool eeprom_get_index(uint8_t *data);
/*
void eeprom_flush();
*/

#endif // __MEMORY_DRIVER_H__  