/**
 * @brief
 *     Box Type: 'vmhd' - Video Media Header Box
 *     Container: Media Information Box ('minf')
 *     Mandatory: Yes
 *     Quantity: Exactly one specific media header shall be present
 *
 *     The video media header contains general presentation information,
 *     independent of the coding, for video media. Note that the flags
 *     field has the value 1.
 * */
#ifndef VMHD_BOX_H_
#define VMHD_BOX_H_

#include "Mp4.h"

typedef struct Tag_Vmhd_Box {
    FullBox_Header * boxheader;

    // specifies a composition mode for this video track, from the following
    // enumerated set, which may be extended by derived specifications:
    // copy = 0 copy over the existing image
    unsigned int graphicsmode;
    // is a set of 3 colour values (red, green, blue) available for use
    // by graphics modes
    unsigned int opcolor[3];
} Vmhd_Box;

int AllocStruct_Vmhd_Box(Vmhd_Box ** vmhdbox);
int FreeStruct_Vmhd_Box(Vmhd_Box * vmhdbox);
int Write_Vmhd_Vmhd_Box(unsigned char * Buf);

#endif
