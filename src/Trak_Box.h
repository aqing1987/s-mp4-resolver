/**
 * @brief
 *    Box Type: 'trak' - Track Box
 *    Container: Movie Box ('moov')
 *    Mandatory: Yes
 *    Quantity: One or more
 *
 *    This is a container box for a single track of a presentation. A
 *    presentation consists of one or more tracks. Each track is indepen-
 *    dent of the other tracks in the presentation and carries its own
 *    temporal and spatial information. Each track will contain its asso-
 *    ciated Media Box.
 *
 *    Tracks are used for two purposes: (a) to contain media data (media
 *    tracks) and (b) to contain packetization information for streaming
 *    protocols (hint tracks).
 *
 *    There shall be at least one media track within an ISO file, and all
 *    the media tracks that contributed to the hint tracks shall remain in
 *    the file, even if the media data within them is not referenced by
 *    the hint tracks; after deleting all hint tracks, the entire un-hinted
 *    presentation shall remain.
 * */
#ifndef TRAK_BOX_H_
#define TRAK_BOX_H_

#include "Tkhd_Box.h"
#include "Mdia_Box.h"

typedef struct Tag_Trak_Box {
    Box_Header * boxheader;
} Trak_Box;

int AllocStruct_Trak_Box(Trak_Box ** trakbox);
int FreeStruct_Trak_Box(Trak_Box * trakbox);

int Write_Trak_Trak_Box_Video(unsigned char * Buf,
                              unsigned int duration_tkhd_video,
                              unsigned int width_tkhd,
                              unsigned int height_tkhd,
                              unsigned int timescale_mdhd,
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
                              unsigned char * spsbuf,
                              unsigned int spslength,
                              unsigned char * ppsbuf,
                              unsigned int ppslength);

int Write_Trak_Trak_Box_Audio(unsigned char * Buf,
                              unsigned int duration_tkhd_audio,
                              unsigned int width_tkhd,
                              unsigned int height_tkhd,
                              unsigned int timescale_mdhd,
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
