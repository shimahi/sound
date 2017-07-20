#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include<math.h>
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
    head[24] = SMPL >> 0  & 0xff;  //整数SMPLを8bitに分解して配列に入れてる
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
    size_t size = 20 * (SMPL * (BIT/8));    /* 1秒間 */
    unsigned char *buf = (char *) malloc(size);
    
    int i;
    double sig, p = 0; //pはphase,初期値0
    
    //---------正弦波によるカエルの歌-----------------------------------------------------
    for (i = 0; i < size/32; i++) {
        p += 261.63 / SMPL;
        p -= floor(p);         sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = size/32; i < 2*size/32; i++) {
        p += 293.66 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 2*size/32; i < 3*size/22; i++) {
        p += 329.62 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 3*size/32; i < 4*size/32; i++) {
        p += 349.22 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 4*size/32; i < 5*size/32; i++) {
        p += 329.62 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 5*size/32; i < 6*size/32; i++) {
        p += 293.66 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 6*size/32; i < 8*size/32; i++) {
        p += 261.63 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    //
    for (i = 8*size/32; i < 9*size/22; i++) {
        p += 329.62 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 9*size/32; i < 10*size/32; i++) {
        p += 349.22 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 10*size/32; i < 11*size/22; i++) {
        p += 391.99 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 11*size/32; i < 12*size/32; i++) {
        p += 440.00 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 12*size/32; i < 13*size/32; i++) {
        p += 391.99 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 13*size/32; i < 14*size/32; i++) {
        p += 349.22 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 14*size/32; i < 31*size/64; i++) {
        p += 329.62 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 31*size/64; i < 16*size/32; i++) {
        sig = 0;
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    //
    for (i = 16*size/32; i < 17*size/32; i++) {
        p += 261.63 / SMPL;
        p -= floor(p);         sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 17*size/32; i < 18*size/32; i++) {
        sig = 0;
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 18*size/32; i < 19*size/32; i++) {
        p += 261.63 / SMPL;
        p -= floor(p);         sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 19*size/32; i < 20*size/32; i++) {
        sig = 0;
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 20*size/32; i < 21*size/32; i++) {
        p += 261.63 / SMPL;
        p -= floor(p);         sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 21*size/32; i < 22*size/32; i++) {
        sig = 0;
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 22*size/32; i < 23*size/32; i++) {
        p += 261.63 / SMPL;
        p -= floor(p);         sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 23*size/32; i < 24*size/32; i++) {
        sig = 0;
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    //
    for (i = 24*size/32; i < 25*size/32; i++) {
        p += 261.63 / SMPL;
        p -= floor(p);         sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 25*size/32; i < 26*size/32; i++) {
        p += 293.66 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 26*size/32; i < 27*size/32; i++) {
        p += 329.62 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 27*size/32; i < 28*size/32; i++) {
        p += 349.22 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 28*size/32; i < 29*size/32; i++) {
        p += 329.62 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 29*size/32; i < 30*size/32; i++) {
        p += 293.66 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }
    for (i = 30*size/32; i < size; i++) {
        p += 261.63 / SMPL;
        p -= floor(p);
        sig = sin(2.0*M_PI*p);
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    //--------------------------------------------------------------
    wav_write("frog4.wav", buf, size);
    
    free(buf);
    return 0;
}
