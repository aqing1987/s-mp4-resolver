#include <pthread.h>
#include "Stsd_Box.h"

int AllocStruct_Stsd_Box(Stsd_Box ** stsdbox)
{
    Stsd_Box * stsdbox_t = *stsdbox;
    
    if ((stsdbox_t = (Stsd_Box *) calloc(1, sizeof(Stsd_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Stsd_Box Buffer Failed\n");
        return -1;
    }
    if (AllocStruct_FullBox_Box_Header(&(stsdbox_t->boxheader))) {
        AllocStruct_Stsd_Vide(&stsdbox_t->std_vide, stsdbox_t->entry_count);
        AllocStruct_Stsd_Soun(&stsdbox_t->stsd_soun, stsdbox_t->entry_count);
    }
    *stsdbox = stsdbox_t;
    
    return 1;
}

int FreeStruct_Stsd_Box(Stsd_Box * stsdbox)
{
    if (stsdbox) {
        if (FreeStruct_FullBox_Box_Header(stsdbox->boxheader)) {
            if (stsdbox->std_vide) {
                FreeStruct_Stsd_Vide(stsdbox->std_vide, stsdbox->entry_count);
            }
            if (stsdbox->stsd_soun) {
                FreeStruct_Stsd_Soun(stsdbox->stsd_soun, stsdbox->entry_count);
            }
        }
        free(stsdbox);
        stsdbox = NULL;
    }
    
    return 1;
}

int AllocStruct_Stsd_Vide(Stsd_Vide ** stsdvide, unsigned int entry_count_num)
{
    Stsd_Vide * stsdvide_t = *stsdvide;
    unsigned int i = 0;
    unsigned int j = 0;
    
    if ((stsdvide_t = (Stsd_Vide *) calloc(entry_count_num, sizeof(Stsd_Vide))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Stsd_Vide Buffer Failed\n");
        return -1;
    }
    
    for (i = 0; i < entry_count_num; i++) {
        for (j = 0; j < 32; j++) {
            if (((stsdvide_t[i]).compressorname[j]
                 = (unsigned char *) calloc(1, sizeof(unsigned char))) == NULL) {
                printf("Error: Allocate Meory To AllocStruct_Stsd_Vide Buffer Failed\n");
                return -1;
            }
        }
        if ((stsdvide_t[i].avccbox = (Avcc_Box *) calloc(1, sizeof(Avcc_Box))) == NULL) {
            printf("Error: Allocate Meory To stsdvide_t->avccbox Buffer Failed\n");
            return -1;
        }
        
        if (AllocStruct_Box_Header(&(stsdvide_t[i].avccbox->boxheader))) {
            if ((stsdvide_t[i].avccbox->sequenceParameterSetNALUnit
                 = (unsigned char *) calloc(1, sizeof(char))) == NULL) {
                printf("Error: Allocate Meory To stsdvide_t[i].avccbox->sequenceParameterSetNALUnit Buffer Failed\n");
                return -1;
            }
            
            if ((stsdvide_t[i].avccbox->pictureParameterSetNALUnit
                 = (unsigned char *) calloc(1, sizeof(char))) == NULL) {
                printf("Error: Allocate Meory To stsdvide_t[i].avccbox->pictureParameterSetNALUnit Buffer Failed\n");
                return -1;
            }
        }
    }
    *stsdvide = stsdvide_t;
    
    return 1;
}
int FreeStruct_Stsd_Vide(Stsd_Vide * stsdvide, unsigned int entry_count_num)
{
    if (stsdvide) {
        unsigned int i = 0;
        unsigned int j = 0;
        
        for (i = 0; i < entry_count_num; i++) {
            for (j = 0; j < 32; j++) {
                if ((stsdvide[i]).compressorname[j]) {
                    free((stsdvide[i]).compressorname[j]);
                    (stsdvide[i]).compressorname[j] = NULL;
                }
            }
            if (stsdvide[i].avccbox) {
                if (stsdvide[i].avccbox->boxheader) {
                    free(stsdvide[i].avccbox->boxheader);
                    stsdvide[i].avccbox->boxheader = NULL;
                }
                if (stsdvide[i].avccbox->sequenceParameterSetNALUnit) {
                    free(stsdvide[i].avccbox->sequenceParameterSetNALUnit);
                    stsdvide[i].avccbox->sequenceParameterSetNALUnit = NULL;
                }
                if (stsdvide[i].avccbox->pictureParameterSetNALUnit) {
                    free(stsdvide[i].avccbox->pictureParameterSetNALUnit);
                    stsdvide[i].avccbox->pictureParameterSetNALUnit = NULL;
                }
                free(stsdvide[i].avccbox);
                stsdvide[i].avccbox = NULL;
            }
        }
        free(stsdvide);
        stsdvide = NULL;
    }
    
    return 1;
}

int AllocStruct_Stsd_Soun(Stsd_Soun ** stsdsoun, unsigned int entry_count_num)
{
    Stsd_Soun * stsdsoun_t = *stsdsoun;
    
    if ((stsdsoun_t = (Stsd_Soun *) calloc(entry_count_num, sizeof(Stsd_Soun))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Stsd_Soun Buffer Failed\n");
        return -1;
    }
    *stsdsoun = stsdsoun_t;
    
    return 1;
}

int FreeStruct_Stsd_Soun(Stsd_Soun * stsdsoun, unsigned int entry_count_num)
{
    if (stsdsoun) {
        free(stsdsoun);
        stsdsoun = NULL;
    }
    
    return 1;
}

int Write_Stsd_Stsd_Box_Video(unsigned char * Buf, unsigned int width_stsd, unsigned int height_stsd,
                              unsigned char * spsbuf, unsigned int spslength, unsigned char * ppsbuf,
                              unsigned int ppslength)
{
    Stsd_Box * stsdbox = NULL;
    unsigned int stsdbox_pos = 0;
    unsigned int avc1box_size = 0;
    unsigned int avc1box_front_size = 0;
    unsigned int avccbox_size = 0;
    unsigned int avccbox_front_size = 0;

    AllocStruct_Stsd_Box(&stsdbox);
    //////////////////////////////////////////////////////////////////////////

    stsdbox_pos += 4;
    //boxtype（4字节）：0x73 74 73 64：“stsd”的ASCII码，box的标识；
    Buf[stsdbox_pos] = 0x73;
    Buf[stsdbox_pos + 1] = 0x74;
    Buf[stsdbox_pos + 2] = 0x73;
    Buf[stsdbox_pos + 3] = 0x64;
    stsdbox_pos += 4;
    //version,flag
    stsdbox_pos += 4;
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x01;
    stsdbox_pos += 4;

    avc1box_front_size = stsdbox_pos;

    //avc1 box  //boxheader:  box : 0x61 76 63 31
    stsdbox_pos += 4; //acc1 box size;
    //boxtype 0x61 76 63 31 avc1
    Buf[stsdbox_pos] = 0x61;
    Buf[stsdbox_pos + 1] = 0x76;
    Buf[stsdbox_pos + 2] = 0x63;
    Buf[stsdbox_pos + 3] = 0x31;
    stsdbox_pos += 4;
    //reserved_format[6]
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    Buf[stsdbox_pos + 4] = 0x00;
    Buf[stsdbox_pos + 5] = 0x00;
    stsdbox_pos += 6;
    //data_reference_index  0x01;
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x01;
    stsdbox_pos += 2;
    //pre_defined
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    stsdbox_pos += 2;
    //reserved
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    stsdbox_pos += 2;
    //pre_defined_1[3]
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    stsdbox_pos += 4;
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    stsdbox_pos += 4;
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    stsdbox_pos += 4;
    //width
    Buf[stsdbox_pos] = width_stsd >> 8;
    Buf[stsdbox_pos + 1] = width_stsd & 0xFF;
    stsdbox_pos += 2;
    //height
    Buf[stsdbox_pos] = height_stsd >> 8;
    Buf[stsdbox_pos + 1] = height_stsd & 0xFF;
    stsdbox_pos += 2;
    //horizresolution 0x00480000
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x48;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    stsdbox_pos += 4;
    //vertresolution 0x00480000
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x48;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    stsdbox_pos += 4;
    //reserved_1
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    stsdbox_pos += 4;
    //frame_count
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x01;
    stsdbox_pos += 2;
    //compressorname
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    Buf[stsdbox_pos + 4] = 0x00;
    Buf[stsdbox_pos + 5] = 0x00;
    Buf[stsdbox_pos + 6] = 0x00;
    Buf[stsdbox_pos + 7] = 0x00;
    Buf[stsdbox_pos + 8] = 0x00;
    Buf[stsdbox_pos + 9] = 0x00;
    Buf[stsdbox_pos + 10] = 0x00;
    Buf[stsdbox_pos + 11] = 0x00;
    Buf[stsdbox_pos + 12] = 0x00;
    Buf[stsdbox_pos + 13] = 0x00;
    Buf[stsdbox_pos + 14] = 0x00;
    Buf[stsdbox_pos + 15] = 0x00;
    Buf[stsdbox_pos + 16] = 0x00;
    Buf[stsdbox_pos + 17] = 0x00;
    Buf[stsdbox_pos + 18] = 0x00;
    Buf[stsdbox_pos + 19] = 0x00;
    Buf[stsdbox_pos + 20] = 0x00;
    Buf[stsdbox_pos + 21] = 0x00;
    Buf[stsdbox_pos + 22] = 0x00;
    Buf[stsdbox_pos + 23] = 0x00;
    Buf[stsdbox_pos + 24] = 0x00;
    Buf[stsdbox_pos + 25] = 0x00;
    Buf[stsdbox_pos + 26] = 0x00;
    Buf[stsdbox_pos + 27] = 0x00;
    Buf[stsdbox_pos + 28] = 0x00;
    Buf[stsdbox_pos + 29] = 0x00;
    Buf[stsdbox_pos + 30] = 0x00;
    Buf[stsdbox_pos + 31] = 0x00;
    stsdbox_pos += 32;
    //depth 0x0018 DEBUG
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    stsdbox_pos += 2;
    //pre_defined_2 -1
    Buf[stsdbox_pos] = 0xFF;
    Buf[stsdbox_pos + 1] = 0xFF;
    stsdbox_pos += 2;

    avccbox_front_size = stsdbox_pos;

    //boxtype 0x61 76 63 43 avcc
    stsdbox_pos += 4;
    Buf[stsdbox_pos] = 0x61;
    Buf[stsdbox_pos + 1] = 0x76;
    Buf[stsdbox_pos + 2] = 0x63;
    Buf[stsdbox_pos + 3] = 0x43;
    stsdbox_pos += 4;
    //configurationVersion
    Buf[stsdbox_pos] = 0x01;
    stsdbox_pos++;
    //AVCProfileIndication
    Buf[stsdbox_pos] = spsbuf[1];
    stsdbox_pos++;
    //profile_compatibility
    Buf[stsdbox_pos] = spsbuf[2];
    stsdbox_pos++;
    //AVCLevelIndication
    Buf[stsdbox_pos] = spsbuf[3];
    stsdbox_pos++;
    //reserved_1 lengthSizeMinusOne
    Buf[stsdbox_pos] = 0xFF;
    stsdbox_pos++;
    //reserved_2 numOfSequenceParameterSets
    Buf[stsdbox_pos] = 0xE1;
    stsdbox_pos++;
    //sequenceParameterSetLength 16
    Buf[stsdbox_pos] = spslength >> 8;
    Buf[stsdbox_pos + 1] = spslength & 0xFF;
    stsdbox_pos += 2;
    //sequenceParameterSetNALUnit
    memcpy(Buf + stsdbox_pos, spsbuf, spslength);
    stsdbox_pos += spslength;
    //numOfPictureParameterSets
    Buf[stsdbox_pos] = 0x01;
    stsdbox_pos++;
    //pictureParameterSetLength
    Buf[stsdbox_pos] = ppslength >> 8;
    Buf[stsdbox_pos + 1] = ppslength & 0xFF;
    stsdbox_pos += 2;
    //pictureParameterSetNALUnit
    memcpy(Buf + stsdbox_pos, ppsbuf, ppslength);
    stsdbox_pos += ppslength;

    //DEBUG
    Buf[stsdbox_pos] = 0xfd;
    Buf[stsdbox_pos + 1] = 0xf8;
    Buf[stsdbox_pos + 2] = 0xf8;
    Buf[stsdbox_pos + 3] = 0x0;
//	Buf[stsdbox_pos+2] = 0x01;
//	Buf[stsdbox_pos+3] = 0x1f;
//	Buf[stsdbox_pos+4] = 0x0f;
//	Buf[stsdbox_pos+5] = 0x1f;
//	Buf[stsdbox_pos+6] = 0x0;
//	Buf[stsdbox_pos+7] = 0x0;
    stsdbox_pos += 4;

    avccbox_size = stsdbox_pos - avccbox_front_size;

    Write_Box_Header(Buf + avccbox_front_size, avccbox_size);

    //  DEBUG
    unsigned int colrbox_size = 0;
    unsigned int colr_front_size = stsdbox_pos;

    stsdbox_pos += 4;
    Buf[stsdbox_pos] = 'c';
    Buf[stsdbox_pos + 1] = 'o';
    Buf[stsdbox_pos + 2] = 'l';
    Buf[stsdbox_pos + 3] = 'r';
    stsdbox_pos += 4;

//	Buf[stsdbox_pos] = 15 >> 24;
//	Buf[stsdbox_pos+1] = (15 >> 16) & 0xFF;
//	Buf[stsdbox_pos+2] = (15 >> 8) & 0xFF;
//	Buf[stsdbox_pos+3] = 15 & 0xFF;
//	stsdbox_pos+=4;

    Buf[stsdbox_pos] = 'n';
    Buf[stsdbox_pos + 1] = 'c';
    Buf[stsdbox_pos + 2] = 'l';
    Buf[stsdbox_pos + 3] = 'x';
    stsdbox_pos += 4;

    Buf[stsdbox_pos] = (2 >> 8) & 0xFF;
    Buf[stsdbox_pos + 1] = 2 & 0xFF;
    stsdbox_pos += 2;

    Buf[stsdbox_pos] = (2 >> 8) & 0xFF;
    Buf[stsdbox_pos + 1] = 2 & 0xFF;
    stsdbox_pos += 2;

    Buf[stsdbox_pos] = (2 >> 8) & 0xFF;
    Buf[stsdbox_pos + 1] = 2 & 0xFF;
    stsdbox_pos += 2;

    Buf[stsdbox_pos] = (1 << 7 | 0);
    stsdbox_pos += 1;

    colrbox_size = stsdbox_pos - colr_front_size;
    Write_Box_Header(Buf + colr_front_size, colrbox_size);

    avc1box_size = stsdbox_pos - avc1box_front_size;
    Write_Box_Header(Buf + avc1box_front_size, avc1box_size);

    Write_FullBox_Header(Buf, stsdbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////

    FreeStruct_Stsd_Box(stsdbox);
    return stsdbox_pos;
}

int Write_Stsd_Stsd_Box_Audio_AudioSpecificConfig(unsigned char * Buf, unsigned int channelcount_stsd,
                                                  unsigned int samplerate_stsd,
                                                  unsigned char audioObjectType_stsd)
{
    Stsd_Box * stsdbox = NULL;
    unsigned int stsdbox_pos = 0;
    unsigned int mp4abox_size = 0;
    unsigned int mp4abox_front_size = 0; //
    unsigned int audiospecificconfig_size = 0;
    unsigned int audiospecificconfig_front_size = 0; //

    AllocStruct_Stsd_Box(&stsdbox);
    //////////////////////////////////////////////////////////////////////////

    stsdbox_pos += 4;
    //boxtype 0x73 74 73 64 stsd
    Buf[stsdbox_pos] = 0x73;
    Buf[stsdbox_pos + 1] = 0x74;
    Buf[stsdbox_pos + 2] = 0x73;
    Buf[stsdbox_pos + 3] = 0x64;
    stsdbox_pos += 4;
    //version,flag
    stsdbox_pos += 4;
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x01;
    stsdbox_pos += 4;

    mp4abox_front_size = stsdbox_pos;

    //mp4a box : 0x6D 70 34 61
    stsdbox_pos += 4; //mp4a box size;
    //boxtype 0x6D 70 34 61 mp4a
    Buf[stsdbox_pos] = 0x6D;
    Buf[stsdbox_pos + 1] = 0x70;
    Buf[stsdbox_pos + 2] = 0x34;
    Buf[stsdbox_pos + 3] = 0x61;
    stsdbox_pos += 4;
    //reserved_format[6]
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    Buf[stsdbox_pos + 4] = 0x00;
    Buf[stsdbox_pos + 5] = 0x00;
    stsdbox_pos += 6;
    //data_reference_index  0x01;
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x01;
    stsdbox_pos += 2;
    //reserved[2]
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    stsdbox_pos += 4;
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    stsdbox_pos += 4;
    //channelcount = 2
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = channelcount_stsd; //  DEBUG

    stsdbox_pos += 2;
    //samplesize = 16 0x10
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x10;
    stsdbox_pos += 2;
    //pre_defined
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    stsdbox_pos += 2;
    //reserved_1
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    stsdbox_pos += 2;
    //samplerate samplerate = {timescale of media}<<16;
    int samplerate_stsd_t = samplerate_stsd << 16;
    Buf[stsdbox_pos] = samplerate_stsd_t >> 24;
    Buf[stsdbox_pos + 1] = (samplerate_stsd_t >> 16) & 0xFF;
    Buf[stsdbox_pos + 2] = (samplerate_stsd_t >> 8) & 0xFF;
    Buf[stsdbox_pos + 3] = samplerate_stsd_t & 0xFF;
    stsdbox_pos += 4;

    audiospecificconfig_front_size = stsdbox_pos;

    //esds box : 0x65 73 64 73
    stsdbox_pos += 4; //esds box size;
    //boxtype 0x65 73 64 73 esds
    Buf[stsdbox_pos] = 0x65;
    Buf[stsdbox_pos + 1] = 0x73;
    Buf[stsdbox_pos + 2] = 0x64;
    Buf[stsdbox_pos + 3] = 0x73;
    stsdbox_pos += 4;
    //version,flag
    stsdbox_pos += 4;
    //audioObjectType //samplingFrequencyIndex
    int samplerate_stsd_t_1 = 0;
    if (samplerate_stsd == 8000) {
        samplerate_stsd_t_1 = 0x0b;
    }
    Buf[stsdbox_pos] = (audioObjectType_stsd << 3) | (samplerate_stsd_t_1 >> 1);
    Buf[stsdbox_pos + 1] = ((samplerate_stsd_t_1 & 0x01) << 7) | (channelcount_stsd << 3) | (0x00) | (0x00) | (0x00);
    stsdbox_pos += 2;

    mp4abox_size = stsdbox_pos - mp4abox_front_size;
    audiospecificconfig_size = stsdbox_pos - audiospecificconfig_front_size;

    Write_FullBox_Header(Buf + audiospecificconfig_front_size, audiospecificconfig_size, 0x00, 0x00);

    Write_Box_Header(Buf + mp4abox_front_size, mp4abox_size);

    Write_FullBox_Header(Buf, stsdbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stsd_Box(stsdbox);
    return stsdbox_pos;
}

int Write_Stsd_Stsd_Box_Audio_Mp4a_Esds(unsigned char * Buf, unsigned int channelcount_stsd,
                                        unsigned int samplerate_stsd,
                                        unsigned char audioObjectType_stsd)
{
    Stsd_Box * stsdbox = NULL;
    unsigned int stsdbox_pos = 0;
    unsigned int mp4abox_size = 0;
    unsigned int mp4abox_front_size = 0; //
    unsigned int esdsbox_size = 0;
    unsigned int esdsbox_front_size = 0; //

    AllocStruct_Stsd_Box(&stsdbox);
    //////////////////////////////////////////////////////////////////////////

    stsdbox_pos += 4;
    // boxtype 0x73 74 73 64 stsd
    Buf[stsdbox_pos] = 0x73;
    Buf[stsdbox_pos + 1] = 0x74;
    Buf[stsdbox_pos + 2] = 0x73;
    Buf[stsdbox_pos + 3] = 0x64;
    stsdbox_pos += 4;
    
    // version,flag
    stsdbox_pos += 4;
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x01;
    stsdbox_pos += 4;

    mp4abox_front_size = stsdbox_pos;

    // mp4a box : 0x6D 70 34 61
    stsdbox_pos += 4; //mp4a box size;
    // boxtype 0x6D 70 34 61 mp4a
    Buf[stsdbox_pos] = 0x6D;
    Buf[stsdbox_pos + 1] = 0x70;
    Buf[stsdbox_pos + 2] = 0x34;
    Buf[stsdbox_pos + 3] = 0x61;
    stsdbox_pos += 4;
    // reserved_format[6]
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    Buf[stsdbox_pos + 4] = 0x00;
    Buf[stsdbox_pos + 5] = 0x00;
    stsdbox_pos += 6;
    // data_reference_index  0x01;
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x01;
    stsdbox_pos += 2;
    // reserved[2]
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    stsdbox_pos += 4;
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    Buf[stsdbox_pos + 2] = 0x00;
    Buf[stsdbox_pos + 3] = 0x00;
    stsdbox_pos += 4;
    // channelcount = 2
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = channelcount_stsd;
    stsdbox_pos += 2;
    // samplesize = 16 0x10
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x10;
    stsdbox_pos += 2;
    // pre_defined
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    stsdbox_pos += 2;
    // reserved_1
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    stsdbox_pos += 2;
    // samplerate = {timescale of media}<<16;
    int samplerate_stsd_t = samplerate_stsd << 16;
    Buf[stsdbox_pos] = samplerate_stsd_t >> 24;
    Buf[stsdbox_pos + 1] = (samplerate_stsd_t >> 16) & 0xFF;
    Buf[stsdbox_pos + 2] = (samplerate_stsd_t >> 8) & 0xFF;
    Buf[stsdbox_pos + 3] = samplerate_stsd_t & 0xFF;
    stsdbox_pos += 4;

    esdsbox_front_size = stsdbox_pos;

    // esds box : 0x65 73 64 73
    stsdbox_pos += 4; //esds box size;
    // boxtype 0x65 73 64 73 esds
    Buf[stsdbox_pos] = 0x65;
    Buf[stsdbox_pos + 1] = 0x73;
    Buf[stsdbox_pos + 2] = 0x64;
    Buf[stsdbox_pos + 3] = 0x73;
    stsdbox_pos += 4;
    // version,flag
    stsdbox_pos += 4;

    // tag
    Buf[stsdbox_pos] = 0x03;
    stsdbox_pos++;
    // Length_Field
    Buf[stsdbox_pos] = 0x19;
    stsdbox_pos++;
    // ES_ID
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x00;
    stsdbox_pos += 2;
    // streamDependenceFlag |  URL_Flag | OCRstreamFlag | streamPriority
    Buf[stsdbox_pos] = 0x00;
    stsdbox_pos++;
    //DecoderConfigDescriptor_tag
    Buf[stsdbox_pos] = 0x04;
    stsdbox_pos++;
    //Length_Field_1
    Buf[stsdbox_pos] = 0x11;
    stsdbox_pos++;
    //objectTypeIndication
    Buf[stsdbox_pos] = 0x40;
    stsdbox_pos++;
    //streamType | upStream | reserved
    Buf[stsdbox_pos] = 0x15;
    stsdbox_pos++;
    //bufferSizeDB
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x01;
    Buf[stsdbox_pos + 2] = 0xD5;
    stsdbox_pos += 3;
    //maxBitrate
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x01;
    Buf[stsdbox_pos + 2] = 0xF4;
    Buf[stsdbox_pos + 3] = 0xF8;
    stsdbox_pos += 4;
    //avgBitrate
    Buf[stsdbox_pos] = 0x00;
    Buf[stsdbox_pos + 1] = 0x01;
    Buf[stsdbox_pos + 2] = 0x73;
    Buf[stsdbox_pos + 3] = 0xF8;
    stsdbox_pos += 4;
    //DecSpecificInfotag
    Buf[stsdbox_pos] = 0x05;
    stsdbox_pos++;
    //Length_Field_2
    Buf[stsdbox_pos] = 0x02;
    stsdbox_pos++;

    // audioObjectType_2_GASpecificConfig : 5 | samplingFrequencyIndex :4 |
    // channelConfiguration : 4 | cpConfig : 2 | directMapping : 1
    int samplerate_stsd_t_1 = 0;
    if (samplerate_stsd == 8000) {
        samplerate_stsd_t_1 = 0x0b;
    }
    Buf[stsdbox_pos] = (audioObjectType_stsd << 3) | (samplerate_stsd_t_1 >> 1);
    Buf[stsdbox_pos + 1] = ((samplerate_stsd_t_1 & 0x01) << 7)
        | (channelcount_stsd << 3) | (0x00) | (0x00) | (0x00);
    stsdbox_pos += 2;

    // stsdbox_pos += 2;   DEBUG
    // SLConfigDescrTag
    Buf[stsdbox_pos] = 0x06;
    stsdbox_pos++;
    // Length_Field_3;//8    = 0x01
    Buf[stsdbox_pos] = 0x01;
    stsdbox_pos++;
    // predefined
    Buf[stsdbox_pos] = 0x02;
    stsdbox_pos++;

    mp4abox_size = stsdbox_pos - mp4abox_front_size;
    esdsbox_size = stsdbox_pos - esdsbox_front_size;

    Write_FullBox_Header(Buf + esdsbox_front_size, esdsbox_size, 0x00, 0x00);
    Write_Box_Header(Buf + mp4abox_front_size, mp4abox_size);
    Write_FullBox_Header(Buf, stsdbox_pos, 0x00, 0x00);
    
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stsd_Box(stsdbox);
    
    return stsdbox_pos;
}
