/**
 * @brief
 *    Box Type: 'stsc' - Sample To Chunk Box
 *    Container: Sample Table Box ('stbl')
 *    Mandatory: Yes
 *    Quantity: Exactly one
 *
 *    Samples within the media data are grouped into chunks. Chunks can be
 *    of different sizes, and the samples within a chunk can have different
 *    sizes. This table can be used to find the chunk that contains a sample
 *    its position, and the associated sample description.
 *
 *    The table is compactly coded. Each entry gives the index of the first
 *    chunk of a run of chunks with the same characteristics. By subtracting
 *    one entry here from the previous one, you can compute how many chunks
 *    are in this run. You can convert this to a sample count by multiplying
 *    by the appropriate samples-per-chunk.
 * */
#ifndef STSC_BOX_H_
#define STSC_BOX_H_

#include "Mp4.h"

typedef struct Tag_Stsc_Box {
    FullBox_Header * boxheader;
    unsigned int entry_count;
    unsigned int * first_chunk;
    unsigned int * samples_per_chunk;
    unsigned int * sample_description_index;
} Stsc_Box;

int AllocStruct_Stsc_Box(Stsc_Box ** stscbox);
int FreeStruct_Stsc_Box(Stsc_Box * stscbox);

int Write_Stsc_Stsc_Box_Video(unsigned char * Buf,
                              unsigned int entry_count_stsc_video,
                              unsigned int * first_chunk_stsc_video,
                              unsigned int * samples_per_chunk_stsc_video,
                              unsigned int * sample_description_index_stsc_video);

int Write_Stsc_Stsc_Box_Audio(unsigned char * Buf,
                              unsigned int entry_count_stsc_audio,
                              unsigned int * first_chunk_stsc_audio,
                              unsigned int * samples_per_chunk_stsc_audio,
                              unsigned int * sample_description_index_stsc_audio);

#endif
