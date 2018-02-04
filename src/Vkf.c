#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include "../include/Vkf.h"
#include "../include/Riq.h"

int wav2array(char* fName)
{
    FILE *file, *fTmp, *fCorr;
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

//    int len = header.bitsPerSample >> 3;
    fTmp = fopen("wavdata.dat", "w");
    fCorr = fopen("icf.dat", "w");

    int dataBytes = header.subchunk2Size;
    int records = (dataBytes >> 1) / sizeof(_s16);
    int samples = records << 1;
//    _s16* pTmp = (_s16*) malloc(dataBytes / sizeof(_s16));
    _u8* pTmp = (_u8*) malloc(dataBytes);

//    int recs = fread(pTmp, sizeof(_s16), dataBytes, file);
    fread(pTmp, 1, dataBytes, file);

	_s16* pIn1 = (_s16*)malloc(records*sizeof(_s16));
	_s16* pIn2 = (_s16*)malloc(records*sizeof(_s16));
	_f64* pCorr = (_f64*)malloc(records*sizeof(_f64));
    for (int i=0; i<samples; i++)
    {
        pIn1[i] = pTmp[i];
        pIn2[i] = pTmp[i+1];
    }
    if (!corrFunc(pIn1, pIn2, pCorr, records))
    {
        printf("Ошибка при расчете корреляционной функции !!!");
    };

    fwrite(pCorr, sizeof(_f64), records, fCorr);
    fwrite(pTmp, 1, dataBytes, fTmp);

    fclose(fTmp);
    fclose(file);
    fclose(fCorr);

    free(pTmp);
    free(pCorr);
    free(pIn1);
    free(pIn2);

    return 0;
}

int corrFunc(_s16* in1, _s16* in2, _f64* corr, int records)
{
    printf("Обрабатываемое число точек: %d\n", records);
//    _s16 p1, p2;
//    _f64 c;
    for (int n=0; n<records/2; n++)
    {
        corr[n] = 0;
//        printf("Цикл: %d\n", n);
        for(int i=0; i<records; i++)
        {
            if (i+n < records)
            {
//                p1 = in1[i];
//                p2 = in2[i];
                corr[n] += sqrt(in1[i] * in2[i+n]);
//                c = corr[n];
            }
            else
                break;
        }
        printf("%16lf\t", corr[n]);
        if (n % 8 == 0) printf("\n");
    }
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
