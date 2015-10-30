#include "FileIo.h"

FILE *pVideo_H264_File = NULL;
FILE *pAudio_Aac_File = NULL;
FILE *pVideo_Audio_Mp4_File = NULL;

FILE *OpenFile(const char *FileName, const char *OpenMode)
{
    if (FileName == NULL || OpenMode == NULL)
        return NULL;
    
    FILE *pFile = NULL;
    
    pFile = fopen(FileName, OpenMode);
    if (NULL == pFile) {
        printf("open file failed %s\n", FileName);
    }

    return pFile;
}

void CloseFile(FILE *pFile)
{
    fclose(pFile);
}

int ReadFile(FILE *pFile, unsigned char *Buffer, int BufferSize)
{
    return fread(Buffer, 1, BufferSize, pFile);
}

int WriteFile(FILE *pFile, char *Buffer, int BufferSize)
{
    int WriteSize = 0;

    WriteSize = fwrite(Buffer, 1, BufferSize, pFile);

    return WriteSize;
}
