#include "Stsc_Box.h"

int AllocStruct_Stsc_Box(Stsc_Box ** stscbox)
{
    Stsc_Box * stscbox_t = *stscbox;
    
    if ((stscbox_t = (Stsc_Box *) calloc(1, sizeof(Stsc_Box))) == NULL) {
        printf("Error: Allocate Meory To AllocStruct_Stsc_Box Buffer Failed\n");
        return -1;
    }
    
    if (AllocStruct_FullBox_Box_Header(&(stscbox_t->boxheader))) {
        if ((stscbox_t->first_chunk
             = (unsigned int *) calloc(1, sizeof(unsigned int))) == NULL) {
            printf("Error: Allocate Meory To stscbox_t->first_chunk  Buffer Failed\n");
            return -1;
        }
        if ((stscbox_t->samples_per_chunk
             = (unsigned int *) calloc(1, sizeof(unsigned int))) == NULL) {
            printf("Error: Allocate Meory To stscbox_t->samples_per_chunk  Buffer Failed\n");
            return -1;
        }
        if ((stscbox_t->sample_description_index
             = (unsigned int *) calloc(1, sizeof(unsigned int))) == NULL) {
            printf("Error: Allocate Meory To stscbox_t->sample_description_index Buffer Failed\n");
            return -1;
        }
    }
    *stscbox = stscbox_t;
    
    return 1;
}

int FreeStruct_Stsc_Box(Stsc_Box * stscbox)
{
    if (stscbox) {
        if (FreeStruct_FullBox_Box_Header(stscbox->boxheader)) {
            if (stscbox->first_chunk) {
                free(stscbox->first_chunk);
                stscbox->first_chunk = NULL;
            }
            if (stscbox->samples_per_chunk) {
                free(stscbox->samples_per_chunk);
                stscbox->samples_per_chunk = NULL;
            }
            if (stscbox->sample_description_index) {
                free(stscbox->sample_description_index);
                stscbox->sample_description_index = NULL;
            }
        }
        free(stscbox);
        stscbox = NULL;
    }
    
    return 1;
}

int Write_Stsc_Stsc_Box_Video(unsigned char * Buf,
                              unsigned int entry_count_stsc_video,
                              unsigned int * first_chunk_stsc_video,
                              unsigned int * samples_per_chunk_stsc_video,
                              unsigned int * sample_description_index_stsc_video)
{
    Stsc_Box * stscbox = NULL;
    unsigned int stscbox_pos = 0;
    unsigned int i = 0;
    
    AllocStruct_Stsc_Box(&stscbox);
    //////////////////////////////////////////////////////////////////////////

    stscbox->entry_count = entry_count_stsc_video;

    stscbox_pos += 4;
    // boxtype 0x73 74 73 63 stsc
    Buf[stscbox_pos] = 0x73;
    Buf[stscbox_pos + 1] = 0x74;
    Buf[stscbox_pos + 2] = 0x73;
    Buf[stscbox_pos + 3] = 0x63;
    stscbox_pos += 4;
    // version,flag
    stscbox_pos += 4;
    // entry_count
    Buf[stscbox_pos] = stscbox->entry_count >> 24;
    Buf[stscbox_pos + 1] = (stscbox->entry_count >> 16) & 0xFF;
    Buf[stscbox_pos + 2] = (stscbox->entry_count >> 8) & 0xFF;
    Buf[stscbox_pos + 3] = (stscbox->entry_count) & 0xFF;
    stscbox_pos += 4;
    // first_chunk, samples_per_chunk, sample_description_index
    for ( i = 0; i < entry_count_stsc_video; i++) {
        // first_chunk
        Buf[stscbox_pos] = first_chunk_stsc_video[i] >> 24;
        Buf[stscbox_pos + 1] = (first_chunk_stsc_video[i] >> 16) & 0xFF;
        Buf[stscbox_pos + 2] = (first_chunk_stsc_video[i] >> 8) & 0xFF;
        Buf[stscbox_pos + 3] = (first_chunk_stsc_video[i]) & 0xFF;
        stscbox_pos += 4;
        // samples_per_chunk
        Buf[stscbox_pos] = samples_per_chunk_stsc_video[i] >> 24;
        Buf[stscbox_pos + 1] = (samples_per_chunk_stsc_video[i] >> 16) & 0xFF;
        Buf[stscbox_pos + 2] = (samples_per_chunk_stsc_video[i] >> 8) & 0xFF;
        Buf[stscbox_pos + 3] = (samples_per_chunk_stsc_video[i]) & 0xFF;
        stscbox_pos += 4;
        // sample_description_index
        Buf[stscbox_pos] = sample_description_index_stsc_video[i] >> 24;
        Buf[stscbox_pos + 1] = (sample_description_index_stsc_video[i] >> 16) & 0xFF;
        Buf[stscbox_pos + 2] = (sample_description_index_stsc_video[i] >> 8) & 0xFF;
        Buf[stscbox_pos + 3] = (sample_description_index_stsc_video[i]) & 0xFF;
        stscbox_pos += 4;
    }

    Write_FullBox_Header(Buf, stscbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stsc_Box(stscbox);
    
    return stscbox_pos;
}

int Write_Stsc_Stsc_Box_Audio(unsigned char * Buf,
                              unsigned int entry_count_stsc_audio,
                              unsigned int * first_chunk_stsc_audio,
                              unsigned int * samples_per_chunk_stsc_audio,
                              unsigned int * sample_description_index_stsc_audio)
{
    Stsc_Box * stscbox = NULL;
    unsigned int stscbox_pos = 0;
    unsigned int i = 0;
    
    AllocStruct_Stsc_Box(&stscbox);
    //////////////////////////////////////////////////////////////////////////

    stscbox->entry_count = entry_count_stsc_audio;

    stscbox_pos += 4;
    // boxtype 0x73 74 73 63 stsc
    Buf[stscbox_pos] = 0x73;
    Buf[stscbox_pos + 1] = 0x74;
    Buf[stscbox_pos + 2] = 0x73;
    Buf[stscbox_pos + 3] = 0x63;
    stscbox_pos += 4;
    // version,flag
    stscbox_pos += 4;
    // entry_count
    Buf[stscbox_pos] = stscbox->entry_count >> 24;
    Buf[stscbox_pos + 1] = (stscbox->entry_count >> 16) & 0xFF;
    Buf[stscbox_pos + 2] = (stscbox->entry_count >> 8) & 0xFF;
    Buf[stscbox_pos + 3] = (stscbox->entry_count) & 0xFF;
    stscbox_pos += 4;
    // first_chunk, samples_per_chunk, sample_description_index
    for (i = 0; i < entry_count_stsc_audio; i++) {
        // first_chunk
        Buf[stscbox_pos] = first_chunk_stsc_audio[i] >> 24;
        Buf[stscbox_pos + 1] = (first_chunk_stsc_audio[i] >> 16) & 0xFF;
        Buf[stscbox_pos + 2] = (first_chunk_stsc_audio[i] >> 8) & 0xFF;
        Buf[stscbox_pos + 3] = (first_chunk_stsc_audio[i]) & 0xFF;
        stscbox_pos += 4;
        // samples_per_chunk
        Buf[stscbox_pos] = samples_per_chunk_stsc_audio[i] >> 24;
        Buf[stscbox_pos + 1] = (samples_per_chunk_stsc_audio[i] >> 16) & 0xFF;
        Buf[stscbox_pos + 2] = (samples_per_chunk_stsc_audio[i] >> 8) & 0xFF;
        Buf[stscbox_pos + 3] = (samples_per_chunk_stsc_audio[i]) & 0xFF;
        stscbox_pos += 4;
        // sample_description_index
        Buf[stscbox_pos] = sample_description_index_stsc_audio[i] >> 24;
        Buf[stscbox_pos + 1] = (sample_description_index_stsc_audio[i] >> 16) & 0xFF;
        Buf[stscbox_pos + 2] = (sample_description_index_stsc_audio[i] >> 8) & 0xFF;
        Buf[stscbox_pos + 3] = (sample_description_index_stsc_audio[i]) & 0xFF;
        stscbox_pos += 4;
    }

    Write_FullBox_Header(Buf, stscbox_pos, 0x00, 0x00);
    //////////////////////////////////////////////////////////////////////////
    FreeStruct_Stsc_Box(stscbox);
    
    return stscbox_pos;
}
