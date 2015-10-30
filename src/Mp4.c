#include "Mp4.h"

int AllocStruct_Box_Header(Box_Header **boxheader)
{
    Box_Header *boxheader_t = *boxheader;

    boxheader_t = (Box_Header *)calloc(1, sizeof(Box_Header));
    if (boxheader_t == NULL) {
        printf("Allocate Memory To boxheader Buffer Failed\n");
        return Failure;
    }
    
    *boxheader = boxheader_t;
    return Success;
}

int AllocStruct_FullBox_Box_Header(FullBox_Header **fullboxheader)
{
    FullBox_Header *boxheader_t = *fullboxheader;

    boxheader_t = (FullBox_Header *)calloc(1, sizeof(FullBox_Header));
    if (boxheader_t == NULL) {
        printf("Allocate Memory To fullboxheader Buffer Failed\n");
        return Failure;
    }
    
    *fullboxheader = boxheader_t;
    return Success;
}

int FreeStruct_Box_Header(Box_Header *boxheader)
{
    if (boxheader) {
        free(boxheader);
        boxheader = NULL;
    }
    
    return Success;
}

int FreeStruct_FullBox_Box_Header(FullBox_Header *fullboxheader)
{
    if (fullboxheader) {
        free(fullboxheader);
        fullboxheader = NULL;
    }
    
    return Success;
}

int Write_Box_Header(unsigned char *buf, unsigned int size)
{
    Box_Header *boxheader = NULL;

    AllocStruct_Box_Header(&boxheader);
    boxheader->size = size;

    buf[0] = boxheader->size >> 24;
    buf[1] = (boxheader->size >> 16) & 0xFF;
    buf[2] = (boxheader->size >> 8) & 0xFF;
    buf[3] = boxheader->size & 0xFF;

    FreeStruct_Box_Header(boxheader);
    return Success;
}

/**
 * @brief
 *    keep size, version and flags info to *buf
 *  
 * @param
 *    [out] buf - size, version, flags buffer pointer
 *    [in] size - sample size
 *    [in] version - version info
 *    [in] flags - flags
 * */
int Write_FullBox_Header(unsigned char *buf, unsigned int size,
                         unsigned char version, unsigned int flags)
{
    FullBox_Header * fullboxheader = NULL;
    
    AllocStruct_FullBox_Box_Header(&fullboxheader);
    
    fullboxheader->size = size;
    fullboxheader->version = version;
    fullboxheader->flags = flags;

    buf[0] = fullboxheader->size >> 24;
    buf[1] = (fullboxheader->size >> 16) & 0xFF;
    buf[2] = (fullboxheader->size >> 8) & 0xFF;
    buf[3] = fullboxheader->size & 0xFF;

    buf[8] = fullboxheader->version;
    buf[9] = fullboxheader->flags >> 16;
    buf[10] = (fullboxheader->flags >> 8) & 0xFF;
    buf[11] = fullboxheader->flags & 0xFF;

    FreeStruct_FullBox_Box_Header(fullboxheader);
    
    return Success;
}
