#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 44100Hz, 8bit のWAVデータ
#define SMPL 44100
#define BIT  8

void wav_write(const char *filename, unsigned char *buffer, size_t size)
{
    size_t filesize;
    unsigned char head[44];
    
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) return;
    
    /* RIFFヘッダ (8バイト) */
    memcpy(head, "RIFF", 4);
    filesize = sizeof(head) + size;
    head[4] = (filesize - 8) >> 0  & 0xff;
    head[5] = (filesize - 8) >> 8  & 0xff;
    head[6] = (filesize - 8) >> 16 & 0xff;
    head[7] = (filesize - 8) >> 24 & 0xff;
    
    /* WAVEヘッダ (4バイト) */
    memcpy(head + 8, "WAVE", 4);
    
    /* fmtチャンク (24バイト) */
    memcpy(head + 12, "fmt ", 4);
    head[16] = 16;
    head[17] = 0;
    head[18] = 0;
    head[19] = 0;
    head[20] = 1;
    head[21] = 0;
    head[22] = 1;
    head[23] = 0;
    head[24] = SMPL >> 0  & 0xff;
    head[25] = SMPL >> 8  & 0xff;
    head[26] = SMPL >> 16 & 0xff;
    head[27] = SMPL >> 24 & 0xff;
    head[28] = (SMPL * (BIT / 8)) >> 0  & 0xff;
    head[29] = (SMPL * (BIT / 8)) >> 8  & 0xff;
    head[30] = (SMPL * (BIT / 8)) >> 16 & 0xff;
    head[31] = (SMPL * (BIT / 8)) >> 24 & 0xff;
    head[32] = (BIT / 8) >> 0 & 0xff;
    head[33] = (BIT / 8) >> 8 & 0xff;
    head[34] = BIT >> 0 & 0xff;
    head[35] = BIT >> 8 & 0xff;
    
    /* dataチャンク (8 + size バイト) */
    memcpy(head + 36, "data", 4);
    head[40] = size >> 0  & 0xff;
    head[41] = size >> 8  & 0xff;
    head[42] = size >> 16 & 0xff;
    head[43] = size >> 24 & 0xff;
    
    /* 書き出し */
    fwrite(head, sizeof(head), 1, fp);
    fwrite(buffer, size, 1, fp);
    fclose(fp);
}

int main(void)
{
    size_t size = 1 * (SMPL * (BIT/8));    /* 1秒間 */
    unsigned char *buf = (char *) malloc(size);
    
    memset(buf, 0x80, size); //出力音声を0x80(10進数で128)で埋め尽くす→128は8bit量子化データで振幅0の意→音声が無音になる
    
    wav_write("muon.wav", buf, size);
    
    free(buf);
    return 0;
}
