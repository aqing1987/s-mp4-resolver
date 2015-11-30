#include "Mdat_Box.h"

int AllocStruct_Mdat_Box_MorNum(Mdat_Box ** mdatbox,
                                unsigned int mdat_box_num)
{
    Mdat_Box * mdatbox_t = *mdatbox;

    if ((mdatbox_t
         = (Mdat_Box *) realloc(mdatbox_t, (mdat_box_num + 1) * sizeof(Mdat_Box))) == NULL) {
        printf("Error: Allocate Memory To AllocStruct_Mdat_Box_MorNum Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_Box_Header(&(mdatbox_t[mdat_box_num].boxheader))) {
        if ((mdatbox_t[mdat_box_num].data
             = (unsigned char *) calloc(MAX_ONE_FRAME_DATA_SIZE, sizeof(char))) == NULL) {
            printf("Error: Allocate Meory To mdatbox_t->data_brands Buffer Failed\n");
            return -1;
        }
    }
    *mdatbox = mdatbox_t;
    
    return 1;
}

int FreeStruct_Mdat_Box_MorNum(Mdat_Box * mdatbox,
                               unsigned int mdat_box_num)
{
    if (mdatbox) {
        int i = 0;
        for (; i < mdat_box_num; i++) {
            if (FreeStruct_Box_Header(mdatbox[i].boxheader)) {
                if (mdatbox[i].data) {
                    free(mdatbox[i].data);
                    mdatbox[i].data = NULL;
                }
            }
        }
        free(mdatbox);
        mdatbox = NULL;
    }
    
    return 1;
}

int AllocStruct_Mdat_Box(Mdat_Box ** mdatbox)
{
    Mdat_Box * mdatbox_t = *mdatbox;

    if ((mdatbox_t = (Mdat_Box *) calloc(1, sizeof(Mdat_Box))) == NULL) {
        printf("Error: Allocate Memory To AllocStruct_Mdat_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_Box_Header(&(mdatbox_t->boxheader))) {
        if ((mdatbox_t->data
             = (unsigned char *) calloc(MAX_ONE_FRAME_DATA_SIZE, sizeof(char))) == NULL) {
            printf("Error: Allocate Meory To mdatbox_t->data_brands Buffer Failed\n");
            return -1;
        }
    }
    *mdatbox = mdatbox_t;
    
    return 1;
}

int FreeStruct_Mdat_Box(Mdat_Box * mdatbox)
{
    if (mdatbox) {
        if (FreeStruct_Box_Header(mdatbox->boxheader)) {
            if (mdatbox->data) {
                free(mdatbox->data);
                mdatbox->data = NULL;
            }
        }
        free(mdatbox);
        mdatbox = NULL;
    }
    
    return 1;
}

int Write_Mdat_Mdat_Box(unsigned char * Buf, unsigned int Mdat_Box_Size_But_Header)
{
    Mdat_Box * mdatbox = NULL;
    unsigned int mdatbox_pos = 0;

    AllocStruct_Mdat_Box(&mdatbox);
    //////////////////////////////////////////////////////////////////////////
    
    mdatbox_pos += 4;
    // boxtype 0x6D646174 Mdat
    Buf[mdatbox_pos] = 0x6D;
    Buf[mdatbox_pos + 1] = 0x64;
    Buf[mdatbox_pos + 2] = 0x61;
    Buf[mdatbox_pos + 3] = 0x74;
    mdatbox_pos += 4;
    Write_Box_Header(Buf, Mdat_Box_Size_But_Header);
    
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Mdat_Box(mdatbox);
    
    return mdatbox_pos;
}

int Write_Mdat_Frame_Data(unsigned char * Buf, unsigned int WriteFrameType)
{
    unsigned int readsize;
    unsigned int mdatbox_pos = 0;
    ADTS_HEADER adts_header;
    NALU_t * n = NULL;

    //////////////////////////////////////////////////////////////////////////
    if (WriteFrameType == 0) {
        mdatbox_pos = Read_One_Aac_Frame(Buf);
    }

    else if (WriteFrameType == 1) {
        mdatbox_pos = Read_One_H264_Frame(Buf);
    }
    //////////////////////////////////////////////////////////////////////////
    return mdatbox_pos;
}
