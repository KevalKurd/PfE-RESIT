//
// Created by k3-mustafa on 28/06/2026.
//
#ifndef CW_STATS_H
#define CW_STATS_H

#include <stdint.h>
#include "adc.h"


//This struct will store the calculated statistics for 1 singular ADC channel
typedef struct {
    double mean_voltage;
    double min_voltage;
    double max_voltage;
    double standard_deviation;
    int sample_count;
} ChannelStats;


// Stores fault totals for one ADC channel
typedef struct {
    int over_voltage_count;
    int under_voltage_count;
    int sensor_fault_count;
} FaultStats;


// Function prototype
// samples pointer to first ADC sample in dynamically allocated memory
// record_count - total number of records that need to be checked
// faults - array that will store the total faults for all 4 channels
void detectFaults(ADCSample *samples, uint32_t record_count, FaultStats faults[ADC_CHANNELS]);

//Calculates mean, minimum, maximum and standard deviation for each of the 4 ADC channels
void calculateChannelStats(ADCSample *samples,uint32_t record_count,ChannelStats stats[ADC_CHANNELS]);

#endif // CW_STATS_H