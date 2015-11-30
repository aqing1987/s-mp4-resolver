#include "Moov_Box.h"

int AllocStruct_Moov_Box(Moov_Box ** moovbox)
{
    Moov_Box * moovbox_t = *moovbox;
    
    if ((moovbox_t = (Moov_Box *) calloc(1, sizeof(Moov_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Moov_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_Box_Header(&(moovbox_t->boxheader))) {
    }
    *moovbox = moovbox_t;
    
    return 1;
}

int FreeStruct_Moov_Box(Moov_Box * moovbox)
{
    if (moovbox) {
        if (FreeStruct_Box_Header(moovbox->boxheader)) {
        }
        free(moovbox);
        moovbox = NULL;
    }
    
    return 1;
}

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
                        unsigned char audioObjectType_stsd)
{
    Moov_Box * moovbox = NULL;
    unsigned int moovbox_pos = 0;
    unsigned int mvhd_box_size = 0;
    unsigned int trak_box_video_size = 0;
    unsigned int trak_box_audio_size = 0;

    AllocStruct_Moov_Box(&moovbox);
    //////////////////////////////////////////////////////////////////////////
    
    moovbox_pos += 4;
    // boxtype 0x6D6F6F76 Moov
    Buf[moovbox_pos] = 0x6D;
    Buf[moovbox_pos + 1] = 0x6F;
    Buf[moovbox_pos + 2] = 0x6F;
    Buf[moovbox_pos + 3] = 0x76;
    moovbox_pos += 4;

    // mvhdbox
    mvhd_box_size = Write_Mvhd_Mvhd_Box(Buf + moovbox_pos,
                                        90000, duration_Mvhd,
                                        next_track_ID_Mvhd);
    moovbox_pos += mvhd_box_size;
    // trak_videobox
    trak_box_video_size = Write_Trak_Trak_Box_Video(Buf + moovbox_pos,
                                                    duration_tkhd_video,
                                                    width_tkhd, height_tkhd,
                                                    90000,
                                                    sample_count_stsz_video,
                                                    entry_size_stsz_video,
                                                    entry_count_stss,
                                                    sample_number_stss,
                                                    entry_count_stsc_video,
                                                    first_chunk_stsc_video,
                                                    samples_per_chunk_stsc_video,
                                                    sample_description_index_stsc_video,
                                                    entry_count_stco_video,
                                                    chunk_offset_stco_video,
                                                    entry_count_stts_video,
                                                    sample_count_stts_video,
                                                    sample_delta_stts_video,
                                                    entry_count_ctts,
                                                    sample_count_ctts,
                                                    sample_offset_ctts,
                                                    spsbuf,
                                                    spslength,
                                                    ppsbuf,
                                                    ppslength);
    moovbox_pos += trak_box_video_size;
    // trak_audiobox
    trak_box_audio_size = Write_Trak_Trak_Box_Audio(Buf + moovbox_pos,
                                                    duration_tkhd_audio,
                                                    width_tkhd,
                                                    height_tkhd,
                                                    8000,
                                                    sample_count_stsz_audio,
                                                    entry_size_stsz_audio,
                                                    entry_count_stsc_audio,
                                                    first_chunk_stsc_audio,
                                                    samples_per_chunk_stsc_audio,
                                                    sample_description_index_stsc_audio,
                                                    entry_count_stco_audio,
                                                    chunk_offset_stco_audio,
                                                    entry_count_stts_audio,
                                                    sample_count_stts_audio,
                                                    sample_delta_stts_audio,
                                                    channelcount_stsd,
                                                    samplerate_stsd,
                                                    audioObjectType_stsd);
    moovbox_pos += trak_box_audio_size;
    Write_Box_Header(Buf, moovbox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Moov_Box(moovbox);
    
    return moovbox_pos;
}
