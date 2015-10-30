/**
 * @brief
 *    Mp4 files are formed as a series of objects, called boxes in this
 *    specification. All data is contained in boxes; there is no other
 *    data within the file.
 *
 *    Boxes start with a header which gives both size and type. The header
 *    permits compact or extended size (32 or 64 bits) and compact or
 *    extended types (32 bits or full UUIDs).
 * */
#ifndef MP4_H_
#define MP4_H_

#include "FileIo.h"

#define Success              0
#define Failure             -1

typedef struct Tag_Box_Header {
        /*
         * specifies the number of bytes in this box, including all its
         * fields and contained boxes;
         * if size is 1, then the actual size is in the field largesize;
         * if size is 0, then this box is the last one in the file, and
         * its contents extend to the end of the file (normally only used
         * for a Media Data Box)
         * */
    unsigned int size;

        /*
         * identifies that box type; standard boxes use a compact type,
         * which is normally four printable characters, to permit ease of
         * identification, and is shown so in the boxes below.
         * User extensions use an extended type; in this case, the type
         * field is set to 'uuid'.
         * */
    unsigned int type;
    
    unsigned long largesize;
    unsigned char usertype[16];
} Box_Header;

typedef struct Tag_FullBox_Header {
    unsigned int size;
    unsigned int type;
    unsigned long largesize;
    unsigned char usertype[16];

        // specifies the version of this format of the box
    unsigned char version;
        // a map of flags
    unsigned int flags : 24;
} FullBox_Header;

int AllocStruct_Box_Header(Box_Header ** boxheader);
int AllocStruct_FullBox_Box_Header(FullBox_Header ** fullboxheader);

int FreeStruct_Box_Header(Box_Header * boxheader);
int FreeStruct_FullBox_Box_Header(FullBox_Header * fullboxheader);

int Write_Box_Header(unsigned char * buf, unsigned int size);
int Write_FullBox_Header(unsigned char * buf, unsigned int size,
                         unsigned char version, unsigned int flags);

#endif // MP4_H_
