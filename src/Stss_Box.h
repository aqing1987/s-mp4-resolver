/**
 * @brief
 *    Box Type: 'stss' - Sync Sample Box
 *    Container: Sample Table Box ('stbl')
 *    Mandatory: No
 *    Quantity: Zero or one
 *
 *    This box provides a compact marking of the random access points within
 *    the stream. The table is arranged in strictly increasing order of
 *    sample number.
 *
 *    If the sync sample box is not present, every sample is a random access
 *    point.
 *    
 * */
#ifndef STSS_BOX_H_
#define STSS_BOX_H_

#include "Mp4.h"

typedef struct Tag_Stss_Box {
    FullBox_Header * boxheader;

    // an integer that gives the number of entries in the following table.
    // If entry_count is zero, there are no random access points within
    // the stream and the following table is empty.
    unsigned int entry_count;
    // gives the numbers of the samples that are random access points
    // in the stream.
    unsigned int * sample_number;
} Stss_Box;

int AllocStruct_Stss_Box(Stss_Box ** stssbox);
int FreeStruct_Stss_Box(Stss_Box * stssbox);
int Write_Stss_Stss_Box(unsigned char * Buf,
                        unsigned int entry_count_stss,
                        unsigned int * sample_number_stss);

#endif
