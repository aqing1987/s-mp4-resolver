#include "Free_Box.h"

int AllocStruct_Free_Box_MorNum(Free_Box ** freebox,
                                unsigned int free_box_num)
{
    Free_Box * freebox_t = *freebox;

    if ((freebox_t
         = (Free_Box *) realloc(freebox_t, (free_box_num + 1) * sizeof(Free_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Free_Box_MorNum Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_Box_Header(&(freebox_t[free_box_num].boxheader))) {
        if ((freebox_t[free_box_num].data
             = (unsigned char *) calloc(MAX_FREE_BOX_SIZE, sizeof(char))) == NULL) {
            printf("Error: Allocate Meory To freebox_t->data Buffer Failed\n");
            return -1;
        }
    }
    *freebox = freebox_t;
    
    return 1;
}

int FreeStruct_Free_Box_MorNum(Free_Box * freebox,
                               unsigned int free_box_num)
{
    if (freebox) {
        int i = 0;
        for (; i < free_box_num; i++) {
            if (FreeStruct_Box_Header(freebox[i].boxheader)) {
                if (freebox[i].data) {
                    free(freebox[i].data);
                    freebox[i].data = NULL;
                }
            }
        }
        free(freebox);
        freebox = NULL;
    }
    
    return 1;
}

int AllocStruct_Free_Box(Free_Box ** freebox)
{
    Free_Box * freebox_t = *freebox;

    if ((freebox_t = (Free_Box *) calloc(1, sizeof(Free_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Free_Box Buffer Failed ");
        return -1;
    }
    if (AllocStruct_Box_Header(&(freebox_t->boxheader))) {
        if ((freebox_t->data
             = (unsigned char *) calloc(MAX_FREE_BOX_SIZE, sizeof(char))) == NULL) {
            printf("Error: Allocate Meory To freebox_t->data Buffer Failed ");
            return -1;
        }
    }
    *freebox = freebox_t;
    
    return 1;
}

int FreeStruct_Free_Box(Free_Box * freebox)
{
    if (freebox) {
        if (FreeStruct_Box_Header(freebox->boxheader)) {
            if (freebox->data) {
                free(freebox->data);
                freebox->data = NULL;
            }
        }
        free(freebox);
        freebox = NULL;
    }
    
    return 1;
}

int Write_Free_Free_Box(unsigned char * Buf)
{
    Free_Box * freebox = NULL;
    unsigned int freebox_pos = 0;

    AllocStruct_Free_Box(&freebox);
    //////////////////////////////////////////////////////////////////////////
    
    freebox_pos += 4;
    // boxtype 0x66726565 free
    Buf[freebox_pos] = 0x66;
    Buf[freebox_pos + 1] = 0x72;
    Buf[freebox_pos + 2] = 0x65;
    Buf[freebox_pos + 3] = 0x65;
    freebox_pos += 4;

    Write_Box_Header(Buf, freebox_pos);
    
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Free_Box(freebox);

    return freebox_pos;
}
