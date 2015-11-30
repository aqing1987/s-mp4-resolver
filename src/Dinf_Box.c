#include "Dinf_Box.h"

int AllocStruct_Dinf_Box(Dinf_Box ** dinfbox)
{
    Dinf_Box * dinfbox_t = *dinfbox;
    
    if ((dinfbox_t = (Dinf_Box *) calloc(1, sizeof(Dinf_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Dinf_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_Box_Header(&(dinfbox_t->boxheader))) {
    }
    *dinfbox = dinfbox_t;
    
    return 1;
}

int FreeStruct_Dinf_Box(Dinf_Box * dinfbox)
{
    if (dinfbox) {
        if (FreeStruct_Box_Header(dinfbox->boxheader)) {
        }
        free(dinfbox);
        dinfbox = NULL;
    }
    
    return 1;
}

int Write_Dinf_Dinf_Box_Video(unsigned char * Buf)
{
    Dinf_Box * dinfbox = NULL;
    unsigned int dinfbox_pos = 0;

    AllocStruct_Dinf_Box(&dinfbox);
    //////////////////////////////////////////////////////////////////////////

    dinfbox_pos += 4;
    // boxtype 0x64 69 6E 66 Dinf
    Buf[dinfbox_pos] = 0x64;
    Buf[dinfbox_pos + 1] = 0x69;
    Buf[dinfbox_pos + 2] = 0x6E;
    Buf[dinfbox_pos + 3] = 0x66;
    dinfbox_pos += 4;
    // dref_box
    Buf[dinfbox_pos] = 0x00;
    Buf[dinfbox_pos + 1] = 0x00;
    Buf[dinfbox_pos + 2] = 0x00;
    Buf[dinfbox_pos + 3] = 0x1C;
    dinfbox_pos += 4;
    // boxtype 0x64 72 65 66 dref
    Buf[dinfbox_pos] = 0x64;
    Buf[dinfbox_pos + 1] = 0x72;
    Buf[dinfbox_pos + 2] = 0x65;
    Buf[dinfbox_pos + 3] = 0x66;
    dinfbox_pos += 4;
    Buf[dinfbox_pos] = 0x00;
    Buf[dinfbox_pos + 1] = 0x00;
    Buf[dinfbox_pos + 2] = 0x00;
    Buf[dinfbox_pos + 3] = 0x00;
    dinfbox_pos += 4;
    // unsigned int(32) entry_count;
    Buf[dinfbox_pos] = 0x00;
    Buf[dinfbox_pos + 1] = 0x00;
    Buf[dinfbox_pos + 2] = 0x00;
    Buf[dinfbox_pos + 3] = 0x01;
    dinfbox_pos += 4;
    // urlbox
    Buf[dinfbox_pos] = 0x00;
    Buf[dinfbox_pos + 1] = 0x00;
    Buf[dinfbox_pos + 2] = 0x00;
    Buf[dinfbox_pos + 3] = 0x0C;
    dinfbox_pos += 4;
    // boxtype 0x75 72 6C 20 url
    Buf[dinfbox_pos] = 0x75;
    Buf[dinfbox_pos + 1] = 0x72;
    Buf[dinfbox_pos + 2] = 0x6C;
    Buf[dinfbox_pos + 3] = 0x20;
    dinfbox_pos += 4;
    Buf[dinfbox_pos] = 0x00;
    Buf[dinfbox_pos + 1] = 0x00;
    Buf[dinfbox_pos + 2] = 0x00;
    Buf[dinfbox_pos + 3] = 0x01;
    dinfbox_pos += 4;

    Write_Box_Header(Buf, dinfbox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Dinf_Box(dinfbox);
    
    return dinfbox_pos;
}

int Write_Dinf_Dinf_Box_Audio(unsigned char * Buf)
{
    Dinf_Box * dinfbox = NULL;
    unsigned int dinfbox_pos = 0;

    AllocStruct_Dinf_Box(&dinfbox);
    //////////////////////////////////////////////////////////////////////////
    
    dinfbox_pos += 4;
    // boxtype 0x64 69 6E 66 Dinf
    Buf[dinfbox_pos] = 0x64;
    Buf[dinfbox_pos + 1] = 0x69;
    Buf[dinfbox_pos + 2] = 0x6E;
    Buf[dinfbox_pos + 3] = 0x66;
    dinfbox_pos += 4;

    // dref_box
    Buf[dinfbox_pos] = 0x00;
    Buf[dinfbox_pos + 1] = 0x00;
    Buf[dinfbox_pos + 2] = 0x00;
    Buf[dinfbox_pos + 3] = 0x1C;
    dinfbox_pos += 4;
    // boxtype 0x64 72 65 66 dref
    Buf[dinfbox_pos] = 0x64;
    Buf[dinfbox_pos + 1] = 0x72;
    Buf[dinfbox_pos + 2] = 0x65;
    Buf[dinfbox_pos + 3] = 0x66;
    dinfbox_pos += 4;
    Buf[dinfbox_pos] = 0x00;
    Buf[dinfbox_pos + 1] = 0x00;
    Buf[dinfbox_pos + 2] = 0x00;
    Buf[dinfbox_pos + 3] = 0x00;
    dinfbox_pos += 4;
    // unsigned int(32) entry_count;
    Buf[dinfbox_pos] = 0x00;
    Buf[dinfbox_pos + 1] = 0x00;
    Buf[dinfbox_pos + 2] = 0x00;
    Buf[dinfbox_pos + 3] = 0x01;
    dinfbox_pos += 4;
    // urlbox
    Buf[dinfbox_pos] = 0x00;
    Buf[dinfbox_pos + 1] = 0x00;
    Buf[dinfbox_pos + 2] = 0x00;
    Buf[dinfbox_pos + 3] = 0x0C;
    dinfbox_pos += 4;
    // boxtype 0x75 72 6C 20 url
    Buf[dinfbox_pos] = 0x75;
    Buf[dinfbox_pos + 1] = 0x72;
    Buf[dinfbox_pos + 2] = 0x6C;
    Buf[dinfbox_pos + 3] = 0x20;
    dinfbox_pos += 4;
    Buf[dinfbox_pos] = 0x00;
    Buf[dinfbox_pos + 1] = 0x00;
    Buf[dinfbox_pos + 2] = 0x00;
    Buf[dinfbox_pos + 3] = 0x01;
    dinfbox_pos += 4;

    Write_Box_Header(Buf, dinfbox_pos);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Dinf_Box(dinfbox);
    
    return dinfbox_pos;
}

