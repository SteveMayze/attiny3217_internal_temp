/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "util/delay.h"
// #include <stdio.h>
#include "mcc_generated_files/include/adc0.h"

volatile adc_result_t ADC_0_measurement;
volatile uint8_t      ADC_0_measurement_normalized;

uint8_t read_temperature(void)
{

	// Test driver functions, assume that an AIN channel is enabled and that
	// the Result Ready IRQ is enabled.

	// Test polled mode

	// Get conversion from specified ADC channel
	ADC_0_measurement = ADC0_GetConversion(ADC_MUXPOS_TEMPSENSE_gc);

	int8_t signrow_offset = SIGROW.TEMPSENSE1;
	uint8_t sigrow_gain = SIGROW.TEMPSENSE0;

	uint32_t temp = ADC_0_measurement - signrow_offset;
	temp *= sigrow_gain;
	temp += 0x80;
	temp >>= 8;

	// Get 8 MSB of conversion result
	ADC_0_measurement_normalized = ADC_0_measurement >> (ADC0_GetResolution() - 8);

	volatile uint8_t temp_c = temp - 273.15;
	volatile uint8_t temp_f = temp * (9/5) - 459.67;

	// printf( ">> measurement: %d, normalised: %d, temp in K: %ld, temp C: %d, temp F: %d   \n", 
	printf( ">> temp C: %d, temp F: %d   \r\n", 
            // ADC_0_measurement, 
            // ADC_0_measurement_normalized, 
            // temp, 
            temp_c, 
            temp_f
            );

	return 1;
}




/*
    Main application
*/
int main(void)
{
    /* Initializes MCU, drivers and middleware */
    SYSTEM_Initialize();
    
    printf("Starting...\r\n");
    USER_LED0_SetHigh();
    /* Replace with your application code */
    while (1){
        USER_LED0_Toggle();
        printf(">> reading...\r\n");
        read_temperature();
        _delay_ms(5000);
    }
}
/**
    End of File
*/