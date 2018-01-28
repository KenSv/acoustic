//#include <iostream>
#include <math.h>
#include <string.h>
#include "../include/db.h"
#include "../include/Riq.h"
// for windows
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//using namespace std;
//#define KALMAN

#ifdef KALMAN
// Kalman filter
void filter(_u8* pIn, _f64* pOut, int block_size, float percent = 100)
{
    double gain = 1;
    double offset = 0;

    double ps;              // predicted state
    double pc;              // predicted covariance

    double factor_r = 1;    // factor of real value to previous real value
    double noise_m = 0.2;   // measurement noise
    double factor_m = 1;    // factor of measured value to real value
    double noise_env = 0.8; // environment noise

    double state;
    double covariance;
    double K;
    int i;

    // Задаем начальные значение state и covariance
    state = pOut[0];
    covariance = 0.1;
    // оставил в отдельном цикле, на случай применения рекурсивной обработки
    for (i=0; i < block_size; i++)
        pOut[i] = pow(10, (pIn[i] * gain + offset)/10);

    for (i=0; i< block_size; i++)
    {
        //time update - prediction
        ps = factor_r*state;
        pc = factor_r*covariance*factor_r + noise_m;
        //measurement update - correction
        K = factor_m*pc/(factor_m*pc*factor_m + noise_env);
        state = ps + K*(pOut[i] - factor_m*ps);
        covariance = (1 - K*factor_m)*pc;
        pOut[i] = state;
    }
    for (i=0; i < block_size; i++)
        pOut[i] = 10.*log10(pOut[i]);
}
#else
// simple filter
void filter(_u8* pIn, _f64* pOut, int block_size, float percent)
{
//    double dmax, kmax = 0;
    double ymax = 0, ymin = 10000000L, dmax = 0;
//    double percent = 100;
    double delta = 0;
    double gain = 1;
    double offset = 0;
    int i;
//    _f64* pKoef;

//    pKoef   = (_f64*) malloc(block_size * sizeof(_f64));

    for (i=0; i < block_size; i++)
        pOut[i] = pow(10, (pIn[i] * gain + offset)/10);

    ymax = pOut[0];
    for (i=1; i < block_size; i++)
        {
        delta = pOut[i] - pOut[i-1];
        if (fabs(delta) > dmax)
        {
            dmax = fabs(delta);
//            kmax = pOut[i];
        }
        if (pOut[i] > ymax) ymax = pOut[i];
        if (pOut[i] < ymin) ymin = pOut[i];
//        if (fabs(delta) > dmax) dmax = pOut[i];
//        pKoef[i] = delta;
    }
//double k = 0.1;
//double mult, div;
//    for (i=0; i < block_size; i++)
//    {
//        if (percent == 100)
//            pOut[i] = 1+ pOut[i] * pOut[i] / ymax;
////        pOut[i] = 1+ pOut[i] * pOut[i] / ymax /k;
////        pOut[i] = 1+ pOut[i] * pOut[i] / ymax/ (1 - (pOut[i] - ymax)/pow((ymin-ymax), 2));
////        pOut[i] = 1+ pOut[i] * pOut[i] / ymax/ (1 - (percent/100) *(pOut[i] - ymax)/pow((ymin-ymax), 2));
////        if (i == 0  )
////        pOut[i] = 1 +   pOut[i] * pow((1 - pOut[i]/ymax), 2);
//
//        else
//        {
//// вариант с параболической функцией ослабления в диапазоне от 1(ymax) до 0
////            k = (100 - percent) / 100
////            mult = pow(pOut[i], 2) * ((1 - k)/pow(ymax, 2)) + k;
////            if (i == 0 )
////                pOut[i] = 1 + pOut[i] * mult;
////            else
////                if (pOut[i] > pOut[i-1])
////                    pOut[i] = 1 + pOut[i] * mult;
////                else
////                    pOut[i] = 1 + pOut[i] * (2 - mult);
//// =======================================================
//// вариант с параболической функцией ослабления от 1(ymax) до бесконечности :)
////            div = ymax-ymin * percent / 100 + 1;
//            if (ymin == 0) ymin = 1;
//            div = ((ymax - ymin) * 0.01) * percent / 100 + 1;
//            mult = (div - 1) * pow((ymax - pOut[i]), 2)/(ymax * ymax) + 1;
//            pOut[i] = 1 + pOut[i] / mult;
//        }
//    }

//// =======================================================
//// вариант со сглаживанием дельты
//    long double dc, kmin, treshold;
//percent = 98;
//    kmin = 1 - percent / 100;
//    treshold = dmax / 1E15;  // значение 1E15 по физическому смыслу - соотношение сигнал/шум
//    for (i = 1; i < block_size; i++)
//    {
//        delta = pOut[i] - pOut[i-1];
////        if (fabs(delta) > treshold) continue;
//        k = 1 - (pow(delta/dmax, 2) * (1 - kmin) + kmin);
////        dc = delta * k;
////        pOut[i] -=dc;
//
//        dc = delta * k /2;
////        dc = delta/2;
//        pOut[i-1] += dc;
//        pOut[i] -= dc;
////        for (int n = 0; n < i; n++) pOut[n] += dc;
////        for (int n = i; n < block_size; n++) pOut[n] -= dc;
//    }

// =======================================================
// вариант со сглаживанием в рамках скользящего окна
//    long double dc, kmin, treshold;
//percent = 90;
int wmin, wmax, pcur, wcur;
    _f64* pTmp;
    pTmp  = (_f64*) malloc(block_size * sizeof(_f64));
    wmax = 0; wmin = block_size; pcur = 0;
    for (i = 1; i < block_size-1; i++) {
        if (pOut[i-1]<pOut[i] && pOut[i]>pOut[i+1]) {
            wcur = i - pcur;
            if (wcur > wmax)
                wmax = wcur;
            if (wcur < wmin && wcur >5)
                wmin = wcur;
            pcur = i;
        }
    }

//    int frame_size = wmax;
    int frame_size = round(wmax*2 *percent/100); // макс ширина окна - процент от удвоенного макс интервала между пиками
//    int frame_size = round(block_size*percent/1000); // макс ширина окна - 1/10 от размера блока данных
    // делаем размер окна нечетным
    frame_size |= 1;
    // центр окна
    int frame_shift = frame_size >> 1;
    long double frame_sum = 0;
    for (i = 0; i<frame_size; i++)  frame_sum += pOut[i];
    pTmp[frame_shift] = frame_sum / frame_size;

    for (i = 1; i < block_size-frame_size+1; i++)
    {
//        frame_sum = frame_sum - pOut[i-1] + pOut[i+frame_size-1];
        frame_sum = 0;
        for (int n = i; n < i+frame_size; n++) frame_sum += pOut[n];
        pTmp[i+frame_shift] = frame_sum / frame_size;
    }
    // дописываем необработанные участки в начале и в конце последовательности
    for (i = 0; i<frame_shift; i++) pTmp[i] = pTmp[frame_shift];
    for (i = block_size-frame_shift; i<block_size; i++) pTmp[i] = pTmp[block_size-frame_shift-1];

    // поиск максимума и минимума в отфильтрованной последовательности
    _f64 pmin = 1E15, pmax = 0;
    for (i = 0; i < block_size; i++)
    {
        if (pTmp[i] > pmax) pmax = pTmp[i];
        if (pTmp[i] < pmin) pmin = pTmp[i];
    }

    long double treshold = (pmax-pmin)*2/3;
    frame_size = wmin | 1;
    frame_shift = frame_size >> 1;
    for (i=0; i<block_size; i++)
    {
        if (pTmp[i] > treshold)
        {
            frame_sum = 0;
            for (int n = 0; n < frame_size; n++) frame_sum += pOut[i+n-frame_shift-1];
            pTmp[i-2] = frame_sum / frame_size;
        }
    }

    for (i = 0; i < block_size; i++) pOut[i] = pTmp[i];
//    memcpy(pOut, pTmp, block_size);
    free(pTmp);



//// =======================================================
//// вариант интегрирования (branch integral)
//    long double dc, kmin, treshold;
////percent = 90;
//    long double sum = 0;
//    int cnt = 0;
//    kmin = 1 - percent / 100;
//    treshold = dmax / 1E6;  // значение 1E15 по физическому смыслу - соотношение сигнал/шум
////    treshold = ymax / 1E6;  // значение 1E15 по физическому смыслу - соотношение сигнал/шум
//    for (i = 1; i < block_size; i++)
//    {
//        delta = pOut[i] - pOut[i-1];
//        if (fabs(delta) > treshold) continue;
////        if (pOut[i] > treshold || pOut[i-1] > treshold) continue;
//        cnt++;
//        sum += (pOut[i] + pOut[i-1]) /2;
//        k = 1 - (pow(delta/dmax, 2) * (1 - kmin) + kmin);
//        dc = (pOut[i] - sum / cnt) * k;
//        pOut[i] -= dc;
//    }



//// =======================================================
//// вариант со сглаживанием дельты по трем точкам (triangle)
//    long double dc, kmin, treshold;
//percent = 98;
//    kmin = 1 - percent / 100;
//    treshold = dmax / 1E15;  // значение 1E15 по физическому смыслу - соотношение сигнал/шум
//    for (i = 1; i < block_size - 1; i++)
//    {
//        delta = pOut[i] - (pOut[i+1] + pOut[i-1]) / 2;
//        if (fabs(delta) > treshold) continue;
//        k = 1 - (pow(delta/dmax, 2) * (1 - kmin) + kmin);
//        dc = delta * k;
//        pOut[i] -=dc;
//    }

//// =======================================================
//// вариант со сглаживанием дельты в два прохода в прямом и обратном направлении
//    long double dc, kmin, treshold;
//    _f64* pOlr;
//    _f64* pOrl;
//    pOlr  = (_f64*) malloc(block_size * sizeof(_f64));
//    pOrl  = (_f64*) malloc(block_size * sizeof(_f64));
//percent = 90;
//    kmin = 1 - percent / 100;
//    treshold = dmax / 1E15;  // значение 1E15 по физическому смыслу - соотношение сигнал/шум
//    for (i = 1; i < block_size; i++)
//    {
//        delta = pOut[i] - pOut[i-1];
//        if (fabs(delta) > treshold)
//        {
//            pOlr[i] = pOut[i];
//            continue;
//        }
//        k = 1 - (pow(delta/dmax, 2) * (1 - kmin) + kmin);
//        dc = delta * k;
//        pOlr[i] = pOut[i] - dc;
//    }
//    pOlr[0] = pOut[0];
//    for (i = block_size-2; i >= 0; i--)
//    {
//        delta = pOut[i] - pOut[i+1];
//        if (fabs(delta) > treshold)
//        {
//            pOrl[i] = pOut[i];
//            continue;
//        }
//        k = 1 - (pow(delta/dmax, 2) * (1 - kmin) + kmin);
//        dc = delta * k;
//        pOrl[i] = pOut[i] - dc;
//    }
//    pOrl[block_size-1] = pOut[block_size-1];
//    for (i = 0; i < block_size; i++)
//    {
//        pOut[i] = (pOlr[i] + pOrl[i]) /2;
////        pOut[i] = pOrl[i];
////        pOut[i] = pOlr[i];
//    }
//    free(pOlr);
//    free(pOrl);
//// =======================================================


    for (i=0; i < block_size; i++)
        pOut[i] = 10.*log10(pOut[i]);

//    free(pKoef);
}
#endif

void dumpArray(_u8** buf, unsigned int bytes, unsigned int items, unsigned short itemsOnLine)
{
    for (unsigned int i=0; i < items; i++)
    {
        switch (bytes)
        {
            case 1:
                printf("%4d ", *((unsigned char *) *buf + i*bytes));
                break;
            case 2:
                printf("%6d ", *((short *) *buf + i*bytes));
                break;
            case 4:
                printf("%10d ", *((int *) *buf + i*bytes));
                break;
            case 8:
                printf("%f ", *((double *) *buf + i*bytes));
                break;
            default:
                break;
        }
        if ((i+1)%itemsOnLine == 0) printf("\n");
    }
}

void dumpTimeStamp(_u8** buf, const char* msg)
{
    printf("%s: (%li) %s", msg, *((long *) *buf), ctime((time_t *) *buf));
    *buf += 8;
}

void dump_u8(_u8** buf, const char* msg)
{
    printf("%16ui\t- %s\n", *((_u8 *) *buf), msg);
    (*buf) ++;
}

void dump_s16(_u8** buf, const char* msg)
{
    printf("%16ui\t- %s\n", *((_s16 *) *buf), msg);
    *buf += 2;
}

void dump_u16(_u8** buf, const char* msg)
{
    printf("%16ui\t- %s\n", *((_u16 *) *buf), msg);
    *buf += 2;
}

void dump_s32(_u8** buf, const char* msg)
{
    printf("%16i\t- %s\n", *((_s32 *) *buf), msg);
    *buf += 4;
}

void dump_u32(_u8** buf, const char* msg)
{
    printf("%16i\t- %s\n", *((_u32 *) *buf), msg);
    *buf += 4;
}

void dump_u64(_u8** buf, const char* msg)
{
    printf("%16li\t- %s\n", *((_u64 *) *buf), msg);
    *buf += 8;
}

void dump_s64(_u8** buf, const char* msg)
{
    printf("%16li\t- %s\n", *((_s64 *) *buf), msg);
    *buf += 8;
}

void dump_f64(_u8** buf, const char* msg)
{
    printf("%16lf\t- %s\n", *((_f64 *) *buf), msg);
    *buf += 8;
}

void dumpHex64(_u8** buf, const char* msg)
{
    printf("%16lx\t- %s\n", *((_u64 *) *buf), msg);
    *buf += 8;
}

void dumpHex32(_u8** buf, const char* msg)
{
    printf("%16x\t- %s\n", *((_u32 *) *buf), msg);
    *buf += 4;
}

void dumpHex8(_u8** buf, const char* msg)
{
    printf("%16x\t- %s\n", *((_u8 *) *buf), msg);
    (*buf)++;
}

bool parseArray(_u8** buf, _u8** pRiq, float percent)
{
    unsigned int varType = *((int *) *buf);
    unsigned int nb = (varType >> 24) & 0x0F;
    *buf += 4;
    unsigned int arItems = *((int *) *buf);
    *buf += 4; // длина массива следует за типом значения - 4 байта?

    memcpy(*pRiq, (*buf) - 8, arItems * nb + 8);
    *pRiq += 8;

    printf(" Массив %u записей: ", arItems);
    _f64* ptrOut = (_f64*)malloc(arItems * sizeof(_f64));

    switch (varType)
    {
    case DBI_IQDATA_S16:
        printf(" IQ-канал (_s16)\n");
        break;
    case DBI_IQDATA_F64:
        printf(" IQ-данные (_f64)\n");
        break;
    case DBI_ODATA_S16:
        printf(" осциллограф (_s16)\n");
        break;
    case DBI_ODATA_F64:
        printf(" осциллограф (_f64)\n");
        break;
    case DBI_FFT_F64:
        printf(" спектр (_f64)\n");
        break;
    case DBI_FFT_U16:
        printf(" спектр (_s16)\n");
        break;
    case DBI_FFT_U8:
        filter(*buf, ptrOut, arItems, percent);
        for (unsigned int i=0; i < arItems; i++)
        {
           (*pRiq)[i] = (_u8)((unsigned int) ptrOut[i]);
//           (*pRiq)[i] = (_u8) ptrOut[i];
        }
        printf(" спектр (_u8)\n");
        break;
    case DBI_FFT_F64_2D:
        printf(" 2D спектр (_f64)\n");
        break;
    case DBI_TGRAM_F64:
        printf(" 2D топограма (_f64)\n");
        break;
    case DBI_BYTEARRAY:
        printf(" массив данных\n");
        break;
    case DBI_DESC_S8:
        printf(" описание\n");
        break;
    case DBI_DESC_U16:
        printf(" описание\n");
        break;
    case DBI_FRAMES:
        printf(" типы представления фреймов в каждом окне\n");
        printf(/*"buf: %x *buf: %x*/ "nb: %i items: %i \n", nb, arItems);
        break;
    case DBI_MD_NAME:
        printf(" имя мультимедиа файла\n");
        break;
    case DBI_CHAN_FFT:
        printf(" канальный спектр\n");
        break;
    case DBI_SIGN_MASK:
        printf(" маска сигнатуры\n");
        break;
    case DBI_NAME_S8:
        printf(" имя\n");
        break;
    case DBI_NAME_U16:
        printf(" \n");
        break;
    case DBI_GROUPNAME:
        printf(" название под-группы эталона\n");
        break;
    case DBI_SUBGROUPNAME:
        printf(" название под-группы эталона\n");
        break;
    case DBI_THRR_OFFSETS:
        printf(" смещения порога записи\n");
        break;
    case DBI_SPLITTERS:
        printf(" разделители окна\n");
        break;
    case DBI_CHAN_STATES:
        printf(" состояния каналов\n");
        break;
// пользователи
    case DBI_USER_LOGIN:
        printf(" логин пользователя\n");
        break;
    case DBI_USER_NAME:
        printf(" полное имя пользователя\n");
        break;
    case DBI_USER_AVATAR:
        printf(" аватар пользователя\n");
        break;
    case DBI_USER_PASSWD:
        printf(" хеш пароля пользователя\n");
        break;

// проекты
    case DBI_PROJ_NAME:
        printf(" имя проекта\n");
        break;
    case DBI_PROJ_PASSWD:
        printf(" хеш пароля проекта\n");
        break;
    case DBI_PROJ_DATA_DIR:
        printf(" папка для сохранения файлов\n");
        break;
    case DBI_PROJ_OPTIONS:
        printf(" опции проекта\n");
        break;

// приёмники
    case DBI_RCV_NAME:
        printf(" имя приёмника\n");
        break;
    case DBI_RCV_CHN_NAME:
        printf(" имя канала приёмника\n");
        break;
    default:
        break;
    }
        free(ptrOut);

    dumpArray(buf, nb, arItems, 32);
    printf("<<< Конец массива\n");
//    *pRiq += (arItems * nb  + 8);
    *pRiq += arItems * nb;
    *buf += arItems * nb;
    return true;
}

bool parseVar(_u8** buf, _u8** pRiq)
{
    unsigned int varType = *((int *) *buf);
    if(varType & (DBIT_ARRAY|DBIT_ARRAY2|DBIT_ARRAY3)) return false;
    unsigned int nb = (varType >> 24) & 0x0000000F;
    memcpy(*pRiq, *buf, nb + 4);
    *pRiq += (nb + 4);
    *buf += 4;
//    long value;
    switch (varType)
    {
    case DBI_VERSION:
        dump_u32(buf, "версия DB API");
        break;
    case DBI_SFLAGS:
        dump_u32(buf, "системные флаги");
        break;
    case DBI_UFLAGS:
        dumpHex32(buf, "пользовательские флаги");
        break;
    case DBI_UIN:
        dump_u64(buf, "Уникальный номер");
        break;
    case DBI_UID:
        dump_u64(buf, "Уникальный идентификатор");
        break;
    case DBI_PROJECTID:
        dump_u64(buf, "Уникальный идентификатор проекта");
        break;
    case DBI_SCANID:
        dump_u32(buf, "Идентификатор сканирования");
        break;
    case DBI_FMIN:
        dump_f64(buf, "Начальная частота (МГц)");
        break;
    case DBI_FMAX:
        dump_f64(buf, "Конечная частота (МГц)");
        break;
    case DBI_FCUR:
        dump_f64(buf, "Центральная частота (МГц)");
        break;
    case DBI_BAND:
        dump_f64(buf, "полоса (МГц)");
        break;
    case DBI_GFMIN:
        dump_f64(buf, "общая начальная частота (МГц)");
        break;
    case DBI_GFMAX:
        dump_f64(buf, "общая конечная частота (МГц)");
        break;
    case DBI_FDIFF:
        dump_f64(buf, "отклонение частоты (МГц)");
        break;

    case DBI_COUNT:
        dump_s32(buf, "число выборок в данном сканировании");
        break;
    case DBI_SAMPLES:
        dump_u32(buf, "число отсчётов в каждой выборке");
        break;
    case DBI_TOTAL_SAMPLES:
        dump_u64(buf, "общее число отсчётов");
        break;
    case DBI_RBW_KHZ:
        dump_u16(buf, "rbw в кГц");
        break;

    case DBI_NPACK:
        dump_s32(buf, "номер выборки в пачке");
        break;
    case DBI_NPACKS:
        dump_s32(buf, "всего выборок в пачке");
        break;
    case DBI_PEAK_MAX:
        dump_f64(buf, "максимум пик-детектора");
        break;
    case DBI_PEAK_MEAN:
        dump_f64(buf, "среднее пик-детектора");
        break;

    case DBI_STIME:
        dumpTimeStamp(buf, "Время начала сканирования");
        break;
    case DBI_ETIME:
        dumpTimeStamp(buf, "Время конца сканирования");
        break;
    case DBI_CTIME:
        dump_u32(buf, "время получения выборки");
        break;

    case DBI_INDEX:
        dump_s32(buf, "номер выборки");
        break;
    case DBI_OFFSET:
        dump_s64(buf, "смещение выборки в файле");
        break;
    case DBI_SIZE:
        dump_u32(buf, "размер выборки в файле");
        break;

     case DBI_DATA_GAIN:
        dump_f64(buf, "множитель для перевода FFT в _f64");
        break;
     case DBI_DATA_OFFSET:
        dump_f64(buf, "смещение для перевода FFT в _f64");
        break;
     case DBI_IQDATA_GAIN:
        dump_f64(buf, "множитель для перевода IQ в _f64");
        break;
     case DBI_IQDATA_OFFSET:
        dump_f64(buf, "смещение для перевода IQ в _f64");
        break;
     case DBI_ODATA_GAIN:
        dump_f64(buf, "множитель для перевода LF в _f64");
        break;
     case DBI_ODATA_OFFSET:
        dump_f64(buf, "смещение для перевода LF в _f64");
        break;

     case DBI_X:
        dump_s32(buf, "положение по горизонтали");
        break;
     case DBI_Y:
        dump_s32(buf, "положение по вертикали");
        break;
     case DBI_CX:
        dump_s32(buf, "размер по горизонтали");
        break;
     case DBI_CY:
        dump_s32(buf, "размер по вертикали");
        break;
    case DBI_STYPE:
        dump_u8(buf, "тип сигнала");
        break;
    case DBI_MTYPE:
        dump_u8(buf, "тип модуляции");
        break;
     case DBI_ACOUNT:
        dump_s16(buf, "счётчик активности");
        break;
     case DBI_TCOUNT:
        dump_u32(buf, "счётчик общего числа обнаружений");
        break;
     case DBI_NRCV:
        dump_s32(buf, "количество приёмников, на которых сигнал был найден");
        break;
     case DBI_CHANNEL:
        dump_u16(buf, "номер канала по стандарту");
        break;

     case DBI_CLVL:
        dump_f64(buf, "текущий уровень");
        break;
     case DBI_MLVL:
        dump_f64(buf, "максимальный уровень");
        break;
     case DBI_RLVL:
        dump_f64(buf, "опорный уровень");
        break;
     case DBI_NLVL:
        dump_f64(buf, "уровень шума");
        break;

     case DBI_AZIMUTH:
        dump_s16(buf, "азимут");
        break;
     case DBI_ELEVATION:
        dump_s16(buf, "угол места");
        break;
     case DBI_DTIME:
        dumpTimeStamp(buf, "Время обнаружения");
        break;
     case DBI_FTIME:
        dumpTimeStamp(buf, "Время первого появления");
        break;
     case DBI_LTIME:
        dumpTimeStamp(buf, "Время последнего появления");
        break;
     case DBI_MTIME:
        dumpTimeStamp(buf, "Время максимального уровня");
        break;

     case DBI_VFILTER:
        dump_u16(buf, "видеофильтр");
        break;
     case DBI_RANGE_NUM:
        dump_u16(buf, "номер диапазона");
        break;
     case DBI_RBW:
        dump_f64(buf, "разрешение Гц/отсчет");
        break;
     case DBI_NOISE_S32:
        dump_s32(buf, "уровень шума дБм (32)");
        break;
     case DBI_NOISE_F64:
        dump_f64(buf, "уровень шума дБм (64)");
        break;
     case DBI_BANDWIDTH:
        dump_f64(buf, "полоса сигнала Гц");
        break;

     case DBI_FLAGS:
        dump_u32(buf, "флаги");
        break;
     case DBI_GROUP1:
        dump_s32(buf, "ID группы 1");
        break;
     case DBI_MASK:
        dump_u32(buf, "маска");
        break;
     case DBI_GROUP2:
        dump_s32(buf, "ID группы 2");
        break;

// Opt структура
     case DBI_AMPL:
        dump_f64(buf, "Амплитуда");
        break;
     case DBI_ATTEN:
        dump_f64(buf, "аттенюатор");
        break;
     case DBI_REF_LEVEL:
        dump_f64(buf, "опорный уровень");
        break;
     case DBI_NSAMPLES:
        dump_u32(buf, "количество отсчетов");
        break;
     case DBI_BSAMPLES:
        dump_u32(buf, "количество отсчетов в полосе");
        break;
     case DBI_VFMIN:
        dump_f64(buf, "");
        break;
     case DBI_VFMAX:
        dump_f64(buf, "");
        break;
     case DBI_AVGNUM:
        dump_u32(buf, "число усреднений");
        break;
     case DBI_AVGTYPE:
        dump_u32(buf, "тип усреднения");
        break;
     case DBI_FFTWIN:
        dump_u32(buf, "тип окна БПФ");
        break;
     case DBI_DBW:
        dump_f64(buf, "ширина полосы демодуляции");
        break;
     case DBI_VBW:
        dump_f64(buf, "разрешение по частоте");
        break;
     case DBI_BW:
        dump_f64(buf, "ширина полосы");
        break;
     case DBI_THRW:
        dump_f64(buf, "порог водопада");
        break;
     case DBI_THRR:
        dump_f64(buf, "порог записи");
        break;
     case DBI_THRS:
        dump_f64(buf, "порог обнаружения относительно уровня шума");
        break;
     case DBI_THR_TYPE:
        dump_u32(buf, "тип порога записи");
        break;
     case DBI_THR_APASS:
        dump_u32(buf, "число превышений для активности");
        break;
     case DBI_THR_IAPASS:
        dump_u32(buf, "число проходов без превышения для неактивности");
        break;
     case DBI_SCALE:
        dump_f64(buf, "масштаб по оси частот");
        break;
     case DBI_SOD_SCALE:
        dump_u32(buf, "режим выбора границ спектра осциллографа детектора");
        break;
     case DBI_DMD_TYPE:
        dump_u32(buf, "тип демодулятора");
        break;
     case DBI_DBANDWIDTH:
        dump_f64(buf, "полоса демодулятора");
        break;
     case DBI_YFMIN:
        dump_f64(buf, "Нижняя граница спектра");
        break;
     case DBI_YFMAX:
        dump_f64(buf, "Верхняя граница спектра");
        break;
     case DBI_TGRAM_MIN:
        dump_f64(buf, "Нижняя граница топограмы");
        break;
     case DBI_TGRAM_MAX:
        dump_f64(buf, "Верхняя граница топограмы");
        break;
     case DBI_TRACES:
        dump_u32(buf, "флаги трасс");
        break;
// каналы
     case DBI_CHAN_ASTATES:
        dump_s32(buf, "количество состояний каналов");
        break;
     case DBI_CHAN_NPARTS:
        dump_s32(buf, "количество частей столбца");
        break;
// пользователи
     case DBI_USER_ID:
        dump_u32(buf, "ID пользователя");
        break;
// проекты
     case DBI_PROJ_FLAGS:
        dump_u32(buf, "флаги проекта");
        break;
     case DBI_PROJ_CREATED:
        dump_s64(buf, "когда создан");
        break;
     case DBI_PROJ_CREATED_BY:
        dump_u32(buf, "кем создан");
        break;
     case DBI_PROJ_OPENED:
        dump_s64(buf, "когда открыт последний раз");
        break;
     case DBI_PROJ_OPENED_BY:
        dump_u32(buf, "кем открыт последний раз");
        break;
     case DBI_PROJ_DEF_RCV:
        dump_s32(buf, "приёмник по умолчанию (панорама)");
        break;
     case DBI_PROJ_DEF_ARCV:
        dump_s32(buf, "приёмник по умолчанию (панорама)");
        break;
// приёмники
     case DBI_RCV_FLAGS:
        dump_u32(buf, "флаги приёмника");
        break;
     case DBI_RCV_COLOR:
        dump_u32(buf, "цвет приёмника");
        break;
     case DBI_RCV_CHN_CVR_UIN:
        dump_u64(buf, "ID конвертера канала приёмника");
        break;
     case DBI_RCV_CHN_FLAGS:
        dump_u32(buf, "флаги канала приёмника");
        break;
// правила
     case DBI_RULE_FREQ_USE:
        dump_u32(buf, "использование границ частоты");
        break;
     case DBI_RULE_BAND_USE:
        dump_u32(buf, "использование границ полосы");
        break;
     case DBI_RULE_REFD_USE:
        dump_s32(buf, "использование опорного уровня");
        break;
     case DBI_RULE_STAT_USE:
        dump_s32(buf, "использование стационарности");
        break;
     case DBI_RULE_LAST_USE:
        dump_u32(buf, "использование последнего времени");
        break;
     case DBI_RULE_DESC_USE:
        dump_u32(buf, "использование нового описания");
        break;
// сигналы/связи
     case DBI_SGN_COUNT:
        dump_u32(buf, "количество сигналов");
        break;
     case DBI_LNK_COUNT:
        dump_u32(buf, "количество связей");
        break;
     case DBI_NET_COUNT:
        dump_u32(buf, "количество связей");
        break;
     case DBI_TYPE:
        dump_u32(buf, "логический тип устройства / тип сигнала");
        break;
     case DBI_STATE:
        dump_u32(buf, "дополнительные данные о состоянии");
        break;
     case DBI_DEV_CHANEL:
        dump_u32(buf, "дополнительные данные о состоянии");
        break;
     case DBI_BT_FEAT:
        dump_u32(buf, "особенности Bluetooth");
        break;
     case DBI_ID:
        dump_u64(buf, "идентификатор");
        break;
     case DBI_ID2:
        dump_u64(buf, "идентификатор 2");
        break;
     case DBI_ID3:
        dump_u64(buf, "идентификатор 3");
        break;
     case DBI_ID4:
        dump_u64(buf, "идентификатор 4");
        break;
     case DBI_DATA_TX:
        dump_u64(buf, "передано");
        break;
     case DBI_DATA_RX:
        dump_u64(buf, "принято");
        break;
     case DBI_BTS_IMEI:
        dump_u64(buf, "IMEI");
        break;

     case DBI_DEV_TYPE:
        dump_s32(buf, "тип устройства");
        break;
     case DBI_LINKS:
        dump_s32(buf, "количество связей");
        break;

     case DBI_WF_STD:
        dump_u16(buf, "стандарт WiFi");
        break;
     case DBI_WF_SEC:
        dump_u16(buf, "безопасность WiFi");
        break;
     case DBI_ZB_PAN:
        dump_u16(buf, "ID сети ZigBee");
        break;
     case DBI_CELL_LAC:
        dump_u16(buf, "LAC");
        break;
     case DBI_CELL_BSIC:
        dump_u16(buf, "BSIC");
        break;
     case DBI_BTS_MCC:
        dump_u16(buf, "MCC");
        break;
     case DBI_BTS_MNC:
        dump_u16(buf, "MNC");
        break;
     case DBI_EOF:
        dump_u8(buf, "конец файла");
        break;
    default:
//        nb = (varType >> 24) & 0x0F;
        *buf += nb;
        printf("%16x\t - ?????\n", varType);
    }

    return true;
}
