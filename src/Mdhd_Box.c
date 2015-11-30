#include "Mdhd_Box.h"

int AllocStruct_Mdhd_Box(Mdhd_Box ** mdhdbox)
{
    Mdhd_Box * mdhdbox_t = *mdhdbox;
    
    if ((mdhdbox_t = (Mdhd_Box *) calloc(1, sizeof(Mdhd_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Mdhd_Box Buffer Failed\n");
        return -1;
    }
    AllocStruct_FullBox_Box_Header(&(mdhdbox_t->boxheader));
    *mdhdbox = mdhdbox_t;
    
    return 1;
}

int FreeStruct_Mdhd_Box(Mdhd_Box * mdhdbox)
{
    if (mdhdbox) {
        FreeStruct_FullBox_Box_Header(mdhdbox->boxheader);
        free(mdhdbox);
        mdhdbox = NULL;
    }
    
    return 1;
}

int Write_Mdhd_Mdhd_Box_Video(unsigned char * Buf,
                              unsigned int timescale_mdhd_video,
                              unsigned int duration_mdhd_video)
{
    Mdhd_Box * mdhdbox = NULL;
    unsigned int mdhdbox_pos = 0;

    AllocStruct_Mdhd_Box(&mdhdbox);
    //////////////////////////////////////////////////////////////////////////

    mdhdbox->timescale = timescale_mdhd_video;
    mdhdbox->duration = duration_mdhd_video;

    mdhdbox_pos += 4;
    // boxtype 0x6D 64 68 64 Mdhd
    Buf[mdhdbox_pos] = 0x6D;
    Buf[mdhdbox_pos + 1] = 0x64;
    Buf[mdhdbox_pos + 2] = 0x68;
    Buf[mdhdbox_pos + 3] = 0x64;
    mdhdbox_pos += 4;
    // version,flag
    mdhdbox_pos += 4;
    // creation_time
    unsigned int a = 0;
    a = GetDateSeconds();
    memcpy(Buf + mdhdbox_pos, &a, 4);
    mdhdbox_pos += 4;
    // modification_time
    memcpy(Buf + mdhdbox_pos, &a, 4);
    mdhdbox_pos += 4;
    // timescale
    Buf[mdhdbox_pos] = mdhdbox->timescale >> 24;
    Buf[mdhdbox_pos + 1] = (mdhdbox->timescale >> 16) & 0xFF;
    Buf[mdhdbox_pos + 2] = (mdhdbox->timescale >> 8) & 0xFF;
    Buf[mdhdbox_pos + 3] = mdhdbox->timescale & 0xFF;
    mdhdbox_pos += 4;
    // duration
    Buf[mdhdbox_pos] = mdhdbox->duration >> 24;
    Buf[mdhdbox_pos + 1] = (mdhdbox->duration >> 16) & 0xFF;
    Buf[mdhdbox_pos + 2] = (mdhdbox->duration >> 8) & 0xFF;
    Buf[mdhdbox_pos + 3] = mdhdbox->duration & 0xFF;
    mdhdbox_pos += 4;
    // pad  //language[3] //0x15 0xc7
    Buf[mdhdbox_pos] = 0x15;
    Buf[mdhdbox_pos + 1] = 0xC7;
    mdhdbox_pos += 2;
    // pre_defined
    Buf[mdhdbox_pos] = 0x00;
    Buf[mdhdbox_pos + 1] = 0x00;
    mdhdbox_pos += 2;

    Write_FullBox_Header(Buf, mdhdbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Mdhd_Box(mdhdbox);
    
    return mdhdbox_pos;
}

int Write_Mdhd_Mdhd_Box_Audio(unsigned char * Buf,
                              unsigned int timescale_mdhd_audio,
                              unsigned int duration_mdhd_audio)
{
    Mdhd_Box * mdhdbox = NULL;
    unsigned int mdhdbox_pos = 0;

    AllocStruct_Mdhd_Box(&mdhdbox);
    //////////////////////////////////////////////////////////////////////////

    mdhdbox->timescale = timescale_mdhd_audio;
    mdhdbox->duration = duration_mdhd_audio;

    mdhdbox_pos += 4;
    // boxtype 0x6D 64 68 64 Mdhd
    Buf[mdhdbox_pos] = 0x6D;
    Buf[mdhdbox_pos + 1] = 0x64;
    Buf[mdhdbox_pos + 2] = 0x68;
    Buf[mdhdbox_pos + 3] = 0x64;
    mdhdbox_pos += 4;
    // version,flag
    mdhdbox_pos += 4;
    // creation_time
    unsigned int a = 0;
    a = GetDateSeconds();
    memcpy(Buf + mdhdbox_pos, &a, 4);
    mdhdbox_pos += 4;
    // modification_time
    memcpy(Buf + mdhdbox_pos, &a, 4);
    mdhdbox_pos += 4;
    // timescale
    Buf[mdhdbox_pos] = mdhdbox->timescale >> 24;
    Buf[mdhdbox_pos + 1] = (mdhdbox->timescale >> 16) & 0xFF;
    Buf[mdhdbox_pos + 2] = (mdhdbox->timescale >> 8) & 0xFF;
    Buf[mdhdbox_pos + 3] = mdhdbox->timescale & 0xFF;
    mdhdbox_pos += 4;
    // duration
    Buf[mdhdbox_pos] = mdhdbox->duration >> 24;
    Buf[mdhdbox_pos + 1] = (mdhdbox->duration >> 16) & 0xFF;
    Buf[mdhdbox_pos + 2] = (mdhdbox->duration >> 8) & 0xFF;
    Buf[mdhdbox_pos + 3] = mdhdbox->duration & 0xFF;
    mdhdbox_pos += 4;
    // pad  //language[3] //0x15 0xc7
#define LSMASH_PACK_ISO_LANGUAGE( a, b, c ) ((((a-0x60)&0x1f)<<10) | (((b-0x60)&0x1f)<<5) | ((c-0x60)&0x1f))
    unsigned short b = LSMASH_PACK_ISO_LANGUAGE('u','n','d');
    Buf[mdhdbox_pos] = (b >> 8) & 0xFF;
    Buf[mdhdbox_pos + 1] = b & 0xFF;
    mdhdbox_pos += 2;
    // pre_defined
    Buf[mdhdbox_pos] = 0x00;
    Buf[mdhdbox_pos + 1] = 0x00;
    mdhdbox_pos += 2;

    Write_FullBox_Header(Buf, mdhdbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Mdhd_Box(mdhdbox);
    
    return mdhdbox_pos;
}
