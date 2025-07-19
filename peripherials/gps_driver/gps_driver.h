/**
 * \file gps_driver.h
 * \brief Driver to interface UART of the NEO6M GPS module.
 * \author SJVH
 * \date
 * \version
 */


 #ifndef __GPS_H__
 #define __GPS_H__
 #include <stdint.h>
 #include <string.h>
 #include "hardware/gpio.h"
 #include "hardware/uart.h"
 
 
 #define BAUD_RATE 9600 ///< Baud rate for GPS communication
 
 /** 
  * TODO: Review the buffer size, this is a guess*/
 #define GPS_BUFFER_SIZE 1024 ///< Size of the GPS data buffer
 
 typedef struct {
     int_least32_t value; ///< The integer part of the float value
     int_least32_t scale; ///< The scale factor
 } minmea_float_t;
 
 typedef struct {
     int hours; ///< Hours part of the time
     int minutes; ///< Minutes part of the time
     int seconds; ///< Seconds part of the time
     int microseconds; ///< Microseconds part of the time
 } minmea_time_t;
 
 typedef struct {
     float latitude; ///< Latitude in degrees
     char ns; ///< 'N' for North, 'S' for South
     float longitude; ///< Longitude in degrees
     char ew; ///< 'E' for East, 'W' for West
     char status; ///< 'A' for valid, 'V' for invalid
 } gps_data_t;
 
 typedef struct{
     minmea_float_t latitude; ///< Latitude as a minmea_float_t
     minmea_float_t longitude; ///< Longitude as a minmea_float_t
     minmea_time_t time; ///< Time as a minmea_time_t
     //enum minmea_gll_status status; ///< Status of the GPS data, valid or not
 } gps_full_data_t;
 
 enum minmea_gll_status {
     MINMEA_GLL_STATUS_DATA_VALID = 'A', ///< Data is valid
     MINMEA_GLL_STATUS_DATA_NOT_VALID = 'V', ///< Data is not valid
 };
 
 //bool is_valid; ///< Flag to indicate if the GPS data is valid
 
 /**  TODO: Configure with reserved memory (malloc, etc.) */
 // char gps_buffer[GPS_BUFFER_SIZE]; ///< Buffer to store incoming GPS data
 // uint16_t rx_buffer_index = 0; ///< Index for the GPS buffer
 
 /**
  * \brief Callback function to handle UART RX interrupts.
  * This function is called when data is received on the UART.
  * It reads characters from the UART and processes them
  * until a newline or carriage return is encountered.
  */
 void on_uart_rx();
 
 /**
  * \brief Initializes the GPS module.
  * This function sets up the UART for communication with the GPS module.
  * It configures the UART pins, sets the baud rate, and enables RX interrupts.
  * @param uart_id Pointer to the UART instance to use (e.g., uart0, uart1).
  * @param tx_pin GPIO pin number for UART TX (transmit).
  * @param rx_pin GPIO pin number for UART RX (receive).
  * @param gps_data Pointer to a gps_data_t structure to store the GPS data.
  */
 void init_gps(uart_inst_t * uart_id, uint8_t tx_pin, uint8_t rx_pin, gps_data_t *gps_data);
 
 // Function to extract 
 /**
  * \brief Checks if the GPS has a valid fix.
  * This function checks the status of the GPS data
  * to determine if it has a valid fix.
  * @return true if the GPS has a valid fix, false otherwise.
  */
 bool gps_has_fix();
 
 // Get the current GPS data
 int get_gps_data(void);
 
 /**
  * \brief Converts GPS coordinates from GMS (Degrees, Minutes, Seconds) to GDD (Degrees Decimal).
  * This function takes latitude and longitude in GMS format and converts them to GDD format.
  * @param latitude Pointer to the latitude value in GMS format.
  * @param ns Character indicating North ('N') or South ('S').
  * @param longitude Pointer to the longitude value in GMS format.
  * @param ew Character indicating East ('E') or West ('W').
  */
 void convert_gms_to_gdd(float *latitude, char ns, float *longitude, char ew);
 
 // TODO:
 // Get the full GPS data including time and status
 void get_gps_full_data(char *data, gps_full_data_t *gps_data);
 
 bool is_empty_field(const char *field);
 #endif // __GPS_H__