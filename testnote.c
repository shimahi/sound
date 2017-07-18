#include<stdio.h>
#include<math.h>

#define PI 3.14159265 /*円周率*/
#define SR 10000 /*サンプリング周波数*/
#define FRQ 440 /*正弦波の周波数*/
#define DUR 500 /*正弦波の長さ[msec]*/
#define MAXPNT 15000 /*正弦波の長さ[標本点数]の最大値*/

int main(){
    
    short iwave[MAXPNT]; /* 正弦波格納用配列 */
    int dur_in_ms = DUR; /* 正弦波の長さ [msec] */
    int dur_in_pnt = 0; /* 正弦波の長さ [標本点数] */
    int frq = FRQ; /* 正弦波の周波数 [Hz] */
    int i;
    
    
    for(i=0;i<dur_in_pnt; ++i){
        printf("%f %d\n", (double)i/SR*1000, iwave[i]);
    }
    
    return 0;
}
