#include "Stss_Box.h"

int AllocStruct_Stss_Box(Stss_Box ** stssbox)
{
    Stss_Box * stssbox_t = *stssbox;
    
    if ((stssbox_t = (Stss_Box *) calloc(1, sizeof(Stss_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Stts_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_FullBox_Box_Header(&(stssbox_t->boxheader))) {
        if ((stssbox_t->sample_number
             = (unsigned int *) calloc(1, sizeof(unsigned int))) == NULL) {
            printf("Error: Allocate Meory To sttsbox_t->sample_number  Buffer Failed\n");
            return -1;
        }
    }
    *stssbox = stssbox_t;
    
    return 1;
}

int FreeStruct_Stss_Box(Stss_Box * stssbox)
{
    if (stssbox) {
        if (FreeStruct_FullBox_Box_Header(stssbox->boxheader)) {
            if (stssbox->sample_number) {
                free(stssbox->sample_number);
                stssbox->sample_number = NULL;
            }
        }
        free(stssbox);
        stssbox = NULL;
    }
    
    return 1;
}

int Write_Stss_Stss_Box(unsigned char * Buf,
                        unsigned int entry_count_stss,
                        unsigned int * sample_number_stss)
{
    Stss_Box * stssbox = NULL;
    unsigned int stssbox_pos = 0;
    int i = 0;
    
    AllocStruct_Stss_Box(&stssbox);
    //////////////////////////////////////////////////////////////////////////

    stssbox->entry_count = entry_count_stss;

    stssbox_pos += 4;
    // boxtype 0x73 74 73 73 stss
    Buf[stssbox_pos] = 0x73;
    Buf[stssbox_pos + 1] = 0x74;
    Buf[stssbox_pos + 2] = 0x73;
    Buf[stssbox_pos + 3] = 0x73;
    stssbox_pos += 4;
    // version,flag
    stssbox_pos += 4;
    // entry_count
    Buf[stssbox_pos] = stssbox->entry_count >> 24;
    Buf[stssbox_pos + 1] = (stssbox->entry_count >> 16) & 0xFF;
    Buf[stssbox_pos + 2] = (stssbox->entry_count >> 8) & 0xFF;
    Buf[stssbox_pos + 3] = (stssbox->entry_count) & 0xFF;
    stssbox_pos += 4;
    // sample_number
    for (i = 0; i < stssbox->entry_count; i++) {
        Buf[stssbox_pos] = sample_number_stss[i] >> 24;
        Buf[stssbox_pos + 1] = (sample_number_stss[i] >> 16) & 0xFF;
        Buf[stssbox_pos + 2] = (sample_number_stss[i] >> 8) & 0xFF;
        Buf[stssbox_pos + 3] = (sample_number_stss[i]) & 0xFF;
        stssbox_pos += 4;
    }

    Write_FullBox_Header(Buf, stssbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stss_Box(stssbox);
    
    return stssbox_pos;
}
