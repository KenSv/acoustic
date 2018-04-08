#include <string.h>
#include <unistd.h>
#include <time.h>
#include "../include/db.h"
#include "../include/Riq.h"
#include "../include/Vkf.h"

// for windows
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int res = 0;
    double start = clock(), end;
    char dirName[1000];
    char f_sample[255];
    char f_frag[255];
    char* pSample;   // область данных образца
    char* pFrag;     // область данных иследуемого фрагмента
    int cntSample = -1, cntFrag = -1;

    pSample = (char*) malloc(20000000);
    pFrag = (char*) malloc(20000000);

    printf("Current path: %s\n", getcwd(dirName, sizeof(dirName)));

    switch (argc)
    {
        case 3:
            strcpy(f_frag, argv[2]);
        case 2:
            strcpy(f_sample, argv[1]);
            break;
        default:
            strcpy(f_sample, "ring-original.wav");
            strcpy(f_frag, "ring-demod.wav");
            break;
    }

// закомментировал временно для проверки обработки на простых синтетических сигналах
    printf("Файл образца: ");
    cntSample = readWav((char*) &f_sample, pSample);
    if (cntSample <= 0)
    {
        printf("Ошибка открытия файла образца\n");
        res = -1;
    }
    printf("Исследуемый файл: ");
    printf("%s\n", f_frag);
    cntFrag = readWav((char*) &f_frag, pFrag);
    if (cntFrag <= 0)
    {
        printf("Ошибка открытия исследуемого файла\n");
        res = -2;
    }

// эмуляция образца и исследуемого сигналов для проверки работы функции ВКФ
//    cntSample = 1000;
//    cntFrag = 10000;
//    int shift = 100;
//    int ampSample = 32767;
//    int ampFrag = 1000;
//    generateSample(pSample, cntSample, ampSample, 0);
//    generateSample(pFrag, cntFrag, ampFrag, shift);

    // собственно подсчет ВКФ
    _s16 result = 0;
    getVKF(pSample, cntSample, pFrag, cntFrag, &result);
    printf("процент сходства: %i%% \n", result);
//    wav2array(f_sample);

    free(pSample);
    free(pFrag);
    end = clock();
    printf("Время выполнения обработки: %.1lf сек\n", (end - start) / (CLOCKS_PER_SEC));
   exit(res);
}
