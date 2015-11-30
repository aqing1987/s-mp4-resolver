#ifndef FILEIO_H_
#define FILEIO_H_

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

#define MAXH264FRAMESIZE     1024 *100
#define MAXAACFRAMESIZE      1024 *10

#define INPUTH264FILENAME    "xx.264"
#define INPUTAACFILENAME     "xx.aac"
#define OUTPUTMP4FILENAME    "xx.mp4"

extern FILE *pVideo_H264_File;
extern FILE *pAudio_Aac_File;
extern FILE *pVideo_Audio_Mp4_File;

FILE *OpenFile(const char *FileName, const char *OpenMode);
void CloseFile(FILE *pFile);
int ReadFile(FILE *pFile, unsigned char *Buffer, int BufferSize);
int WriteFile(FILE *pFile, char *Buffer, int BufferSize);

#endif // FILEIO_H_
