#include <stdio.h>
#include <stdlib.h> //Allows malloc(), free(), exit()

#include "adc.h" //Includes my structs, constants and function prototypes
#include "io.h"
#include "stats.h"


//argc means argument count when program is run
//argv stores every word in the argument
int main(int argc, char *argv[])
{

    //This creates ONE structure
    ADCFileHeader header;


    //This pointer allows the header to be read to identify how much memory to reserve.
    ADCSample *samples = NULL;


    //The code would expect two arguments, the program name and the filename
    if (argc != 2)
    {
        printf("Usage: %s adc_sensor_log.bin\n", argv[0]); //This would print cw.exe

        return 1; //Stops program
    }


    //argv[1] would be adc_sensor_log.bin for my coursework
    //&header is the address of the header
    //Right now the pointer samples inside readADCFile() is empty - malloc() will change this
    //&samples is basically a pointer to a pointer as samples is declared as a pointer through *
    if (readADCFile(argv[1], &header, &samples) == 0)
    {
        printf("Program stopped.\n");

        return 1;
    }
    calculateVoltages(samples, header.record_count);


    //Create an array containing one structure for each of the 4 channels
    ChannelStats stats[ADC_CHANNELS];


    // Now sending the samples to the stats function - this function will calc then store in the stats array
    calculateChannelStats(samples, header.record_count, stats);


    //Creating an array to store fault total - one structure per channel
    FaultStats faults[ADC_CHANNELS];



    //Checking every sample for a fault
    detectFaults(samples, header.record_count, faults);


    //Creates 1 structure to store the results of the sequence number integrity check
    IntegrityStats integrity;


    //Check all the sequence numbers
    //Passing &integrity so the function can change the values inside the structure
    checkSequenceIntegrity(samples,header.record_count,&integrity);


    //If we reach here, the file header has been read correctly.
    printf("\nADC file opened successfully.\n");

    printf("Channels     : %u\n", header.channel_count);

    printf("Records      : %u\n", header.record_count);

    printf("Sample Rate  : %u Hz\n", header.sample_rate_hz);

    printf("\nFirst 5 ADC records:\n");

    for (int i = 0; i < 5; i++)
    {
        printf("Record %d | Time: %.4f s | CH: %u | Raw: %u | Voltage: %.6f V | Temp: %.1f C | Flags: 0x%02X | Seq: %u\n",
               i,
               samples[i].timestamp,
               samples[i].channel_id,
               samples[i].raw_value,
               samples[i].voltage,
               samples[i].temperature / 10.0,
               samples[i].status_flags,
               samples[i].sequence_number);
    }


    printf("\nChannel Statistics:\n");



    // Now go through each channel and print the results
    for (int ch = 0; ch < ADC_CHANNELS; ch++)
    {
        printf("\nChannel %d\n", ch);

        printf("Samples             : %d\n", stats[ch].sample_count);

        printf("Mean voltage        : %.6f V\n", stats[ch].mean_voltage);

        printf("Minimum voltage     : %.6f V\n", stats[ch].min_voltage);

        printf("Maximum voltage     : %.6f V\n", stats[ch].max_voltage);

        printf("Standard deviation  : %.6f V\n", stats[ch].standard_deviation);
    }


    printf("\nFault Detection:\n");


    for (int ch = 0; ch < ADC_CHANNELS; ch++)
    {
        printf("\nChannel %d\n", ch);

        //Show how many samples were above 3.0 V
        printf("Over-voltage faults  : %d\n",
               faults[ch].over_voltage_count);

        //Show how many samples were below 0.3 V
        printf("Under-voltage faults : %d\n",
               faults[ch].under_voltage_count);

        //Show how many samples had status flag bit 0 set
        printf("Sensor fault flags   : %d\n",
               faults[ch].sensor_fault_count);
    }


    //Display the integrity results
    printf("\nData Integrity Check:\n");

    printf("Missing records       : %d\n",
           integrity.missing_records);

    printf("Out-of-order records  : %d\n",
           integrity.out_of_order_records);


    //Writing the final analysis results to the file
    if (writeResultsFile("results.txt", stats, faults, &integrity) == 0)
    {
        printf("Error: Results file was not written.\n");
    }
    else
    {
        printf("\nResults written to results.txt\n");
    }

    //free() will release the memory back
    free(samples);
    return 0;
}