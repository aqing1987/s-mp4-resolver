/**
 * @brief
 *    Box Type: 'mdhd'
 *    Container: Media Box (‘mdia’)
 *    Mandatory: Yes
 *    Quantity: Exactly one
 *
 *    The media header declares overall information that is media-indepen-
 *    dent, and relevant to characteristics of the media in a track.
 * */
#ifndef MDHD_BOX_H_
#define MDHD_BOX_H_

#include "Mp4.h"

typedef struct Tag_Mdhd_Box {
    FullBox_Header * boxheader;

    // is an integer that declares the creation time of the media in this
    // track (in seconds since midnight, Jan. 1, 1904, in UTC time)
    unsigned long creation_time;

    // is an integer that declares the most recent time the media in this
    // track was modified (in seconds since midnight, Jan. 1, 1904, in UTC
    // time)
    unsigned long modification_time;

    // is an integer that specifies the time-scale for this media; this is
    // the number of time units that pass in one second. For example, a
    // time coordinate system that measures time in sixtieths of a second
    // has a time scale of 60.
    unsigned int timescale;

    // is an integer that declares the duration of this media (in the
    // scale of the timescale).
    unsigned long duration;
    unsigned char pad;

    // declares the language code for this media. See ISO 639-2/T for the
    // set of three character codes. Each character is packed as the
    // difference between its ASCII value and 0x60. Since the code is
    // confined to being three lower-case letters, these values are
    // strictly positive.
    unsigned char language[3];
    unsigned short pre_defined;
} Mdhd_Box;

int AllocStruct_Mdhd_Box(Mdhd_Box ** mdhdbox);
int FreeStruct_Mdhd_Box(Mdhd_Box * mdhdbox);
int Write_Mdhd_Mdhd_Box_Video(unsigned char * Buf,
                              unsigned int timescale_mdhd_video,
                              unsigned int duration_mdhd_video);

int Write_Mdhd_Mdhd_Box_Audio(unsigned char * Buf,
                              unsigned int timescale_mdhd_audio,
                              unsigned int duration_mdhd_audio);

#endif // MDHD_BOX_H_
