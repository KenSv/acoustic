#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include "../include/Vkf.h"

int wav2array(char* fName, double* pOut)
{
    FILE *file, *fTmp;
    file = fopen(fName, "rb");
    if (!file)
    {
        printf("Failed open file, error %d", errno);
        return 1;
    }

    WAVHEADER header;

    fread(&header, sizeof(WAVHEADER), 1, file);

    // Выводим полученные данные
    printf("Sample rate: %d\n", header.sampleRate);
    printf("Channels: %d\n", header.numChannels);
    printf("Bits per sample: %d\n", header.bitsPerSample);
    printf("ChunkSize: %d\n", header.subchunk2Size);

    // Посчитаем длительность воспроизведения в секундах
//    float fDurationSeconds = 1.f * header.subchunk2Size / (header.bitsPerSample / 8) / header.numChannels / header.sampleRate;
//    int iDurationMinutes = (int)floor(fDurationSeconds) / 60;
//    fDurationSeconds = fDurationSeconds - (iDurationMinutes * 60);
//    printf("Duration: %02d:%02.f\n", iDurationMinutes, fDurationSeconds);

    int* pTmp;
    int len = header.bitsPerSample >> 3;
    pTmp = (int*)malloc(header.subchunk2Size * len);
    fTmp = fopen("wavdata.dat", "w");
    fread(pTmp, len, header.subchunk2Size, file);

    fwrite(pTmp, len, header.subchunk2Size, fTmp);
    fclose(fTmp);

    fclose(file);
    return 0;
}

double maxCorrelation(double* a, double* b, int N){
	double* AR = (double*)malloc(N*sizeof(double));
//	double* AI = (double*)malloc(N*sizeof(double));
	double* BR = (double*)malloc(N*sizeof(double));
//	double* BI = (double*)malloc(N*sizeof(double));
	double* c = (double*)malloc(N*sizeof(double));
//	fft.fft(a,AR,AI,N);
//	fft.fft(b,BR,BI,N);
	double CR;//, CI;

	for(int i = 0; i<N; i++){
		CR = AR[i]*BR[i]; //+AI[i]*BI[i];
//		CI = AI[i]*BR[i]-AR[i]*BI[i];
		AR[i] = CR;
//		AI[i] = CI;
	}
//	fft.real_ifft(AR,AI,c,N);
	double Ea = 0.0, Eb = 0.0;;
	for(int i = 0; i<N; i++){
		Ea += a[i]*a[i];
		Eb += b[i]*b[i];
	}
	double E = sqrt(Ea*Eb);
	double mc = c[0]/E;
	for(int i = 0; i<N; i++)
		mc = fmax(mc, c[i]/E);
	free(AR);
//	free(AI);
	free(BR);
//	free(BI);
	free(c);
	return mc;
}
