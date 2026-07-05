#include <stdio.h>
#include <stdlib.h>

#include "io.h"

#define EXPECTED_MAGIC        0xADC1BEEF
#define EXPECTED_VERSION      1
#define EXPECTED_CHANNELS     4
#define EXPECTED_SAMPLE_RATE  1000



/*Function: readADCFile()
 * Purpose:
 * Opens the binary ADC file
 * Reads the file header
 * Checks that the header information is correct
 * Allocates memory ready for all ADC samples
 *  It only prepares the program
*/
int readADCFile(const char *filename,ADCFileHeader *header,ADCSample **samples)
{

    FILE *file = fopen(filename, "rb"); //rb = read binary


    if (file == NULL) //Checking if fopen() worked
    {
        printf("Error: Could not open %s\n", filename);
        return 0;
    }

    // fread() reads raw binary bytes directly into the ADCFileHeader structure.
    if (fread(header,sizeof(ADCFileHeader),1,file) != 1)
    {
        printf("Error: Could not read the file header.\n");

        fclose(file);

        return 0;
    }

    //Checks the magic number
    //Every valid file should start with 0xADC1BEEF
    if (header->magic != EXPECTED_MAGIC)
    {
        printf("Error: Incorrect file type.\n");

        fclose(file);

        return 0;
    }


    //Checking the version number
    if (header->version != EXPECTED_VERSION)
    {
        printf("Error: Unsupported file version.\n");

        fclose(file);

        return 0;
    }

    //Checking that the number of channels is 4
    if (header->channel_count != EXPECTED_CHANNELS)
    {
        printf("Error: Expected 4 ADC channels.\n");

        fclose(file);

        return 0;
    }


    //Checks that the sample rate is at an expected rate of 1000 samples per second
    if (header->sample_rate_hz != EXPECTED_SAMPLE_RATE)
    {
        printf("Error: Incorrect sample rate.\n");

        fclose(file);

        return 0;
    }


    //Memory allocation
    //Number of ADC samples stored is unknown until the file header is read
    //So i have used malloc(). record_count tells us xactly how many ADCSample structures we need
    *samples = malloc(header->record_count * sizeof(ADCSample));


    //Checking malloc() - if malloc() cannot reserve enough RAM, it will return NULL
    if (*samples == NULL)
    {
        printf("Error: Memory allocation failed.\n");

        fclose(file);

        return 0;
    }
    /*
        * Now read every 16-byte binary record from the file.
        *
        * We read into ADCBinaryRecord first because that matches the file.
        * Then we copy the values into ADCSample, which is the struct our
        * analysis code will use later.
        */
    //Read every 16-byte binary record from the file
    //Read into ADCBinaryRecord first because that matches the file
    //Then we copy the values into ADCSample, which is the struct that the analysis code uses
    for (uint32_t i = 0; i < header->record_count; i++)
    {
        ADCBinaryRecord record;


        //fread reads 1 binary record from the file
        //If 1 is not returned, the reading has failed
        if (fread(&record, sizeof(ADCBinaryRecord), 1, file) != 1)
        {
            printf("Error: Could not read ADC record %u.\n", i);

            free(*samples);
            *samples = NULL;

            fclose(file);
            return 0;
        }


        //Record stored in our main samples array
        //Pointer arithmetic is used to go to a specific pointer
        ADCSample *currentSample = (*samples + i);

        currentSample->timestamp = record.timestamp;
        currentSample->channel_id = record.channel_id;
        currentSample->raw_value = record.raw_value;


        // Voltage set to 0 as it has not been calculated as of yet
        currentSample->voltage = 0.0;

        currentSample->temperature = record.temperature;
        currentSample->status_flags = record.status_flags;
        currentSample->sequence_number = record.sequence_number;
    }


    fclose(file);

    return 1;
}