/**
 * @brief
 *    Box Type: 'smhd' - Sound Media Header Box
 *    Container: Media Information Box('minf')
 *    Mandatory: Yes
 *    Quantity: Exactly one specific media header shall be present
 *
 *    The sound media header contains general presentation information,
 *    independent of the coding, for audio media. This header is used for
 *    all tracks containing audio.
 * */
#ifndef SMHD_BOX_H_
#define SMHD_BOX_H_

#include "Mp4.h"

// _fixme_ different with spec
typedef struct Tag_Smhd_Box {
    FullBox_Header * boxheader;
    unsigned int graphicsmode;
    unsigned int opcolor[3];
} Smhd_Box;

int AllocStruct_Smhd_Box(Smhd_Box ** smhdbox);
int FreeStruct_Smhd_Box(Smhd_Box * smhdbox);
int Write_Smhd_Smhd_Box(unsigned char * Buf);

#endif
