/**
 * @brief
 *    Box Type: 'tkhd' - Track Header Box
 *    Container: Track Box (‘trak’)
 *    Mandatory: yes
 *    Quantity: Exactly one
 *
 *    This box specifies the characteristics of a single track. Exactly one
 *    Track Header Box is contained in a track.
 *
 *    In the absence of an edit list, the presentation of a track starts at
 *    the beginning of the overall presentation. An empty edit is used to
 *    offset the start time of a track.
 *
 *    The default value of the track header flags for media tracks is 7
 *    (track_enabled, track_in_movie, track_in_preview). If in a presentat-
 *    ion all tracks have neither track_in_movie nor track_in_preview set,
 *    then all tracks shall be treated as if both flags were set on all
 *    tracks. Hint tracks should have the track header flags set to 0, so
 *    that they are ignored for local playback and preview.
 * */
#ifndef TKHD_BOX_H_
#define TKHD_BOX_H_

#include "Mp4.h"

#define  VIDEO_TRAK_ID 0x01;
#define  AUDIO_TRAK_ID 0x02;

typedef struct Tag_Tkhd_Box {
    FullBox_Header * boxheader;

    // is an integer that declares the creation time of this track (in
    // seconds since midnight, Jan. 1, 1904, in UTC time)
    unsigned long creation_time;

    // is an integer that declares the most recent time the track was
    // modified (in seconds since midnight, Jan. 1, 1904, in UTC time)
    unsigned long modification_time;

    // is an integer that uniquely identifies this track over the entire
    // life-time of this presentation. Track IDs are never re-used and
    // cannot be zero.
    unsigned int track_ID;
    unsigned int reserved;

    // is an integer that indicates the duration of this track (in the
    // timescale indicated in the Movie Header Box). The value of this
    // field is equal to the sum of the durations of all of the track’s
    // edits. If there is no edit list, then the duration is the sum of
    // the sample durations, converted into the timescale in the Movie
    // Header Box. If the duration of this track cannot be determined then
    // duration is set to all 1s (32-bit maxint).
    unsigned long duration;
    unsigned int reserved_1[2];

    // specifies the front-to-back ordering of video tracks; tracks with
    // lower numbers are closer to the viewer. 0 is the normal value,
    // and -1 would be in front of track 0, and so on.
    int layer;
    int alternate_group;

    // is a fixed 8.8 value specifying the track's relative audio volume.
    // Full volume is 1.0 (0x0100) and is the normal value. Its value is
    // irrelevant for a purely visual track. Tracks may be composed by
    // combining them according to their volume, and then using the overall
    // Movie Header Box volume setting; or more complex audio composition
    // (e.g. MPEG-4 BIFS) may be used.
    int volume;
    unsigned int reserved_2;
    int matrix[9];
    // unity matrix

    // specify the track's visual presentation size as fixed-point 16.16
    // values. These need not be the same as the pixel dimensions of the
    // images, which is documented in the sample description(s); all images
    // in the sequence are scaled to this size, before any overall transf-
    // ormation of the track represented by the matrix. The pixel dimensio-
    // ns of the images are the default values.
    unsigned int width;
    unsigned int height;
} Tkhd_Box;

int AllocStruct_Tkhd_Box(Tkhd_Box ** tkhdbox);
int FreeStruct_Tkhd_Box(Tkhd_Box * tkhdbox);

int Write_Tkhd_Tkhd_Box_Video(unsigned char * Buf,
                              unsigned int duration_tkhd_video,
                              unsigned int width_tkhd,
                              unsigned int height_tkhd);

int Write_Tkhd_Tkhd_Box_Audio(unsigned char * Buf,
                              unsigned int duration_tkhd_audio,
                              unsigned int width_tkhd,
                              unsigned int height_tkhd);

#endif
