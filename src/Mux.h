#ifndef MUX_H_
#define MUX_H_

#include "Ftyp_Box.h"
#include "Free_Box.h"
#include "Mdat_Box.h"
#include "Moov_Box.h"

#define  MAX_SPS_LENGTH   100*2
#define  MAX_PPS_LENGTH   100
#define  MAX_VFRAME_SIZE  9000    //max video frames
#define  MAX_AFRAME_SIZE  2400    //max audio frames

typedef struct Tag_Video_Audio_Info {
    unsigned char *spsbuf;  // sps buffer
    unsigned int spslength; // sps buffer len
    unsigned char *ppsbuf;  // pps buffer
    unsigned int ppslength; // pps len
    unsigned int audiosamplerate; //
    unsigned int audiosamplerate_idx; //samplerateindex
    unsigned int channel_configuration; //
    unsigned int audio_object_type_stsd; //AAC level
    unsigned int video_width; //
    unsigned int video_height; //
//    unsigned int vframe_rate; //video frame rate
    unsigned int timescale; //
    unsigned int duration_file; //
    unsigned int videostream_id; //
    unsigned int duration_video; //
    unsigned int duration_audio; //
    unsigned int sample_count_stsz_video; //
    unsigned int *entry_size_stsz_video; //
    unsigned int sample_count_stsz_audio; //
    unsigned int *entry_size_stsz_audio; //
    unsigned int entry_count_stss_Info; //
    unsigned int *sample_number_stss_Info; //
    unsigned int entry_count_stsc_video; //
    unsigned int *first_chunk_stsc_video; //
    unsigned int *samples_per_chunk_stsc_video; //
    unsigned int *sample_description_index_stsc_video; //;
    unsigned int entry_count_stsc_audio; //
    unsigned int *first_chunk_stsc_audio; //
    unsigned int *samples_per_chunk_stsc_audio; //
    unsigned int *sample_description_index_stsc_audio; //0x01;
    unsigned int entry_count_stco_video; //
    unsigned int *chunk_offset_stco_video; //
    unsigned int entry_count_stco_audio; //
    unsigned int *chunk_offset_stco_audio; //
    unsigned int entry_count_stts_video; //
    unsigned int *sample_count_stts_video;//
    unsigned int *sample_delta_stts_video; //
    unsigned int entry_count_stts_audio; //
    unsigned int *sample_count_stts_audio; //
    unsigned int *sample_delta_stts_audio; //
    unsigned int entry_count_ctts;
    unsigned int *sample_count_ctts;
    unsigned int *sample_offset_ctts;
    unsigned int video_sample_number; //
    unsigned int audio_sample_number; //
    unsigned int key_frame_number; //
    unsigned int write_video_size;
    unsigned int write_audio_size;
    unsigned int mdat_front_size; //
    unsigned int timestamp_video; //
    unsigned int timestamp_audio; //
    unsigned int timestamp_base;
    unsigned int write_size_pos;
    unsigned int write_ftyp_box_size;
    unsigned int write_free_box_size;
    unsigned int write_mdat_header_size;
    unsigned int write_mdat_box_size;
    unsigned int write_moov_box_size;

    char mp4_file_name[256];
    FILE * mp4_fd;
    FILE * moov_fd;
    unsigned char *moov_buf;
} Video_Audio_Info;

extern unsigned char Ftyp_Box_Buf[MAX_FTYP_BOXSIZE];
extern unsigned char Free_Box_Buf[MAX_FREE_BOX_SIZE];
extern unsigned char Mdat_Box_Buf[MAX_ONE_FRAME_DATA_SIZE];
extern unsigned char Moov_Box_Buf[MAX_MOOV_BOX_SIZE];

int AllocStruct_Video_Audio_Info(Video_Audio_Info ** video_audio_info);
int FreeStruct_Video_Audio_Info(Video_Audio_Info *video_audio_info);

int Write_Ftyp_Box(FILE*fp,unsigned char * Buf);
int Write_Free_Box(FILE*fp,unsigned char * Buf);

int Write_Mdat_Box_Header(FILE*fp,unsigned char * Buf,
                          unsigned int Mdat_Box_Size_But_Header); //Header
int Write_Mdat_Box_Frame(FILE*fp,unsigned char * Buf,
                         unsigned int WriteFrameType); //Frame

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
                   unsigned int * chunk_offset_stco_audio,
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
                   unsigned char audioObjectType_stsd); //

#endif
