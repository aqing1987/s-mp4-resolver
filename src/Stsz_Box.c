#include "Stsz_Box.h"

int AllocStruct_Stsz_Box(Stsz_Box **stszbox)
{
    int ret = 0;
    Stsz_Box * stszbox_t = *stszbox;

    stszbox_t = (Stsz_Box *)calloc(1, sizeof(Stsz_Box));
    if (stszbox_t == NULL) {
        printf("Allocate Memory for Stsz_Box Buffer Failed\n");
        return Failure;
    }

    ret = AllocStruct_FullBox_Box_Header(&(stszbox_t->boxheader));
    if (ret == Success) {
        stszbox_t->entry_size
            = (unsigned int *)calloc(1, sizeof(unsigned int));
        if (stszbox_t->entry_size == NULL) {
            printf("Allocate Memory for stszbox_t->entry_size buf Failed\n");
            return Failure;
        }
    }
    
    *stszbox = stszbox_t;
    
    return Success;
}

int FreeStruct_Stsz_Box(Stsz_Box *stszbox)
{
    if (stszbox) {
            // free box header
        FreeStruct_FullBox_Box_Header(stszbox->boxheader);

        if (stszbox->entry_size) {
            free(stszbox->entry_size);
            stszbox->entry_size = NULL;
        }
        
        free(stszbox);
        stszbox = NULL;
    }
    
    return Success;
}

/**
 * @return
 *    bytes written in *buf
 * */
int Write_Stsz_Stsz_Box_Video(unsigned char *Buf,
                              unsigned int sample_count_stsz_video,
                              unsigned int *entry_size_stsz_video)
{
    Stsz_Box * stszbox = NULL;
    unsigned int stszbox_pos = 0;
    unsigned int i = 0;
    
    AllocStruct_Stsz_Box(&stszbox);
    ///////////////////////////////////////////////////////////////////////

        // 0 ~ 3, belong to box header size
    stszbox_pos += 4;
    
        // 4 ~ 7, boxtype（4B）：0x73 74 73 7A：ascii for “stsz”, box identifier
    Buf[stszbox_pos] = 0x73;
    Buf[stszbox_pos + 1] = 0x74;
    Buf[stszbox_pos + 2] = 0x73;
    Buf[stszbox_pos + 3] = 0x7A;
    stszbox_pos += 4;
    
        // 8 ~ 11, belong to version, flag
    stszbox_pos += 4;
    
        // sample_size
        // the samples have different sizes, set as 0
    Buf[stszbox_pos] = 0x00;
    Buf[stszbox_pos + 1] = 0x00;
    Buf[stszbox_pos + 2] = 0x00;
    Buf[stszbox_pos + 3] = 0x00;
    stszbox_pos += 4;
    
        // sample_count
        // gives the number of samples in the track
    stszbox->sample_count = sample_count_stsz_video;
    Buf[stszbox_pos] = stszbox->sample_count >> 24;
    Buf[stszbox_pos + 1] = (stszbox->sample_count >> 16) & 0xFF;
    Buf[stszbox_pos + 2] = (stszbox->sample_count >> 8) & 0xFF;
    Buf[stszbox_pos + 3] = (stszbox->sample_count) & 0xFF;
    stszbox_pos += 4;
    
        // entry_size
        // get every sample size from the size table
    for (i = 0; i < stszbox->sample_count; i++) {
        Buf[stszbox_pos] = entry_size_stsz_video[i] >> 24;
        Buf[stszbox_pos + 1] = (entry_size_stsz_video[i] >> 16) & 0xFF;
        Buf[stszbox_pos + 2] = (entry_size_stsz_video[i] >> 8) & 0xFF;
        Buf[stszbox_pos + 3] = entry_size_stsz_video[i] & 0xFF;
        stszbox_pos += 4;
    }

        // fill size, version and flag info to boxheader
    Write_FullBox_Header(Buf, stszbox_pos, 0x00, 0x00);
    
    //////////////////////////////////////////////////////////////////////
    FreeStruct_Stsz_Box(stszbox);
    
    return stszbox_pos;
}

/**
 * @return
 *    bytes written in *buf
 * */
int Write_Stsz_Stsz_Box_Audio(unsigned char *Buf,
                              unsigned int sample_count_stsz_audio,
                              unsigned int *entry_size_stsz_audio)
{
    Stsz_Box * stszbox = NULL;
    unsigned int stszbox_pos = 0;
    unsigned int i = 0;
    
    AllocStruct_Stsz_Box(&stszbox);
        /////////////////////////////////////////////////////////////////////

        // 0 ~ 3, belong to box header size
    stszbox_pos += 4;

        // 4 ~ 7, boxtype（4B）：0x73 74 73 7A：ascii for “stsz”, box identifier
    Buf[stszbox_pos] = 0x73;
    Buf[stszbox_pos + 1] = 0x74;
    Buf[stszbox_pos + 2] = 0x73;
    Buf[stszbox_pos + 3] = 0x7A;
    stszbox_pos += 4;
    
        // 8 ~ 11, belong to version, flag
    stszbox_pos += 4;
    
        // sample_size
        // the samples have different sizes, set as 0
    Buf[stszbox_pos] = 0x00;
    Buf[stszbox_pos + 1] = 0x00;
    Buf[stszbox_pos + 2] = 0x00;
    Buf[stszbox_pos + 3] = 0x00;
    stszbox_pos += 4;
    
        //sample_count
        // gives the number of samples in the track
    stszbox->sample_count = sample_count_stsz_audio;
    Buf[stszbox_pos] = stszbox->sample_count >> 24;
    Buf[stszbox_pos + 1] = (stszbox->sample_count >> 16) & 0xFF;
    Buf[stszbox_pos + 2] = (stszbox->sample_count >> 8) & 0xFF;
    Buf[stszbox_pos + 3] = (stszbox->sample_count) & 0xFF;
    stszbox_pos += 4;
    
        // entry_size
        // get every sample size from the size table
    for (i = 0; i < stszbox->sample_count; i++) {
        Buf[stszbox_pos] = entry_size_stsz_audio[i] >> 24;
        Buf[stszbox_pos + 1] = (entry_size_stsz_audio[i] >> 16) & 0xFF;
        Buf[stszbox_pos + 2] = (entry_size_stsz_audio[i] >> 8) & 0xFF;
        Buf[stszbox_pos + 3] = entry_size_stsz_audio[i] & 0xFF;
        stszbox_pos += 4;
    }
    
        // fill size, version and flag info to boxheader
    Write_FullBox_Header(Buf, stszbox_pos, 0x00, 0x00);

        ///////////////////////////////////////////////////////////////////
    FreeStruct_Stsz_Box(stszbox);
    
    return stszbox_pos;
}
