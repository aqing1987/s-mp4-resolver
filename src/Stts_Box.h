/**
 * @brief
 *    Box Type: 'stts' - TimeToSampleBox
 *    Container: Sample Table Box ('stbl')
 *    Mandatory: Yes
 *    Quantity: exactly one
 *
 *    This box contains a compact version of a table that allows indexing
 *    from decoding time to sample number. Other tables give sample sizes
 *    and pointers, from the sample number. Each entry in the table gives
 *    the number of consecutive samples with the same time delta, and the
 *    delta of those samples. By adding the deltas a complete time-to-sample
 *    map may be built.
 *
 *    The Decoding Time to Sample Box contains decode time delta's:
 *    DT(n+1) = DT(n) + STTS(n) where STTS(n) is the (uncompressed) table
 *    entry for sample n.
 *
 *    The sample entries are ordered by decoding time stamps; therefore the
 *    deltas are all non-negative.
 *
 *    The DT axis has a zero origin; DT(i) = SUM(for j = 0 to i-1 of del-
 *    ta(j)), and the sum of all deltas gives the length of the media in the
 *    track (not mapped to the overall timescale, and not considering any
 *    edit list).
 *
 *    The Edit List Box provides the initial CT value if it is non-empty
 *    (non-zero).
 * */
#ifndef STTS_BOX_H_
#define STTS_BOX_H_

#include "Mp4.h"

typedef struct Tag_Stts_Box {
    FullBox_Header * boxheader;
    unsigned int entry_count;
    unsigned int * sample_count;
    unsigned int * sample_delta;
} Stts_Box;

int AllocStruct_Stts_Box(Stts_Box ** sttsbox);
int FreeStruct_Stts_Box(Stts_Box * sttsbox);

int Write_Stts_Stts_Box_Video(unsigned char * Buf,
                              unsigned int entry_count_stts_video,
                              unsigned int *sample_count_stts_video,
                              unsigned int *sample_delta_stts_video);

int Write_Stts_Stts_Box_Audio(unsigned char * Buf,
                              unsigned int entry_count_stts_audio,
                              unsigned int *sample_count_stts_audio,
                              unsigned int *sample_delta_stts_audio);

#endif
