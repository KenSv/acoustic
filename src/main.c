//#include <iostream>
//#include <fstream>
#include <string.h>
#include <unistd.h>
#include "../include/db.h"
#include "../include/Riq.h"
#include "../include/Vkf.h"

// for windows
#include <stdlib.h>
#include <stdio.h>

//using namespace std;



int main(int argc, char* argv[])
{
//    float percent = 100;
    char dirName[1000];
//    long int length;
//    FILE *fRiq, *fIn, *fOut;
//    _u8 *buf, *pRiq;
    char fname[30], f_frag[30];

    printf("Current path: %s\n", getcwd(dirName, sizeof(dirName)));
    switch (argc)
    {
        case 3:
//            percent = atof(argv[2]);
            strcpy(f_frag, argv[1]);
        case 2:
            strcpy(fname, argv[1]);
            break;
        default:
            strcpy(fname, "ring-original.wav");
            break;
    }
    printf("File name: %s\n", fname);


//    fIn = fopen(fname, "rb");
//
//    fOut = fopen("origin.riq", "wb");
//    fseek(fIn, 0L, SEEK_END);
//    length = ftell(fIn);
//
//    printf("File length: %li\n", length);
//    printf("===========================================\n");
//    printf("  Адрес |\tЗначение\tОписание\n");
//    printf("===========================================\n");
//
//    buf = (_u8*) malloc(sizeof(_u8)*length);

//    _f64* os = (_f64*) malloc(sizeof(_f64)*10000);
    wav2array(fname);
//    free(os);

//        rewind(fIn);
//        fread((char *) buf, sizeof(char), length, fIn);
//        pRiq = (_u8*) malloc(sizeof(_u8)*length);
//        _u8* readPtr = buf;
//        _u8* writePtr = pRiq;
//
//        fRiq = fopen("filtered.riq", "wb");
//        while(readPtr < &buf[length]) {
//            printf("%8x ", (unsigned int) (readPtr - buf));
//            if(!parseVar(&readPtr, &writePtr))
//                parseArray(&readPtr, &writePtr, percent);
//        }
//        fwrite(buf, sizeof(_u8), length, fOut);
//        fwrite(pRiq, sizeof(_u8), length, fRiq);
//
//        fclose(fIn);
//        fclose(fOut);
//        fclose(fRiq);
//        free(buf);
//        free(pRiq);
   exit(0);
}


//void getType(char** buf) {
//    unsigned int varType = *((int *) *buf);
//    string typeName = "";
//    if(varType & (DBIT_ARRAY|DBIT_ARRAY2|DBIT_ARRAY3))
//    {
//        typeName = "A";
//        switch (varType & 0x30000000)
//        {
//        case DBIT_ARRAY2:
//            typeName += "2D";
//            break;
//        case DBIT_ARRAY3:
//            typeName += "3D";
//            break;
//        default:
//            typeName += "1D";
//            break;
//        }
//    } else {
//        typeName = "  N";
//    }
//
//    typeName += ((varType & 0xC0000000)? "U": "S");
//    unsigned int nb = (varType >> 24) & 0x0F;
//    typeName +=  (nb == 1? "0": "\0") + to_string(nb << 3);
//
//    *buf += 4;  // указатель на значение переменной
//    long value;
//    int arItems;
//    if (typeName[0] == 'A')
//    {
//        arItems = *((int *) *buf);
//        *buf += sizeof(int) + arItems * nb;  // длина массива следует за типом значения - 4 байта?
//        value = arItems;
//    }
//    else
//    {
//        switch (nb)
//        {
//        case 1:
//            value = *((char *) *buf);
//            break;
//        case 2:
//            value = *((short *) *buf);
//            break;
//        case 4:
//            value = *((int *) *buf);
//            break;
//        case 8:
//            value = *((long *) *buf);
//            break;
//        default:
//            break;
//        }
//        *buf += nb;
//    }
//    printf("Тип: %s\t Код типа: %#08x\t Значение: %li\n", &typeName[0], varType, value);
//}
