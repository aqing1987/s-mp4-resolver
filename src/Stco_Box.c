#include "Stco_Box.h"

int AllocStruct_Stco_Box(Stco_Box ** stcobox)
{
    Stco_Box * stcobox_t = *stcobox;
    
    if ((stcobox_t = (Stco_Box *) calloc(1, sizeof(Stco_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Stco_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_FullBox_Box_Header(&(stcobox_t->boxheader))) {
        if ((stcobox_t->chunk_offset
             = (unsigned int *) calloc(1, sizeof(unsigned int))) == NULL) {
            printf("Error: Allocate Meory To stcobox_t->chunk_offset  Buffer Failed\n");
            return -1;
        }
    }
    *stcobox = stcobox_t;
    
    return 1;
}

int FreeStruct_Stco_Box(Stco_Box * stcobox)
{
    if (stcobox) {
        if (FreeStruct_FullBox_Box_Header(stcobox->boxheader)) {
            if (stcobox->chunk_offset) {
                free(stcobox->chunk_offset);
                stcobox->chunk_offset = NULL;
            }
        }
        free(stcobox);
        stcobox = NULL;
    }
    
    return 1;
}

int Write_Stco_Stco_Box_Video(unsigned char * Buf,
                              unsigned int entry_count_stco_video,
                              unsigned int * chunk_offset_stco_video)
{
    Stco_Box * stcobox = NULL;
    unsigned int stcobox_pos = 0;
    unsigned int i = 0;
    
    AllocStruct_Stco_Box(&stcobox);
    //////////////////////////////////////////////////////////////////////////

    stcobox->entry_count = entry_count_stco_video;

    stcobox_pos += 4;
    // boxtype 0x73 74 63 6F stco
    Buf[stcobox_pos] = 0x73;
    Buf[stcobox_pos + 1] = 0x74;
    Buf[stcobox_pos + 2] = 0x63;
    Buf[stcobox_pos + 3] = 0x6F;
    stcobox_pos += 4;
    // version,flag
    stcobox_pos += 4;
    // entry_count
    Buf[stcobox_pos] = stcobox->entry_count >> 24;
    Buf[stcobox_pos + 1] = (stcobox->entry_count >> 16) & 0xFF;
    Buf[stcobox_pos + 2] = (stcobox->entry_count >> 8) & 0xFF;
    Buf[stcobox_pos + 3] = stcobox->entry_count & 0xFF;
    stcobox_pos += 4;
    // chunk_offset
    for (;i < stcobox->entry_count; i++) {
        Buf[stcobox_pos] = chunk_offset_stco_video[i] >> 24;
        Buf[stcobox_pos + 1] = (chunk_offset_stco_video[i] >> 16) & 0xFF;
        Buf[stcobox_pos + 2] = (chunk_offset_stco_video[i] >> 8) & 0xFF;
        Buf[stcobox_pos + 3] = chunk_offset_stco_video[i] & 0xFF;
        stcobox_pos += 4;
    }

    Write_FullBox_Header(Buf, stcobox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stco_Box(stcobox);
    
    return stcobox_pos;
}

int Write_Stco_Stco_Box_Audio(unsigned char * Buf,
                              unsigned int entry_count_stco_audio,
                              unsigned int * chunk_offset_stco_audio)
{
    Stco_Box * stcobox = NULL;
    unsigned int stcobox_pos = 0;
    unsigned int i = 0;
    
    AllocStruct_Stco_Box(&stcobox);
    //////////////////////////////////////////////////////////////////////////
    stcobox->entry_count = entry_count_stco_audio;

    stcobox_pos += 4;
    // boxtype 0x73 74 63 6F stco
    Buf[stcobox_pos] = 0x73;
    Buf[stcobox_pos + 1] = 0x74;
    Buf[stcobox_pos + 2] = 0x63;
    Buf[stcobox_pos + 3] = 0x6F;
    stcobox_pos += 4;
    // version,flag
    stcobox_pos += 4;
    // entry_count
    Buf[stcobox_pos] = stcobox->entry_count >> 24;
    Buf[stcobox_pos + 1] = (stcobox->entry_count >> 16) & 0xFF;
    Buf[stcobox_pos + 2] = (stcobox->entry_count >> 8) & 0xFF;
    Buf[stcobox_pos + 3] = stcobox->entry_count & 0xFF;
    stcobox_pos += 4;
    // chunk_offset
    for (; i < stcobox->entry_count; i++) {
        Buf[stcobox_pos] = chunk_offset_stco_audio[i] >> 24;
        Buf[stcobox_pos + 1] = (chunk_offset_stco_audio[i] >> 16) & 0xFF;
        Buf[stcobox_pos + 2] = (chunk_offset_stco_audio[i] >> 8) & 0xFF;
        Buf[stcobox_pos + 3] = chunk_offset_stco_audio[i] & 0xFF;
        stcobox_pos += 4;
    }

    Write_FullBox_Header(Buf, stcobox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stco_Box(stcobox);
    
    return stcobox_pos;
}
