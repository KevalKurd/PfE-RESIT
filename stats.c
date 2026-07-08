//
// Created by k3-mustafa on 28/06/2026.
//

#include <math.h> //Needed for sqrt() function
#include "stats.h"


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


// Function that checks every sample for faults
void detectFaults(ADCSample *samples, uint32_t record_count, FaultStats faults[ADC_CHANNELS])
{
    //Setting all values to 0
    for (int ch = 0; ch < ADC_CHANNELS; ch++)
    {
        faults[ch].over_voltage_count = 0;
        faults[ch].under_voltage_count = 0;
        faults[ch].sensor_fault_count = 0;
    }

    // Go through every sample and record that number using record_count
    for (uint32_t i = 0; i < record_count; i++)
    {
        ADCSample *currentSample = samples + i;

        //Getting and checking channel number
        int ch = currentSample->channel_id;

        if (ch >= 0 && ch < ADC_CHANNELS)
        {
            //Overvoltage check
            if (currentSample->voltage > 3.0)
            {
                faults[ch].over_voltage_count++;
            }

            //Undervoltage check
            if (currentSample->voltage < 0.3)
            {
                faults[ch].under_voltage_count++;
            }


            // Bit 0 means sensor fault and 0x01 in binary is 00000001
            if ((currentSample->status_flags & 0x01) != 0)
            {
                faults[ch].sensor_fault_count++;
            }
        }
    }
}


 //Function to check the sequence numbers of the records
 // Each ADC record has a sequence number that should increase by 1 each time
 // Therefore is easy to spot out of order or missing records
void checkSequenceIntegrity(ADCSample *samples, uint32_t record_count, IntegrityStats *integrity)
{

    // Setting both values to 0
    integrity->missing_records = 0;
    integrity->out_of_order_records = 0;


    // Number of records must be 2 or more to be compared
    if (record_count < 2)
    {
        return;
    }


    // Starting loop at record 1 as that would mean there are 2 records to compare
    for (uint32_t i = 1; i < record_count; i++)
    {

        // Pointer arithmetic to access the samples
        ADCSample *currentSample = samples + i;
        ADCSample *previousSample = samples + i - 1;


        // The expected sequence number should always be 1 more than the previous
        uint32_t expectedSequence = previousSample->sequence_number + 1;


        // If current sequence is greater than expected - there are 1 or more records missing
        if (currentSample->sequence_number > expectedSequence)
        {
            integrity->missing_records += currentSample->sequence_number - expectedSequence;
        }


        // If current sequence number is less than or equal to previous sequence number, data has gone backwards or repeated
        else if (currentSample->sequence_number <= previousSample->sequence_number)
        {
            integrity->out_of_order_records++;
        }
    }
}