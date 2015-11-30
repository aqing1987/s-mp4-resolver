/**
 * @brief
 *    Box Type: 'minf' - Media Information Box
 *    Container: Media Box ('mdia')
 *    Mandatory: Yes
 *    Quantity: Exactly one
 *
 *    This box contains all the objects that declare characteristic
 *    information of the media in the track.
 * */
#ifndef MINF_BOX_H_
#define MINF_BOX_H_

#include "Vmhd_Box.h"
#include "Smhd_Box.h"
#include "Dinf_Box.h"
#include "Stbl_Box.h"

typedef struct Tag_Minf_Box {
    Box_Header * boxheader;
    Vmhd_Box * vmhdbox;
    Smhd_Box * smhdbox;
    Dinf_Box * dinfbox;
    Stbl_Box * stblbox;
} Minf_Box;

int AllocStruct_Minf_Box(Minf_Box ** minfbox);
int FreeStruct_Minf_Box(Minf_Box * minfbox);

int Write_Minf_Minf_Box_Video(unsigned char * Buf,
                              unsigned int sample_count_stsz_video,
                              unsigned int * entry_size_stsz_video,
                              unsigned int entry_count_stss,
                              unsigned int * sample_number_stss,
                              unsigned int entry_count_stsc_video,
                              unsigned int * first_chunk_stsc_video,
                              unsigned int * samples_per_chunk_stsc_video,
                              unsigned int * sample_description_index_stsc_video,
                              unsigned int entry_count_stco_video,
                              unsigned int * chunk_offset_stco_video,
                              unsigned int entry_count_stts_video,
                              unsigned int *sample_count_stts_video,
                              unsigned int *sample_delta_stts_video,
                              unsigned int entry_count_ctts,
                              unsigned int * sample_count_ctts,
                              unsigned int *sample_offset_ctts,
                              unsigned int width_stsd,
                              unsigned int height_stsd,
                              unsigned char * spsbuf,
                              unsigned int spslength,
                              unsigned char * ppsbuf,
                              unsigned int ppslength);

int Write_Minf_Minf_Box_Audio(unsigned char * Buf,
                              unsigned int sample_count_stsz_audio,
                              unsigned int * entry_size_stsz_audio,
                              unsigned int entry_count_stsc_audio,
                              unsigned int * first_chunk_stsc_audio,
                              unsigned int * samples_per_chunk_stsc_audio,
                              unsigned int * sample_description_index_stsc_audio,
                              unsigned int entry_count_stco_audio,
                              unsigned int * chunk_offset_stco_audio,
                              unsigned int entry_count_stts_audio,
                              unsigned int *sample_count_stts_audio,
                              unsigned int *sample_delta_stts_audio,
                              unsigned int channelcount_stsd,
                              unsigned int samplerate_stsd,
                              unsigned char audioObjectType_stsd);

#endif
