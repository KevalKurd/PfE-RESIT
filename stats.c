//
// Created by k3-mustafa on 28/06/2026.
//

#include <math.h> //Needed for sqrt() function
#include "stats.h"

/*
 * Function: calculateChannelStats()
 *
 * Purpose:
 * Calculates voltage statistics for each ADC channel.
 *
 * Engineering idea:
 * Each channel represents a different sensor input.
 * Therefore, we analyse each channel separately.
 */
//This function calculates the mean, minimum voltage, maximum voltage and the standard deviation
void calculateChannelStats(ADCSample *samples,uint32_t record_count,ChannelStats stats[ADC_CHANNELS])
{

    //Setting all starting values
    for (int ch = 0; ch < ADC_CHANNELS; ch++)
    {
        stats[ch].mean_voltage = 0.0;
        stats[ch].min_voltage = 9999.0;
        stats[ch].max_voltage = -9999.0;
        stats[ch].standard_deviation = 0.0;
        stats[ch].sample_count = 0;
    }


    //First pass to add voltages, find min/max and count samples per channel.
    for (uint32_t i = 0; i < record_count; i++)
    {
        ADCSample *currentSample = samples + i; // 'samples' points to the first ADCSample - samples + i moves forward by i

        int ch = currentSample->channel_id; //Retrieving channel number from the current sample

        if (ch >= 0 && ch < ADC_CHANNELS) // Checking validity of channel number
        {
            double voltage = currentSample->voltage; // Simply copying current voltage into a shorter name (easier to read)

            stats[ch].mean_voltage += voltage; // Add the current voltage to the running total
            stats[ch].sample_count++; // Increase the sample count by for this channel by 1

            if (voltage < stats[ch].min_voltage) // Checking if this voltage is lower than the current lowest
            {
                stats[ch].min_voltage = voltage; // If yes, save as the new minimum
            }

            if (voltage > stats[ch].max_voltage) //Checking if this voltage is higher than the highest voltage found
            {
                stats[ch].max_voltage = voltage; // If yes, save it as the new maximum
            }
        }
    }

    // Calculating the mean voltage
    for (int ch = 0; ch < ADC_CHANNELS; ch++)
    {
        if (stats[ch].sample_count > 0) // Checking that the channel has samples
        {
            stats[ch].mean_voltage = stats[ch].mean_voltage / stats[ch].sample_count;
        }
    }


    //Second pass needed as s.d requires the mean and mean was not able to be calculated during first pass
    for (uint32_t i = 0; i < record_count; i++)
    {
        ADCSample *currentSample = samples + i; //Pointing to first sample

        int ch = currentSample->channel_id; //Finding channel number

        if (ch >= 0 && ch < ADC_CHANNELS) // Checking validation of channel number
        {
            double difference = currentSample->voltage - stats[ch].mean_voltage; // Checking how far the current voltage is from the mean voltage

            stats[ch].standard_deviation += difference * difference; //Squaring the difference and add it to the total
        }
    }


    // Dividing the total squared difference by sample count
    for (int ch = 0; ch < ADC_CHANNELS; ch++)
    {
        if (stats[ch].sample_count > 0)
        {
            stats[ch].standard_deviation = sqrt(stats[ch].standard_deviation / stats[ch].sample_count);
        }
    }
}