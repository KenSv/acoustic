#ifndef RIQ_H
#define RIQ_H
typedef unsigned char   _u8;
typedef signed char     _s8;
typedef short           _s16;
typedef unsigned short  _u16;
typedef unsigned int    _u32;
typedef int             _s32;
typedef long            _s64;
typedef unsigned long   _u64;
typedef double          _f64;


bool parseVar(_u8** buf, _u8** pRiq);
bool parseArray(_u8** buf, _u8** pRiq, float percent);
void dumpArray(_u8** buf, unsigned int bytes, unsigned int items, unsigned short itemsOnLine);
void dumpTimeStamp(_u8** buf, const char* msg);
void fiterBlackman(_u8** buf, const double in[], double out[], int sizeIn);
void fiterKalman(_u8* pIn, _f64* pOut, int block_size, float percent = 100);
void filterSimple(_u8* pIn, _f64* pOut, int block_size, float percent = 100);

#endif // RIQ_H
