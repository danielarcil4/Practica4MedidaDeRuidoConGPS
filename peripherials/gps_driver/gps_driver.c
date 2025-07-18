#include "gps_driver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

/**
 * Versión 1: Con interrupciones
 * Se "deshabilita" el FIFO, se lee dato caracter a caracter
 * Cada que llegue al caracter de fin de línea, se procesa la línea completa
 * y se llena la estructura de datos.
 */
// RX interrupt handler
uart_inst_t *UART_ID = uart1; ///< UART instance to use for GPS communication
static int chars_rxed = 0;


static char gps_buffer[GPS_BUFFER_SIZE]; ///< Buffer to store incoming GPS data
static uint16_t rx_buffer_index = 0; ///< Index for the GPS buffer
static gps_data_t *local_gps_data; ///< GPS data structure to store parsed data
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        if(ch == '\n' || ch == '\r') {
            // End of line, process the line
            if (rx_buffer_index > 0) {
                gps_buffer[rx_buffer_index] = '\0'; // Null-terminate the string
                //printf("Received GPS data: %s\n", gps_buffer);
                get_gps_data(); // Process the GPS data
                rx_buffer_index = 0; // Reset index for next line
                chars_rxed++;
            }
        }else {
            if(rx_buffer_index >= GPS_BUFFER_SIZE){
                // Buffer overflow, reset index
                rx_buffer_index = 0;
            }
            gps_buffer[rx_buffer_index++] = ch; // Store the character
            
            }
        }
    }

void init_gps(uart_inst_t * uart_id, uint8_t tx_pin, uint8_t rx_pin, gps_data_t *gps_data) {
    UART_ID = uart_id; // Set the UART instance
    uart_init(uart_id, BAUD_RATE);
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);

    // Turn off FIFO's - we want to do this character by character
    // TODO: Compare perfomance with FIFO enabled
    uart_set_fifo_enabled(uart_id, false);

    // Configurar el UART para recibir interrupciones
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    
    // Enable RX interrupts, disable TX interrupts
    uart_set_irq_enables(uart_id, true, false);
    
    // Copy the gps_data pointer to a local variable
    // This allows us to fill the gps_data structure with parsed data
    local_gps_data = gps_data;
    local_gps_data->latitude = 0.0f;
    local_gps_data->longitude = 0.0f;
    local_gps_data->status = 'V'; // Default to invalid status
}

// Decode necessary functions

static int hex2int(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

bool is_empty_field(const char *field) {
    return field == NULL || strlen(field) == 0;
}
int get_gps_data(void) {
    char *token;
    int field = 0;

    char *lat = NULL, *ns = NULL;
    char *lon = NULL, *ew = NULL;
    char *estado = NULL;
    //printf("%s\n", gps_buffer);
    token = strtok(gps_buffer, ",");
    while (token != NULL) {
        switch (field) {
            case 0:
                if (strcmp(token, "$GPGLL") != 0) {
                    //printf("No es un mensaje GPGLL\n");
                    return 1;
                }
                break;
            case 1: lat = token; break;
            case 2: ns  = token; break;
            case 3: lon = token; break;
            case 4: ew  = token; break;
            case 6: estado = token; break;
        }
        // Increment field index
        field++;
        // Get the next token
        token = strtok(NULL, ",");
    }
    
    // Check if we have all the necessary fields
    if (is_empty_field(lat) || is_empty_field(lon) || 
        strcmp(estado, "A") != 0) {
        //printf("Mensaje no válido o sin datos de posición\n");
        local_gps_data->latitude = 0.0f;
        local_gps_data->ns = 'V'; // Default to invalid status
        local_gps_data->longitude = 0.0f;
        local_gps_data->ew = 'V'; // Default to invalid status
        local_gps_data->status = 'V'; // Default to invalid status
        return 1; // Indicate that the data is not valid
    } else {
        //printf("Lat: %s %s | Lon: %s %s\n", lat, ns, lon, ew);

        // Convert latitude and longitude to float
        local_gps_data->latitude = atof(lat);
        local_gps_data->ns = ns[0]; // Assuming ns is a single character
        local_gps_data->longitude = atof(lon);
        local_gps_data->ew = ew[0]; // Assuming ew is a single character
        local_gps_data->status = estado[0]; // Assuming estado is a single character
    }
}

bool gps_has_fix() {
    // Check if the GPS data has a valid fix
    return local_gps_data->status == 'A';
}

/**
 * Versión 2: Con FIFO enabled
 * Se habilita el FIFO del UART
 * ¿Cómo sacar los datos del FIFO?
 * Cuando se llena el FIFO, se procesa la línea completa
 */