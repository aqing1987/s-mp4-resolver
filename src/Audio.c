#include "Audio.h"

unsigned int decode_audio_done = 0;

int Detach_Head_Aac(ADTS_HEADER * adtsheader)
{
    unsigned char Adts_Headr_Buf[ADTS_HEADER_LENGTH];
    unsigned int readsize = 0;
    
    readsize = ReadFile(pAudio_Aac_File, Adts_Headr_Buf, ADTS_HEADER_LENGTH);
    if (readsize != ADTS_HEADER_LENGTH) {
        printf("ReadFile : pAudio_Aac_File ERROR\n");
        return -1;
    }

    // syncword twelve 1
    if ((Adts_Headr_Buf[0] == 0xFF) && ((Adts_Headr_Buf[1] & 0xF0) == 0xF0)) {
        adtsheader->syncword = (Adts_Headr_Buf[0] << 4) | (Adts_Headr_Buf[1] >> 4);
        adtsheader->id = ((unsigned int) Adts_Headr_Buf[1] & 0x08) >> 3;
        adtsheader->layer = ((unsigned int) Adts_Headr_Buf[1] & 0x06) >> 1;
        adtsheader->protection_absent = (unsigned int) Adts_Headr_Buf[1] & 0x01;
        adtsheader->profile = ((unsigned int) Adts_Headr_Buf[2] & 0xc0) >> 6;
        adtsheader->sf_index = ((unsigned int) Adts_Headr_Buf[2] & 0x3c) >> 2;
        adtsheader->private_bit = ((unsigned int) Adts_Headr_Buf[2] & 0x02) >> 1;
        adtsheader->channel_configuration = ((((unsigned int) Adts_Headr_Buf[2] & 0x01) << 2)
                                             | (((unsigned int) Adts_Headr_Buf[3] & 0xc0) >> 6));
        adtsheader->original = ((unsigned int) Adts_Headr_Buf[3] & 0x20) >> 5;
        adtsheader->home = ((unsigned int) Adts_Headr_Buf[3] & 0x10) >> 4;
        adtsheader->copyright_identification_bit = ((unsigned int) Adts_Headr_Buf[3] & 0x08) >> 3;
        adtsheader->copyright_identification_start = (unsigned int) Adts_Headr_Buf[3] & 0x04 >> 2;
        adtsheader->aac_frame_length = (((((unsigned int) Adts_Headr_Buf[3]) & 0x03) << 11)
                                        | (((unsigned int) Adts_Headr_Buf[4] & 0xFF) << 3)
                                        | ((unsigned int) Adts_Headr_Buf[5] & 0xE0) >> 5);
        
        adtsheader->adts_buffer_fullness = (((unsigned int) Adts_Headr_Buf[5] & 0x1f) << 6
                                            | ((unsigned int) Adts_Headr_Buf[6] & 0xfc) >> 2);
        adtsheader->no_raw_data_blocks_in_frame = ((unsigned int) Adts_Headr_Buf[6] & 0x03);
    }
    else {
        printf("ADTS_HEADER : BUF ERROR\n");
        return -1;
    }
    return readsize;
}

int Read_One_Aac_Frame(unsigned char * buf)
{
    ADTS_HEADER adts_header;
    unsigned int readsize = 0;

    if (!Detach_Head_Aac(&adts_header)) {
        decode_audio_done = 1;
        return 0;
    }
    readsize = ReadFile(pAudio_Aac_File, buf, adts_header.aac_frame_length - ADTS_HEADER_LENGTH);
    if (readsize != adts_header.aac_frame_length - ADTS_HEADER_LENGTH) {
        printf("READ ADTS_DATA : BUF LENGTH ERROR\n");
        return -1;
    }
    
    return adts_header.aac_frame_length - ADTS_HEADER_LENGTH;
}
