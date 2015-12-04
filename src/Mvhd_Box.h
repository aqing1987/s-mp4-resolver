/**
 * @brief
 *    Box Type: 'mvhd' - Movie Header Box
 *    Container: Movie Box ('moov')
 *    Mandatory: Yes
 *    Quantity: Exactly one
 *
 *    This box defines overall information which is media-independent,
 *    and relevant to the entire presentation considered as a whole
 * */
#ifndef MVHD_BOX_H_
#define MVHD_BOX_H_

#include "Mp4.h"

typedef struct Tag_Mvhd_Box {
    FullBox_Header * boxheader;

    // is an integer that declares the creation time of the presentation
    // (in seconds since midnight, Jan. 1, 1904, in UTC time)
    unsigned long creation_time;

    // is an integer that declares the most recent time the presentation
    // was modified (in seconds since midnight, Jan. 1, 1904, in UTC time)
    unsigned long modification_time;

    // is an integer that specifies the time-scale for the entire
    // presentation; this is the number of time units that pass in one
    // second. For example, a time coordinate system that measures time
    // in sixtieths of a second has a time scale of 60.
    unsigned int timescale;

    // is an integer that declares length of the presentation (in the
    // indicated timescale). This property is derived from the
    // presentation’s tracks: the value of this field corresponds to the
    // duration of the longest track in the presentation.
    unsigned long duration;

    // is a fixed point 16.16 number that indicates the preferred rate
    // to play the presentation; 1.0 (0x00010000) is normal forward playback
    int rate;

    // is a fixed point 8.8 number that indicates the preferred playback
    // volume. 1.0 (0x0100) is full volume.
    unsigned short volume;
    unsigned short reserved;
    unsigned int reserved_1[2];

    // provides a transformation matrix for the video; (u,v,w) are
    // restricted here to (0,0,1), hex values (0,0,0x40000000).
    int matrix[9]; //= { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
    // Unity matrix
    unsigned int pre_defined[6]; //= 0;

    // is a non-zero integer that indicates a value to use for the track ID
    // of the next track to be added to this presentation. Zero is not a
    // valid track ID value. The value of next_track_ID shall be larger
    // than the largest track-ID in use. If this value is equal to or
    // larger than all 1s (32-bit maxint), and a new media track is to be
    // added, then a search must be made in the file for a unused track
    // identifier.
    unsigned int next_track_ID;
} Mvhd_Box;

int AllocStruct_Mvhd_Box(Mvhd_Box ** mvhdbox);
int FreeStruct_Mvhd_Box(Mvhd_Box * mvhdbox);
int Write_Mvhd_Mvhd_Box(unsigned char * Buf, unsigned int timescale, unsigned int duration, unsigned int next_track_ID);

#endif
