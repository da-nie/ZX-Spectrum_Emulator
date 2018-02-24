#ifndef CTAPE
#define CTAPE

#include "stdafx.h"

#include <vector>
using namespace std;

//��������� ������������ ���� � ������
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
 //���� - ������ ���������
 BLOCK_TYPE_BASIC,
 //���� - �������
 BLOCK_TYPE_CODE,
 //���� - �������� ������
 BLOCK_TYPE_NUMBER_ARRAY,
 //���� - ���������� ������
 BLOCK_TYPE_SYMBOL_ARRAY,
 //��� ����� ����������
 BLOCK_TYPE_UNKNOW
};

struct SBlock
{
 vector<unsigned short> vector_Tape;//������ ����� � ������� �����������
 vector<unsigned char> vector_Data;//��������� ������ �����
 BLOCK_TYPE BlockType;//��� �����
 char Name[11];//�������� �����, ���� ��� �������� ��������
 unsigned short Addr;//����� �����
 unsigned short Length;//����� �����
};

class CTape
{
 protected:
  //-���������� ������-------------------------------------------------------
  vector<SBlock> vector_SBlock;//������ ������
  unsigned char Out;//��������� ������ �����������
  long OutCounter;//������� �� ����� ��������� ������ �����������
  long BlockIndex;//������ �����
  long BlockOffset;//�������� ������ �����
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CTape(void);
  //-���������� ������-------------------------------------------------------
  ~CTape();
  //-���������� ������-------------------------------------------------------
  //-���������� ������� ������-----------------------------------------------
  bool LoadTAP(const char *FileName);//��������� ���� � �������
  void Start(long blockindex);//������������� ���� � �������
  void Stop(void);//���������� ���������������
  unsigned char GetTapeState(void);//�������� ��������� �������������� �����
  vector<SBlock> *GetBlockVectorPtr(void);//�������� ��������� �� ������ ������
  long GetCurrentBlockIndex(void);//�������� ������ �������� ���������������� �����
  //-����� ������� ������----------------------------------------------------
 protected: 

  //-������------------------------------------------------------------------
};
#endif
