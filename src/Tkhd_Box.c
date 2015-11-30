#include "Tkhd_Box.h"

int AllocStruct_Tkhd_Box(Tkhd_Box ** tkhdbox)
{
    Tkhd_Box * tkhdbox_t = *tkhdbox;
    
    if ((tkhdbox_t = (Tkhd_Box *) calloc(1, sizeof(Tkhd_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Tkhd_Box Buffer Failed\n");
        return -1;
    }
    AllocStruct_FullBox_Box_Header(&(tkhdbox_t->boxheader));
    *tkhdbox = tkhdbox_t;
    
    return 1;
}

int FreeStruct_Tkhd_Box(Tkhd_Box * tkhdbox)
{
    if (tkhdbox) {
        FreeStruct_FullBox_Box_Header(tkhdbox->boxheader);
        free(tkhdbox);
        tkhdbox = NULL;
    }
    
    return 1;
}

// _fixme_ width and height
int Write_Tkhd_Tkhd_Box_Video(unsigned char * Buf,
                              unsigned int duration_tkhd_video,
                              unsigned int width_tkhd,
        unsigned int height_tkhd)
{
    Tkhd_Box * tkhdbox = NULL;
    unsigned int tkhdbox_pos = 0;
    unsigned int trak_id = VIDEO_TRAK_ID;
    unsigned int duration = duration_tkhd_video;

    AllocStruct_Tkhd_Box(&tkhdbox);
    //////////////////////////////////////////////////////////////////////////

    tkhdbox->track_ID = trak_id;
    tkhdbox->duration = duration;
    tkhdbox->width = width_tkhd;
    tkhdbox->height = height_tkhd;

    tkhdbox_pos += 4;
    // boxtype 0x74 6B 68 64 tkhd
    Buf[tkhdbox_pos] = 0x74;
    Buf[tkhdbox_pos + 1] = 0x6B;
    Buf[tkhdbox_pos + 2] = 0x68;
    Buf[tkhdbox_pos + 3] = 0x64;
    tkhdbox_pos += 4;
    // version,flag
    tkhdbox_pos += 4;
    // creation_time
    unsigned int a = 0;
    a = GetDateSeconds();
    memcpy(Buf + tkhdbox_pos, &a, 4);
    tkhdbox_pos += 4;
    // modification_time
    memcpy(Buf + tkhdbox_pos, &a, 4);
    tkhdbox_pos += 4;
    // track_ID
    Buf[tkhdbox_pos] = tkhdbox->track_ID >> 24;
    ;
    Buf[tkhdbox_pos + 1] = (tkhdbox->track_ID >> 16) & 0xFF;
    Buf[tkhdbox_pos + 2] = (tkhdbox->track_ID >> 8) & 0xFF;
    Buf[tkhdbox_pos + 3] = tkhdbox->track_ID & 0xFF;
    tkhdbox_pos += 4;
    // reserved
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    // duration
    Buf[tkhdbox_pos] = tkhdbox->duration >> 24;
    Buf[tkhdbox_pos + 1] = (tkhdbox->duration >> 16) & 0xFF;
    Buf[tkhdbox_pos + 2] = (tkhdbox->duration >> 8) & 0xFF;
    Buf[tkhdbox_pos + 3] = tkhdbox->duration & 0xFF;
    tkhdbox_pos += 4;
    // reserved_1[0]
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    // reserved_1[1]
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    // layer
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    tkhdbox_pos += 2;
    // alternate_group
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    tkhdbox_pos += 2;
    // volume if track_is_audio 0x0100 else 0};
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    tkhdbox_pos += 2;
    // reserved_2
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    tkhdbox_pos += 2;
    // matrix 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x01;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x01;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x40;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    
    // width
    Buf[tkhdbox_pos] = tkhdbox->width >> 24;
    ;
    Buf[tkhdbox_pos + 1] = (tkhdbox->width >> 16) & 0xFF;
    Buf[tkhdbox_pos + 2] = (tkhdbox->width >> 8) & 0xFF;
    Buf[tkhdbox_pos + 3] = tkhdbox->width & 0xFF;
    // DEBUG
    Buf[tkhdbox_pos] = 0x02;
    Buf[tkhdbox_pos + 1] = 0xd0;
    Buf[tkhdbox_pos + 2] = 0x0;
    Buf[tkhdbox_pos + 3] = 0x0;
//	printf(" DEBUG width %x %x %x %x\n",Buf[tkhdbox_pos],Buf[tkhdbox_pos+1],Buf[tkhdbox_pos+2],Buf[tkhdbox_pos+3]);
    tkhdbox_pos += 4;
    //height
//	Buf[tkhdbox_pos] = tkhdbox->height >> 24;;
//	Buf[tkhdbox_pos + 1] = (tkhdbox->height >> 16) & 0xFF;
//	Buf[tkhdbox_pos + 2] = (tkhdbox->height >> 8) & 0xFF;
//	Buf[tkhdbox_pos + 3] = tkhdbox->height & 0xFF;
    Buf[tkhdbox_pos] = 0x01;
    Buf[tkhdbox_pos + 1] = 0xe0;
    Buf[tkhdbox_pos + 2] = 0x0;
    Buf[tkhdbox_pos + 3] = 0x0;
    tkhdbox_pos += 4;

    Write_FullBox_Header(Buf, tkhdbox_pos, 0x00, 0x01);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Tkhd_Box(tkhdbox);
    
    return tkhdbox_pos;
}

int Write_Tkhd_Tkhd_Box_Audio(unsigned char * Buf,
                              unsigned int duration_tkhd_audio,
                              unsigned int width_tkhd,
        unsigned int height_tkhd)
{
    Tkhd_Box * tkhdbox = NULL;
    unsigned int tkhdbox_pos = 0;
    unsigned int trak_id = AUDIO_TRAK_ID;
    unsigned int duration = duration_tkhd_audio;

    AllocStruct_Tkhd_Box(&tkhdbox);
    //////////////////////////////////////////////////////////////////////////

    tkhdbox->track_ID = 2;
    tkhdbox->duration = duration;
    tkhdbox->width = width_tkhd;
    tkhdbox->height = height_tkhd;

    tkhdbox_pos += 4;
    // boxtype 0x74 6B 68 64 tkhd
    Buf[tkhdbox_pos] = 0x74;
    Buf[tkhdbox_pos + 1] = 0x6B;
    Buf[tkhdbox_pos + 2] = 0x68;
    Buf[tkhdbox_pos + 3] = 0x64;
    tkhdbox_pos += 4;
    // version,flag
    tkhdbox_pos += 4;
    // creation_time
    unsigned int a = 0;
    a = GetDateSeconds();
    memcpy(Buf + tkhdbox_pos, &a, 4);
    tkhdbox_pos += 4;
    // modification_time
    memcpy(Buf + tkhdbox_pos, &a, 4);
    tkhdbox_pos += 4;
    // track_ID 
    Buf[tkhdbox_pos] = tkhdbox->track_ID >> 24;
    
    Buf[tkhdbox_pos + 1] = (tkhdbox->track_ID >> 16) & 0xFF;
    Buf[tkhdbox_pos + 2] = (tkhdbox->track_ID >> 8) & 0xFF;
    Buf[tkhdbox_pos + 3] = tkhdbox->track_ID & 0xFF;
    tkhdbox_pos += 4;
    // reserved
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    // duration
    Buf[tkhdbox_pos] = tkhdbox->duration >> 24;
    ;
    Buf[tkhdbox_pos + 1] = (tkhdbox->duration >> 16) & 0xFF;
    Buf[tkhdbox_pos + 2] = (tkhdbox->duration >> 8) & 0xFF;
    Buf[tkhdbox_pos + 3] = tkhdbox->duration & 0xFF;
    tkhdbox_pos += 4;
    // reserved_1[0]
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    // reserved_1[1]
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    // layer
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    tkhdbox_pos += 2;
    // alternate_group
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    tkhdbox_pos += 2;
    // volume if track_is_audio 0x0100 else 0};
    Buf[tkhdbox_pos] = 0x01;
    Buf[tkhdbox_pos + 1] = 0x00;
    tkhdbox_pos += 2;
    // reserved_2
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    tkhdbox_pos += 2;
    // matrix 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x01;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x01;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x00;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    Buf[tkhdbox_pos] = 0x40;
    Buf[tkhdbox_pos + 1] = 0x00;
    Buf[tkhdbox_pos + 2] = 0x00;
    Buf[tkhdbox_pos + 3] = 0x00;
    tkhdbox_pos += 4;
    // width
    //  DEBUG
    tkhdbox->width = 0;
    Buf[tkhdbox_pos] = tkhdbox->width >> 24;
    ;
    Buf[tkhdbox_pos + 1] = (tkhdbox->width >> 16) & 0xFF;
    Buf[tkhdbox_pos + 2] = (tkhdbox->width >> 8) & 0xFF;
    Buf[tkhdbox_pos + 3] = tkhdbox->width & 0xFF;
    tkhdbox_pos += 4;
    // height
    tkhdbox->height = 0;
    Buf[tkhdbox_pos] = tkhdbox->height >> 24;
    ;
    Buf[tkhdbox_pos + 1] = (tkhdbox->height >> 16) & 0xFF;
    Buf[tkhdbox_pos + 2] = (tkhdbox->height >> 8) & 0xFF;
    Buf[tkhdbox_pos + 3] = tkhdbox->height & 0xFF;
    tkhdbox_pos += 4;

    Write_FullBox_Header(Buf, tkhdbox_pos, 0x00, 0x01);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Tkhd_Box(tkhdbox);
    
    return tkhdbox_pos;
}
