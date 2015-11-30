#ifndef MYBS_H_
#define MYBS_H_

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

#define MAXH264FRAMESIZE 1024 *100
#define MAXAACFRAMESIZE  1024 *10


typedef struct Tag_bs_t {
    unsigned char *p_start; //
    unsigned char *p; //
    unsigned char *p_end; //
    int i_left; //
} bs_t;

void bs_init(bs_t *s, void *p_data, int i_data);
int bs_read(bs_t *s, int i_count);
int bs_read1(bs_t *s);
int bs_read_ue(bs_t *s);

#endif
