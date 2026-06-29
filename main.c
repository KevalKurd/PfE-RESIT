#include <stdio.h>
#include "adc.h"

int main(void)
{
    uint16_t testRawValue = 2046; //Test raw ADC value

    double voltage = convertRawToVoltage(testRawValue); //Convert the raw value into a voltage

    //Display the results
    printf("Raw ADC value : %u\n", testRawValue);
    printf("Voltage       : %.3f V\n", voltage);

    return 0;
}