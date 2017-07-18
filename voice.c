#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SMPL    44100

typedef enum {
    VOWEL_A,
    VOWEL_I,
    VOWEL_U,
    VOWEL_E,
    VOWEL_O
} VowelType;

#define MAX(a,b)    (((a)<(b))?(b):(a))
#define MIN(a,b)    (((a)<(b))?(a):(b))

void WriteWavFile(const char *filename, char *buf, int size)
{
    /* WAVファイルへ書き出し */
    int filesize = 44 + size;
    FILE *fp;
    char *work;
    
    fp = fopen(filename, "wb");
    if (fp == NULL) { return; }
    work = (char *) malloc(filesize);
    if (work == NULL) { return; }
    
    memcpy(work, "RIFF", 4);
    work[4] = (filesize - 8) >> 0  & 0xff;
    work[5] = (filesize - 8) >> 8  & 0xff;
    work[6] = (filesize - 8) >> 16 & 0xff;
    work[7] = (filesize - 8) >> 24 & 0xff;
    memcpy(work+8, "WAVEfmt ", 8);
    work[16] = 16; work[20] = work[22] = 1;
    work[17] = work[18] = work[19] = work[21] = work[23] = 0;
    work[24] = work[28] = SMPL >> 0  & 0xff;
    work[25] = work[29] = SMPL >> 8  & 0xff;
    work[26] = work[30] = SMPL >> 16 & 0xff;
    work[27] = work[31] = SMPL >> 24 & 0xff;
    work[32] = 1; work[33] = 0;
    work[34] = 8; work[35] = 0;
    memcpy(work+36, "data", 4);
    work[40] = size >> 0  & 0xff; work[41] = size >> 8  & 0xff;
    work[42] = size >> 16 & 0xff; work[43] = size >> 24 & 0xff;
    memcpy(work + 44, buf, size);
    
    fwrite(work, filesize, 1, fp);
    fclose(fp);
    free(work);
}

double GenRosenberg(int freq)
{
    /* Rosenberg波を生成 */
    static double t;
    double tau  = 0.90;  /* 声門開大期 */
    double tau2 = 0.95;  /* 声門閉小期 */
    double sample = 0.0;
    
    t += (double)freq / (double)SMPL;
    t -= floor(t);
    if (t <= tau) {
        sample = 3.0*pow(t/tau,2.0)-2.0*pow(t/tau,3.0);
    } else if (t < tau+tau2) {
        sample = 1.0-pow((t-tau)/tau2,2.0);
    }
    return 2.0*(sample-0.5);
}

void IIR_SettingReso(double f, double Q, double param[])
{
    /* BPFのフィルタ係数を求める */
    double omega = 2.0 * M_PI * f / (double)SMPL;
    double alpha = sin(omega) / (2.0 * Q);
    double a0    = 1.0 + alpha;
    param[0] = alpha             / a0;
    param[1] = 0.0               / a0;
    param[2] = -alpha            / a0;
    param[3] = -2.0 * cos(omega) / a0;
    param[4] = (1.0 - alpha)     / a0;
}

double IIR_ApplyFilter(double base, double param[], double delay[])
{
    /* フィルタの適用 */
    double sample = 0.0;
    sample += param[0] * base;
    sample += param[1] * delay[0];
    sample += param[2] * delay[1];
    sample -= param[3] * delay[2];
    sample -= param[4] * delay[3];
    delay[1] = delay[0]; delay[0] = base;
    delay[3] = delay[2]; delay[2] = sample;
    return sample;
}

int main(void)
{
    int i, j, size;
    char *buf;
    double freq, vtlen, in, out;
    double param[5][5] = {0}, delay[5][4] = {0}, formant[5];
    double male[5][5] = {
        /* 共鳴周波数の変動値 */
        /* F1,  F2,  F3,  F4,  F5 */
        {1.60,0.70,1.10,1.00,1.00},
        {0.70,1.40,1.20,1.00,1.00},
        {0.80,0.90,0.90,1.00,1.00},
        {1.20,1.30,1.10,1.00,1.00},
        {1.15,0.50,1.20,1.00,1.00}
    };
    
    /* 設定 */
    size  = SMPL * 1;    /* 1秒間 */
    buf   = (char *) malloc(size);
    freq  = 220.0;     /* 基本周波数 */
    vtlen = 15.0;     /* 声道の長さ */
    
    /* 共鳴周波数を計算 */
    for (i = 0; i < 5; i++) {
        formant[i] = (double)((34000.0*(double)(2*i+1))/(4.0*vtlen));
        formant[i] *= male[VOWEL_A][i];  /* 「あ」 */
    }
    
    /* 波形の生成 */
    for (i = 0; i < size; i++) {
        out = 0.0;
        in = GenRosenberg(freq);
        for (j = 0; j < 5; j++) {
            IIR_SettingReso(formant[j], 20.0, param[j]);
            out += IIR_ApplyFilter(in, param[j], delay[j]);
        }
        buf[i] = (char)(128.0 * MIN(MAX(out, -1.0), 1.0)) + 128;
    }
    
    /* 書き出し */
    WriteWavFile("voice.wav", buf, size);
    free(buf);
    return 0;
}
