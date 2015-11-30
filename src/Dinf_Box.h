/**
 * @brief
 *    Box Type: 'dinf' - Data Information Box
 *    Container: Media Information Box ('minf') or Meta Box ('meta')
 *    Mandatory: Yes (required within 'minf' box) and No
 *               (optional within 'meta' box)
 *    Quantity: Exactly one
 *
 *    The data information box contains objects that declare the location
 *    of the media information in a track.
 *
 *    --->
 *    Box Types: 'url', 'dref'
 *    Container: Data Information Box('dinf')
 *    mandatory: Yes
 *    Quantity: Exactly one
 *
 *    The data reference object contains a table of data references (norma-
 *    lly URLs) that declare the location(s) of the media data used within
 *    the presentation. The data reference index in the sample description
 *    ties entries in this table to the samples in the track. A track may
 *    be split over several sources in this way.
 * */
#ifndef DINF_BOX_H_
#define DINF_BOX_H_

#include "Mp4.h"

typedef struct Tag_Dinf_Box {
    Box_Header * boxheader;

} Dinf_Box;

int AllocStruct_Dinf_Box(Dinf_Box ** dinfbox);
int FreeStruct_Dinf_Box(Dinf_Box * dinfbox);
int Write_Dinf_Dinf_Box_Video(unsigned char * Buf);
int Write_Dinf_Dinf_Box_Audio(unsigned char * Buf);

#endif
