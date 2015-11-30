/**
 * @brief
 *    Box Type: ‘stco’, ‘co64’ - Chunk Offset Box
 *    Container: Sample Table Box (‘stbl’)
 *    Mandatory: Yes
 *    Quantity: Exactly one variant must be present
 *
 *    The chunk offset table gives the index of each chunk into the contai-
 *    ning file. There are two variants, permitting the use of 32-bit or
 *    64-bit offsets. The latter is useful when managing very large presen-
 *    tations. At most one of these variants will occur in any single inst-
 *    ance of a sample table.
 *
 *    Offsets are file offsets, not the offset into any box within the file
 *    (e.g. Media Data Box). This permits referring to media data in files
 *    without any box structure. It does also mean that care must be taken
 *    when constructing a self-contained ISO file with its metadata (Movie
 *    Box) at the front, as the size of the Movie Box will affect the chunk
 *    offsets to the media data.
 * */
#ifndef STCO_BOX_H_
#define STCO_BOX_H_

#include "Mp4.h"

typedef struct Tag_Stco_Box {
    FullBox_Header * boxheader;
    unsigned int entry_count;
    // gives the offset of the start of a chunk into its
    // containing media file.
    unsigned int * chunk_offset;
} Stco_Box;

int AllocStruct_Stco_Box(Stco_Box ** stcobox);
int FreeStruct_Stco_Box(Stco_Box * stcobox);

int Write_Stco_Stco_Box_Video(unsigned char * Buf,
                              unsigned int entry_count_stco_video,
                              unsigned int * chunk_offset_stco_video);

int Write_Stco_Stco_Box_Audio(unsigned char * Buf,
                              unsigned int entry_count_stco_audio,
                              unsigned int * chunk_offset_stco_audio);

#endif
