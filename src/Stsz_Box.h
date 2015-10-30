/**
 * @brief
 *    Sample Size Box
 *    Box Type: 'stsz', 'stz2'
 *    Container: Sample Table Box ('stbl')
 *    Mandatory: Yes
 *    Quantity: Exactly one variant must be present
 *
 *    This box contains the sample count and a table giving the size
 *    in bytes of each sample. This allows the media data itself to be
 *    unframed. 
 * */

#ifndef STSZ_BOX_H_
#define STSZ_BOX_H_

#include "Mp4.h"

typedef struct Tag_Stsz_Box {
    FullBox_Header * boxheader;

        // specifying the default sample size. If all the samples are
        // the same size, this field contains that size value. If this
        // field is set to 0, then the samples have different sizes,
        // and those sizes are stored in the sample size table. If this
        // field is not 0, it specifies the constant sample size, and
        // no array follows.
    unsigned int sample_size;

        // gives the number of samples in the track; if sample_size is
        // 0, then it is also the number of entries in the follwing table
    unsigned int sample_count;
    
        // specifying the size of a sample, indexed by its number
    unsigned int * entry_size;
} Stsz_Box;

int AllocStruct_Stsz_Box(Stsz_Box ** stszbox);
int FreeStruct_Stsz_Box(Stsz_Box * stszbox);
int Write_Stsz_Stsz_Box_Video(unsigned char * Buf, unsigned int sample_count_stsz_video, unsigned int * entry_size_stsz_video);
int Write_Stsz_Stsz_Box_Audio(unsigned char * Buf, unsigned int sample_count_stsz_audio, unsigned int * entry_size_stsz_audio);

#endif
