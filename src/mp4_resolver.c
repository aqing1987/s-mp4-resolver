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

typedef enum {
    TYPE_VIDEO,
    TYPE_AUDIO,
} video_audio_e;

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
    
    strftime(szBuf, sizeof(szBuf),
             "%Y-%m-%d %H:%M:%S", localtime(&time));
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

    printf("\t=== ftyp start (1) ===\n");

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
    
    printf("\t=== ftyp end (1) ===\n\n");

    return 0;
}

// 2
static int analysis_free_box()
{
    unsigned int box_size = 0;
    unsigned long remain_size = 0;

    printf("\t=== free box start (2) ===\n");

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

    printf("\t=== free box end (2) ===\n\n");
    
    return 0;
}

// 3
static int analysis_mdat_box()
{
    unsigned int box_size = 0;
    unsigned long remain_size = 0;

    printf("\t=== mdat box start (3) ===\n");

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

    printf("\t=== mdat box end (3) ===\n\n");

    return 0;
}

// 4-1
static int moov_mvhd_box_ana(unsigned int *moov_remain_bytes)
{
    Mvhd_Box mvhd_bx;
    unsigned int box_size;
    unsigned int mvhd_remain_bytes;
    
    printf("\t=== mvhd(in moov) box start (4-1) ===\n");
    get_box_size_type(&box_size);

    *moov_remain_bytes -= box_size;

    
    mvhd_remain_bytes = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);

    unsigned int version_flags = 0;
    // get version & flags
    fread(&version_flags, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("version&falgs = %u (0x%x)\n", ntohl(version_flags), ntohl(version_flags));
    mvhd_remain_bytes -= sizeof(unsigned int);

    // get date
    fread(&mvhd_bx.creation_time, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    ulong2time_show(mvhd_bx.creation_time);
    mvhd_remain_bytes -= sizeof(unsigned long);
    
    // modification time
    fread(&mvhd_bx.modification_time, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    ulong2time_show(mvhd_bx.modification_time);
    mvhd_remain_bytes -= sizeof(unsigned long);

    // timescale
    fread(&mvhd_bx.timescale, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("timescale = %u (0x%x)\n", ntohl(mvhd_bx.timescale), ntohl(mvhd_bx.timescale));
    mvhd_remain_bytes -= sizeof(unsigned int);

    // unsigned long duration
    fread(&mvhd_bx.duration, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    printf("duration = %u (0x%x)\n", ntohl(mvhd_bx.duration), ntohl(mvhd_bx.duration));
    mvhd_remain_bytes -= sizeof(unsigned long);

    // int rate
    fread(&mvhd_bx.rate, sizeof(int), 1, g_mp4_info_t.fp);
    printf("rate = %u (0x%x)\n", ntohl(mvhd_bx.rate), ntohl(mvhd_bx.rate));
    mvhd_remain_bytes -= sizeof(int);

    // unsigned short volume
    fread(&mvhd_bx.volume, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    printf("volume = %u (0x%x)\n", ntohs(mvhd_bx.volume), ntohs(mvhd_bx.volume));
    mvhd_remain_bytes -= sizeof(unsigned short);

    // unsigned short reserved
    fread(&mvhd_bx.reserved, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    mvhd_remain_bytes -= sizeof(unsigned short);

    // unsigned int reserved_1[2];
    fread(&mvhd_bx.reserved_1[0], sizeof(unsigned int), 2, g_mp4_info_t.fp);
    mvhd_remain_bytes -= 2*sizeof(unsigned int);

    // int matrix[9]
    fread(&mvhd_bx.matrix[0], sizeof(int), 9, g_mp4_info_t.fp);
    mvhd_remain_bytes -= 9*sizeof(int);

    // unsigned int pre_defined[6]
    fread(&mvhd_bx.pre_defined[0], sizeof(unsigned int), 6, g_mp4_info_t.fp);
    mvhd_remain_bytes -= 6*sizeof(unsigned int);

    // unsigned int next_track_ID
    fread(&mvhd_bx.next_track_ID, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("next_track_ID = %u (0x%x)\n", ntohs(mvhd_bx.next_track_ID), ntohs(mvhd_bx.next_track_ID));
    mvhd_remain_bytes -= sizeof(unsigned int);
    printf("mvhd_box_remain bytes = %u\n", mvhd_remain_bytes);
    
    printf("\t=== mvhd(in moov) box end (4-1) ===\n");

    return 0;
}


static int moov_trak_tkhd_box_ana(unsigned int *trak_remain_bytes)
{
    Tkhd_Box tkhd_bx;
    unsigned int box_size;
    unsigned int tkhd_remain_bytes;
    
    printf("\t=== tkhd box start (4-2.3-1) ===\n");
    get_box_size_type(&box_size);

    *trak_remain_bytes -= box_size;

    
    tkhd_remain_bytes = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("tkhd_box_remain bytes = %u\n", tkhd_remain_bytes);

    unsigned int version_flags = 0;
    // get version & flags
    fread(&version_flags, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("version&falgs = %u (0x%x)\n", ntohl(version_flags), ntohl(version_flags));
    tkhd_remain_bytes -= sizeof(unsigned int);

    // creation time
    fread(&tkhd_bx.creation_time, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    ulong2time_show(tkhd_bx.creation_time);
    tkhd_remain_bytes -= sizeof(unsigned long);
    
    // modification time
    fread(&tkhd_bx.modification_time, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    ulong2time_show(tkhd_bx.modification_time);
    tkhd_remain_bytes -= sizeof(unsigned long);

    // track id
    fread(&tkhd_bx.track_ID, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("track_ID = %u (0x%x)\n", ntohl(tkhd_bx.track_ID), ntohl(tkhd_bx.track_ID));
    tkhd_remain_bytes -= sizeof(unsigned int);

    // reserved
    fread(&tkhd_bx.reserved, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    tkhd_remain_bytes -= sizeof(unsigned int);

    // duration
    fread(&tkhd_bx.duration, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("duration = %u (0x%x)\n", ntohl(tkhd_bx.duration), ntohl(tkhd_bx.duration));
    tkhd_remain_bytes -= sizeof(unsigned int);

    // unsigned int reserved_1[2];
    fread(&tkhd_bx.reserved_1[0], sizeof(unsigned int), 2, g_mp4_info_t.fp);
    tkhd_remain_bytes -= 2*sizeof(unsigned int);

    // unsigned short layer
    fread(&tkhd_bx.layer, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    printf("layer = %u (0x%x)\n", ntohl(tkhd_bx.layer), ntohl(tkhd_bx.layer));
    tkhd_remain_bytes -= sizeof(unsigned short);

    // unsigned short alternate_group
    fread(&tkhd_bx.alternate_group, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    printf("alternate_group = %u (0x%x)\n", ntohl(tkhd_bx.alternate_group), ntohl(tkhd_bx.alternate_group));
    tkhd_remain_bytes -= sizeof(unsigned short);

    // unsigned short volume
    fread(&tkhd_bx.volume, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    printf("volume = %u (0x%x)\n", ntohl(tkhd_bx.volume), ntohl(tkhd_bx.volume));
    tkhd_remain_bytes -= sizeof(unsigned short);

    // unsigned short reserved_2
    fread(&tkhd_bx.reserved_2, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    tkhd_remain_bytes -= sizeof(unsigned short);

    // int matrix[9]
    fread(&tkhd_bx.matrix[0], sizeof(int), 9, g_mp4_info_t.fp);
    tkhd_remain_bytes -= 9*sizeof(int);

    // unsigned int width
    fread(&tkhd_bx.width, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("width = %u (0x%x)\n", ntohl(tkhd_bx.width), ntohl(tkhd_bx.width));
    tkhd_remain_bytes -= sizeof(unsigned int);

    // unsigned int height
    fread(&tkhd_bx.height, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("height = %u (0x%x)\n", ntohl(tkhd_bx.height), ntohl(tkhd_bx.height));
    tkhd_remain_bytes -= sizeof(unsigned int);
    printf("tkhd_box_remain bytes = %u\n", tkhd_remain_bytes);
    
    printf("\t=== tkhd box end (4-2.3-1) ===\n");
    
    return 0;
}

static int moov_trak_mdia_mdhd_box_ana(unsigned int *mdia_remain_bytes)
{
    Mdhd_Box mdhd_bx;
    unsigned int box_size;
    unsigned int mdhd_remain_bytes;

    printf("\t=== mdhd box start (4-2.3-2-1) ===\n");
    
    get_box_size_type(&box_size);
    *mdia_remain_bytes -= box_size;
    
    mdhd_remain_bytes = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("mdhd_box_remain bytes = %u\n", mdhd_remain_bytes);

    // get version & flags
    unsigned int version_flags = 0;
    fread(&version_flags, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("version&falgs = %u (0x%x)\n", ntohl(version_flags), ntohl(version_flags));
    mdhd_remain_bytes -= sizeof(unsigned int);

    // creation_time
    fread(&mdhd_bx.creation_time, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    ulong2time_show(mdhd_bx.creation_time);
    mdhd_remain_bytes -= sizeof(unsigned long);
    
    // modification time
    fread(&mdhd_bx.modification_time, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    ulong2time_show(mdhd_bx.modification_time);
    mdhd_remain_bytes -= sizeof(unsigned long);

    // unsigned int timescale
    fread(&mdhd_bx.timescale, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("timescale = %u (0x%x)\n", ntohl(mdhd_bx.timescale), ntohl(mdhd_bx.timescale));
    mdhd_remain_bytes -= sizeof(unsigned int);

    // unsigned long duration
    fread(&mdhd_bx.duration, sizeof(unsigned long), 1, g_mp4_info_t.fp);
    printf("duration = %u (0x%x)\n", ntohl(mdhd_bx.duration), ntohl(mdhd_bx.duration));
    mdhd_remain_bytes -= sizeof(unsigned long);
    
    // language
    unsigned short lang;
    fread(&lang, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    printf("language = %u (0x%x)\n", ntohs(lang), ntohs(lang));
    mdhd_remain_bytes -= sizeof(unsigned short);

    // pre_defined
    fread(&mdhd_bx.pre_defined, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    mdhd_remain_bytes -= sizeof(unsigned short);

    printf("mdhd_box_remain bytes = %u\n", mdhd_remain_bytes);
    
    printf("\t=== mdhd box end (4-2.3-2-1) ===\n");
    
    return 0;
}

static int moov_trak_mdia_hdlr_box_ana(unsigned int *mdia_remain_bytes)
{
    Hdlr_Box hdlr_bx;
    unsigned int box_size;
    unsigned int hdlr_remain_bytes;

    printf("\t=== hdlr box start (4-2.3-2-2) ===\n");
    
    get_box_size_type(&box_size);
    *mdia_remain_bytes -= box_size;
    
    hdlr_remain_bytes = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("hdlr_box_remain bytes = %u\n", hdlr_remain_bytes);

    // get version & flags
    unsigned int version_flags = 0;
    fread(&version_flags, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("version&falgs = %u (0x%x)\n", ntohl(version_flags), ntohl(version_flags));
    hdlr_remain_bytes -= sizeof(unsigned int);

    // unsigned int pre_defined
    fread(&hdlr_bx.pre_defined, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    hdlr_remain_bytes -= sizeof(unsigned int);

    // unsigned int handler_type
    fread(&hdlr_bx.handler_type, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("handler_type: ");
    show_uint_char(ntohl(hdlr_bx.handler_type));
    hdlr_remain_bytes -= sizeof(unsigned int);

    // unsigned int reserved[3]
    fread(&hdlr_bx.reserved[0], sizeof(unsigned int), 3, g_mp4_info_t.fp);
    hdlr_remain_bytes -= 3*sizeof(unsigned int);

    // name
    unsigned char t_name[6];
    fread(t_name, sizeof(unsigned char), 6, g_mp4_info_t.fp);
    hdlr_remain_bytes -= 6*sizeof(unsigned char);
    printf("name: ");
    int i = 0;
    for (i = 0; i < 6; i++) {
        printf("%c", t_name[i]);
    }
    printf("\n");
    printf("hdlr_box_remain bytes = %u\n", hdlr_remain_bytes);
    
    printf("\t=== hdlr box end (4-2.3-2-2) ===\n");
    
    return 0;
}

static int moov_trak_mdia_minf_vmhd_box_ana(unsigned int *minf_remain_bytes)
{
    Vmhd_Box vmhd_bx;
    unsigned int box_size;
    unsigned int vmhd_remain_bytes;

    printf("\t=== vmhd box start (4-2-2-3-1) ===\n");
    
    get_box_size_type(&box_size);
    *minf_remain_bytes -= box_size;

    vmhd_remain_bytes = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("vmhd_box_remain bytes = %u\n", vmhd_remain_bytes);

    // get version & flags
    unsigned int version_flags = 0;
    fread(&version_flags, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("version&falgs = %u (0x%x)\n", ntohl(version_flags), ntohl(version_flags));
    vmhd_remain_bytes -= sizeof(unsigned int);

    // unsigned short graphicsmode
    fread(&vmhd_bx.graphicsmode, sizeof(unsigned short), 1, g_mp4_info_t.fp);
    printf("graphicsmode = 0x%x\n", ntohs(vmhd_bx.graphicsmode));
    vmhd_remain_bytes -= sizeof(unsigned short);

    // unsigned short opcolor[3]
    fread(&vmhd_bx.opcolor[0], sizeof(unsigned short), 3, g_mp4_info_t.fp);
    vmhd_remain_bytes -= 3*sizeof(unsigned short);

    printf("vmhd_box_remain bytes = %u\n", vmhd_remain_bytes);
    printf("\t=== vmhd box end (4-2-2-3-1) ===\n");
    
    return 0;
}

static int moov_trak_mdia_minf_smhd_box_ana(unsigned int *minf_remain_bytes)
{
    unsigned int box_size;
    unsigned int smhd_remain_bytes;

    printf("\t=== smhd box start (4-3-2-3-1) ===\n");
    
    get_box_size_type(&box_size);
    *minf_remain_bytes -= box_size;

    
    
    return 0;
}

static int moov_trak_mdia_minf_dinf_box_ana(unsigned int *minf_remain_bytes)
{
    unsigned int box_size;
    unsigned int dinf_remain_bytes;
    unsigned int dref_box_size;
    unsigned int url_box_size;

    printf("\t=== dinf box start (4-2.3-2-3-2) ===\n");

    get_box_size_type(&box_size);
    *minf_remain_bytes -= box_size;

    dinf_remain_bytes = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("dinf_box_remain bytes = %u\n", dinf_remain_bytes);

    get_box_size_type(&dref_box_size);
    dinf_remain_bytes -= 2*sizeof(unsigned int);

    // pad
    unsigned int pad;
    fread(&pad, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    dinf_remain_bytes -= sizeof(unsigned int);

    // entry_count
    unsigned int entry_count;
    fread(&entry_count, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("entry_count = 0x%x\n", ntohl(entry_count));
    dinf_remain_bytes -= sizeof(unsigned int);

    // urlbox
    get_box_size_type(&url_box_size);
    dinf_remain_bytes -= 2*sizeof(unsigned int);

    // dummy
    unsigned int dummy;
    fread(&dummy, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    printf("dummy = 0x%x\n", ntohl(dummy));
    dinf_remain_bytes -= sizeof(unsigned int);
    printf("dinf_box_remain bytes = %u\n", dinf_remain_bytes);

    printf("\t=== dinf box end (4-2.3-2-3-2) ===\n");
    
    return 0;
}

static int moov_trak_mdia_minf_stbl_box_ana(unsigned int *minf_remain_bytes)
{
    unsigned int box_size;
    unsigned int stbl_remain_bytes;

    printf("\t=== minf box start (4-2.3-2-3-3) ===\n");

    
    return 0;
}


static int moov_trak_mdia_minf_box_ana(video_audio_e type, unsigned int *mdia_remain_bytes)
{
    unsigned int box_size;
    unsigned int minf_remain_bytes;

    printf("\t=== minf box start (4-2.3-2-3) ===\n");
    
    get_box_size_type(&box_size);
    *mdia_remain_bytes -= box_size;

    minf_remain_bytes = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("minf_box_remain bytes = %u\n", minf_remain_bytes);

    if (type == TYPE_VIDEO) {
        moov_trak_mdia_minf_vmhd_box_ana(&minf_remain_bytes);
    }
    else {
        moov_trak_mdia_minf_smhd_box_ana(&minf_remain_bytes);
    }
    printf("minf_box_remain bytes = %u\n", minf_remain_bytes);

    moov_trak_mdia_minf_dinf_box_ana(&minf_remain_bytes);
    printf("minf_box_remain bytes = %u\n", minf_remain_bytes);

    printf("\t=== minf box end (4-2.3-2-3) ===\n");
    
    return 0;
}


static int moov_trak_mdia_box_ana(video_audio_e type, unsigned int *trak_remain_bytes)
{
    unsigned int box_size;
    unsigned int mdia_remain_bytes;
    
    printf("\t=== mdia box start (4-2.3-2) ===\n");
    get_box_size_type(&box_size);

    *trak_remain_bytes -= box_size;
    
    mdia_remain_bytes = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("mdia_box_remain bytes = %u\n", mdia_remain_bytes);

    moov_trak_mdia_mdhd_box_ana(&mdia_remain_bytes);
    printf("mdia_box_remain bytes = %u\n", mdia_remain_bytes);

    moov_trak_mdia_hdlr_box_ana(&mdia_remain_bytes);
    printf("mdia_box_remain bytes = %u\n", mdia_remain_bytes);

    if (type == TYPE_VIDEO) {
        moov_trak_mdia_minf_box_ana(TYPE_VIDEO, &mdia_remain_bytes);
    }
    else {
        moov_trak_mdia_minf_box_ana(TYPE_AUDIO, &mdia_remain_bytes);
    }
    printf("mdia_box_remain bytes = %u\n", mdia_remain_bytes);

    printf("\t=== mdia box end (4-2.3-2) ===\n");

    return 0;
}

// 4-2
static int moov_trak_video_box_ana(unsigned int *moov_remain_bytes)
{
    unsigned int moov_trak_video_remain_bytes;
    unsigned int box_size;
    
    printf("\t=== trak video box(in moov)start (4-2) ===\n");
    get_box_size_type(&box_size);

    *moov_remain_bytes -= box_size;
    
    moov_trak_video_remain_bytes = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("moov_trak_video_remain_bytes = %u\n", moov_trak_video_remain_bytes);

    moov_trak_tkhd_box_ana(&moov_trak_video_remain_bytes);
    printf("moov_trak_video_remain_bytes = %u\n", moov_trak_video_remain_bytes);

    moov_trak_mdia_box_ana(TYPE_VIDEO, &moov_trak_video_remain_bytes);
    printf("moov_trak_video_remain_bytes = %u\n", moov_trak_video_remain_bytes);

    printf("\t=== trak video box(in moov)end (4-2) ===\n");
    
    return 0;
}

// 4-3
static int moov_trak_audio_box_ana(unsigned int *moov_remain_bytes)
{
    printf("\t=== trak audio box(in moov)start (4-3) ===\n");

    printf("\t=== trak audio box(in moov)end (4-3) ===\n");
    
    return 0;
}

// 4
static int analysis_moov_box()
{
    unsigned int box_size = 0;
    unsigned int remain_size = 0;

    printf("\t=== moov box start (4) ===\n");

    get_box_size_type(&box_size);
    remain_size = box_size - sizeof(unsigned int) - sizeof(unsigned int);
    printf("moov_box_remain bytes = %u\n", remain_size);

    moov_mvhd_box_ana(&remain_size);
    printf("moov_box_remain bytes = %u\n", remain_size);

    moov_trak_video_box_ana(&remain_size);
    printf("moov_box_remain bytes = %u\n", remain_size);

    printf("\t=== moov box end (4) ===\n");

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
