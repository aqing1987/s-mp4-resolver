#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "Mux.h"

unsigned char Ftyp_Box_Buf[MAX_FTYP_BOXSIZE];
unsigned char Free_Box_Buf[MAX_FREE_BOX_SIZE];
unsigned char Mdat_Box_Buf[MAX_ONE_FRAME_DATA_SIZE];
unsigned char Moov_Box_Buf[MAX_MOOV_BOX_SIZE];

int AllocStruct_Video_Audio_Info(Video_Audio_Info ** video_audio_info)
{
    Video_Audio_Info * video_audio_info_t = *video_audio_info;
    
    if ((video_audio_info_t
         = (Video_Audio_Info *) calloc(1, sizeof(Video_Audio_Info))) == NULL) {
        printf("Error: Allocate Meory To Video_Audio_Info Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->spsbuf
         = (unsigned char *) calloc(MAX_SPS_LENGTH, sizeof(char))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->spsbuf  Buffer Failed\n");
        return -1;
    }
    if ((video_audio_info_t->ppsbuf
         = (unsigned char *) calloc(MAX_PPS_LENGTH, sizeof(char))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->ppsbuf  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->entry_size_stsz_video
         = (unsigned int *) calloc(MAX_VFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->entry_size_stsz_video  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->entry_size_stsz_audio
         = (unsigned int *) calloc(MAX_AFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->entry_size_stsz_audio  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->sample_number_stss_Info
         = (unsigned int *) calloc(MAX_VFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_number_stss_Info  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->first_chunk_stsc_video
         = (unsigned int *) calloc(MAX_VFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->first_chunk_stsc_video  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->samples_per_chunk_stsc_video
         = (unsigned int *) calloc(MAX_VFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->samples_per_chunk_stsc_video  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->sample_description_index_stsc_video
         = (unsigned int *) calloc(MAX_VFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_description_index_stsc_video  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->first_chunk_stsc_audio
         = (unsigned int *) calloc(MAX_AFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->first_chunk_stsc_audio  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->samples_per_chunk_stsc_audio
         = (unsigned int *) calloc(MAX_AFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->samples_per_chunk_stsc_audio  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->sample_description_index_stsc_audio
         = (unsigned int *) calloc(MAX_AFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_description_index_stsc_audio  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->chunk_offset_stco_video
         = (unsigned int *) calloc(MAX_VFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->chunk_offset_stco_video  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->chunk_offset_stco_audio
         = (unsigned int *) calloc(MAX_AFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->chunk_offset_stco_audio  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->sample_count_stts_video
         = (unsigned int *) calloc(MAX_VFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_count_stts_video  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->sample_delta_stts_video
         = (unsigned int *) calloc(MAX_VFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_delta_stts_video  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->sample_count_stts_audio
         = (unsigned int *) calloc(MAX_AFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_count_stts_audio  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->sample_delta_stts_audio
         = (unsigned int *) calloc(MAX_AFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_delta_stts_audio  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->sample_count_ctts
         = (unsigned int *) calloc(MAX_VFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_count_ctts  Buffer Failed \n");
        return -1;
    }
    if ((video_audio_info_t->sample_offset_ctts
         = (unsigned int *) calloc(MAX_VFRAME_SIZE, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_offset_ctts  Buffer Failed \n");
        return -1;
    }
    *video_audio_info = video_audio_info_t;
    
    return 1;
}

int FreeStruct_Video_Audio_Info(Video_Audio_Info *video_audio_info)
{
    if (video_audio_info) {

        if (video_audio_info->spsbuf) {
            free(video_audio_info->spsbuf);
            video_audio_info->spsbuf = NULL;
        }
        if (video_audio_info->ppsbuf) {
            free(video_audio_info->ppsbuf);
            video_audio_info->ppsbuf = NULL;
        }
        if (video_audio_info->entry_size_stsz_video) {
            free(video_audio_info->entry_size_stsz_video);
            video_audio_info->entry_size_stsz_video = NULL;
        }
        if (video_audio_info->entry_size_stsz_audio) {
            free(video_audio_info->entry_size_stsz_audio);
            video_audio_info->entry_size_stsz_audio = NULL;
        }
        if (video_audio_info->sample_number_stss_Info) {
            free(video_audio_info->sample_number_stss_Info);
            video_audio_info->sample_number_stss_Info = NULL;
        }
        if (video_audio_info->first_chunk_stsc_video) {
            free(video_audio_info->first_chunk_stsc_video);
            video_audio_info->first_chunk_stsc_video = NULL;
        }
        if (video_audio_info->samples_per_chunk_stsc_video) {
            free(video_audio_info->samples_per_chunk_stsc_video);
            video_audio_info->samples_per_chunk_stsc_video = NULL;
        }
        if (video_audio_info->sample_description_index_stsc_video) {
            free(video_audio_info->sample_description_index_stsc_video);
            video_audio_info->sample_description_index_stsc_video = NULL;
        }
        if (video_audio_info->first_chunk_stsc_audio) {
            free(video_audio_info->first_chunk_stsc_audio);
            video_audio_info->first_chunk_stsc_audio = NULL;
        }
        if (video_audio_info->samples_per_chunk_stsc_audio) {
            free(video_audio_info->samples_per_chunk_stsc_audio);
            video_audio_info->samples_per_chunk_stsc_audio = NULL;
        }
        if (video_audio_info->sample_description_index_stsc_audio) {
            free(video_audio_info->sample_description_index_stsc_audio);
            video_audio_info->sample_description_index_stsc_audio = NULL;
        }
        if (video_audio_info->chunk_offset_stco_video) {
            free(video_audio_info->chunk_offset_stco_video);
            video_audio_info->chunk_offset_stco_video = NULL;
        }
        if (video_audio_info->chunk_offset_stco_audio) {
            free(video_audio_info->chunk_offset_stco_audio);
            video_audio_info->chunk_offset_stco_audio = NULL;
        }
        if (video_audio_info->sample_count_stts_video) {
            free(video_audio_info->sample_count_stts_video);
            video_audio_info->sample_count_stts_video = NULL;
        }
        if (video_audio_info->sample_delta_stts_video) {
            free(video_audio_info->sample_delta_stts_video);
            video_audio_info->sample_delta_stts_video = NULL;
        }
        if (video_audio_info->sample_count_stts_audio) {
            free(video_audio_info->sample_count_stts_audio);
            video_audio_info->sample_count_stts_audio = NULL;
        }
        if (video_audio_info->sample_count_ctts) {
            free(video_audio_info->sample_count_ctts);
            video_audio_info->sample_count_ctts = NULL;
        }
        if (video_audio_info->sample_offset_ctts) {
            free(video_audio_info->sample_offset_ctts);
            video_audio_info->sample_offset_ctts = NULL;
        }

        free(video_audio_info);
        video_audio_info = NULL;
    }
    
    return 1;
}

int Write_Ftyp_Box(FILE * fp,unsigned char * Buf)
{
    unsigned int writelength = 0;
    
    writelength = Write_Ftype_Ftyp_Box(Buf);
    return WriteFile(fp, (char *) Buf, writelength);
}

int Write_Free_Box(FILE * fp,unsigned char * Buf)
{
    unsigned int writelength = 0;
    
    writelength = Write_Free_Free_Box(Buf);
    return WriteFile(fp, (char *) Buf, writelength);
}

int Write_Mdat_Box_Header(FILE * fp,unsigned char * Buf, unsigned int Mdat_Box_Size_But_Header)
{
    unsigned int writelength = 0;
    
    writelength = Write_Mdat_Mdat_Box(Buf, Mdat_Box_Size_But_Header);
    return WriteFile(fp, (char *) Buf, writelength);
}

int Write_Mdat_Box_Frame(FILE * fp,unsigned char * Buf, unsigned int WriteFrameType)
{
    unsigned int writelength = 0;
    
    writelength = Write_Mdat_Frame_Data(Buf, WriteFrameType);
    return WriteFile(fp, (char *) Buf, writelength);
}

int Write_Moov_Box(FILE *fp,unsigned char * Buf,
                   unsigned int timescale_Mvhd,
                   unsigned int duration_Mvhd,
                   unsigned int next_track_ID_Mvhd,
                   unsigned int duration_tkhd_video,
                   unsigned int duration_tkhd_audio,
                   unsigned int width_tkhd,
                   unsigned int height_tkhd,
                   unsigned int sample_count_stsz_video,
                   unsigned int * entry_size_stsz_video,
                   unsigned int sample_count_stsz_audio,
                   unsigned int * entry_size_stsz_audio,
                   unsigned int entry_count_stss,
                   unsigned int * sample_number_stss,
                   unsigned int entry_count_stsc_video,
                   unsigned int * first_chunk_stsc_video,
                   unsigned int * samples_per_chunk_stsc_video,
                   unsigned int * sample_description_index_stsc_video,
                   unsigned int entry_count_stsc_audio,
                   unsigned int * first_chunk_stsc_audio,
                   unsigned int * samples_per_chunk_stsc_audio,
                   unsigned int * sample_description_index_stsc_audio,
                   unsigned int entry_count_stco_video,
                   unsigned int * chunk_offset_stco_video,
                   unsigned int entry_count_stco_audio,
                   unsigned int * chunk_offset_stco_audi,
                   unsigned int entry_count_stts_video,
                   unsigned int *sample_count_stts_video,
                   unsigned int *sample_delta_stts_video,
                   unsigned int entry_count_stts_audio,
                   unsigned int *sample_count_stts_audio,
                   unsigned int *sample_delta_stts_audio,
                   unsigned int entry_count_ctts,
                   unsigned int * sample_count_ctts,
                   unsigned int *sample_offset_ctts,
                   unsigned char * spsbuf,
                   unsigned int spslength,
                   unsigned char * ppsbuf,
                   unsigned int ppslength,
                   unsigned int channelcount_stsd,
                   unsigned int samplerate_stsd,
                   unsigned char audioObjectType_stsd)
{
    unsigned int writelength = 0;
    
    writelength = Write_Moov_Moov_Box(Buf, timescale_Mvhd,
                                      duration_Mvhd,
                                      next_track_ID_Mvhd,
                                      duration_tkhd_video,
                                      duration_tkhd_audio,
                                      width_tkhd, height_tkhd,
                                      sample_count_stsz_video,
                                      entry_size_stsz_video,
                                      sample_count_stsz_audio,
                                      entry_size_stsz_audio,
                                      entry_count_stss,
                                      sample_number_stss,
                                      entry_count_stsc_video,
                                      first_chunk_stsc_video,
                                      samples_per_chunk_stsc_video,
                                      sample_description_index_stsc_video,
                                      entry_count_stsc_audio,
                                      first_chunk_stsc_audio,
                                      samples_per_chunk_stsc_audio,
                                      sample_description_index_stsc_audio,
                                      entry_count_stco_video,
                                      chunk_offset_stco_video,
                                      entry_count_stco_audio,
                                      chunk_offset_stco_audi,
                                      entry_count_stts_video,
                                      sample_count_stts_video,
                                      sample_delta_stts_video,
                                      entry_count_stts_audio,
                                      sample_count_stts_audio,
                                      sample_delta_stts_audio,
                                      entry_count_ctts,
                                      sample_count_ctts,
                                      sample_offset_ctts,
                                      spsbuf,
                                      spslength,
                                      ppsbuf,
                                      ppslength,
                                      channelcount_stsd,
                                      samplerate_stsd,
                                      audioObjectType_stsd);
    
    return WriteFile(fp, (char *) Buf, writelength);
}
