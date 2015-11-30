/**
 * @brief
 *    Box Type: 'mdat'
 *    Container: File
 *    Mandatory: No
 *    Quantity: Any number
 *
 *    This box contains the media data. In video tracks, this box would
 *    contain video frames. A presentation may contain zero or more Media
 *    Data Boxes. The actual media data follows the type field; its struc-
 *    ture is described by the metadata (see particularly the sample table,
 *    subclause 8.14, and the item location box, subclause 8.44.3).
 *
 *    In large presentations, it may be desirable to have more data in this
 *    box than a 32-bit size would permit. In this case, the large variant
 *    of the size field, above in subclause 6.2, is used.
 *
 *    There may be any number of these boxes in the file (including zero,
 *    if all the media data is in other files). The metadata refers to
 *    media data by its absolute offset within the file (see subclause 8.19,
 *    the Chunk Offset Box); so Media Data Box headers and free space may
 *    easily be skipped, and files without any box structure may also be
 *    referenced and used.
 * */
#ifndef MDAT_BOX_H_
#define MDAT_BOX_H_

#include "Mp4.h"
#include "Video.h"
#include "Audio.h"

#define MAX_ONE_FRAME_DATA_SIZE  1024*1024

typedef struct Tag_Mdat_Box {
    Box_Header * boxheader;

    // data is the contained media data
    unsigned char * data;
} Mdat_Box;

int AllocStruct_Mdat_Box_MorNum(Mdat_Box ** mdatbox,
                                unsigned int mdat_box_num);
int FreeStruct_Mdat_Box_MorNum(Mdat_Box * mdatbox,
                               unsigned int mdat_box_num);
int AllocStruct_Mdat_Box(Mdat_Box ** mdatbox);
int FreeStruct_Mdat_Box(Mdat_Box * mdatbox);
int Write_Mdat_Mdat_Box(unsigned char * Buf,
                        unsigned int Mdat_Box_Size_But_Header);
int Write_Mdat_Frame_Data(unsigned char * Buf,
                          unsigned int WriteFrameType);

#endif
