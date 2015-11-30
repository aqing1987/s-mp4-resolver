#include "Ctts_Box.h"

int AllocStruct_Ctts_Box(Ctts_Box ** cttsbox)
{
    Ctts_Box * cttsbox_t = *cttsbox;
    
    if ((cttsbox_t = (Ctts_Box *) calloc(1, sizeof(Ctts_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Ctts_Box Buffer Failed\n");
        return -1;
    }
    
    if (AllocStruct_FullBox_Box_Header(&(cttsbox_t->boxheader))) {
        if ((cttsbox_t->sample_count = (unsigned int *) calloc(1, sizeof(unsigned int))) == NULL) {
            printf("Error: Allocate Meory To cttsbox_t->sample_count  Buffer Failed\n");
            return -1;
        }
        if ((cttsbox_t->sample_offset = (unsigned int *) calloc(1, sizeof(unsigned int))) == NULL) {
            printf("Error: Allocate Meory To cttsbox_t->sample_offset  Buffer Failed\n");
            return -1;
        }
    }
    *cttsbox = cttsbox_t;
    
    return 1;
}

int FreeStruct_Ctts_Box(Ctts_Box * cttsbox)
{
    if (cttsbox) {
        if (FreeStruct_FullBox_Box_Header(cttsbox->boxheader)) {
            if (cttsbox->sample_count) {
                free(cttsbox->sample_count);
                cttsbox->sample_count = NULL;
            }
            if (cttsbox->sample_offset) {
                free(cttsbox->sample_offset);
                cttsbox->sample_offset = NULL;
            }
        }
        free(cttsbox);
        cttsbox = NULL;
    }
    
    return 1;
}

int Write_Ctts_Ctts_Box(unsigned char * Buf, unsigned int entry_count_ctts,
                        unsigned int * sample_count_ctts, unsigned int *sample_offset_ctts)
{
    Ctts_Box * cttsbox = NULL;
    unsigned int cttsbox_pos = 0;
    int i = 0;
    
    AllocStruct_Ctts_Box(&cttsbox);
    //////////////////////////////////////////////////////////////////////////
    cttsbox->entry_count = entry_count_ctts;

    cttsbox_pos += 4;
    // boxtype:0x63 74 74 73：ctts
    Buf[cttsbox_pos] = 0x63;
    Buf[cttsbox_pos + 1] = 0x74;
    Buf[cttsbox_pos + 2] = 0x74;
    Buf[cttsbox_pos + 3] = 0x73;
    cttsbox_pos += 4;
    // version,flag
    cttsbox_pos += 4;
    // entry_count_ctts
    Buf[cttsbox_pos] = cttsbox->entry_count >> 24;
    Buf[cttsbox_pos + 1] = (cttsbox->entry_count >> 16) & 0xFF;
    Buf[cttsbox_pos + 2] = (cttsbox->entry_count >> 8) & 0xFF;
    Buf[cttsbox_pos + 3] = (cttsbox->entry_count) & 0xFF;
    cttsbox_pos += 4;
    // sample_count_ctts, sample_offset_ctts
    for (;i < entry_count_ctts; i++) {
        Buf[cttsbox_pos] = sample_count_ctts[i] >> 24;
        Buf[cttsbox_pos + 1] = (sample_count_ctts[i] >> 16) & 0xFF;
        Buf[cttsbox_pos + 2] = (sample_count_ctts[i] >> 8) & 0xFF;
        Buf[cttsbox_pos + 3] = (sample_count_ctts[i]) & 0xFF;
        cttsbox_pos += 4;
        Buf[cttsbox_pos] = sample_offset_ctts[i] >> 24;
        Buf[cttsbox_pos + 1] = (sample_offset_ctts[i] >> 16) & 0xFF;
        Buf[cttsbox_pos + 2] = (sample_offset_ctts[i] >> 8) & 0xFF;
        Buf[cttsbox_pos + 3] = (sample_offset_ctts[i]) & 0xFF;
        cttsbox_pos += 4;
    }

    Write_FullBox_Header(Buf, cttsbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Ctts_Box(cttsbox);
    
    return cttsbox_pos;
}
