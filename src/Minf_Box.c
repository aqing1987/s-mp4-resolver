#include "Minf_Box.h"

int AllocStruct_Minf_Box(Minf_Box ** minfbox)
{
    Minf_Box * minfbox_t = *minfbox;
    
    if ((minfbox_t = (Minf_Box *) calloc(1, sizeof(Minf_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Minf_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_Box_Header(&(minfbox_t->boxheader))) {
    }
    *minfbox = minfbox_t;
    
    return 1;
}

int FreeStruct_Minf_Box(Minf_Box * minfbox)
{
    if (minfbox) {
        if (FreeStruct_Box_Header(minfbox->boxheader)) {
        }
        free(minfbox);
        minfbox = NULL;
    }
    
    return 1;
}

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
                              unsigned int ppslength)
{
    Minf_Box * minfbox = NULL;
    unsigned int minfbox_pos = 0;
    unsigned int vmhd_box_size = 0;
    unsigned int dinf_box_size = 0;
    unsigned int stbl_box_size = 0;

    AllocStruct_Minf_Box(&minfbox);
    //////////////////////////////////////////////////////////////////////////
    
    minfbox_pos += 4;
    // boxtype 0x6D 69 6E 66 Minf
    Buf[minfbox_pos] = 0x6D;
    Buf[minfbox_pos + 1] = 0x69;
    Buf[minfbox_pos + 2] = 0x6E;
    Buf[minfbox_pos + 3] = 0x66;
    minfbox_pos += 4;

    // vmhdbox
    vmhd_box_size = Write_Vmhd_Vmhd_Box(Buf + minfbox_pos);
    minfbox_pos += vmhd_box_size;
    // dinfbox
    dinf_box_size = Write_Dinf_Dinf_Box_Video(Buf + minfbox_pos);
    minfbox_pos += dinf_box_size;
    // stblbox
    stbl_box_size = Write_Stbl_Stbl_Box_Video(Buf + minfbox_pos,
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
                                              height_stsd,
                                              spsbuf,
                                              spslength,
                                              ppsbuf,
                                              ppslength);
    minfbox_pos += stbl_box_size;

    Write_Box_Header(Buf, minfbox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Minf_Box(minfbox);
    
    return minfbox_pos;
}

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
                              unsigned char audioObjectType_stsd)
{
    Minf_Box * minfbox = NULL;
    unsigned int minfbox_pos = 0;
    unsigned int smhd_box_size = 0;
    unsigned int dinf_box_size = 0;
    unsigned int stbl_box_size = 0;

    AllocStruct_Minf_Box(&minfbox);
    //////////////////////////////////////////////////////////////////////////
    
    minfbox_pos += 4;
    // boxtype 0x6D 69 6E 66 Minf
    Buf[minfbox_pos] = 0x6D;
    Buf[minfbox_pos + 1] = 0x69;
    Buf[minfbox_pos + 2] = 0x6E;
    Buf[minfbox_pos + 3] = 0x66;
    minfbox_pos += 4;

    // smhdbox
    smhd_box_size = Write_Smhd_Smhd_Box(Buf + minfbox_pos);
    minfbox_pos += smhd_box_size;
    // dinfbox
    dinf_box_size = Write_Dinf_Dinf_Box_Audio(Buf + minfbox_pos);
    minfbox_pos += dinf_box_size;
    // stblbox
    stbl_box_size = Write_Stbl_Stbl_Box_Audio(Buf + minfbox_pos,
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
    minfbox_pos += stbl_box_size;

    Write_Box_Header(Buf, minfbox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Minf_Box(minfbox);
    
    return minfbox_pos;
}
