//
// Created by k3-mustafa on 28/06/2026.
//

#include "adc.h"

//Function called convertRawToVoltage
//Converts a raw ADC reading into its equivalent voltage using the formula: Voltage = (Raw ADC Value / 4095) x 3.3
//The parameter is raw_value which is the 12-bit value
//The return is the voltage
double convertRawToVoltage(uint16_t raw_value) //u = unsigned, int = integer, 16 = 16 bits, _t = type.
{
    return (raw_value / ADC_MAX_VALUE) * ADC_VREF;
}

//This function will go through every sample and calculate each voltage
void calculateVoltages(ADCSample *samples, uint32_t record_count)
{
    for (uint32_t i = 0; i < record_count; i++)
    {
         // samples + i points to sample number i

        ADCSample *currentSample = samples + i;

        // Store the calculated voltage inside the structure

        currentSample->voltage = convertRawToVoltage(currentSample->raw_value);
    }
}