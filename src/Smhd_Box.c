#include "Smhd_Box.h"

int AllocStruct_Smhd_Box(Smhd_Box ** smhdbox)
{
    Smhd_Box * smhdbox_t = *smhdbox;
    
    if ((smhdbox_t = (Smhd_Box *) calloc(1, sizeof(Smhd_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Smhd_Box Buffer Failed\n");
        return -1;
    }
    AllocStruct_FullBox_Box_Header(&(smhdbox_t->boxheader));
    *smhdbox = smhdbox_t;
    
    return 1;
}

int FreeStruct_Smhd_Box(Smhd_Box * smhdbox) {
    if (smhdbox) {
        FreeStruct_FullBox_Box_Header(smhdbox->boxheader);
        free(smhdbox);
        smhdbox = NULL;
    }
    
    return 1;
}

int Write_Smhd_Smhd_Box(unsigned char * Buf)
{
    Smhd_Box * smhdbox = NULL;
    unsigned int smhdbox_pos = 0;

    AllocStruct_Smhd_Box(&smhdbox);
    //////////////////////////////////////////////////////////////////////////

    smhdbox_pos += 4;
    // boxtype 0x73 6D 68 64 smhd
    Buf[smhdbox_pos] = 0x73;
    Buf[smhdbox_pos + 1] = 0x6D;
    Buf[smhdbox_pos + 2] = 0x68;
    Buf[smhdbox_pos + 3] = 0x64;
    smhdbox_pos += 4;
    // version,flag
    smhdbox_pos += 4;
    // graphicsmode
    Buf[smhdbox_pos] = 0x00;
    Buf[smhdbox_pos + 1] = 0x00;
    smhdbox_pos += 2;
    // opcolor[3]
    Buf[smhdbox_pos] = 0x00;
    Buf[smhdbox_pos + 1] = 0x00;
    smhdbox_pos += 2;

    Write_FullBox_Header(Buf, smhdbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Smhd_Box(smhdbox);
    
    return smhdbox_pos;
}
