#ifndef AUDIO_H_
#define AUDIO_H_

#include "Mp4.h"

#define ADTS_HEADER_LENGTH         7

extern unsigned int decode_audio_done;

typedef struct {
    unsigned int syncword; //
    unsigned int id; //
    unsigned int layer; //
    unsigned int protection_absent; //
    unsigned int profile; //
    unsigned int sf_index; //
    unsigned int private_bit; //
    unsigned int channel_configuration; //
    unsigned int original; //
    unsigned int home; //

    unsigned int copyright_identification_bit; //
    unsigned int copyright_identification_start; //
    unsigned int aac_frame_length; //
    unsigned int adts_buffer_fullness; //
    unsigned int no_raw_data_blocks_in_frame; //
} ADTS_HEADER;

int Detach_Head_Aac(ADTS_HEADER * adtsheader); //
int Read_One_Aac_Frame(unsigned char * buf); //

#endif
