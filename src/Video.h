#ifndef VIDEO_H_
#define VIDEO_H_

#include "Mp4.h"
#include "Mybs.h"

#define  MAX_VIDEO_TAG_BUF_SIZE   1024 * 1024
#define  VIDEO_TAG_HEADER_LENGTH  11

extern unsigned int decode_video_done;
extern unsigned int Is_KyeFrame; //

typedef struct Tag_NALU_t {
    unsigned char forbidden_bit; //! Should always be FALSE
    unsigned char nal_reference_idc; //! NALU_PRIORITY_xxxx
    unsigned char nal_unit_type; //! NALU_TYPE_xxxx
    unsigned int startcodeprefix_len; //
    unsigned int len; //
    unsigned int max_size; //
    unsigned char * buf; //
    unsigned char Frametype; //
    unsigned int lost_packets; //
} NALU_t;

// nal type
enum nal_unit_type_e {
    NAL_UNKNOWN = 0,
    NAL_SLICE = 1,
    NAL_SLICE_DPA = 2,
    NAL_SLICE_DPB = 3,
    NAL_SLICE_DPC = 4,
    NAL_SLICE_IDR = 5, // ref_idc != 0
    NAL_SEI = 6,       // ref_idc == 0
    NAL_SPS = 7,
    NAL_PPS = 8
// ref_idc == 0 for 6,9,10,11,12
};

enum Frametype_e {
    FRAME_I = 15,
    FRAME_P = 16,
    FRAME_B = 17
};

NALU_t *AllocNALU(int buffersize); //
void FreeNALU(NALU_t * n); //
int FindStartCode2(unsigned char *Buf); //
int FindStartCode3(unsigned char *Buf); //
int GetAnnexbNALU(NALU_t *nalu); //
int GetFrameType(NALU_t * n); //
int GainSpsPps(unsigned char * spsbuf, unsigned int *spslength,
               unsigned char * ppsbuf, unsigned int *ppslength); //
int Read_One_H264_Frame(unsigned char * buf);

#endif
