#include <stdio.h>
#include <stdlib.h> //Allows malloc(), free(), exit()

#include "adc.h" //Includes my structs, constants and function prototypes
#include "io.h"


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


    //If we reach here, the file header has been read correctly.
    printf("\nADC file opened successfully.\n");

    printf("Channels     : %u\n", header.channel_count);

    printf("Records      : %u\n", header.record_count);

    printf("Sample Rate  : %u Hz\n", header.sample_rate_hz);


    //free() will release the memory back
    free(samples);
    return 0;
}