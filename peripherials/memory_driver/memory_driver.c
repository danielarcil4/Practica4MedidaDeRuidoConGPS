#include "peripherials/memory_driver/memory_driver.h"
#include "pico/stdlib.h"

#include <stdio.h>
#include <string.h>

#define I2C_MEMORY_PORT i2c0
#define SDA_PIN 14
#define SCL_PIN 15
#define I2C_FREQUENCY 100000 // 100kHz



static i2c_inst_t *i2c_eeprom;

void eeprom_init() {
    i2c_eeprom = I2C_MEMORY_PORT;
    i2c_init(I2C_MEMORY_PORT, I2C_FREQUENCY); // 100kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

static uint8_t eeprom_get_control_byte(uint8_t mem_addr) {
    uint8_t block = mem_addr / EEPROM_BLOCK_SIZE;
    //printf("Control byte: %02X, Block: %d, Read: %d\n", (SLAVE_ADD << 4) , (block << 1));
    return  ((SLAVE_ADD<<3) | (block));
}

bool eeprom_write_byte(uint16_t mem_addr, uint8_t data) {
    printf("Writing byte to EEPROM at address 0x%04X: 0x%02X\n", mem_addr, data);
    uint8_t control = eeprom_get_control_byte(mem_addr);
    uint8_t addr = mem_addr % EEPROM_BLOCK_SIZE;
    uint8_t buf[2] = {addr, data};

    //print control
    printf("Control byte: 0x%02X, Addr: 0x%02X, Data: 0x%02X\n", control, addr, data);

    int ret = i2c_write_blocking(i2c_eeprom, control, buf, 2, false);
    printf("Write result: %d\n", ret);
    sleep_ms(5); // tiempo de escritura típico
    return ret == 2;
}

bool eeprom_write_page(uint16_t mem_addr, const uint8_t *data, uint8_t len) {
    if (len > EEPROM_PAGE_SIZE) return false;

    uint8_t control = eeprom_get_control_byte(mem_addr);
    uint8_t addr = mem_addr % EEPROM_BLOCK_SIZE;

    uint8_t buf[1 + EEPROM_PAGE_SIZE];
    buf[0] = addr;
    for (uint8_t i = 0; i < len; i++)
        buf[i + 1] = data[i];

    int ret = i2c_write_blocking(i2c_eeprom, control, buf, len + 1, false);
    sleep_ms(5);
    return ret == len + 1;
}

bool eeprom_read_byte(uint16_t mem_addr, uint8_t *data) {
    uint8_t control_w = eeprom_get_control_byte(mem_addr);
    uint8_t control_r = eeprom_get_control_byte(mem_addr);
    uint8_t addr = mem_addr % EEPROM_BLOCK_SIZE;

    int ret = i2c_write_blocking(i2c_eeprom, control_w, &addr, 1, true);
    if (ret != 1) return false;

    ret = i2c_read_blocking(i2c_eeprom, control_r, data, 1, false);
    return ret == 1;
}

bool eeprom_read_data(uint16_t mem_addr, uint8_t *buffer, uint8_t len) {
    uint8_t control_w = eeprom_get_control_byte(mem_addr);
    uint8_t control_r = eeprom_get_control_byte(mem_addr);
    uint8_t addr = mem_addr % EEPROM_BLOCK_SIZE;

    int ret = i2c_write_blocking(i2c_eeprom, control_w, &addr, 1, true);
    if (ret != 1) return false;

    ret = i2c_read_blocking(i2c_eeprom, control_r, buffer, len, false);
    return ret == len;
}

bool eeprom_get_index(uint8_t *data) {
    return eeprom_read_byte(DIR_INDEX, data);
}

void eeprom_set_index(uint8_t index){
    // escribir el índice en la dirección DIR_INDEX
    if (index > MAX_CAPTURAS) {
        printf("Índice fuera de rango: %d\n", index);
        return; // Índice fuera de rango
    }else{
        if (!eeprom_write_byte(DIR_INDEX, index)) {
            printf("Error al escribir el índice en la EEPROM\n");
        } else {
            printf("Índice actualizado a: %d\n", index);
        }
    }
}

void save_measurement(char * datos){
    eeprom_write_page(0x00,datos,30);
}

bool eeprom_write_measurement(uint16_t mem_addr, float noise, float lat, float lon) {
    uint8_t index;
    if (!eeprom_get_index(&index)){
        printf("ERROR EN LEER INDEX");
        return false; // Failed to read index
    }
    // Prepare a buffer to hold the 12 bytes (3 floats)
    uint8_t buf[4];

    //escribir float de noise 4 datos
    if (mem_addr % EEPROM_PAGE_SIZE + sizeof(buf) > EEPROM_PAGE_SIZE) {
        return false; // Not enough space in the current page
    }
    memcpy(buf, &noise, sizeof(float));  // Copy noise
    eeprom_write_page(BLOQUE_NOISE*DIR_BASE + index, buf, sizeof(float)); // Write noise

    memcpy(buf, &lat, sizeof(float)); // Copy lat
    eeprom_write_page(BLOQUE_LAT*DIR_BASE+ index, buf, sizeof(float)); // Write noise

    memcpy(buf, &lon, sizeof(float)); // Copy lon
    eeprom_write_page(BLOQUE_LON*DIR_BASE+ index, buf, sizeof(float)); // Write noise

    return eeprom_write_page(mem_addr, buf, sizeof(buf));
}

bool eeprom_read_measurement(uint16_t mem_addr, float *noise, float *lat, float *lon) {
    uint8_t index;
    if (!eeprom_get_index(&index)){
        printf("ERROR EN LEER INDEX");
        return false; // Failed to read index
    }

    uint8_t buf[4];
    
    // Read the 12 bytes of data from EEPROM
    if (eeprom_read_data(BLOQUE_LAT*DIR_BASE + index, buf, sizeof(buf))) {
        memcpy(noise, buf, sizeof(float)); 
    }
    if (eeprom_read_data(BLOQUE_LAT*DIR_BASE + index, buf, sizeof(buf))) {
        memcpy(lat, buf, sizeof(float));   
    }
    if (eeprom_read_data(BLOQUE_LAT*DIR_BASE + index, buf, sizeof(buf))) {
        memcpy(lon, buf, sizeof(float));    
    }

    return true; // Successful read
}