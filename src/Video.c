#include "Video.h"

unsigned int decode_video_done = 0;
unsigned int Is_KyeFrame = 0;

NALU_t *AllocNALU(int buffersize)
{
    NALU_t *n;

    if ((n = (NALU_t*) calloc(1, sizeof(NALU_t))) == NULL) {
        printf("AllocNALU Error: Allocate Meory To NALU_t Failed\n");
        return NULL;
    }

    n->max_size = buffersize; //Assign buffer size

    if ((n->buf = (unsigned char*) calloc(buffersize, sizeof(char))) == NULL) {
        free(n);
        printf("AllocNALU Error: Allocate Meory To NALU_t Buffer Failed\n");
        return NULL;
    }
    return n;
}

void FreeNALU(NALU_t *n)
{
    if (n) {
        if (n->buf) {
            free(n->buf);
            n->buf = NULL;
        }
        free(n);
    }
}

int FindStartCode2(unsigned char *Buf)
{
    //Check whether buf is 0x000001
    if (Buf[0] != 0 || Buf[1] != 0 || Buf[2] != 1) {
        return 0;
    }
    else {
        return 1;
    }
}

int FindStartCode3(unsigned char *Buf)
{
    //Check whether buf is 0x00000001
    if (Buf[0] != 0 || Buf[1] != 0 || Buf[2] != 0 || Buf[3] != 1) {
        return 0;
    }
    else {
        return 1;
    }
}

int GetAnnexbNALU(NALU_t * nalu)
{
    int pos = 0; //
    int StartCodeFound = 0; //
    int rewind = 0; //
    unsigned char * Buf = NULL;
    static int info2 = 0;
    static int info3 = 0;

    if ((Buf = (unsigned char*) calloc(nalu->max_size, sizeof(char))) == NULL) {
        printf("GetAnnexbNALU Error: Could not allocate Buf memory\n");
    }

    nalu->startcodeprefix_len = 3; //

    if (3 != fread(Buf, 1, 3, pVideo_H264_File)) {
        free(Buf);
        return 0;
    }
    info2 = FindStartCode2(Buf); //Check whether Buf is 0x000001
    if (info2 != 1) {
        //If Buf is not 0x000001,then read one more byte
        if (1 != fread(Buf + 3, 1, 1, pVideo_H264_File)) {
            free(Buf);
            return 0;
        }
        info3 = FindStartCode3(Buf); //Check whether Buf is 0x00000001
        //If not the return -1
        if (info3 != 1) {
            free(Buf);
            return -1;
        }
        else {
            //If Buf is 0x00000001,set the prefix length to 4 bytes
            pos = 4;
            nalu->startcodeprefix_len = 4;
        }
    }
    else {
        //If Buf is 0x000001,set the prefix length to 3 bytes
        pos = 3;
        nalu->startcodeprefix_len = 3;
    }
    
    StartCodeFound = 0;
    info2 = 0;
    info3 = 0;
    while (!StartCodeFound) {
        if (feof(pVideo_H264_File)) {
            nalu->len = (pos - 1) - nalu->startcodeprefix_len; //
            memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
            nalu->forbidden_bit = nalu->buf[0] & 0x80; // 1 bit--10000000
            nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit--01100000
            nalu->nal_unit_type = (nalu->buf[0]) & 0x1f; // 5 bit--00011111
            free(Buf);
            return ((info3 == 1) ? 4 : 3);
        }
        Buf[pos++] = fgetc(pVideo_H264_File); //Read one char to the Buffer
        info3 = FindStartCode3(&Buf[pos - 4]); //Check whether Buf is 0x00000001
        if (info3 != 1) {
            info2 = FindStartCode2(&Buf[pos - 3]); //Check whether Buf is 0x000001
        }
        StartCodeFound = (info2 == 1 || info3 == 1); //
    }

    rewind = (info3 == 1) ? -4 : -3;

    if (0 != fseek(pVideo_H264_File, rewind, SEEK_CUR)) {
        free(Buf);
        printf("GetAnnexbNALU Error: Cannot fseek in the bit stream file");
    }

    nalu->len = (pos + rewind) - nalu->startcodeprefix_len; //
    memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len); //
    nalu->forbidden_bit = nalu->buf[0] & 0x80; //
    nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
    nalu->nal_unit_type = (nalu->buf[0]) & 0x1f; // 5 bit
    free(Buf);
    
    return ((info3 == 1) ? 4 : 3);
}

int GetFrameType(NALU_t * nal)
{
    bs_t s;
    int frame_type = 0;
    unsigned char * OneFrameBuf_H264 = NULL;
    
    if ((OneFrameBuf_H264
         = (unsigned char *) calloc(nal->len + 4, sizeof(unsigned char))) == NULL) {
        printf("Error malloc OneFrameBuf_H264\n");
        return -1;
    }
    
    if (nal->startcodeprefix_len == 3) {
        OneFrameBuf_H264[0] = 0x00;
        OneFrameBuf_H264[1] = 0x00;
        OneFrameBuf_H264[2] = 0x01;
        memcpy(OneFrameBuf_H264 + 3, nal->buf, nal->len);
    }
    else if (nal->startcodeprefix_len == 4) {
        OneFrameBuf_H264[0] = 0x00;
        OneFrameBuf_H264[1] = 0x00;
        OneFrameBuf_H264[2] = 0x00;
        OneFrameBuf_H264[3] = 0x01;
        memcpy(OneFrameBuf_H264 + 4, nal->buf, nal->len);
    }
    else {
        printf("H264 read err！\n");
    }
    
    bs_init(&s, OneFrameBuf_H264 + nal->startcodeprefix_len + 1, nal->len - 1);

    if (nal->nal_unit_type == NAL_SLICE || nal->nal_unit_type == NAL_SLICE_IDR) {
        // i_first_mb
        bs_read_ue(&s);

        // picture type
        frame_type = bs_read_ue(&s);
        switch (frame_type) {
        case 0:
        case 5: // P
            nal->Frametype = FRAME_P;
            break;
        case 1:
        case 6: // B
            nal->Frametype = FRAME_B;
            break;
        case 3:
        case 8: // SP
            nal->Frametype = FRAME_P;
            break;
        case 2:
        case 7: // I
            nal->Frametype = FRAME_I;
            break;
        case 4:
        case 9: // SI
            nal->Frametype = FRAME_I;
            break;
        }
    }
    else if (nal->nal_unit_type == NAL_SEI) {
        nal->Frametype = NAL_SEI;
    }
    else if (nal->nal_unit_type == NAL_SPS) {
        nal->Frametype = NAL_SPS;
    }
    else if (nal->nal_unit_type == NAL_PPS) {
        nal->Frametype = NAL_PPS;
    }
    if (OneFrameBuf_H264) {
        free(OneFrameBuf_H264);
        OneFrameBuf_H264 = NULL;
    }
    
    return 1;
}

int GainSpsPps(unsigned char * spsbuf, unsigned int * spslength,
               unsigned char * ppsbuf, unsigned int * ppslength)
{
    NALU_t * n_1 = NULL;
    NALU_t * n_2 = NULL;

    n_1 = AllocNALU(MAX_VIDEO_TAG_BUF_SIZE);
    n_2 = AllocNALU(MAX_VIDEO_TAG_BUF_SIZE);
    
  loop_1_1:
    GetAnnexbNALU(n_1);
    GetFrameType(n_1);
    if (n_1->nal_unit_type == NAL_SPS) {
      loop_1_2:
        GetAnnexbNALU(n_2);
        GetFrameType(n_2);
        if (n_2->nal_unit_type == NAL_PPS) {
            memcpy(spsbuf, n_1->buf, n_1->len);
            *spslength = n_1->len;
            memcpy(ppsbuf, n_2->buf, n_2->len);
            *ppslength = n_2->len;
        }
        else {
            goto loop_1_2;
        }
    }
    else {
        goto loop_1_1;
    }
    
    if (fseek(pVideo_H264_File, 0, 0) < 0) {
        printf("fseek : pVideo_H264_File Error\n");
        return -1;
    }
    FreeNALU(n_1);
    FreeNALU(n_2);
    
    return 1;
}

int Read_One_H264_Frame(unsigned char * buf)
{
    NALU_t * n = NULL;
    unsigned int video_buf_size = 0;
    int startcodeprefix_size = 0;
    Is_KyeFrame = 0;
    n = AllocNALU(MAX_VIDEO_TAG_BUF_SIZE);

  loop_2:
    startcodeprefix_size = GetAnnexbNALU(n);
    if (startcodeprefix_size == 0) {
        decode_video_done = 1;
        return 0;
    }
    GetFrameType(n);

    if (n->Frametype == FRAME_I) {

        buf[0] = n->len >> 24;
        buf[1] = (n->len >> 16) & 0xFF;
        buf[2] = (n->len >> 8) & 0xFF;
        buf[3] = n->len & 0xFF;
        memcpy(buf + 4, n->buf, n->len);
        video_buf_size = n->len + 4;
        Is_KyeFrame = 1;
    }
    else if (n->Frametype == FRAME_B || n->Frametype == FRAME_P) {
        buf[0] = n->len >> 24;
        buf[1] = (n->len >> 16) & 0xFF;
        buf[2] = (n->len >> 8) & 0xFF;
        buf[3] = n->len & 0xFF;
        memcpy(buf + 4, n->buf, n->len);
        video_buf_size = n->len + 4;
    }
    else
    {
        goto loop_2;
    }

    FreeNALU(n);
    
    return video_buf_size;
}
