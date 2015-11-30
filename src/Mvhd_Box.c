#include "Mvhd_Box.h"

int AllocStruct_Mvhd_Box(Mvhd_Box ** mvhdbox)
{
    Mvhd_Box * mvhdbox_t = *mvhdbox;
    
    if ((mvhdbox_t = (Mvhd_Box *) calloc(1, sizeof(Mvhd_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Mvhd_Box Buffer Failed\n");
        return -1;
    }
    AllocStruct_FullBox_Box_Header(&(mvhdbox_t->boxheader));
    *mvhdbox = mvhdbox_t;
    
    return 1;
}

int FreeStruct_Mvhd_Box(Mvhd_Box * mvhdbox)
{
    if (mvhdbox) {
        FreeStruct_FullBox_Box_Header(mvhdbox->boxheader);
        free(mvhdbox);
        mvhdbox = NULL;
    }
    
    return 1;
}

int Write_Mvhd_Mvhd_Box(unsigned char * Buf, unsigned int timescale,
                        unsigned int duration, unsigned int next_track_ID)
{
    Mvhd_Box * mvhdbox = NULL;
    unsigned int mvhdbox_pos = 0;

    AllocStruct_Mvhd_Box(&mvhdbox);
    //////////////////////////////////////////////////////////////////////////

    mvhdbox->timescale = timescale;
    mvhdbox->duration = duration;
    mvhdbox->next_track_ID = next_track_ID;

    mvhdbox_pos += 4;
    // boxtype 0x6D 76 68 64 Mvhd
    Buf[mvhdbox_pos] = 0x6D;
    Buf[mvhdbox_pos + 1] = 0x76;
    Buf[mvhdbox_pos + 2] = 0x68;
    Buf[mvhdbox_pos + 3] = 0x64;
    mvhdbox_pos += 4;
    // version,flag
    mvhdbox_pos += 4;
    // creation_time
    unsigned int a = 0;
    a = GetDateSeconds();
    memcpy(Buf + mvhdbox_pos, &a, 4);
    mvhdbox_pos += 4;
    // modification_time
    memcpy(Buf + mvhdbox_pos, &a, 4);
    mvhdbox_pos += 4;
    // timescale
    Buf[mvhdbox_pos] = mvhdbox->timescale >> 24;
    Buf[mvhdbox_pos + 1] = (mvhdbox->timescale >> 16) & 0xFF;
    Buf[mvhdbox_pos + 2] = (mvhdbox->timescale >> 8) & 0xFF;
    Buf[mvhdbox_pos + 3] = mvhdbox->timescale & 0xFF;
    mvhdbox_pos += 4;
    // duration
    Buf[mvhdbox_pos] = mvhdbox->duration >> 24;
    Buf[mvhdbox_pos + 1] = (mvhdbox->duration >> 16) & 0xFF;
    Buf[mvhdbox_pos + 2] = (mvhdbox->duration >> 8) & 0xFF;
    Buf[mvhdbox_pos + 3] = mvhdbox->duration & 0xFF;
    mvhdbox_pos += 4;
    // rate 0x00 01 00 00
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x01;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    // volume 0x01 00
    Buf[mvhdbox_pos] = 0x01;
    Buf[mvhdbox_pos + 1] = 0x00;
    mvhdbox_pos += 2;
    // reserved 0
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    mvhdbox_pos += 2;
    // reserved_1[0] 0
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    // reserved_1[1] 0
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    // matrix = { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 }
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x01;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x01;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x40;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    // pre_defined
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    Buf[mvhdbox_pos] = 0x00;
    Buf[mvhdbox_pos + 1] = 0x00;
    Buf[mvhdbox_pos + 2] = 0x00;
    Buf[mvhdbox_pos + 3] = 0x00;
    mvhdbox_pos += 4;
    // next_track_ID
    Buf[mvhdbox_pos] = mvhdbox->next_track_ID >> 24;
    Buf[mvhdbox_pos + 1] = (mvhdbox->next_track_ID >> 16) & 0xFF;
    Buf[mvhdbox_pos + 2] = (mvhdbox->next_track_ID >> 8) & 0xFF;
    Buf[mvhdbox_pos + 3] = mvhdbox->next_track_ID & 0xFF;
    mvhdbox_pos += 4;
    Write_FullBox_Header(Buf, mvhdbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Mvhd_Box(mvhdbox);
    
    return mvhdbox_pos;
}
