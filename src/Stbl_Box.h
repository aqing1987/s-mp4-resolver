#ifndef STBL_BOX_H_
#define STBL_BOX_H_

#include "Stsd_Box.h"
#include "Stts_Box.h"
#include "Ctts_Box.h"
#include "Stsc_Box.h"
#include "Stsz_Box.h"
#include "Stco_Box.h"
#include "Stss_Box.h"

typedef struct Tag_Stbl_Box {
    Box_Header *boxheader;

    Stsd_Box *stsdbox; // sample descriptions(codec types, initialization etc)
    Stts_Box *sttsbox; // (decoding) time-to-sample
    Ctts_Box *cttsbox; // (composition) time to sample
    Stsc_Box *stscbox; // sample-to-chunk, partial data-offset information
    Stsz_Box *stszbox; // sample sizes (framing)
    Stco_Box *stcobox; // chunk offset, partial data-offset information
    Stss_Box *stssbox; // sync sample table (random access points)
    
} Stbl_Box;

int AllocStruct_Stbl_Box(Stbl_Box **stblbox);
int FreeStruct_Stbl_Box(Stbl_Box *stblbox);
int Write_Stbl_Stbl_Box_Video(unsigned char *Buf,
                              unsigned int sample_count_stsz_video,
                              unsigned int *entry_size_stsz_video,
                              unsigned int entry_count_stss,
                              unsigned int *sample_number_stss,
                              unsigned int entry_count_stsc_video,
                              unsigned int *first_chunk_stsc_video,
                              unsigned int *samples_per_chunk_stsc_video,
                              unsigned int *sample_description_index_stsc_video,
                              unsigned int entry_count_stco_video,
                              unsigned int *chunk_offset_stco_video,
                              unsigned int entry_count_stts_video,
                              unsigned int *sample_count_stts_video,
                              unsigned int *sample_delta_stts_video,
                              unsigned int entry_count_ctts,
                              unsigned int *sample_count_ctts,
                              unsigned int *sample_offset_ctts,
                              unsigned int width_stsd,
                              unsigned int height_stsd,
                              unsigned char *spsbuf,
                              unsigned int spslength,
                              unsigned char *ppsbuf,
                              unsigned int ppslength);

int Write_Stbl_Stbl_Box_Audio(unsigned char *Buf,
                              unsigned int sample_count_stsz_audio,
                              unsigned int *entry_size_stsz_audio,
                              unsigned int entry_count_stsc_audio,
                              unsigned int *first_chunk_stsc_audio,
                              unsigned int *samples_per_chunk_stsc_audio,
                              unsigned int *sample_description_index_stsc_audio,
                              unsigned int entry_count_stco_audio,
                              unsigned int *chunk_offset_stco_audio,
                              unsigned int entry_count_stts_audio,
                              unsigned int *sample_count_stts_audio,
                              unsigned int *sample_delta_stts_audio,
                              unsigned int channelcount_stsd,
                              unsigned int samplerate_stsd,
                              unsigned char audioObjectType_stsd);

#endif // STBL_BOX_H_
