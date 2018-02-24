#ifndef CTAPE
#define CTAPE

#include "stdafx.h"

#include <vector>
using namespace std;

//настройки длительности тонв в тактах
#define DELAY_PAUSE     10000
#define PAUSE_COUNTER   1000
#define PILOT_COUNTER   2500
#define DELAY_PILOT     2168
#define DELAY_ZERO      855
#define DELAY_SYNCHRO_1 667
#define DELAY_SYNCHRO_2 735
#define DELAY_ONE       1710

enum BLOCK_TYPE
{
 //блок - бейсик программа
 BLOCK_TYPE_BASIC,
 //блок - кодовый
 BLOCK_TYPE_CODE,
 //блок - числовой массив
 BLOCK_TYPE_NUMBER_ARRAY,
 //блок - символьный массив
 BLOCK_TYPE_SYMBOL_ARRAY,
 //тип блока неизвестен
 BLOCK_TYPE_UNKNOW
};

struct SBlock
{
 vector<unsigned short> vector_Tape;//данные блока в формате магнитофона
 vector<unsigned char> vector_Data;//считанные данные блока
 BLOCK_TYPE BlockType;//тип блока
 char Name[11];//название блока, если его возможно прочесть
 unsigned short Addr;//адрес блока
 unsigned short Length;//длина блока
};

class CTape
{
 protected:
  //-Переменные класса-------------------------------------------------------
  vector<SBlock> vector_SBlock;//данные блоков
  unsigned char Out;//состояние выхода магнитофона
  long OutCounter;//счётчик до смены состояния выхода магнитофона
  long BlockIndex;//индекс блока
  long BlockOffset;//смещение внутри блока
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CTape(void);
  //-Деструктор класса-------------------------------------------------------
  ~CTape();
  //-Переменные класса-------------------------------------------------------
  //-Замещённые функции предка-----------------------------------------------
  bool LoadTAP(const char *FileName);//загрузить файл с данными
  void Start(long blockindex);//воспроизвести файл с данными
  void Stop(void);//остановить воспроизведение
  unsigned char GetTapeState(void);//получить состояние магнитофонного входа
  vector<SBlock> *GetBlockVectorPtr(void);//получить указатель на вектор блоков
  long GetCurrentBlockIndex(void);//получить индекс текущего воспроизводимого блока
  //-Новые функции класса----------------------------------------------------
 protected: 

  //-Прочее------------------------------------------------------------------
};
#endif
