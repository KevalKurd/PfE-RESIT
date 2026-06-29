//
// Created by k3-mustafa on 28/06/2026.
//

#include "adc.h"

//Function called convertRawToVoltage
//Converts a raw ADC reading into its equivalent voltage using the formula
//Formula: Voltage = (Raw ADC Value / 4095) x 3.3
//The parameter is raw_value which is the 12-bit value
//The return is the voltage
double convertRawToVoltage(uint16_t raw_value)
{
    return (raw_value / ADC_MAX_VALUE) * ADC_VREF;
}