/**
 * @brief
 *    Box Type: 'moov' - Movie Box
 *    Container: File
 *    Mandatory: yes
 *    Quantity: Exactly one
 *
 *    The metadata for a presentation is stored in the single Movie Box
 *    which occurs at the top-level of a file. Normally this box is close
 *    to the beginning or end of the file, though this is not required.
 * */
#ifndef MOOV_BOX_H_
#define MOOV_BOX_H_

#include "Mvhd_Box.h"
#include "Trak_Box.h"

#define MAX_MOOV_BOX_SIZE   1024 * 100

typedef struct Tag_Moov_Box {
    Box_Header * boxheader;
    Mvhd_Box * mvhdbox;
    Trak_Box * trakbox_video;
    Trak_Box * trakbox_audio;
} Moov_Box;

int AllocStruct_Moov_Box(Moov_Box ** moovbox);
int FreeStruct_Moov_Box(Moov_Box * moovbox);

int Write_Moov_Moov_Box(unsigned char * Buf,
                        unsigned int timescale_Mvhd,
                        unsigned int duration_Mvhd,
                        unsigned int next_track_ID_Mvhd,
                        unsigned int duration_tkhd_video,
                        unsigned int duration_tkhd_audio,
                        unsigned int width_tkhd,
                        unsigned int height_tkhd,
                        unsigned int sample_count_stsz_video,
                        unsigned int * entry_size_stsz_video,
                        unsigned int sample_count_stsz_audio,
                        unsigned int * entry_size_stsz_audio,
                        unsigned int entry_count_stss,
                        unsigned int * sample_number_stss,
                        unsigned int entry_count_stsc_video,
                        unsigned int * first_chunk_stsc_video,
                        unsigned int * samples_per_chunk_stsc_video,
                        unsigned int * sample_description_index_stsc_video,
                        unsigned int entry_count_stsc_audio,
                        unsigned int * first_chunk_stsc_audio,
                        unsigned int * samples_per_chunk_stsc_audio,
                        unsigned int * sample_description_index_stsc_audio,
                        unsigned int entry_count_stco_video,
                        unsigned int * chunk_offset_stco_video,
                        unsigned int entry_count_stco_audio,
                        unsigned int * chunk_offset_stco_audio,
                        unsigned int entry_count_stts_video,
                        unsigned int *sample_count_stts_video,
                        unsigned int *sample_delta_stts_video,
                        unsigned int entry_count_stts_audio,
                        unsigned int *sample_count_stts_audio,
                        unsigned int *sample_delta_stts_audio,
                        unsigned int entry_count_ctts,
                        unsigned int * sample_count_ctts,
                        unsigned int *sample_offset_ctts,
                        unsigned char * spsbuf,
                        unsigned int spslength,
                        unsigned char * ppsbuf,
                        unsigned int ppslength,
                        unsigned int channelcount_stsd,
                        unsigned int samplerate_stsd,
                        unsigned char audioObjectType_stsd);

#endif
