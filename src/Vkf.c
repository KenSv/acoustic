#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include "../include/Vkf.h"
#include "../include/Riq.h"

#define NDEBUG // расскомментировать для отключения отладки
#include <assert.h>

int readWav(char* fName, char* wavData)
{
    FILE *file;
    int n;

    printf("%s\n", fName);
    file = fopen(fName, "rb");
    if (!file)
    {
        printf("Failed open file, error %d", errno);
        return -1;
    }

    WAVHEADER header;
    WAV_DATA_HEADER dataHeader;

    fread(&header, sizeof(WAVHEADER), 1, file);
//    if (header.subchunk1Size > 0x10){
//        int extraLen = header.subchunk1Size - 0x10;
//        char* extraFmt = (char*) malloc(extraLen);
//
//        fread(extraFmt, 1, extraLen, file);
//        for (int i=0; i<extraLen; i++)
//            printf("%x\t", extraFmt[i]);
//        free(extraFmt);
//    }
//    fread(&dataHeader, sizeof(WAV_DATA_HEADER), 1, file);
    int t;
    fread(&t, sizeof(t), 1, file);
//    if (t != 0x6164){
    if (t != DataID){
        printf("Ошибка в заголовке файла!!! Попытка поиска блока данных...\n");
        int cnt = 0;
        int chunkId;
        fread(&chunkId, sizeof(int), 1, file);
        while (chunkId != DataID && cnt <255) {
            fread(&chunkId, sizeof(int), 1, file);
//            printf("%x\n", chunkId);
            cnt++;
        }
        if (cnt == 255)
            return -1;
        else {
            fseek(file, -sizeof(int), SEEK_CUR);
            fread(&dataHeader, sizeof(WAV_DATA_HEADER), 1, file);
        }
    } else {
        fseek(file, -sizeof(t), SEEK_CUR);
        fread(&dataHeader, sizeof(WAV_DATA_HEADER), 1, file);
    }


    // Выводим полученные данные
    printf("subchunk1Size: %d\n", header.subchunk1Size);
    printf("Битрейт: %d\n", header.sampleRate);
    printf("Число каналов: %d\n", header.numChannels);
    printf("Число бит в одном значении: %d\n\n", header.bitsPerSample);

//    printf("Идентификатор блока данных: %s\t\n", dataHeader.subchunk2Id);
    printf("Количество исследуемых данных (байт): %d\n", dataHeader.subchunk2Size);
    printf("==============================================\n\n");

//    short len = header.bitsPerSample >> 3;
    if (header.numChannels > 1)
    {
        char* pTmp = (char*) malloc(dataHeader.subchunk2Size*sizeof(char));
        wavData = realloc(wavData, dataHeader.subchunk2Size/header.numChannels);
        int cnt = fread(pTmp, 1, dataHeader.subchunk2Size, file);
        for (n=0; n<dataHeader.subchunk2Size/header.numChannels; n+=2)
        {
            wavData[n] = pTmp[n*header.numChannels];
            wavData[n+1] = pTmp[n*header.numChannels+1];
        }
        free(pTmp);
        return cnt/header.numChannels;
    }
    else
    {
        wavData = realloc(wavData, dataHeader.subchunk2Size);
        return fread(wavData, 1, dataHeader.subchunk2Size, file);
    }
}

// вычисление взаимной корреляционной функции
// pSample - указатель на образцовую выборку
// countSample - число записей в образцовой выборке
// pFrag - указатель на исследуемую выборку
// countFrag - число записей в исследуемой выборке
int getVKF(char* pSample, int countSample, char* pFrag, int countFrag, _s16* similarity)
{
    _s16* pS;
    _s16* pF;
    int res = 0;
    int i, n;
    // максимумы образца и исследуемого сигналов для нормирования образца
    _s16 maxS = 0;
    _s16 maxF = 0;
    // максимум для корреляционной функции
    _f64 maxCorr = 0;
    // нормирующий коээфициент
    _f64 norm;
    // энергетический максимум для образца и фрагмента
    _f64 EnergyS = 0;
//    _f64 EnergyF = 0;
    short step = 10; // шаг между анализируемыми точками выборки (для уменьшения времени обработки)

    assert(countSample >0);
    assert(countFrag >0);

    int cntS = countSample / sizeof(_s16);
    int cntF = countFrag / sizeof(_s16);
    assert(cntF >0);
    assert(cntS >0);

    pS = (_s16*) pSample;
    pF = (_s16*) pFrag;
    int max_rec = cntF > cntS ? cntF : cntS;

    if (cntS >= cntF)
    {
        printf("Число точек в исследуемой выборке должно быть больше чем в образце!\n");
        return 1;
    }

    _f64* pCorr = (_f64*)malloc(max_rec*sizeof(_f64));
    printf("Число точек исследуемой выборки: %d\tобразца: %d\n", cntF, cntS);

// вычисление нормирующего коэффициента
    for (i=0; i< cntS; i++)
        if (abs(pS[i]) > maxS) maxS = abs(pS[i]);
    for (i=0; i< cntF; i++)
//    {
         if (abs(pF[i]) > maxF) maxF = abs(pF[i]);
//         printf("%i \t%x int %i\n", i, pF[i], pF[i]);
//    }
    norm = (_f64)maxS/maxF;
    printf("Максимум образца: %i\n", maxS);
    printf("Максимум исследуемого фрагмента: %i\n", maxF);
    printf("Нормирующий коэффициент: %f\n", norm);

 // вычисление корреляционной функции
    for (n=0; n<cntF; n++)
    {
        pCorr[n] = 0.0;
//        for(i = 0; i < cntS/10; i+=100)
        for(i = 0; i < cntS/10; i+=step)
        {
            if (i+n < cntF)
            {
                pCorr[n] += fabs(norm * pF[n+i] * pS[i]);
            } else
                break;
        }
    }

    // фильтрация
    _f64* pFiltered = (_f64*) malloc(sizeof(_f64)*cntF);
    short window = 100;
    _f64 sum = 0;
    for (i=0; i<window; i++)
    {
        sum += pCorr[i];
        pFiltered[i] = sum / (i + 1);
    }
    for (i=window; i<cntF; i++)
    {
        sum = sum + pCorr[i] - pCorr[i-window];
        pFiltered[i] = sum / window;
    }

    // анализ совпадения (временно по максимуму ВКФ)
    for(i = 0; i < cntS/10; i+=step){
        EnergyS += pow(pS[i], 2);
    }
    printf("Максимум энергетический: %8.1f\n", EnergyS);

    for  (i=0; i<cntF; i++) {
        if (pCorr[i] > maxCorr) maxCorr = pCorr[i];
    }
    printf("Максимум ВКФ: %8.1f\n", maxCorr);
    *similarity = round(100 * maxCorr / EnergyS);

// запись ВКФ в файл !!! ВРЕМЕННО !!! для визуального анализа
    FILE *fCorr;
    fCorr = fopen("icf.dat", "w");
    //fwrite(pCorr, sizeof(_f64), cntF, fCorr);
    fwrite(pFiltered, sizeof(_f64), cntF, fCorr);
    fclose(fCorr);
    free(pCorr);
    free(pFiltered);
    return res;
}

//int wav2array(char* fName)
//{
//    int res = 0;
//    FILE *file, *fTmp, *fCorr;
//    file = fopen(fName, "rb");
//    if (!file)
//    {
//        printf("Failed open file, error %d", errno);
//        return 1;
//    }
//
//    WAVHEADER header;
//
//    fread(&header, sizeof(WAVHEADER), 1, file);
//
//    // Выводим полученные данные
//    printf("Sample rate: %d\n", header.sampleRate);
//    printf("Channels: %d\n", header.numChannels);
//    printf("Bits per sample: %d\n", header.bitsPerSample);
////    printf("ChunkSize: %d\n", dataHeader.subchunk2Size);
//
//    // Посчитаем длительность воспроизведения в секундах
////    float fDurationSeconds = 1.f * header.subchunk2Size / (header.bitsPerSample / 8) / header.numChannels / header.sampleRate;
////    int iDurationMinutes = (int)floor(fDurationSeconds) / 60;
////    fDurationSeconds = fDurationSeconds - (iDurationMinutes * 60);
////    printf("Duration: %02d:%02.f\n", iDurationMinutes, fDurationSeconds);
//
////    int len = header.bitsPerSample >> 3;
//    fTmp = fopen("wavdata.dat", "w");
//    fCorr = fopen("icf.dat", "w");
//
//    int dataBytes = header.subchunk2Size;
//    int records = (dataBytes >> 1) / sizeof(_s16);
//    int samples = records << 1;
////    _s16* pTmp = (_s16*) malloc(dataBytes / sizeof(_s16));
//    _s16* pTmp = (_s16*) malloc(samples * sizeof(_s16));
////    _u8* pTmp = (_u8*) malloc(dataBytes);
//	_f64* pCorr = (_f64*)malloc(records*sizeof(_f64));
//	_s16* pIn1 = (_s16*)malloc(samples*sizeof(_s16));
//	_s16* pIn2 = (_s16*)malloc(samples*sizeof(_s16));
//
////    int recs = fread(pTmp, sizeof(_s16), dataBytes, file);
//    fread(pTmp, 1, dataBytes, file);
//
//    for (int i=0; i<samples; i++)
//    {
//        pIn1[i] = pTmp[i];
//        pIn2[i] = pTmp[i];
//    }
//
//    if (corrFunc(pIn1, pIn2, pCorr, records) != 0)
//    {
//        printf("Ошибка при расчете корреляционной функции !!!");
//        res = 1;
//    };
//
//    fwrite(pCorr, sizeof(_f64), records, fCorr);
//    fwrite(pTmp, 1, dataBytes, fTmp);
//
//    fclose(fTmp);
//    fclose(file);
//    fclose(fCorr);
//
//    free(pIn2);
//    free(pCorr);
//    free(pIn1);
//    free(pTmp);
//
//    return res;
//}

int corrFunc(_s16* in, _s16* etalon, _f64* corr, int records)
{
    int i, n;
    printf("Обрабатываемое число точек: %d\n", records);
    int et_start = records/2, et_count = 1000;
//    _s16 p1, p2;
//    _f64 c;
    for (n=0; n<records; n++)
    {
        corr[n] = 0.0;
        for(i = 0; i < et_count; i++)
        {
            if (i+n < records)
            {
//                p1 = in[i];
//                p2 = etalon[i];
                corr[n] += sqrt(in[n+i] * etalon[i+et_start]);
//                c = corr[n];
            }
            else
                break;
        }
//        printf("%16.2f\t", corr[n]);
//        if (n % 8 == 0) printf("\n");
    }
    return 0;
}


//double maxCorrelation(double* a, double* b, int N){
//	double* AR = (double*)malloc(N*sizeof(double));
////	double* AI = (double*)malloc(N*sizeof(double));
//	double* BR = (double*)malloc(N*sizeof(double));
////	double* BI = (double*)malloc(N*sizeof(double));
//	double* c = (double*)malloc(N*sizeof(double));
////	fft.fft(a,AR,AI,N);
////	fft.fft(b,BR,BI,N);
//	double CR;//, CI;
//
//	for(int i = 0; i<N; i++){
//		CR = AR[i]*BR[i]; //+AI[i]*BI[i];
////		CI = AI[i]*BR[i]-AR[i]*BI[i];
//		AR[i] = CR;
////		AI[i] = CI;
//	}
////	fft.real_ifft(AR,AI,c,N);
//	double Ea = 0.0, Eb = 0.0;;
//	for(int i = 0; i<N; i++){
//		Ea += a[i]*a[i];
//		Eb += b[i]*b[i];
//	}
//	double E = sqrt(Ea*Eb);
//	double mc = c[0]/E;
//	for(int i = 0; i<N; i++)
//		mc = fmax(mc, c[i]/E);
//	free(AR);
////	free(AI);
//	free(BR);
////	free(BI);
//	free(c);
//	return mc;
//}
