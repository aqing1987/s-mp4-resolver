#include "Vmhd_Box.h"

int AllocStruct_Vmhd_Box(Vmhd_Box ** vmhdbox)
{
    Vmhd_Box * vmhdbox_t = *vmhdbox;
    
    if ((vmhdbox_t = (Vmhd_Box *) calloc(1, sizeof(Vmhd_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Vmhd_Box Buffer Failed\n");
        return -1;
    }
    AllocStruct_FullBox_Box_Header(&(vmhdbox_t->boxheader));
    *vmhdbox = vmhdbox_t;
    
    return 1;
}

int FreeStruct_Vmhd_Box(Vmhd_Box * vmhdbox)
{
    if (vmhdbox) {
        FreeStruct_FullBox_Box_Header(vmhdbox->boxheader);
        free(vmhdbox);
        vmhdbox = NULL;
    }
    
    return 1;
}

int Write_Vmhd_Vmhd_Box(unsigned char * Buf)
{
    Vmhd_Box * vmhdbox = NULL;
    unsigned int vmhdbox_pos = 0;

    AllocStruct_Vmhd_Box(&vmhdbox);
    //////////////////////////////////////////////////////////////////////////

    vmhdbox_pos += 4;
    // boxtype 0x76 6D 68 64 vmhd
    Buf[vmhdbox_pos] = 0x76;
    Buf[vmhdbox_pos + 1] = 0x6D;
    Buf[vmhdbox_pos + 2] = 0x68;
    Buf[vmhdbox_pos + 3] = 0x64;
    vmhdbox_pos += 4;
    // version,flag
    vmhdbox_pos += 4;
    // graphicsmode
    Buf[vmhdbox_pos] = 0x00;
    Buf[vmhdbox_pos + 1] = 0x00;
    vmhdbox_pos += 2;
    // opcolor[3]
    Buf[vmhdbox_pos] = 0x00;
    Buf[vmhdbox_pos + 1] = 0x00;
    vmhdbox_pos += 2;
    Buf[vmhdbox_pos] = 0x00;
    Buf[vmhdbox_pos + 1] = 0x00;
    vmhdbox_pos += 2;
    Buf[vmhdbox_pos] = 0x00;
    Buf[vmhdbox_pos + 1] = 0x00;
    vmhdbox_pos += 2;

    Write_FullBox_Header(Buf, vmhdbox_pos, 0x00, 0x01);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Vmhd_Box(vmhdbox);
    
    return vmhdbox_pos;
}
