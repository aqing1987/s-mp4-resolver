/**
 * @brief
 *    all data in a QuickTime movie is stored in big-endian byte ordering, also known as
 *    network byte ordering, in which the most significant bytes are stored and
 *    transmitted first
 * */
#include <stdio.h>
#include <arpa/inet.h>
#include <time.h>

#include "Mp4.h"
#include "Ftyp_Box.h"
#include "Mdat_Box.h"
#include "Moov_Box.h"

#define FILENAME_LEN 128

typedef struct {
    char file_name[FILENAME_LEN];
    FILE *fp;

} mp4_info_st;

mp4_info_st g_mp4_info_t;

static void show_uint_char(unsigned int val)
{
    unsigned char buf[4] = "";

    memcpy(buf, &val, 4);

    int i = 0; 
    for (i = 3; i >= 0; i--) {
        printf("%c", buf[i]);
    }
    printf("\n");
}

static void ulong2time_show(unsigned long val)
{
    char szBuf[256] = {0};  
    time_t time;

    
    time = (time_t)ntohl(val);
    
    strftime(szBuf, sizeof(szBuf), "%Y-%m-%d %H:%M:%S", localtime(&time));
    printf("%s (0x%x)\n", szBuf, ntohl(val));
}


static void get_box_size_type(unsigned int *bsize)
{
    unsigned int box_size;
    unsigned int box_type;
    
    // atom size
    fread(&box_size, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    box_size = ntohl(box_size);
    printf("size = 0x%x (%u)\n", box_size, box_size);
    *bsize = box_size;

    // type
    fread(&box_type, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    box_type = ntohl(box_type);
    printf("type = 0x%x ", box_type);
    show_uint_char(box_type);
}

// 1
static int analysis_ftyp_box()
{
    Ftyp_Box ftype_content;
    unsigned int box_size = 0;
    unsigned long remain_size = 0;

    printf("\t=== ftyp start ===\n");

    get_box_size_type(&box_size);
    remain_size = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    
    // major_brand
    fread(&ftype_content.major_brand, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    ftype_content.major_brand = ntohl(ftype_content.major_brand);
    printf("major brand = %x ", ftype_content.major_brand);
    show_uint_char(ftype_content.major_brand);
    remain_size -= sizeof(unsigned int);
    
    // minor_version
    fread(&ftype_content.minor_version, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    ftype_content.minor_version = ntohl(ftype_content.minor_version);
    printf("minor_version = %d\n", ftype_content.minor_version);
    remain_size -= sizeof(unsigned int);

    // compatible_brands    
    printf("remain size = %ld, rest brands as follow:\n", remain_size);
    unsigned int *rest_buf = (unsigned int *)malloc(remain_size);
    if (rest_buf == NULL) {
        printf("calloc err.\n");
        printf("\t=== ftyp box end ===\n");
        return -1;
    }

    fread(rest_buf, remain_size, 1, g_mp4_info_t.fp);
    int i = 0;
    for (i = 0; i < remain_size/4; i++) {
        rest_buf[i] = ntohl(rest_buf[i]);
        printf("compatible-brands[%d] = %x ", i, rest_buf[i]);
        show_uint_char(rest_buf[i]);
    }
    free(rest_buf);
    
    printf("\t=== ftyp end ===\n\n");

    return 0;
}

// 2
static int analysis_free_box()
{
    unsigned int box_size = 0;
    unsigned long remain_size = 0;

    printf("\t=== free box start ===\n");

    get_box_size_type(&box_size);
    remain_size = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    
    if (remain_size > 0) {
        printf("remain_size = %lu\n", remain_size);
        unsigned char * rest_buf = (unsigned char*)calloc(1, remain_size);
        if (rest_buf == NULL) {
            printf("calloc err.\n");
            printf("\t=== free box end ===\n");
            return -1;
        }
        fread(rest_buf, remain_size, 1, g_mp4_info_t.fp);
        free(rest_buf);
    }

    printf("\t=== free box end ===\n\n");
    
    return 0;
}

// 3
static int analysis_mdat_box()
{
    unsigned int box_size = 0;
    unsigned long remain_size = 0;

    printf("\t=== mdat box start ===\n");

    get_box_size_type(&box_size);
    remain_size = box_size - sizeof(unsigned int) - sizeof(unsigned int);

    if (remain_size > 0) {
        printf("remain_size = %lu\n", remain_size);

        int res = fseek(g_mp4_info_t.fp, remain_size, SEEK_CUR);
        if (res == -1) {
            printf("fseek err.\n");
        }
        else {
            long pos = ftell(g_mp4_info_t.fp);
            printf("current file position = %ld\n", pos);
        }
    }

    printf("\t=== mdat box end ===\n\n");

    return 0;
}

static int moov_mvhd_box_ana(unsigned int *moov_remain_bytes)
{
    Mvhd_Box mvhd_bx;
    unsigned int box_size;
    unsigned int mvhd_remain_bytes;
    
    printf("\t=== mvhd(in moov) box start ===\n");
    get_box_size_type(&box_size);

    *moov_remain_bytes -= box_size;

    
    mvhd_remain_bytes = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    unsigned int version_flags = 0;
    // get version & flags
    fread(&version_flags, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("version&falgs = %u (0x%x)\n", ntohl(version_flags), ntohl(version_flags));
    mvhd_remain_bytes -= sizeof(unsigned int);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    // get date
    fread(&mvhd_bx.creation_time, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    ulong2time_show(mvhd_bx.creation_time);
    mvhd_remain_bytes -= sizeof(unsigned long);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);
    
    // modification time
    fread(&mvhd_bx.modification_time, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    ulong2time_show(mvhd_bx.modification_time);
    mvhd_remain_bytes -= sizeof(unsigned long);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    // timescale
    fread(&mvhd_bx.timescale, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("timescale = %u (0x%x)\n", ntohl(mvhd_bx.timescale), ntohl(mvhd_bx.timescale));
    mvhd_remain_bytes -= sizeof(unsigned int);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    // unsigned long duration
    fread(&mvhd_bx.duration, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    printf("duration = %u (0x%x)\n", ntohl(mvhd_bx.duration), ntohl(mvhd_bx.duration));
    mvhd_remain_bytes -= sizeof(unsigned long);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    // int rate
    fread(&mvhd_bx.rate, sizeof(int), 1, g_mp4_info_t.fp);
    printf("rate = %u (0x%x)\n", ntohl(mvhd_bx.rate), ntohl(mvhd_bx.rate));
    mvhd_remain_bytes -= sizeof(int);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    // unsigned short volume
    fread(&mvhd_bx.volume, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    printf("volume = %u (0x%x)\n", ntohs(mvhd_bx.volume), ntohs(mvhd_bx.volume));
    mvhd_remain_bytes -= sizeof(unsigned short);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    // unsigned short reserved
    fread(&mvhd_bx.reserved, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    mvhd_remain_bytes -= sizeof(unsigned short);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    // unsigned int reserved_1[2];
    fread(&mvhd_bx.reserved_1[0], sizeof(unsigned int), 2, g_mp4_info_t.fp);
    mvhd_remain_bytes -= 2*sizeof(unsigned int);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    // int matrix[9]
    fread(&mvhd_bx.matrix[0], sizeof(int), 9, g_mp4_info_t.fp);
    mvhd_remain_bytes -= 9*sizeof(int);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    // unsigned int pre_defined[6]
    fread(&mvhd_bx.pre_defined[0], sizeof(unsigned int), 6, g_mp4_info_t.fp);
    mvhd_remain_bytes -= 6*sizeof(unsigned int);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    // unsigned int next_track_ID
    fread(&mvhd_bx.next_track_ID, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("next_track_ID = %u (0x%x)\n", ntohs(mvhd_bx.next_track_ID), ntohs(mvhd_bx.next_track_ID));
    mvhd_remain_bytes -= sizeof(unsigned int);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);
    
    printf("\t=== mvhd(in moov) box end ===\n");

    return 0;
}

static int moov_trak_video_box_ana()
{
    
}

static int moov_trak_audio_box_ana()
{
    
}

// 4
static int analysis_moov_box()
{
    unsigned int box_size = 0;
    unsigned int remain_size = 0;

    printf("\t=== moov box start ===\n");

    get_box_size_type(&box_size);
    remain_size = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("moov_box_remain bytes = %u\n", remain_size);

    moov_mvhd_box_ana(&remain_size);
    printf("moov_box_remain bytes = %u\n", remain_size);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s mp4_name\n", argv[0]);
        return -1;
    }

    memset(&g_mp4_info_t, 0, sizeof(g_mp4_info_t));

    strncpy(g_mp4_info_t.file_name, argv[1], FILENAME_LEN);
    printf("\t$$$ Filename %s $$$\n", g_mp4_info_t.file_name);

    g_mp4_info_t.fp = fopen(g_mp4_info_t.file_name, "rb");
    if (g_mp4_info_t.fp == NULL) {
        printf("fopen err.\n");
        return -1;
    }

    analysis_ftyp_box();
    analysis_free_box();
    analysis_mdat_box();
    analysis_moov_box();

    fclose(g_mp4_info_t.fp);
    
    return 0;
}
