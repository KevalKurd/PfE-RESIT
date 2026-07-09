//
// Created by k3-mustafa on 28/06/2026.
//


#ifndef CW_IO_H
#define CW_IO_H
#include "adc.h"
#include "stats.h"

//Reads the binary ADC file
//Parameters: filename, header(which is the pointer to the header structure) and samples(pointer to the array of ADC samples)
int readADCFile(const char *filename, ADCFileHeader *header, ADCSample **samples);


//Function which will give a file name + calculated results
// char - string characters, const - name can't change
//ChannelStats stats[ADC_CHANNELS] will give the function access to the stats
//FaultStats faults[ADC_CHANNELS] gives the function the fault results for all 4 channels
//IntegrityStats *integrity is a pointer to my integrity structure (contains missing_records and out_of_order_records)
int writeResultsFile(const char *filename, ChannelStats stats[ADC_CHANNELS], FaultStats faults[ADC_CHANNELS], IntegrityStats *integrity);

#endif //CW_IO_H