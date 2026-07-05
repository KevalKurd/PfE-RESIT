//
// Created by k3-mustafa on 28/06/2026.
//


#ifndef CW_IO_H
#define CW_IO_H
#include "adc.h"


//Reads the binary ADC file
//Parameters: filename, header(which is the pointer to the header structure) and samples(pointer to the array of ADC samples)
int readADCFile(const char *filename, ADCFileHeader *header, ADCSample **samples);


#endif //CW_IO_H