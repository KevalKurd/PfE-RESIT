//
// Created by k3-mustafa on 28/06/2026.
//

#ifndef CW_ADC_H
#define CW_ADC_H

#include <stdint.h> //Include fixed-width integer data types.

#define ADC_CHANNELS 4 //Number of channels in the data set
#define ADC_MAX_VALUE 4095.0 //Maxvalue of a 12-bit ADC7
#define ADC_VREF 3.3 //Ref. voltage of the ADC
#define MAGIC_NUMBER 0xADC1BEEF //Expected magic number found in the file header


//This is the structure to store the binary file header
//It is read once at the start of the program
//The information tells us if the file is valid or not along with how many records need to be loaded
typedef struct {
    uint32_t magic; //Checks file is correct
    uint16_t version; //File format version
    uint16_t channel_count; //Number of ADC channels - expected to be 4
    uint32_t record_count; //Number of sensor records in the file
    uint32_t sample_rate_hz; //ADC sample rate - expected to be 1000 Hz
    uint8_t reserved[8]; //Reserved bytes
} ADCFileHeader;


//Structure to store one ADC sensor reading
//Every record that is read from the binary file is stores in 1 ADCSample structure
typedef struct {
    float timestamp; //The time since the recording started in seconds
    uint8_t channel_id; //The ADC channel number from 0-3
    uint16_t raw_value; //12-bit ADC value 0-4095
    double voltage; // Calculated voltage from the 12-bit ADC value
    int16_t temperature; //Temperature in tenths of a degree in Celsius
    uint8_t status_flags; //Status bit to indicate sensor faults
    uint32_t sequence_number; //Detection of missing/out-of-order samples
} ADCSample;


//Structure matches the 16-byte record stored in the binary file
#pragma pack(push, 1)
typedef struct {
    float timestamp;
    uint8_t channel_id;
    uint16_t raw_value;
    int16_t temperature;
    uint8_t status_flags;
    uint32_t sequence_number;
    uint8_t reserved[2];
} ADCBinaryRecord;

#pragma pack(pop)

//Function prototype - converts a raw ADC value to a voltage
//Parameters: raw_value - ADC value between 0 and 4095
//Returns: Calculated voltage between 0 V and 3 V
double convertRawToVoltage(uint16_t raw_value);
#endif //CW_ADC_H
