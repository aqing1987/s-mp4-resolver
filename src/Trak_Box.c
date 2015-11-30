#include "Trak_Box.h"

int AllocStruct_Trak_Box(Trak_Box ** trakbox)
{
    Trak_Box * trakbox_t = *trakbox;

    if ((trakbox_t = (Trak_Box *) calloc(1, sizeof(Trak_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Trak_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_Box_Header(&(trakbox_t->boxheader))) {
    }
    *trakbox = trakbox_t;
    
    return 1;
}

int FreeStruct_Trak_Box(Trak_Box * trakbox)
{
    if (trakbox) {
        if (FreeStruct_Box_Header(trakbox->boxheader)) {
        }
        free(trakbox);
        trakbox = NULL;
    }
    
    return 1;
}

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
                              unsigned int ppslength)
{
    Trak_Box * trakbox = NULL;
    unsigned int trakbox_pos = 0;
    unsigned int tkhd_box_size = 0;
    unsigned int mdia_box_size = 0;

    AllocStruct_Trak_Box(&trakbox);
    //////////////////////////////////////////////////////////////////////////
    
    trakbox_pos += 4;
    // boxtype 0x74 72 61 6B Trak
    Buf[trakbox_pos] = 0x74;
    Buf[trakbox_pos + 1] = 0x72;
    Buf[trakbox_pos + 2] = 0x61;
    Buf[trakbox_pos + 3] = 0x6B;
    trakbox_pos += 4;

    // tkhdbox
    tkhd_box_size = Write_Tkhd_Tkhd_Box_Video(Buf + trakbox_pos,
                                              duration_tkhd_video,
                                              width_tkhd, height_tkhd);
    trakbox_pos += tkhd_box_size;
    // mdiabox
    mdia_box_size = Write_Mdia_Mdia_Box_Video(Buf + trakbox_pos,
                                              timescale_mdhd,
                                              duration_tkhd_video,
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
                                              width_tkhd,
                                              height_tkhd,
                                              spsbuf,
                                              spslength,
                                              ppsbuf,
                                              ppslength);
    trakbox_pos += mdia_box_size;

    Write_Box_Header(Buf, trakbox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Trak_Box(trakbox);
    
    return trakbox_pos;
}

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
                              unsigned char audioObjectType_stsd)
{
    Trak_Box * trakbox = NULL;
    unsigned int trakbox_pos = 0;
    unsigned int tkhd_box_size = 0;
    unsigned int mdia_box_size = 0;

    AllocStruct_Trak_Box(&trakbox);
    //////////////////////////////////////////////////////////////////////////
    
    trakbox_pos += 4;
    // boxtype 0x74 72 61 6B Trak
    Buf[trakbox_pos] = 0x74;
    Buf[trakbox_pos + 1] = 0x72;
    Buf[trakbox_pos + 2] = 0x61;
    Buf[trakbox_pos + 3] = 0x6B;
    trakbox_pos += 4;

    // tkhdbox
    tkhd_box_size = Write_Tkhd_Tkhd_Box_Audio(Buf + trakbox_pos,
                                              duration_tkhd_audio*90,
                                              width_tkhd, height_tkhd);
    trakbox_pos += tkhd_box_size;
    // mdiabox
    mdia_box_size = Write_Mdia_Mdia_Box_Audio(Buf + trakbox_pos,
                                              timescale_mdhd,
                                              duration_tkhd_audio*8,
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
    trakbox_pos += mdia_box_size;

    Write_Box_Header(Buf, trakbox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Trak_Box(trakbox);
    
    return trakbox_pos;
}
