/**
 * @brief
 *    Box Type: 'hdlr' - Handler Reference Box
 *    Container: Media Box (‘mdia’) or Meta Box (‘meta’)
 *    Mandatory: Yes
 *    Quantity: Exactly one
 *
 *    This box within a Media Box declares the process by which the media-
 *    data in the track is presented, and thus, the nature of the media in
 *    a track. For example, a video track would be handled by a video
 *    handler.
 *
 *    This box when present within a Meta Box, declares the structure or
 *    format of the 'meta' box contents.
 * */
#ifndef HDLR_BOX_H_
#define HDLR_BOX_H_

#include "Mp4.h"

typedef struct Tag_Hdlr_Box {
    FullBox_Header * boxheader;
    unsigned int pre_defined;

    // when present in a media box, is an integer containing one of the
    // following values, or a value from a derived specification:
    // ‘vide’ Video track, ‘soun’ Audio track, ‘hint’ Hint track
    // when present in a meta box, contains an appropriate value to
    // indicate the format of the meta box contents
    unsigned int handler_type;
    unsigned int reserved[3];

    // a null-terminated string in UTF-8 characters which gives a
    // human-readable name for the track type (for debugging and
    // inspection purposes).
    unsigned char * name;
} Hdlr_Box;

int AllocStruct_Hdlr_Box(Hdlr_Box ** hdlrbox);
int FreeStruct_Hdlr_Box(Hdlr_Box * hdlrbox);
int Write_Hdlr_Hdlr_Box_Video(unsigned char * Buf);
int Write_Hdlr_Hdlr_Box_Audio(unsigned char * Buf);

#endif
