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


//Calculates mean, minimum, maximum and standard deviation for each of the 4 ADC channels
void calculateChannelStats(ADCSample *samples,uint32_t record_count,ChannelStats stats[ADC_CHANNELS]);

#endif // CW_STATS_H