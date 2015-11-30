#define _XOPEN_SOURCE
#define _BSD_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/wait.h>

#include "mpeg4.h"
#include "Mux.h"

#undef MIN
#define MIN(a, b)   (((a) > (b)) ? (b) : (a))

typedef struct {
    // ! 4 for parameter sets and first slice in picture,
    // 3 for everything else (suggested)
    int startcodeprefix_len;

    // ! Length of the NAL unit (Excluding the start code,
    // which does not belong to the NALU)
    unsigned len;

    // ! Nal Unit Buffer size
    unsigned max_size;

    // ! should be always FALSE
    int forbidden_bit;

    // ! NALU_PRIORITY_xxxx
    int nal_reference_idc;

    // ! NALU_TYPE_xxxx
    int nal_unit_type;

    // ! contains the first byte followed by the EBSP
    unsigned char *buf;

    // ! true, if packet loss is detected
    unsigned short lost_packets;
} nalu_t;

static int verify_moov_size(Video_Audio_Info *video_audio_info, unsigned int alloc_size) 
{
    unsigned int moov_size;
    moov_size =
        Write_Moov_Moov_Box(video_audio_info->moov_buf,
                            video_audio_info->timescale,
                            video_audio_info->duration_file,
                            video_audio_info->videostream_id,
                            video_audio_info->duration_video,
                            video_audio_info->duration_audio,
                            video_audio_info->video_width,
                            video_audio_info->video_height,
                            video_audio_info->sample_count_stsz_video,
                            (unsigned int *)1/*video_audio_info->entry_size_stsz_video*/,
                            video_audio_info->sample_count_stsz_audio,
                            (unsigned int *)1/*video_audio_info->entry_size_stsz_audio*/,
                            1/*video_audio_info->entry_count_stss_Info*/,
                            video_audio_info->sample_number_stss_Info,
                            1/*video_audio_info->entry_count_stsc_video*/,
                            video_audio_info->first_chunk_stsc_video,
                            video_audio_info->samples_per_chunk_stsc_video,
                            video_audio_info->sample_description_index_stsc_video,
                            1/*video_audio_info->entry_count_stsc_audio*/,
                            video_audio_info->first_chunk_stsc_audio,
                            video_audio_info->samples_per_chunk_stsc_audio,
                            video_audio_info->sample_description_index_stsc_audio,
                            1/*video_audio_info->entry_count_stco_video*/,
                            video_audio_info->chunk_offset_stco_video,
                            1/*video_audio_info->entry_count_stco_audio*/,
                            video_audio_info->chunk_offset_stco_audio,
                            1 /*video_audio_info->entry_count_stts_video*/,
                            video_audio_info->sample_count_stts_video,
                            video_audio_info->sample_delta_stts_video,
                            1/*video_audio_info->entry_count_stts_audio*/,
                            video_audio_info->sample_count_stts_audio,
                            video_audio_info->sample_delta_stts_audio,
                            1/*video_audio_info->entry_count_ctts*/,
                            video_audio_info->sample_count_ctts,
                            video_audio_info->sample_offset_ctts,
                            video_audio_info->spsbuf,
                            video_audio_info->spslength,
                            video_audio_info->ppsbuf,
                            video_audio_info->ppslength,
                            video_audio_info->channel_configuration,
                            video_audio_info->audiosamplerate,
                            video_audio_info->audio_object_type_stsd
                            );

    printf("mpeg4 buf verify moov_size %d, alloc_size %d",
           moov_size, alloc_size);

    if (moov_size >= alloc_size){
        printf("Error: mpeg4 buf not enough!!!!");
        return -1;
    }

    return 0;
}

static int alloc_mpeg4_handle(Video_Audio_Info ** video_audio_info,
                              int max_vsize, int max_asize)
{
    Video_Audio_Info * info;
    unsigned int fix_size = MAX_FIX_LEN, var_vsize = 0, var_asize = 0;
    unsigned int moov_size;

    if ((info = (Video_Audio_Info *) calloc(1, sizeof(Video_Audio_Info))) == NULL) {
        printf("Error: Allocate Meory To Video_Audio_Info Buffer Failed ");
        return -1;
    }

    if ((info->spsbuf = (unsigned char *) calloc(MAX_SPS_LEN, sizeof(char))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->spsbuf  Buffer Failed ");
        return -1;
    }

    fix_size += MAX_SPS_LEN;

    if ((info->ppsbuf = (unsigned char *) calloc(MAX_PPS_LEN, sizeof(char))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->ppsbuf  Buffer Failed ");
        return -1;
    }

    fix_size += MAX_PPS_LEN;

    if ((info->entry_size_stsz_video
         = (unsigned int *) calloc(max_vsize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->entry_size_stsz_video  Buffer Failed ");
        return -1;
    }
    var_vsize += max_vsize*sizeof(unsigned int);

    if ((info->entry_size_stsz_audio
         = (unsigned int *) calloc(max_asize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->entry_size_stsz_audio  Buffer Failed ");
        return -1;
    }
    var_asize += max_asize*sizeof(unsigned int);

    if ((info->sample_number_stss_Info
         = (unsigned int *) calloc(max_vsize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_number_stss_Info  Buffer Failed ");
        return -1;
    }

    var_vsize += max_vsize*sizeof(unsigned int);

    if ((info->first_chunk_stsc_video
         = (unsigned int *) calloc(max_vsize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->first_chunk_stsc_video  Buffer Failed ");
        return -1;
    }
    var_vsize += max_vsize*sizeof(unsigned int);

    if ((info->samples_per_chunk_stsc_video
         = (unsigned int *) calloc(max_vsize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->samples_per_chunk_stsc_video  Buffer Failed ");
        return -1;
    }
    var_vsize += max_vsize*sizeof(unsigned int);

    if ((info->sample_description_index_stsc_video
         = (unsigned int *) calloc(max_vsize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_description_index_stsc_video  Buffer Failed ");
        return -1;
    }
    var_vsize += max_vsize*sizeof(unsigned int);

    if ((info->first_chunk_stsc_audio
         = (unsigned int *) calloc(max_asize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->first_chunk_stsc_audio  Buffer Failed ");
        return -1;
    }
    var_asize += max_asize*sizeof(unsigned int);

    if ((info->samples_per_chunk_stsc_audio
         = (unsigned int *) calloc(max_asize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->samples_per_chunk_stsc_audio  Buffer Failed ");
        return -1;
    }
    var_asize += max_asize*sizeof(unsigned int);

    if ((info->sample_description_index_stsc_audio
         = (unsigned int *) calloc(max_asize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_description_index_stsc_audio  Buffer Failed ");
        return -1;
    }
    var_asize += max_asize*sizeof(unsigned int);

    if ((info->chunk_offset_stco_video
         = (unsigned int *) calloc(max_vsize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->chunk_offset_stco_video  Buffer Failed ");
        return -1;
    }
    var_vsize += max_vsize*sizeof(unsigned int);

    if ((info->chunk_offset_stco_audio
         = (unsigned int *) calloc(max_asize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->chunk_offset_stco_audio  Buffer Failed ");
        return -1;
    }
    var_asize += max_asize*sizeof(unsigned int);

    if ((info->sample_count_stts_video
         = (unsigned int *) calloc(max_vsize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_count_stts_video  Buffer Failed ");
        return -1;
    }
    var_vsize += max_vsize*sizeof(unsigned int);

    if ((info->sample_delta_stts_video
         = (unsigned int *) calloc(max_vsize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_delta_stts_video  Buffer Failed ");
        return -1;
    }
    var_vsize += max_vsize*sizeof(unsigned int);

    if ((info->sample_count_stts_audio
         = (unsigned int *) calloc(max_asize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_count_stts_audio  Buffer Failed ");
        return -1;
    }
    var_asize += max_asize*sizeof(unsigned int);

    if ((info->sample_delta_stts_audio
         = (unsigned int *) calloc(max_asize, sizeof(unsigned int))) == NULL) {
        printf("Error: Allocate Meory To video_audio_info_t->sample_delta_stts_audio  Buffer Failed ");
        return -1;
    }
    var_asize += max_asize*sizeof(unsigned int);
/*
  if ((info->sample_count_ctts = (unsigned int *) calloc(max_vsize, sizeof(unsigned int))) == NULL) {
  printf("Error: Allocate Meory To video_audio_info_t->sample_count_ctts  Buffer Failed ");
  return -1;
  }
  var_vsize += max_vsize*sizeof(unsigned int);

  if ((info->sample_offset_ctts = (unsigned int *) calloc(max_vsize, sizeof(unsigned int))) == NULL) {
  printf("Error: Allocate Meory To video_audio_info_t->sample_offset_ctts  Buffer Failed ");
  return -1;
  }
  var_vsize += max_vsize*sizeof(unsigned int);
*/

    moov_size = fix_size + var_vsize + var_asize;
    if ((info->moov_buf = (unsigned char *) calloc(1, moov_size)) == NULL) {
        printf("Error: Allocate Meory To Moov Buffer Failed !");
        return -1;
    }

    verify_moov_size(info, (fix_size + var_vsize/max_vsize + var_asize/max_asize));

    printf("alloc moov_size %d, var_vsize %d, var_asize %d",
           moov_size, var_vsize, var_asize);

    *video_audio_info = info;

    return 0;
}

static void free_mpeg4_handle(Video_Audio_Info *video_audio_info)
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
/*
  if (video_audio_info->sample_count_ctts) {
  free(video_audio_info->sample_count_ctts);
  video_audio_info->sample_count_ctts = NULL;
  }
  if (video_audio_info->sample_offset_ctts) {
  free(video_audio_info->sample_offset_ctts);
  video_audio_info->sample_offset_ctts = NULL;
  }
*/
        if (video_audio_info->moov_buf) {
            free(video_audio_info->moov_buf);
            video_audio_info->moov_buf = NULL;
        }
        free(video_audio_info);
        video_audio_info = NULL;
    }
}


static int find_start_code(unsigned char *buf)
{
    if (buf[0] != 0 || buf[1] != 0 || buf[2] != 1) {
        if (buf[0] != 0 || buf[1] != 0 || buf[2] != 0 || buf[3] != 1)
            return -1;
        else
            return 4;
    }

    return 3;
}

static int fill_nalu(nalu_t *nalu, unsigned char *stream,
                     unsigned char ** p_data, int data_size)
{
    int ret = 0;
    unsigned char *c_data = NULL;

    if (*p_data == NULL)
        return -1;

    while (((ret = find_start_code(*p_data)) < 0) && (*p_data < (stream + data_size - 4)))
        (*p_data)++;

    if (ret < 0)
        return -1;

    nalu->startcodeprefix_len = ret;
    c_data = *p_data;

    (*p_data) = *p_data + nalu->startcodeprefix_len;
    nalu->buf = c_data + nalu->startcodeprefix_len;

    nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
    nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
    nalu->nal_unit_type = (nalu->buf[0]) & 0x1f; // 5 bit
    nalu->len = data_size - (c_data - stream) - nalu->startcodeprefix_len;

    if ((nalu->nal_unit_type != NAL_SLICE_IDR) && (nalu->nal_unit_type != NAL_SLICE)) {
        // this step for get len.
        while (((ret = find_start_code(*p_data)) < 0) && (*p_data < (stream + data_size - 4))) 
            (*p_data)++;

        if (ret < 0)
            *p_data = NULL;
        else
            nalu->len = *p_data - c_data - nalu->startcodeprefix_len;

    }
    else // save cpu, do not search more
        *p_data = NULL;

    return 0;
}

static int write_video_data(Video_Audio_Info *video_audio_info,
                            int type, unsigned char *buf,
                            int size, unsigned int ts)
{
    int ret = 0;
    int wbytes = 0;
    char write_size[8];

    memset(write_size, 0, sizeof(write_size));

    write_size[0] = size >> 24;
    write_size[1] = (size >> 16) & 0xFF;
    write_size[2] = (size >> 8) & 0xFF;
    write_size[3] = size & 0xFF;

    wbytes = WriteFile(video_audio_info->mp4_fd, write_size, 4);
    ret += wbytes;

    wbytes = WriteFile(video_audio_info->mp4_fd, (char *) buf, size);
    ret += wbytes;

    video_audio_info->write_video_size = ret;
//    printf("video ts = %u size %d\n",ts,ret);
    video_audio_info->entry_size_stsz_video[video_audio_info->sample_count_stsz_video]
        = video_audio_info->write_video_size;
    video_audio_info->sample_count_stsz_video++;

    //stts
    video_audio_info->sample_count_stts_video[video_audio_info->video_sample_number] = 1;
    video_audio_info->sample_delta_stts_video[video_audio_info->video_sample_number]
        = (ts - video_audio_info->timestamp_video)*90;//ts/(video_audio_info->video_sample_number+1);

    video_audio_info->video_sample_number++;

    //stss
    if (type == NAL_SLICE_IDR) {
        video_audio_info->sample_number_stss_Info[video_audio_info->key_frame_number]
            = video_audio_info->video_sample_number;
        video_audio_info->key_frame_number++;
    }

    //stco
    video_audio_info->chunk_offset_stco_video[video_audio_info->entry_count_stco_video]
        = video_audio_info->write_size_pos;
    video_audio_info->entry_count_stco_video++;

    //stsc
    video_audio_info->first_chunk_stsc_video[video_audio_info->entry_count_stsc_video] =
        video_audio_info->entry_count_stsc_video + 1;
    video_audio_info->samples_per_chunk_stsc_video[video_audio_info->entry_count_stsc_video] = 0x01;
    video_audio_info->sample_description_index_stsc_video[video_audio_info->entry_count_stsc_video] = 0x01;
    video_audio_info->entry_count_stsc_video++;
    video_audio_info->write_size_pos += video_audio_info->write_video_size;

    video_audio_info->timestamp_video = ts;//1000 / 27;

    if (ret != (4 + size))
        return -1;

//    printf("video_sample number %u\n",video_audio_info->video_sample_number);
    return 0;
}

static int write_audio_data(Video_Audio_Info *video_audio_info,
                            unsigned char *buf,
                            int size, unsigned int ts)
{
    int ret = 0;

    ret = WriteFile(video_audio_info->mp4_fd, (char *) buf, size);

    video_audio_info->write_audio_size = ret;
//    printf("audio ts = %u size %d\n",ts,ret);
    video_audio_info->entry_size_stsz_audio[video_audio_info->sample_count_stsz_audio]
        = video_audio_info->write_audio_size;
    video_audio_info->sample_count_stsz_audio++;

    //stts
    video_audio_info->sample_count_stts_audio[video_audio_info->audio_sample_number]
        = 1;
    video_audio_info->sample_delta_stts_audio[video_audio_info->audio_sample_number]
        = (ts - video_audio_info->timestamp_audio)*8;//ts/(video_audio_info->audio_sample_number + 1);

    video_audio_info->audio_sample_number++;

    //stco
    video_audio_info->chunk_offset_stco_audio[video_audio_info->entry_count_stco_audio]
        = video_audio_info->write_size_pos;
    video_audio_info->entry_count_stco_audio++;

    //stsc
    video_audio_info->first_chunk_stsc_audio[video_audio_info->entry_count_stsc_audio]
        = video_audio_info->entry_count_stsc_audio + 1;
    video_audio_info->samples_per_chunk_stsc_audio[video_audio_info->entry_count_stsc_audio] = 0x01;
    video_audio_info->sample_description_index_stsc_audio[video_audio_info->entry_count_stsc_audio] = 0x01;
    video_audio_info->entry_count_stsc_audio++;
    video_audio_info->write_size_pos += video_audio_info->write_audio_size;

    video_audio_info->timestamp_audio = ts;

    if (ret != size)
        return -1;

//    printf("audio_sample number %u\n",video_audio_info->audio_sample_number);
    return 0;
}

static int write_ftyp_box(Video_Audio_Info *video_audio_info)
{
    unsigned int writelength = 0;
    unsigned char buf[64];
    int ret = 0;

    memset(buf, 0, sizeof(buf));
    writelength = Write_Ftype_Ftyp_Box(buf);
    ret = WriteFile(video_audio_info->mp4_fd, (char *) buf, writelength);
    video_audio_info->write_ftyp_box_size = ret;
    video_audio_info->write_size_pos += video_audio_info->write_ftyp_box_size;

    if (ret != (int)writelength)
        return -1;
    return 0;
}

static int write_free_box(Video_Audio_Info *video_audio_info, int type)
{
    unsigned int writelength = 0;
    unsigned char buf[64];
    int ret = 0;
    FILE * fp = NULL;

    if (type == 0)
        fp = video_audio_info->mp4_fd;
    else
        fp = video_audio_info->moov_fd;

    memset(buf, 0, sizeof(buf));

    writelength = Write_Free_Free_Box(buf);
    ret = WriteFile(fp, (char *) buf, writelength);
    video_audio_info->write_free_box_size = ret;
    if (type == 0)
        video_audio_info->write_size_pos += video_audio_info->write_free_box_size;

    if (ret != (int)writelength)
        return -1;

    return 0;
}

static int write_mdat_box_header(Video_Audio_Info *video_audio_info)
{
    unsigned int writelength = 0;
    unsigned char buf[64];
    int ret = 0;

    memset(buf, 0, sizeof(buf));

    writelength = Write_Mdat_Mdat_Box(buf, video_audio_info->write_size_pos - video_audio_info->mdat_front_size);
    ret = WriteFile(video_audio_info->mp4_fd, (char *) buf, writelength);
    video_audio_info->write_mdat_header_size = ret;

    if (ret != (int)writelength)
        return -1;

    return 0;
}

static unsigned int get_file_size(char *path)
{
    unsigned int filesize = 0;
    struct stat statbuff;

    if (stat(path, &statbuff) < 0)
        return filesize;
    else
        filesize = statbuff.st_size;

    return filesize;
}

static int write_moov_box(Video_Audio_Info *video_audio_info, int type)
{
    FILE * fp = NULL;
    unsigned int wbytes = 0;

    if (type == 0)
        fp = video_audio_info->mp4_fd;
    else
        fp = video_audio_info->moov_fd;

    video_audio_info->duration_file
        = video_audio_info->timestamp_video*90;

    video_audio_info->timescale = 1000; //unused
    video_audio_info->videostream_id = 3; //for next track id
    video_audio_info->duration_video
        = video_audio_info->timestamp_video*90;
    video_audio_info->duration_audio
        = video_audio_info->timestamp_audio;
    video_audio_info->entry_count_stts_video
        = video_audio_info->video_sample_number;//0x01;
//    video_audio_info->sample_count_stts_video[0] = video_audio_info->video_sample_number;
//    video_audio_info->sample_delta_stts_video[0] = video_audio_info->timestamp_video / video_audio_info->video_sample_number + 1; //sample_delta:1001 //1001 * 37=37037 sample_delta*sample_count=duration
    video_audio_info->entry_count_stts_audio
        = video_audio_info->audio_sample_number;//0x01;
//    video_audio_info->sample_count_stts_audio[0] = video_audio_info->audio_sample_number;
//    video_audio_info->sample_delta_stts_audio[0] = video_audio_info->timestamp_audio / video_audio_info->audio_sample_number + 1;
    video_audio_info->entry_count_stss_Info
        = video_audio_info->key_frame_number;
    video_audio_info->write_moov_box_size
        = Write_Moov_Moov_Box(video_audio_info->moov_buf,
                              video_audio_info->timescale,
                              video_audio_info->duration_file,
                              video_audio_info->videostream_id,
                              video_audio_info->duration_video,
                              video_audio_info->duration_audio,
                              video_audio_info->video_width,
                              video_audio_info->video_height,
                              video_audio_info->sample_count_stsz_video,
                              video_audio_info->entry_size_stsz_video,
                              video_audio_info->sample_count_stsz_audio,
                              video_audio_info->entry_size_stsz_audio,
                              video_audio_info->entry_count_stss_Info,
                              video_audio_info->sample_number_stss_Info,
                              video_audio_info->entry_count_stsc_video,
                              video_audio_info->first_chunk_stsc_video,
                              video_audio_info->samples_per_chunk_stsc_video,
                              video_audio_info->sample_description_index_stsc_video,
                              video_audio_info->entry_count_stsc_audio,
                              video_audio_info->first_chunk_stsc_audio,
                              video_audio_info->samples_per_chunk_stsc_audio,
                              video_audio_info->sample_description_index_stsc_audio,
                              video_audio_info->entry_count_stco_video,
                              video_audio_info->chunk_offset_stco_video,
                              video_audio_info->entry_count_stco_audio,
                              video_audio_info->chunk_offset_stco_audio,
                              video_audio_info->entry_count_stts_video,
                              video_audio_info->sample_count_stts_video,
                              video_audio_info->sample_delta_stts_video,
                              video_audio_info->entry_count_stts_audio,
                              video_audio_info->sample_count_stts_audio,
                              video_audio_info->sample_delta_stts_audio,
                              video_audio_info->entry_count_ctts,
                              video_audio_info->sample_count_ctts,
                              video_audio_info->sample_offset_ctts,
                              video_audio_info->spsbuf,
                              video_audio_info->spslength,
                              video_audio_info->ppsbuf,
                              video_audio_info->ppslength,
                              video_audio_info->channel_configuration,
                              video_audio_info->audiosamplerate,
                              video_audio_info->audio_object_type_stsd
                              );

    printf("write moov %d bytes", video_audio_info->write_moov_box_size);

    wbytes = WriteFile(fp, (char *) video_audio_info->moov_buf,
                       video_audio_info->write_moov_box_size);
    if (wbytes != video_audio_info->write_moov_box_size)
        return -1;

    return 0;
}

static char *mp4_box_find(char *buf, int buf_len, char *box)
{
    int box_len = strlen(box), i = 0;
    for (i = 0; i < buf_len - box_len; i++) {
        if (buf[i] == box[0] && buf[i + 1] == box[1]
            && buf[i + 2] == box[2] && buf[i + 3] == box[3])
            return buf + i;
    }

    return NULL;
}

static int load_moov_info_from_file(char *path, char *file_name, int *duration,
                                    time_t *rec_time)
{
    unsigned int fsize = 0;
    char *box_p = NULL, *moov_buf = NULL;
    char buf[256];
    int video_duration = 0;
    int video_scale = 0;
    struct tm tm_time;

    memset(buf, 0, sizeof(buf));
    sprintf(buf,"%s.moov", file_name);
    FILE *fp = fopen(buf, "rb");
    if (fp == NULL)
        return -1;

    fsize = get_file_size(buf);
    if (fsize == 0) {
        fclose(fp);
        return -1;
    }
    moov_buf = calloc(1, fsize);
    if (fread(moov_buf, 1, fsize,fp) != fsize) {
        fclose(fp);
        free(moov_buf);
        return -1;
    }
    box_p = mp4_box_find(moov_buf, fsize, "mdhd");
    if (box_p == NULL) {
        fclose(fp);
        free(moov_buf);
        return -1;
    }

    memcpy(&video_scale,box_p + 16, 4);
    video_scale = ntohl(video_scale);
    memcpy(&video_duration, box_p + 20, 4);
    video_duration = ntohl(video_duration);

    fclose(fp);
    free(moov_buf);
    *duration = video_duration/video_scale;
    memset(buf, 0, sizeof(buf));
    strncpy(buf, file_name + strlen(path) + 18, 15);
    strptime(buf, "%Y%m%d_%H%M%S", &tm_time);
    *rec_time = mktime(&tm_time);

    return 0;
}

int mpeg4_open(void **mpeg4_handle, char *file_name, int record_len, unsigned int base_time)
{
    int ret = 0;
    char buf[256];
    Video_Audio_Info *video_audio_info = NULL;

    ret = alloc_mpeg4_handle((Video_Audio_Info **)mpeg4_handle,
                             record_len*MAX_VFRAME_RATE,
                             record_len*MAX_AFRAME_RATE);
    if (ret < 0)
        return ret;

    memset(buf, 0, sizeof(buf));
    video_audio_info = *mpeg4_handle;
    strcpy(video_audio_info->mp4_file_name, file_name);
    sprintf(buf, "%s.tmp", file_name);
    video_audio_info->mp4_fd = fopen(buf, "wb");
    if (video_audio_info->mp4_fd == NULL) {
        free_mpeg4_handle(video_audio_info);
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s.moov", file_name);
    video_audio_info->moov_fd = fopen(buf, "wb");
    if (video_audio_info->moov_fd == NULL) {
        fclose(video_audio_info->mp4_fd);
        free_mpeg4_handle(video_audio_info);
        return -1;
    }

    video_audio_info->audiosamplerate = AUDIO_SAMPLE;
    video_audio_info->audiosamplerate_idx = AUDIO_SAMPLE_INDEX;
    video_audio_info->channel_configuration = AUDIO_CHANNLE;
    video_audio_info->audio_object_type_stsd = AUDIO_OBJECT_TYPE;//AAC-LC

    write_ftyp_box(video_audio_info);
    ret = write_free_box(video_audio_info, 0);
    if (ret != 0) {
        fclose(video_audio_info->moov_fd);
        fclose(video_audio_info->mp4_fd);
        free_mpeg4_handle(video_audio_info);
        return -1;
    }

    // keep 8 bytes for mdat header
    video_audio_info->mdat_front_size = video_audio_info->write_size_pos;
    if (fseek(video_audio_info->mp4_fd, 8, SEEK_CUR) < 0) {
        fclose(video_audio_info->moov_fd);
        fclose(video_audio_info->mp4_fd);
        free_mpeg4_handle(video_audio_info);
        return -1;
    }

    video_audio_info->write_size_pos += 8;
    video_audio_info->timestamp_base = base_time;
    return 0;
}

int mpeg4_write_check(void *mpeg4_handle, int type, unsigned int max_duration)
{
    Video_Audio_Info *video_audio_info = mpeg4_handle;

    if (mpeg4_handle == NULL)
        return -1;

    if (type == 0) { // video
        if (video_audio_info->video_sample_number >= max_duration * MAX_VFRAME_RATE)
            return -1;
    }
    else {
        if (video_audio_info->audio_sample_number >= max_duration * MAX_AFRAME_RATE)
            return -1;
    }

    return 0;
}

int mpeg4_write_video(void *mpeg4_handle, unsigned char *data,
                      int size, int video_width, int video_height,
                      unsigned int ts)
{
    Video_Audio_Info *video_audio_info = mpeg4_handle;
    int ret = 0;
    unsigned char * pdata = data;
    nalu_t nalu;

    if (mpeg4_handle == NULL)
        return -1;

    video_audio_info->video_width = video_width;
    video_audio_info->video_height = video_height;

    while (1) {
        memset(&nalu, 0, sizeof(nalu));

        if (fill_nalu(&nalu, data, &pdata, size) < 0)
            break;

        switch (nalu.nal_unit_type) {
        case NAL_PPS:
            if (video_audio_info->ppslength == 0) {
                video_audio_info->ppslength = MIN(nalu.len, (MAX_SPS_LEN - 1));
                memcpy(video_audio_info->ppsbuf, nalu.buf,
                       video_audio_info->ppslength);
            }
            break;

        case NAL_SPS:
            if (video_audio_info->spslength == 0) {
                video_audio_info->spslength = MIN(nalu.len, (MAX_PPS_LEN - 1));
                memcpy(video_audio_info->spsbuf, nalu.buf,
                       video_audio_info->spslength);
            }
            break;

        case NAL_SLICE_IDR:
            ret = write_video_data(video_audio_info, nalu.nal_unit_type,
                                   nalu.buf, nalu.len,
                                   ts-video_audio_info->timestamp_base);         
            return ret; //stop finding

        case NAL_SEI://not use
            break;

        case NAL_SLICE:
            if (video_audio_info->video_sample_number > 0)//wait for I frame
                ret = write_video_data(video_audio_info, nalu.nal_unit_type,
                                       nalu.buf, nalu.len,
                                       ts-video_audio_info->timestamp_base);
            return ret; //stop finding

        default:
            printf("unknown nal type %d", nalu.nal_unit_type);
            break;
        }

    }

    printf("warning: cannot find useful NALU");

    return 0;
}

int mpeg4_write_audio(void *mpeg4_handle, unsigned char *data,
                      int size, unsigned int ts)
{
    Video_Audio_Info *video_audio_info = mpeg4_handle;
    if (mpeg4_handle == NULL)
        return -1;

    return write_audio_data(video_audio_info, data,size,
                            (ts-video_audio_info->timestamp_base));
}

int mpeg4_close(void *mpeg4_handle)
{
    Video_Audio_Info *video_audio_info = mpeg4_handle;
    struct stat statbuff;
    char cmd[256];

    if (mpeg4_handle == NULL)
        return -1;

    if (video_audio_info->moov_fd) {
        fclose(video_audio_info->moov_fd);
        video_audio_info->moov_fd = NULL;
    }

    if (video_audio_info->mp4_fd) {
        fseek(video_audio_info->mp4_fd, video_audio_info->mdat_front_size, 0);
        write_mdat_box_header(video_audio_info);

        fseek(video_audio_info->mp4_fd, video_audio_info->write_size_pos, 0);
        write_moov_box(video_audio_info, 0);
        write_free_box(video_audio_info, 0);

        fclose(video_audio_info->mp4_fd);
        video_audio_info->mp4_fd = NULL;
    }

    sprintf(cmd, "mv %s.tmp %s.mp4;rm %s.moov", video_audio_info->mp4_file_name,
            video_audio_info->mp4_file_name, video_audio_info->mp4_file_name);
    system(cmd);

    free_mpeg4_handle(mpeg4_handle);
    return 0;
}

int mpeg4_backup_index(void *mpeg4_handle)
{
    Video_Audio_Info *video_audio_info = mpeg4_handle;

    write_moov_box(video_audio_info, 1);
    write_free_box(video_audio_info, 1);
    fseek(video_audio_info->moov_fd, 0, SEEK_SET);

    return 0;
}

unsigned int mpeg4_get_file_size(void *mpeg4_handle)
{
    Video_Audio_Info *video_audio_info = mpeg4_handle;

    if (mpeg4_handle == NULL)
        return -1;

    return video_audio_info->write_size_pos;
}
