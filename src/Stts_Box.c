#include "Stts_Box.h"

int AllocStruct_Stts_Box(Stts_Box ** sttsbox)
{
    Stts_Box * sttsbox_t = *sttsbox;
    
    if ((sttsbox_t = (Stts_Box *) calloc(1, sizeof(Stts_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Stts_Box Buffer Failed\n");
        return -1;
    }
    
    if (AllocStruct_FullBox_Box_Header(&(sttsbox_t->boxheader))) {
        if ((sttsbox_t->sample_count = (unsigned int *) calloc(1, sizeof(unsigned int))) == NULL) {
            printf("Error: Allocate Meory To sttsbox_t->sample_count  Buffer Failed\n");
            return -1;
        }
        if ((sttsbox_t->sample_delta = (unsigned int *) calloc(1, sizeof(unsigned int))) == NULL) {
            printf("Error: Allocate Meory To sttsbox_t->sample_delta  Buffer Failed\n");
            return -1;
        }
    }
    *sttsbox = sttsbox_t;
    
    return 1;
}

int FreeStruct_Stts_Box(Stts_Box * sttsbox)
{
    if (sttsbox) {
        if (FreeStruct_FullBox_Box_Header(sttsbox->boxheader)) {
            if (sttsbox->sample_count) {
                free(sttsbox->sample_count);
                sttsbox->sample_count = NULL;
            }
            if (sttsbox->sample_delta) {
                free(sttsbox->sample_delta);
                sttsbox->sample_delta = NULL;
            }
        }
        free(sttsbox);
        sttsbox = NULL;
    }
    
    return 1;
}

int Write_Stts_Stts_Box_Video(unsigned char * Buf,
                              unsigned int entry_count_stts_video,
                              unsigned int * sample_count_stts_video,
                              unsigned int * sample_delta_stts_video)
{
    Stts_Box * sttsbox = NULL;
    unsigned int sttsbox_pos = 0;
    unsigned int i = 0;
    
    AllocStruct_Stts_Box(&sttsbox);
    //////////////////////////////////////////////////////////////////////////
    sttsbox->entry_count = entry_count_stts_video;

    sttsbox_pos += 4;
    // boxtype 0x73 74 74 73 stts
    Buf[sttsbox_pos] = 0x73;
    Buf[sttsbox_pos + 1] = 0x74;
    Buf[sttsbox_pos + 2] = 0x74;
    Buf[sttsbox_pos + 3] = 0x73;
    sttsbox_pos += 4;
    // version,flag
    sttsbox_pos += 4;
    // entry_count
    Buf[sttsbox_pos] = sttsbox->entry_count >> 24;
    Buf[sttsbox_pos + 1] = (sttsbox->entry_count >> 16) & 0xFF;
    Buf[sttsbox_pos + 2] = (sttsbox->entry_count >> 8) & 0xFF;
    Buf[sttsbox_pos + 3] = (sttsbox->entry_count) & 0xFF;
    sttsbox_pos += 4;
    // sample_count, sample_delta
    for (i = 0; i < entry_count_stts_video; i++) {
        Buf[sttsbox_pos] = sample_count_stts_video[i] >> 24;
        Buf[sttsbox_pos + 1] = (sample_count_stts_video[i] >> 16) & 0xFF;
        Buf[sttsbox_pos + 2] = (sample_count_stts_video[i] >> 8) & 0xFF;
        Buf[sttsbox_pos + 3] = (sample_count_stts_video[i]) & 0xFF;
        sttsbox_pos += 4;
        Buf[sttsbox_pos] = sample_delta_stts_video[i] >> 24;
        Buf[sttsbox_pos + 1] = (sample_delta_stts_video[i] >> 16) & 0xFF;
        Buf[sttsbox_pos + 2] = (sample_delta_stts_video[i] >> 8) & 0xFF;
        Buf[sttsbox_pos + 3] = (sample_delta_stts_video[i]) & 0xFF;
        sttsbox_pos += 4;
    }

    Write_FullBox_Header(Buf, sttsbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stts_Box(sttsbox);
    return sttsbox_pos;
}

int Write_Stts_Stts_Box_Audio(unsigned char * Buf,
                              unsigned int entry_count_stts_audio,
                              unsigned int *sample_count_stts_audio,
                              unsigned int *sample_delta_stts_audio)
{
    Stts_Box * sttsbox = NULL;
    unsigned int sttsbox_pos = 0;
    unsigned int i = 0;
    
    AllocStruct_Stts_Box(&sttsbox);
    //////////////////////////////////////////////////////////////////////////
    sttsbox->entry_count = entry_count_stts_audio;

    sttsbox_pos += 4;
    // boxtype 0x73 74 74 73 stts
    Buf[sttsbox_pos] = 0x73;
    Buf[sttsbox_pos + 1] = 0x74;
    Buf[sttsbox_pos + 2] = 0x74;
    Buf[sttsbox_pos + 3] = 0x73;
    sttsbox_pos += 4;
    // version,flag
    sttsbox_pos += 4;
    // entry_count
    Buf[sttsbox_pos] = sttsbox->entry_count >> 24;
    Buf[sttsbox_pos + 1] = (sttsbox->entry_count >> 16) & 0xFF;
    Buf[sttsbox_pos + 2] = (sttsbox->entry_count >> 8) & 0xFF;
    Buf[sttsbox_pos + 3] = (sttsbox->entry_count) & 0xFF;
    sttsbox_pos += 4;
    // sample_count, sample_delta
    for (i = 0; i < entry_count_stts_audio; i++) {
        Buf[sttsbox_pos] = sample_count_stts_audio[i] >> 24;
        Buf[sttsbox_pos + 1] = (sample_count_stts_audio[i] >> 16) & 0xFF;
        Buf[sttsbox_pos + 2] = (sample_count_stts_audio[i] >> 8) & 0xFF;
        Buf[sttsbox_pos + 3] = (sample_count_stts_audio[i]) & 0xFF;
        sttsbox_pos += 4;
        Buf[sttsbox_pos] = sample_delta_stts_audio[i] >> 24;
        Buf[sttsbox_pos + 1] = (sample_delta_stts_audio[i] >> 16) & 0xFF;
        Buf[sttsbox_pos + 2] = (sample_delta_stts_audio[i] >> 8) & 0xFF;
        Buf[sttsbox_pos + 3] = (sample_delta_stts_audio[i]) & 0xFF;
        sttsbox_pos += 4;
    }

    Write_FullBox_Header(Buf, sttsbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stts_Box(sttsbox);
    
    return sttsbox_pos;
}
