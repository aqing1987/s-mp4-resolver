#include "Stbl_Box.h"

int AllocStruct_Stbl_Box(Stbl_Box ** stblbox)
{
    Stbl_Box * stblbox_t = *stblbox;
    
    if ((stblbox_t = (Stbl_Box *) calloc(1, sizeof(Stbl_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Stbl_Box Buffer Failed\n");
        return -1;
    }
    
    if (AllocStruct_Box_Header(&(stblbox_t->boxheader))) {
    }
    *stblbox = stblbox_t;

    return 1;
}

int FreeStruct_Stbl_Box(Stbl_Box * stblbox)
{
    if (stblbox) {
        if (FreeStruct_Box_Header(stblbox->boxheader)) {
        }
        free(stblbox);
        stblbox = NULL;
    }
    
    return 1;
}

int Write_Stbl_Stbl_Box_Video(unsigned char * Buf,
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
                              unsigned int ppslength)
{
    Stbl_Box * stblbox = NULL;
    unsigned int stblbox_pos = 0;
    unsigned int stsz_box_size = 0;
    unsigned int stss_box_size = 0;
    unsigned int stsc_box_size = 0;
    unsigned int stco_box_size = 0;
    unsigned int stts_box_size = 0;
    unsigned int ctts_box_size = 0;
    unsigned int stsd_box_size = 0;

    AllocStruct_Stbl_Box(&stblbox);
    //////////////////////////////////////////////////////////////////////////
    
    stblbox_pos += 4;
    // boxtype 0x73 74 62 6C stbl
    Buf[stblbox_pos] = 0x73;
    Buf[stblbox_pos + 1] = 0x74;
    Buf[stblbox_pos + 2] = 0x62;
    Buf[stblbox_pos + 3] = 0x6C;
    stblbox_pos += 4;

    // stsdbox
    stsd_box_size = Write_Stsd_Stsd_Box_Video(Buf + stblbox_pos, width_stsd,
                                              height_stsd, spsbuf,
                                              spslength, ppsbuf, ppslength);
    stblbox_pos += stsd_box_size;
    // sttsbox
    stts_box_size = Write_Stts_Stts_Box_Video(Buf + stblbox_pos, entry_count_stts_video,
                                              sample_count_stts_video, sample_delta_stts_video);
    stblbox_pos += stts_box_size;
    // stssbox
    stss_box_size = Write_Stss_Stss_Box(Buf + stblbox_pos, entry_count_stss, sample_number_stss);
    stblbox_pos += stss_box_size;
    // stscbox
    stsc_box_size = Write_Stsc_Stsc_Box_Video(Buf + stblbox_pos, entry_count_stsc_video,
                                              first_chunk_stsc_video, samples_per_chunk_stsc_video,
                                              sample_description_index_stsc_video);
    stblbox_pos += stsc_box_size;
    // stszbox
    stsz_box_size = Write_Stsz_Stsz_Box_Video(Buf + stblbox_pos, sample_count_stsz_video,
                                              entry_size_stsz_video);
    stblbox_pos += stsz_box_size;
    // stcobox
    stco_box_size = Write_Stco_Stco_Box_Video(Buf + stblbox_pos, entry_count_stco_video,
                                              chunk_offset_stco_video);
    stblbox_pos += stco_box_size;
    // cttsbox
    //ctts_box_size = Write_Ctts_Ctts_Box(Buf + stblbox_pos,entry_count_ctts,
    //                                    sample_count_ctts,sample_offset_ctts);
    //stblbox_pos += ctts_box_size;

    Write_Box_Header(Buf, stblbox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stbl_Box(stblbox);
    
    return stblbox_pos;
}

int Write_Stbl_Stbl_Box_Audio(unsigned char * Buf,
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
    Stbl_Box * stblbox = NULL;
    unsigned int stblbox_pos = 0;
    unsigned int stsz_box_size = 0;
    unsigned int stsc_box_size = 0;
    unsigned int stco_box_size = 0;
    unsigned int stts_box_size = 0;
    unsigned int stsd_box_size = 0;

    AllocStruct_Stbl_Box(&stblbox);
    //////////////////////////////////////////////////////////////////////////
    
    stblbox_pos += 4;
    // boxtype  0x73 74 62 6C stbl
    Buf[stblbox_pos] = 0x73;
    Buf[stblbox_pos + 1] = 0x74;
    Buf[stblbox_pos + 2] = 0x62;
    Buf[stblbox_pos + 3] = 0x6C;
    stblbox_pos += 4;

    // stsdbox
    // stsd_box_size = Write_Stsd_Stsd_Box_Audio_AudioSpecificConfig(Buf + stblbox_pos,channelcount_stsd,
    //                                                               samplerate_stsd,audioObjectType_stsd);
    stsd_box_size = Write_Stsd_Stsd_Box_Audio_Mp4a_Esds(Buf + stblbox_pos, channelcount_stsd,
                                                        samplerate_stsd, audioObjectType_stsd);
    stblbox_pos += stsd_box_size;
    // sttsbox
    stts_box_size = Write_Stts_Stts_Box_Audio(Buf + stblbox_pos, entry_count_stts_audio,
                                              sample_count_stts_audio, sample_delta_stts_audio);
    stblbox_pos += stts_box_size;
    // stscbox
    stsc_box_size = Write_Stsc_Stsc_Box_Audio(Buf + stblbox_pos, entry_count_stsc_audio,
                                              first_chunk_stsc_audio, samples_per_chunk_stsc_audio,
                                              sample_description_index_stsc_audio);
    stblbox_pos += stsc_box_size;
    // stszbox
    stsz_box_size = Write_Stsz_Stsz_Box_Audio(Buf + stblbox_pos, sample_count_stsz_audio,
                                              entry_size_stsz_audio);
    stblbox_pos += stsz_box_size;
    // stcobox
    stco_box_size = Write_Stco_Stco_Box_Audio(Buf + stblbox_pos, entry_count_stco_audio,
                                              chunk_offset_stco_audio);
    stblbox_pos += stco_box_size;

    Write_Box_Header(Buf, stblbox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stbl_Box(stblbox);
    
    return stblbox_pos;
}
