#include <string.h>
#include <unistd.h>
#include "../include/db.h"
#include "../include/Riq.h"
#include "../include/Vkf.h"

// for windows
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    char dirName[1000];
    char f_sample[30], f_frag[30];
    char* pSample;   // область данных образца
    char* pFrag;     // область данных иследуемого фрагмента
    int cntSample = -1, cntFrag = -1;

    pSample = (char*) malloc(1000);
    pFrag = (char*) malloc(1000);

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



    printf("Файл образца\n");
    cntSample = readWav((char*) &f_sample, pSample);
    printf("Исследуемый файл\n");
    cntFrag = readWav((char*) &f_frag, pFrag);
    printf("Байтов образца: %10d, байтов фрагмента: %10d\n", cntSample, cntFrag);

    getVKF(pSample, cntSample, pFrag, cntFrag);

    wav2array(f_sample);

    free(pSample);
    free(pFrag);

   exit(0);
}
