#include "Ftyp_Box.h"

int AllocStruct_Ftype_Box(Ftyp_Box ** ftypbox)
{
    Ftyp_Box * ftypbox_t = *ftypbox;
    
    if ((ftypbox_t = (Ftyp_Box *) calloc(1, sizeof(Ftyp_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Ftype_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_Box_Header(&(ftypbox_t->boxheader))) {
        if ((ftypbox_t->compatible_brands
             = (unsigned int *) calloc(MAX_FTYP_BOXSIZE, sizeof(int))) == NULL) {
            printf("Error: Allocate Meory To ftypbox_t->compatible_brands Buffer Failed\n");
            return -1;
        }
    }
    *ftypbox = ftypbox_t;
    
    return 1;
}

int FreeStruct_Ftype_Box(Ftyp_Box * ftypbox)
{
    if (ftypbox) {
        if (FreeStruct_Box_Header(ftypbox->boxheader)) {
            if (ftypbox->compatible_brands) {
                free(ftypbox->compatible_brands);
                ftypbox->compatible_brands = NULL;
            }
        }
        free(ftypbox);
        ftypbox = NULL;
    }
    
    return 1;
}

int Write_Ftype_Ftyp_Box(unsigned char * Buf)
{
    Ftyp_Box * ftypbox;
    unsigned int ftypbox_pos = 0;
    
    AllocStruct_Ftype_Box(&ftypbox);
    ///////////////////////////////////////////////////////////////////////
    
    ftypbox_pos += 4;
    // boxtype 0x66747970 ftyp
    Buf[ftypbox_pos] = 0x66;
    Buf[ftypbox_pos + 1] = 0x74;
    Buf[ftypbox_pos + 2] = 0x79;
    Buf[ftypbox_pos + 3] = 0x70;
    ftypbox_pos += 4;

    // DEBUG
    Buf[ftypbox_pos] = 'm';
    Buf[ftypbox_pos + 1] = 'p';
    Buf[ftypbox_pos + 2] = '4';
    Buf[ftypbox_pos + 3] = '2';
    ftypbox_pos += 8;

    Buf[ftypbox_pos] = 'm';
    Buf[ftypbox_pos + 1] = 'p';
    Buf[ftypbox_pos + 2] = '4';
    Buf[ftypbox_pos + 3] = '2';
    ftypbox_pos += 4;

    Buf[ftypbox_pos] = 'm';
    Buf[ftypbox_pos + 1] = 'p';
    Buf[ftypbox_pos + 2] = '4';
    Buf[ftypbox_pos + 3] = '1';
    ftypbox_pos += 4;

    Buf[ftypbox_pos] = 'i';
    Buf[ftypbox_pos + 1] = 's';
    Buf[ftypbox_pos + 2] = 'o';
    Buf[ftypbox_pos + 3] = 'm';
    ftypbox_pos += 4;

    Buf[ftypbox_pos] = 'a';
    Buf[ftypbox_pos + 1] = 'v';
    Buf[ftypbox_pos + 2] = 'c';
    Buf[ftypbox_pos + 3] = '1';
    ftypbox_pos += 4;

    Write_Box_Header(Buf, ftypbox_pos);

    ///////////////////////////////////////////////////////////////////////
    FreeStruct_Ftype_Box(ftypbox);
    
    return ftypbox_pos;
}
