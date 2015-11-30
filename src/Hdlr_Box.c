#include "Hdlr_Box.h"

int AllocStruct_Hdlr_Box(Hdlr_Box ** hdlrbox)
{
    Hdlr_Box * hdlrbox_t = *hdlrbox;
    
    if ((hdlrbox_t = (Hdlr_Box *) calloc(1, sizeof(Hdlr_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Hdlr_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_FullBox_Box_Header(&(hdlrbox_t->boxheader))) {
        if ((hdlrbox_t->name = (unsigned char *) calloc(1, sizeof(char))) == NULL) {
            printf("Error: Allocate Meory To hdlrbox_t->name Buffer Failed\n");
            return -1;
        }
    }
    *hdlrbox = hdlrbox_t;
    
    return 1;
}

int FreeStruct_Hdlr_Box(Hdlr_Box * hdlrbox)
{
    if (hdlrbox) {
        if (FreeStruct_FullBox_Box_Header(hdlrbox->boxheader)) {
            if (hdlrbox->name) {
                free(hdlrbox->name);
                hdlrbox->name = NULL;
            }
        }
        free(hdlrbox);
        hdlrbox = NULL;
    }
    
    return 1;
}

int Write_Hdlr_Hdlr_Box_Video(unsigned char * Buf)
{
    Hdlr_Box * hdlrbox = NULL;
    unsigned int hdlrbox_pos = 0;

    AllocStruct_Hdlr_Box(&hdlrbox);
    //////////////////////////////////////////////////////////////////////////

    hdlrbox_pos += 4;
    // boxtype 0x68 64 6C 72 Hdlr
    Buf[hdlrbox_pos] = 0x68;
    Buf[hdlrbox_pos + 1] = 0x64;
    Buf[hdlrbox_pos + 2] = 0x6C;
    Buf[hdlrbox_pos + 3] = 0x72;
    hdlrbox_pos += 4;
    // version,flag
    hdlrbox_pos += 4;
    // pre_defined
    Buf[hdlrbox_pos] = 0x00;
    Buf[hdlrbox_pos + 1] = 0x00;
    Buf[hdlrbox_pos + 2] = 0x00;
    Buf[hdlrbox_pos + 3] = 0x00;
    hdlrbox_pos += 4;
    // handler_type //“vide”— video track 0x76 0x69 0x64 0x65
    Buf[hdlrbox_pos] = 0x76;
    Buf[hdlrbox_pos + 1] = 0x69;
    Buf[hdlrbox_pos + 2] = 0x64;
    Buf[hdlrbox_pos + 3] = 0x65;
    hdlrbox_pos += 4;
    // reserved[3]
    Buf[hdlrbox_pos] = 0x00;
    Buf[hdlrbox_pos + 1] = 0x00;
    Buf[hdlrbox_pos + 2] = 0x00;
    Buf[hdlrbox_pos + 3] = 0x00;
    hdlrbox_pos += 4;
    Buf[hdlrbox_pos] = 0x00;
    Buf[hdlrbox_pos + 1] = 0x00;
    Buf[hdlrbox_pos + 2] = 0x00;
    Buf[hdlrbox_pos + 3] = 0x00;
    hdlrbox_pos += 4;
    Buf[hdlrbox_pos] = 0x00;
    Buf[hdlrbox_pos + 1] = 0x00;
    Buf[hdlrbox_pos + 2] = 0x00;
    Buf[hdlrbox_pos + 3] = 0x00;
    hdlrbox_pos += 4;
    // name
    Buf[hdlrbox_pos] = 'v';
    Buf[hdlrbox_pos + 1] = 'i';
    Buf[hdlrbox_pos + 2] = 'd';
    Buf[hdlrbox_pos + 3] = 'e';
    Buf[hdlrbox_pos + 4] = 'o';
    hdlrbox_pos += 5;
    Buf[hdlrbox_pos] = 0x00;
    hdlrbox_pos++;

    Write_FullBox_Header(Buf, hdlrbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Hdlr_Box(hdlrbox);
    
    return hdlrbox_pos;
}

int Write_Hdlr_Hdlr_Box_Audio(unsigned char * Buf)
{
    Hdlr_Box * hdlrbox = NULL;
    unsigned int hdlrbox_pos = 0;

    AllocStruct_Hdlr_Box(&hdlrbox);
    //////////////////////////////////////////////////////////////////////////

    hdlrbox_pos += 4;
    // boxtype 0x68 64 6C 72 Hdlr
    Buf[hdlrbox_pos] = 0x68;
    Buf[hdlrbox_pos + 1] = 0x64;
    Buf[hdlrbox_pos + 2] = 0x6C;
    Buf[hdlrbox_pos + 3] = 0x72;
    hdlrbox_pos += 4;
    // version,flag
    hdlrbox_pos += 4;
    // pre_defined
    Buf[hdlrbox_pos] = 0x00;
    Buf[hdlrbox_pos + 1] = 0x00;
    Buf[hdlrbox_pos + 2] = 0x00;
    Buf[hdlrbox_pos + 3] = 0x00;
    hdlrbox_pos += 4;
    // handler_type //“soun”— audio track  0x73 0x6F 0x75 0x6E
    Buf[hdlrbox_pos] = 0x73;
    Buf[hdlrbox_pos + 1] = 0x6F;
    Buf[hdlrbox_pos + 2] = 0x75;
    Buf[hdlrbox_pos + 3] = 0x6E;
    hdlrbox_pos += 4;
    // reserved[3]
    Buf[hdlrbox_pos] = 0x00;
    Buf[hdlrbox_pos + 1] = 0x00;
    Buf[hdlrbox_pos + 2] = 0x00;
    Buf[hdlrbox_pos + 3] = 0x00;
    hdlrbox_pos += 4;
    Buf[hdlrbox_pos] = 0x00;
    Buf[hdlrbox_pos + 1] = 0x00;
    Buf[hdlrbox_pos + 2] = 0x00;
    Buf[hdlrbox_pos + 3] = 0x00;
    hdlrbox_pos += 4;
    Buf[hdlrbox_pos] = 0x00;
    Buf[hdlrbox_pos + 1] = 0x00;
    Buf[hdlrbox_pos + 2] = 0x00;
    Buf[hdlrbox_pos + 3] = 0x00;
    hdlrbox_pos += 4;
    // name
    Buf[hdlrbox_pos] = 'a';
    Buf[hdlrbox_pos + 1] = 'u';
    Buf[hdlrbox_pos + 2] = 'd';
    Buf[hdlrbox_pos + 3] = 'i';
    Buf[hdlrbox_pos + 4] = 'o';
    hdlrbox_pos += 5;
    Buf[hdlrbox_pos] = 0x00;
    hdlrbox_pos++;

    Write_FullBox_Header(Buf, hdlrbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Hdlr_Box(hdlrbox);
    
    return hdlrbox_pos;
}
