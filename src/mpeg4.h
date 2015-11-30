#ifndef MPEG4_H_
#define MPEG4_H_

#define MAX_SPS_LEN           256
#define MAX_PPS_LEN           128
#define MAX_FIX_LEN           800

#define MAX_VFRAME_RATE       30
#define MAX_AFRAME_RATE       10
#define AUDIO_SAMPLE          8000
#define AUDIO_SAMPLE_INDEX    0xb
#define AUDIO_CHANNLE         1
#define AUDIO_OBJECT_TYPE     0x2

int mpeg4_open(void **mpeg4_handle, char *file_name,
               int record_len, unsigned int base_time);
int mpeg4_close(void *mpeg4_handle);

int mpeg4_write_check(void *mpeg4_handle, int type,
                      unsigned int max_duration);
int mpeg4_write_video(void *mpeg4_handle, unsigned char *data,
                      int size,int video_width, int video_height, unsigned int ts);
int mpeg4_write_audio(void *mpeg4_handle, unsigned char *data,
                      int size, unsigned int ts);

int mpeg4_backup_index(void *mpeg4_handle);
int mpeg4_recover_file(char *path);
unsigned int mpeg4_get_file_size(void *mpeg4_handle);

#endif // MPEG4_H_ 
