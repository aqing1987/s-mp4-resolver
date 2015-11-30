/**
 * @brief
 *    Box Type: 'ctts' - CompositionOffsetBox
 *    Container: Sample Table Box ('stbl')
 *    Mandatory: No
 *    Quantity: Zero or one
 *
 *    This box provides the offset between decoding time and composition ti-
 *    me. Since decoding time must be less than the composition time, the
 *    offsets are expressed as unsigned numbers such that CT(n) = DT(n) +
 *    CTTS(n) where CTTS(n) is the (uncompressed) table entry for sample n.
 *
 *    The composition time to sample table is optional and must only be
 *    present if DT and CT differ for any samples.
 *
 *    Hint tracks do not use this box.
 * */
#ifndef CTTS_BOX_H_
#define CTTS_BOX_H_

#include "Mp4.h"

typedef struct Tag_Ctts_Box {
    FullBox_Header * boxheader;
    unsigned int entry_count;
    unsigned int * sample_count;
    unsigned int * sample_offset;
} Ctts_Box;

int AllocStruct_Ctts_Box(Ctts_Box ** cttsbox);
int FreeStruct_Ctts_Box(Ctts_Box * cttsbox);
int Write_Ctts_Ctts_Box(unsigned char * Buf,
                        unsigned int entry_count_ctts,
                        unsigned int * sample_count_ctts,
                        unsigned int *sample_offset_ctts);

#endif
