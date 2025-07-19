#ifndef __ADC_NOISE_H
#define __ADC_NOISE_H

/**
 * @file adc_noise.h
 * @brief Header file for ADC noise measurement functions.
 * This file contains the function declarations and data structures for measuring noise using the ADC.
 */

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    uint8_t count; ///< Number of samples taken
    float sum; ///< Sum of the ADC values
    float intensidad; ///< Calculated noise intensity in dB
    bool data_ready; ///< Flag to indicate if data is ready
    bool taking_data; ///< Flag to indicate if data is being taken
} adc_acc_t;


/**
 * @brief Initializes the ADC for noise measurement.
 * This function sets up the ADC peripheral for noise measurement.
 * It should be called before starting any measurements.
 */
void my_adc_init(void);

/**
 * @brief Measures noise using the ADC.
 * This function takes multiple samples from the ADC and calculates the noise intensity.
 * It updates the adc_acc_t structure with the results.
 * @param adc Pointer to the adc_acc_t structure to store results.
 */
void measure_noise(adc_acc_t *adc);

#endif // __ADC_NOISE_H