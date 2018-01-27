#pragma once
//---------------------------------------------------------------------
//#if !defined(_WIN32) && !defined(_LINUX)
#if !defined(_WIN32) && !defined(__linux)
#error "OS not specified!"
#endif
//---------------------------------------------------------------------
// верхний байт - тип
#define DBIT_ARRAY			0x10000000	// массив
#define DBIT_ARRAY2			0x20000000	// 2D массив
#define DBIT_ARRAY3			0x30000000	// 3D массив
#define DBIT_INT			0x40000000	// целочисленный
#define DBIT_UNSIGNED		0x80000000	// беззнаковый

#define DBIT_S8				(0x01000000|DBIT_INT)
#define DBIT_U8				(DBIT_S8|DBIT_UNSIGNED)
#define DBIT_AS8			(DBIT_S8|DBIT_ARRAY)
#define DBIT_AU8			(DBIT_S8|DBIT_UNSIGNED|DBIT_ARRAY)

#define DBIT_S16			(0x02000000|DBIT_INT)
#define DBIT_U16			(DBIT_S16|DBIT_UNSIGNED)
#define DBIT_AS16			(DBIT_S16|DBIT_ARRAY)
#define DBIT_AU16			(DBIT_S16|DBIT_UNSIGNED|DBIT_ARRAY)

#define DBIT_S32			(0x04000000|DBIT_INT)
#define DBIT_U32			(DBIT_S32|DBIT_UNSIGNED)
#define DBIT_AS32			(DBIT_S32|DBIT_ARRAY)
#define DBIT_AU32			(DBIT_S32|DBIT_UNSIGNED|DBIT_ARRAY)

#define DBIT_S64			(0x08000000|DBIT_INT)
#define DBIT_U64			(DBIT_S64|DBIT_UNSIGNED)
#define DBIT_AS64			(DBIT_S64|DBIT_ARRAY)
#define DBIT_AU64			(DBIT_S64|DBIT_UNSIGNED|DBIT_ARRAY)

#define DBIT_F32			0x04000000
#define DBIT_AF32			(DBIT_F32|DBIT_ARRAY)

#define DBIT_F64			0x08000000
#define DBIT_AF64			(DBIT_F64|DBIT_ARRAY)
#define DBIT_A2F64			(DBIT_F64|DBIT_ARRAY2)
//---------------------------------------------------------------------
#define DBI_EOF				0						// конец файла
#define DBI_NULL			(0x00000000|DBIT_AU8)	// пустые данные
#define DBI_VERSION			(0x00000001|DBIT_U32)	// версия DB API
#define DBI_SFLAGS			(0x00000002|DBIT_U32)	// системные флаги
#define DBI_UFLAGS			(0x00000003|DBIT_U32)	// пользовательские флаги

#define DBI_UIN				(0x00000008|DBIT_U64)	// Уникальный номер
#define DBI_UID				(0x00000009|DBIT_U64)	// Уникальный идентификатор
#define DBI_PROJECTID		(0x0000000A|DBIT_U64)	// Уникальный идентификатор проекта
#define DBI_SCANID			(0x0000000B|DBIT_U32)	// Идентификатор сканирования

#define DBI_FMIN			(0x00000010|DBIT_F64)	// начальная частота (МГц)
#define DBI_FMAX			(0x00000011|DBIT_F64)	// конечная частота (МГц)
#define DBI_FCUR			(0x00000012|DBIT_F64)	// центральная частота (МГц)
#define DBI_BAND			(0x00000013|DBIT_F64)	// полоса (МГц)
#define DBI_GFMIN			(0x00000014|DBIT_F64)	// общая начальная частота (МГц)
#define DBI_GFMAX			(0x00000015|DBIT_F64)	// общая конечная частота (МГц)
#define DBI_FDIFF			(0x00000016|DBIT_F64)	// отклонение частоты (МГц)

#define DBI_COUNT			(0x00000018|DBIT_S32)	// число выборок в данном сканировании ??? skn число выборок со знаком ???
#define DBI_SAMPLES			(0x00000019|DBIT_U32)	// число отсчётов в каждой выборке
#define DBI_TOTAL_SAMPLES	(0x00000019|DBIT_U64)	// общее число отсчётов
#define DBI_RBW_KHZ			(0x00000020|DBIT_U16)	// rbw в кГц

#define DBI_IQDATA			0x00000020
#define DBI_FFT				0x00000022
#define DBI_TGRAM			0x00000023
#define DBI_ODATA			0x00000024

#define DBI_IQDATA_S16		(DBI_IQDATA|DBIT_AS16)	// Q-канал (_s16)
#define DBI_IQDATA_F64		(DBI_IQDATA|DBIT_AF64)	// IQ-данные (_f64)
#define DBI_ODATA_S16		(DBI_ODATA|DBIT_AS16)	// осциллограф (_s16)
#define DBI_ODATA_F64		(DBI_ODATA|DBIT_AF64)	// осциллограф (_f64)

#define DBI_FFT_F64			(DBI_FFT|DBIT_AF64)		// спектр (_f64)
#define DBI_FFT_U16			(DBI_FFT|DBIT_AU16)		// спектр (_s16)
#define DBI_FFT_U8			(DBI_FFT|DBIT_AU8)		// спектр (_u8)
#define DBI_FFT_F64_2D		(DBI_FFT|DBIT_A2F64)	// 2D спектр (_f64)

#define DBI_TGRAM_F64		(DBI_TGRAM|DBIT_A2F64)	// 2D топограма (_f64)

#define DBI_NPACK			(0x00000023|DBIT_S32)	// номер выборки в пачке
#define DBI_NPACKS			(0x00000024|DBIT_S32)	// всего выборок в пачке

#define DBI_PEAK_MAX		(0x00000025|DBIT_F64)	// максимум пик-детектора
#define DBI_PEAK_MEAN		(0x00000026|DBIT_F64)	// среднее пик-детектора

#define DBI_STIME			(0x00000028|DBIT_S64)	// время начала сканирования
#define DBI_ETIME			(0x00000029|DBIT_S64)	// время конца сканирования
#define DBI_CTIME			(0x0000002A|DBIT_U32)	// время получения выборки

#define DBI_INDEX			(0x00000030|DBIT_S32)	// номер выборки
#define DBI_OFFSET			(0x00000031|DBIT_S64)	// смещение выборки в файле
#define DBI_SIZE			(0x00000032|DBIT_U32)	// размер выборки в файле

#define DBI_DATA_GAIN		(0x00000034|DBIT_F64)	// множитель для перевода FFT в _f64
#define DBI_DATA_OFFSET		(0x00000035|DBIT_F64)	// смещение для перевода FFT в _f64
#define DBI_IQDATA_GAIN		(0x00000036|DBIT_F64)	// множитель для перевода IQ в _f64
#define DBI_IQDATA_OFFSET	(0x00000037|DBIT_F64)	// смещение для перевода IQ в _f64
#define DBI_ODATA_GAIN		(0x00000038|DBIT_F64)	// множитель для перевода LF в _f64
#define DBI_ODATA_OFFSET	(0x00000039|DBIT_F64)	// смещение для перевода LF в _f64

#define DBI_X				(0x0000003A|DBIT_S32)	// положение по горизонтали
#define DBI_Y				(0x0000003B|DBIT_S32)	// положение по вертикали
#define DBI_CX				(0x0000003C|DBIT_S32)	// размер по горизонтали
#define DBI_CY				(0x0000003D|DBIT_S32)	// размер по вертикали
#define DBI_BYTEARRAY		(0x0000003E|DBIT_AU8)	// массив данных

#define DBI_STYPE			(0x00000040|DBIT_U8)	// тип сигнала
#define DBI_MTYPE			(0x00000041|DBIT_U8)	// тип модуляции
#define DBI_ACOUNT			(0x00000042|DBIT_S16)	// счётчик активности
#define DBI_TCOUNT			(0x00000043|DBIT_U32)	// счётчик общего числа обнаружений
#define DBI_NRCV			(0x00000044|DBIT_S32)	// количество приёмников, на которых сигнал был найден
#define DBI_DESC_S8			(0x00000045|DBIT_AS8)	// описание
#define DBI_DESC_U16		(0x00000045|DBIT_AU16)	// описание
#define DBI_FRAMES			(0x00000046|DBIT_AU32)	// типы представления фреймов в каждом окне
#define DBI_MD_NAME			(0x00000047|DBIT_AS8)	// имя мультимедиа файла
#define DBI_CHANNEL			(0x00000049|DBIT_U16)	// номер канала по стандарту

#define DBI_CLVL			(0x00000050|DBIT_F64)	// текущий уровень
#define DBI_MLVL			(0x00000051|DBIT_F64)	// максимальный уровень
#define DBI_RLVL			(0x00000052|DBIT_F64)	// опорный уровень
#define DBI_NLVL			(0x00000053|DBIT_F64)	// уровень шума

#define DBI_AZIMUTH			(0x00000052|DBIT_S16)	// азимут
#define DBI_ELEVATION		(0x00000053|DBIT_S16)	// угол места

#define DBI_DTIME			(0x00000055|DBIT_S64)	// время обнаружения
#define DBI_FTIME			(0x00000056|DBIT_S64)	// время первого появления
#define DBI_LTIME			(0x00000057|DBIT_S64)	// время последнего появления
#define DBI_MTIME			(0x00000058|DBIT_S64)	// время максимального уровня

#define DBI_CHAN_FFT		(0x00000002|DBIT_A2F64)	// канальный спектр
#define DBI_SIGN_MASK 		(0x00000003|DBIT_AF64)	// маска сигнатуры
#define DBI_NAME_S8			(0x00000060|DBIT_AS8)	// имя
#define DBI_NAME_U16		(0x00000060|DBIT_AU16)	//
#define DBI_GROUPNAME		(0x00000061|DBIT_AU16)	// название группы эталона
#define DBI_VFILTER			(0x00000062|DBIT_U16)	// видеофильтр
#define DBI_RANGE_NUM		(0x00000063|DBIT_U16)	// номер диапазона
#define DBI_RBW				(0x00000064|DBIT_F64)	// разрешение Гц/отсчет
#define DBI_NOISE_S32		(0x00000065|DBIT_S32)	// уровень шума дБм
#define DBI_NOISE_F64		(0x00000065|DBIT_F64)	// уровень шума дБм
#define DBI_BANDWIDTH		(0x00000066|DBIT_F64)	// полоса сигнала Гц
#define DBI_SUBGROUPNAME	(0x00000067|DBIT_AU16)	// название под-группы эталона

#define DBI_FLAGS			(0x0000006A|DBIT_U32)	// флаги
#define DBI_GROUP1			(0x0000006A|DBIT_S32)	// ID группы 1
#define DBI_MASK			(0x0000006B|DBIT_U32)	// маска
#define DBI_GROUP2			(0x0000006B|DBIT_S32)	// ID группы 2

// Opt структура
#define DBI_AMPL 			(0x00000070|DBIT_F64)	// Амплитуда
#define DBI_ATTEN 			(0x00000071|DBIT_F64)	// аттенюатор
#define DBI_REF_LEVEL 		(0x00000072|DBIT_F64)	// опорный уровень
#define DBI_NSAMPLES		(0x00000075|DBIT_U32)	// количество отсчетов
#define DBI_BSAMPLES		(0x00000076|DBIT_U32)	// количество отсчетов в полосе
#define DBI_VFMIN 			(0x00000077|DBIT_F64)	// минимальная видимая граница частот
#define DBI_VFMAX 			(0x00000078|DBIT_F64)	// максимальная видимая граница частот
#define DBI_AVGNUM			(0x00000079|DBIT_U32)	// число усреднений
#define DBI_AVGTYPE			(0x0000007A|DBIT_U32)	// тип усреднения
#define DBI_FFTWIN			(0x0000007B|DBIT_U32)	// тип окна БПФ
#define DBI_DBW 			(0x0000007C|DBIT_F64)	// ширина полосы демодуляции
#define DBI_VBW 			(0x0000007D|DBIT_F64)	// разрешение по частоте
#define DBI_BW 				(0x0000007F|DBIT_F64)	// ширина полосы
#define DBI_THRW			(0x00000080|DBIT_F64)	// порог водопада
#define DBI_THRR			(0x00000081|DBIT_F64)	// порог записи
#define DBI_THRR_OFFSETS	(0x00000081|DBIT_AF64)	// смещения порога записи
#define DBI_THRS			(0x00000082|DBIT_F64)	// порог обнаружения относительно уровня шума
#define DBI_THR_TYPE		(0x00000083|DBIT_U32)	// тип порога записи
#define DBI_THR_APASS		(0x00000085|DBIT_U32)	// число превышений для активности
#define DBI_THR_IAPASS		(0x00000086|DBIT_U32)	// число проходов без превышения для неактивности
#define DBI_SCALE			(0x00000087|DBIT_F64)	// масштаб по оси частот
#define DBI_SOD_SCALE		(0x00000088|DBIT_U32)	// режим выбора границ спектра осциллографа детектора
#define DBI_DMD_TYPE		(0x00000089|DBIT_U32)	// тип демодулятора
#define DBI_DBANDWIDTH		(0x0000008A|DBIT_F64)	// полоса демодулятора
#define DBI_YFMIN 			(0x0000008B|DBIT_F64)	// Нижняя граница спектра
#define DBI_YFMAX 			(0x0000008C|DBIT_F64)	// Верхняя граница спектра
#define DBI_SPLITTERS		(0x0000008D|DBIT_AF32)	// разделители окна
#define DBI_TGRAM_MIN 		(0x0000008E|DBIT_F64)	// Нижняя граница топограмы
#define DBI_TGRAM_MAX 		(0x0000008F|DBIT_F64)	// Верхняя граница топограмы
#define DBI_TRACES			(0x00000090|DBIT_U32)	// флаги трасс

// каналы
#define DBI_CHAN_STATES		(0x00000100|DBIT_AU64)	// состояния каналов
#define DBI_CHAN_ASTATES	(0x00000101|DBIT_S32)	// количество состояний каналов
#define DBI_CHAN_NPARTS		(0x00000105|DBIT_S32)	// количество частей столбца

// пользователи
#define DBI_USER_ID			(0x00000110|DBIT_U32)	// ID пользователя
#define DBI_USER_LOGIN		(0x00000110|DBIT_AS8)	// логин пользователя
#define DBI_USER_NAME		(0x00000111|DBIT_AS8)	// полное имя пользователя
#define DBI_USER_AVATAR		(0x00000110|DBIT_AU32)	// аватар пользователя
#define DBI_USER_PASSWD		(0x00000110|DBIT_AU8)	// хеш пароля пользователя

// проекты
#define DBI_PROJ_NAME		(0x00000120|DBIT_AS8)	// имя проекта
#define DBI_PROJ_PASSWD		(0x00000120|DBIT_AU8)	// хеш пароля проекта
#define DBI_PROJ_FLAGS		(0x00000120|DBIT_U32)	// флаги проекта
#define DBI_PROJ_CREATED	(0x00000120|DBIT_S64)	// когда создан
#define DBI_PROJ_CREATED_BY	(0x00000121|DBIT_U32)	// кем создан
#define DBI_PROJ_OPENED		(0x00000121|DBIT_S64)	// когда открыт последний раз
#define DBI_PROJ_OPENED_BY	(0x00000122|DBIT_U32)	// кем открыт последний раз
#define DBI_PROJ_DATA_DIR	(0x00000121|DBIT_AS8)	// папка для сохранения файлов
#define DBI_PROJ_DEF_RCV	(0x00000120|DBIT_S32)	// приёмник по умолчанию (панорама)
#define DBI_PROJ_DEF_ARCV	(0x00000121|DBIT_S32)	// приёмник по умолчанию (панорама)
#define DBI_PROJ_OPTIONS	(0x00000122|DBIT_AU8)	// опции проекта

// приёмники
#define DBI_RCV_FLAGS		(0x00000130|DBIT_U32)	// флаги приёмника
#define DBI_RCV_NAME		(0x00000130|DBIT_AS8)	// имя приёмника
#define DBI_RCV_COLOR		(0x00000131|DBIT_U32)	// цвет приёмника
#define DBI_RCV_CHN_NAME	(0x00000132|DBIT_AS8)	// имя канала приёмника
#define DBI_RCV_CHN_CVR_UIN	(0x00000132|DBIT_U64)	// ID конвертера канала приёмника
#define DBI_RCV_CHN_FLAGS	(0x00000132|DBIT_U32)	// флаги канала приёмника

// правила
#define DBI_RULE_FREQ_USE	(0x00000140|DBIT_U32)	// использование границ частоты
#define DBI_RULE_BAND_USE	(0x00000141|DBIT_U32)	// использование границ полосы
#define DBI_RULE_REFD_USE	(0x00000140|DBIT_S32)	// использование опорного уровня
#define DBI_RULE_STAT_USE	(0x00000141|DBIT_S32)	// использование стационарности
#define DBI_RULE_LAST_USE	(0x00000142|DBIT_U32)	// использование последнего времени
#define DBI_RULE_DESC_USE	(0x00000143|DBIT_U32)	// использование нового описания

// сигналы/связи
#define DBI_SGN_COUNT		(0x00000150|DBIT_U32)	// количество сигналов
#define DBI_LNK_COUNT		(0x00000151|DBIT_U32)	// количество связей
#define DBI_NET_COUNT		(0x00000152|DBIT_U32)	// количество связей
#define DBI_TYPE			(0x00000153|DBIT_U32)	// логический тип устройства / тип сигнала
#define DBI_STATE			(0x00000154|DBIT_U32)	// дополнительные данные о состоянии
#define DBI_DEV_CHANEL		(0x00000155|DBIT_U32)	// дополнительные данные о состоянии
#define DBI_BT_FEAT			(0x00000156|DBIT_U32)	// особенности Bluetooth

#define DBI_ID				(0x00000150|DBIT_U64)	// идентификатор
#define DBI_ID2				(0x00000151|DBIT_U64)	// идентификатор 2
#define DBI_ID3				(0x00000152|DBIT_U64)	// идентификатор 3
#define DBI_ID4				(0x00000153|DBIT_U64)	// идентификатор 4
#define DBI_DATA_TX			(0x00000154|DBIT_U64)	// передано
#define DBI_DATA_RX			(0x00000155|DBIT_U64)	// принято
#define DBI_BTS_IMEI		(0x00000157|DBIT_U64)	// IMEI

#define DBI_DEV_TYPE		(0x00000150|DBIT_S32)	// тип устройства
#define DBI_LINKS			(0x00000151|DBIT_S32)	// количество связей

#define DBI_WF_STD			(0x00000150|DBIT_U16)	// стандарт WiFi
#define DBI_WF_SEC			(0x00000151|DBIT_U16)	// безопасность WiFi
#define DBI_ZB_PAN			(0x00000152|DBIT_U16)	// ID сети ZigBee
#define DBI_CELL_LAC		(0x00000154|DBIT_U16)	// LAC
#define DBI_CELL_BSIC		(0x00000155|DBIT_U16)	// BSIC
#define DBI_BTS_MCC			(0x00000156|DBIT_U16)	// MCC
#define DBI_BTS_MNC			(0x00000157|DBIT_U16)	// MNC

#define DBI_GLOBAL_ID		0x00001000
#define DBI_MAX_ID			0x00FFFFFF
#define MKDBI_GLOBAL_ID(cnt,type)		(min(DBI_GLOBAL_ID + (cnt), DBI_MAX_ID) | (type))
//---------------------------------------------------------------------
#define DB_GETITEMID(v)		(v & DBI_MAX_ID)
#define DB_GETITEMTYPE(v)	(v & 0xFF000000)
#define DB_GETITEMSIZE(v)	((v & 0x0F000000) >> 24)
#define DB_GETARRAYDIM(v)	((v & 0x30000000) >> 28)
//---------------------------------------------------------------------
#define DBF_IQ				0x00000001		// I данные
#define DBF_FFT 			0x00000004		// спектр
#define DBF_MULTIRANGE		0x00000010		// несколько полос
#define DBF_FIRST_PART		0x00001000		// первый кусок
#define DBF_LAST_PART		0x00002000		// первый кусок
#define DBF_TEMPORARY		0x00001000		// временное (удаляется при закрытии)
//---------------------------------------------------------------------
#define DB_API_VERSION		1
//---------------------------------------------------------------------
