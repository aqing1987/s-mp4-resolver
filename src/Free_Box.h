/**
 * @brief
 *    Box Type: 'free'
 *    Container: File or other box
 *    Mandatory: No
 *    Quantity: Any number
 *
 *    The contents of a free-space box are irrelevant and may be ignored,
 *    or the object deleted, without affecting the presentation. (Care
 *    should be exercised when deleting the object, as this may invalidate
 *    the offsets used in the sample table, unless this object is after all
 *    the media data).
 * */
#ifndef FREE_BOX_H_
#define FREE_BOX_H_

#include "Mp4.h"

#define MAX_FREE_BOX_SIZE      100

typedef struct Tag_Free_Box {
    Box_Header * boxheader;
    unsigned char * data;
} Free_Box;

int AllocStruct_Free_Box_MorNum(Free_Box ** freebox,
                                unsigned int free_box_num);
int FreeStruct_Free_Box_MorNum(Free_Box * freebox,
                               unsigned int free_box_num);
int AllocStruct_Free_Box(Free_Box ** freebox);
int FreeStruct_Free_Box(Free_Box * freebox);
int Write_Free_Free_Box(unsigned char * Buf);

#endif
