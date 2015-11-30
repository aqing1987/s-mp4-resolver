#include "Mdia_Box.h"

int AllocStruct_Mdia_Box(Mdia_Box ** mdiabox)
{
    Mdia_Box * mdiabox_t = *mdiabox;

    if ((mdiabox_t = (Mdia_Box *) calloc(1, sizeof(Mdia_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Mdia_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_Box_Header(&(mdiabox_t->boxheader))) {
    }
    *mdiabox = mdiabox_t;
    
    return 1;
}

int FreeStruct_Mdia_Box(Mdia_Box * mdiabox)
{
    if (mdiabox) {
        if (FreeStruct_Box_Header(mdiabox->boxheader)) {
        }
        free(mdiabox);
        mdiabox = NULL;
    }
    
    return 1;
}

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
                              unsigned int ppslength)
{
    Mdia_Box * mdiabox = NULL;
    unsigned int mdiabox_pos = 0;
    unsigned int mdhd_box_size = 0;
    unsigned int hdlr_box_size = 0;
    unsigned int minf_box_size = 0;

    AllocStruct_Mdia_Box(&mdiabox);
    //////////////////////////////////////////////////////////////////////////
    
    mdiabox_pos += 4;
    // boxtype 0x6D 64 69 61 Mdia
    Buf[mdiabox_pos] = 0x6D;
    Buf[mdiabox_pos + 1] = 0x64;
    Buf[mdiabox_pos + 2] = 0x69;
    Buf[mdiabox_pos + 3] = 0x61;
    mdiabox_pos += 4;

    // mdhdbox
    mdhd_box_size = Write_Mdhd_Mdhd_Box_Video(Buf + mdiabox_pos,
                                              timescale_mdhd_video,
                                              duration_mdhd_video);
    mdiabox_pos += mdhd_box_size;
    // hdlrbox
    hdlr_box_size = Write_Hdlr_Hdlr_Box_Video(Buf + mdiabox_pos);
    mdiabox_pos += hdlr_box_size;
    // minfbox
    minf_box_size = Write_Minf_Minf_Box_Video(Buf + mdiabox_pos,
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
                                              width_stsd,
                                              height_stsd, spsbuf,
                                              spslength,
                                              ppsbuf,
                                              ppslength);
    mdiabox_pos += minf_box_size;

    Write_Box_Header(Buf, mdiabox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Mdia_Box(mdiabox);
    
    return mdiabox_pos;
}

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
                              unsigned char audioObjectType_stsd)
{
    Mdia_Box * mdiabox = NULL;
    unsigned int mdiabox_pos = 0;
    unsigned int mdhd_box_size = 0;
    unsigned int hdlr_box_size = 0;
    unsigned int minf_box_size = 0;

    AllocStruct_Mdia_Box(&mdiabox);
    //////////////////////////////////////////////////////////////////////////
    
    mdiabox_pos += 4;
    // boxtype 0x6D 64 69 61 Mdia
    Buf[mdiabox_pos] = 0x6D;
    Buf[mdiabox_pos + 1] = 0x64;
    Buf[mdiabox_pos + 2] = 0x69;
    Buf[mdiabox_pos + 3] = 0x61;
    mdiabox_pos += 4;

    // mdhdbox
    mdhd_box_size = Write_Mdhd_Mdhd_Box_Audio(Buf + mdiabox_pos,
                                              timescale_mdhd_audio,
                                              duration_mdhd_audio);
    mdiabox_pos += mdhd_box_size;
    // hdlrbox
    hdlr_box_size = Write_Hdlr_Hdlr_Box_Audio(Buf + mdiabox_pos);
    mdiabox_pos += hdlr_box_size;
    // minfbox
    minf_box_size = Write_Minf_Minf_Box_Audio(Buf + mdiabox_pos,
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
    mdiabox_pos += minf_box_size;

    Write_Box_Header(Buf, mdiabox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Mdia_Box(mdiabox);
    
    return mdiabox_pos;
}
