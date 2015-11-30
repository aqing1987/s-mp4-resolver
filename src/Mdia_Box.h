/**
 * @brief
 *    Box Type: 'mdia' - Media Box
 *    Container: Track Box (‘trak’)
 *    Mandatory: yes
 *    Quantity: Exactly one
 *
 *    The media declaration container contains all the objects that declare
 *    information about the media data within a track.
 * */
#ifndef MDIA_BOX_H_
#define MDIA_BOX_H_

#include "Mdhd_Box.h"
#include "Hdlr_Box.h"
#include "Minf_Box.h"

typedef struct Tag_Mdia_Box {
    Box_Header * boxheader;
    Mdhd_Box * mdhdbox;
    Hdlr_Box * hdlrbox;
    Minf_Box * minfbox;
} Mdia_Box;

int AllocStruct_Mdia_Box(Mdia_Box ** mdiabox);
int FreeStruct_Mdia_Box(Mdia_Box * mdiabox);

int Write_Mdia_Mdia_Box_Video(unsigned char * Buf,
                              unsigned int timescale_mdhd_video,
                              unsigned int duration_mdhd_video,
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

int Write_Mdia_Mdia_Box_Audio(unsigned char * Buf,
                              unsigned int timescale_mdhd_audio,
                              unsigned int duration_mdhd_audio,
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
