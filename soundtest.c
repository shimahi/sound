#include<stdio.h>
#include<stdlib.h>
#include "testnote.h"

int main(void){
    MONO_PCM pcm0 \, pcm1;
    int n;
    
    mono_wave_read(&pcm, "a.wav");
    
    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length
    pcm1.s = calloc(pcm1.length, sizeof(double));
    
    for(n=0;n<pcm1.length;n++){
        pcm1.s[n] = pcm0.s[n];
    }
    
    mono_wave_write(&pcm1, "b.wav");
    
    free(pcm0.s);
    free(pcm1.s);
}
