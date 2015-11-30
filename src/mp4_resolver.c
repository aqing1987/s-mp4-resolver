/**
 * @brief
 *    all data in a QuickTime movie is stored in big-endian byte ordering, also known as
 *    network byte ordering, in which the most significant bytes are stored and
 *    transmitted first
 * */
#include <stdio.h>
#include <arpa/inet.h>

#include "Mp4.h"
#include "Ftyp_Box.h"

#define FILENAME_LEN 128

typedef struct {
    char file_name[FILENAME_LEN];
    FILE *fp;

} mp4_info_st;

mp4_info_st g_mp4_info_t;

void show_uint_char(unsigned int val)
{
    unsigned char buf[4] = "";

    memcpy(buf, &val, 4);

    int i = 0; 
    for (i = 3; i >= 0; i--) {
        printf("%c", buf[i]);
    }
    printf("\n");
}

// 1
int analysis_ftyp_box()
{
    Box_Header ftype_header;
    Ftyp_Box ftype_content;
    unsigned long box_size = 0;
    unsigned long remain_size = 0;
    int large_flag = 0;

    printf("\t=== ftyp start ===\n");
    
    memset(&ftype_header, 0, sizeof(Box_Header));
    memset(&ftype_content, 0, sizeof(ftype_content));
    
    // atom size
    fread(&ftype_header.size, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    ftype_header.size = ntohl(ftype_header.size);
    if (ftype_header.size == 1) {
        large_flag = 1;
    }
    else {
        printf("size = %u\n", ftype_header.size);
        box_size = ftype_header.size;
        remain_size = box_size - sizeof(ftype_header.size) - sizeof(ftype_header.type);
    }

    // type
    fread(&ftype_header.type, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    ftype_header.type = ntohl(ftype_header.type);
    printf("type = 0x%x ", ftype_header.type);
    show_uint_char(ftype_header.type);
    if (ftype_header.type != 0x66747970) {
        printf("type err\n");
        printf("\t=== ftyp end ===\n");
        return -1;
    }

    if (large_flag == 1) {
        // largesize
        fread(&ftype_header.largesize, sizeof(unsigned long), 1, g_mp4_info_t.fp);
        ftype_header.largesize = ntohl(ftype_header.largesize);
        printf("largesize = %lu\n", ftype_header.largesize);
        box_size = ftype_header.largesize;
        remain_size = box_size - sizeof(ftype_header.size)
            - sizeof(ftype_header.type) - sizeof(ftype_header.largesize);
    }

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
    
    printf("\t=== ftyp end ===\n");

    return 0;
}

// 2
int analysis_free_box()
{
    Box_Header free_header;
    Ftyp_Box free_content;
    unsigned long box_size = 0;
    unsigned long remain_size = 0;
    int large_flag = 0;

    printf("\t=== free box start ===\n");
    
    memset(&free_header, 0, sizeof(Box_Header));
    memset(&free_content, 0, sizeof(free_content));
    
    // atom size
    fread(&free_header.size, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    free_header.size = ntohl(free_header.size);
    if (free_header.size == 1) {
        large_flag = 1;
    }
    else {
        printf("free box size = %u\n", free_header.size);
        box_size = free_header.size;
        remain_size = box_size - sizeof(free_header.size) - sizeof(free_header.type);
    }

    // type
    fread(&free_header.type, sizeof(unsigned int), 1, g_mp4_info_t.fp);
    free_header.type = ntohl(free_header.type);
    printf("type = 0x%x ", free_header.type);
    show_uint_char(free_header.type);
    if (free_header.type != 0x66726565) {
        printf("type err\n");
        printf("\t=== free box end ===\n");
        return -1;
    }

    // largesize
    if (large_flag == 1) {
        fread(&free_header.largesize, sizeof(unsigned long), 1, g_mp4_info_t.fp);
        free_header.largesize = ntohl(free_header.largesize);
        printf("largesize = %lu\n", free_header.largesize);
        box_size = free_header.largesize;
        remain_size = box_size - sizeof(free_header.size)
            - sizeof(free_header.type) - sizeof(free_header.largesize);
    }

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

    printf("\t=== free box end ===\n");
    
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

    fclose(g_mp4_info_t.fp);
    
    return 0;
}
