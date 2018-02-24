#include "ctape.h"
 
//-Конструктор класса--------------------------------------------------------
CTape::CTape(void)
{
 BlockOffset=-1;
 BlockIndex=0;
 OutCounter=0;
 Out=0;
}
//-Деструктор класса---------------------------------------------------------
CTape::~CTape()
{
 vector_SBlock.clear();
}
//-Замещённые функции предка-------------------------------------------------
//-Новые функции класса-----------------------------------------------------

//----------------------------------------------------------------------------------------------------
//загрузить файл с данными
//----------------------------------------------------------------------------------------------------
bool CTape::LoadTAP(const char *FileName)
{
 BlockOffset=-1;
 BlockIndex=0;

 long n;
 vector_SBlock.clear();
 FILE *file=fopen(FileName,"rb");
 if (file==NULL) return(false);
 while(1)
 { 
  unsigned short length;
  if (fread(&length,1,sizeof(unsigned short),file)<sizeof(unsigned short)) break;
  SBlock sBlock;  
  //создаём пилот-тон
  for(n=0;n<PILOT_COUNTER;n++)
  {
   sBlock.vector_Tape.push_back((1<<15)|DELAY_PILOT);
   sBlock.vector_Tape.push_back((0<<15)|DELAY_PILOT);
  }
  //синхросигнал
  sBlock.vector_Tape.push_back((1<<15)|DELAY_SYNCHRO_1);
  sBlock.vector_Tape.push_back((0<<15)|DELAY_SYNCHRO_2);
  //данные
  for(n=0;n<length;n++)
  {
   unsigned char b;
   if (fread(&b,1,sizeof(unsigned char),file)<sizeof(unsigned char)) break;
   sBlock.vector_Data.push_back(b);
   unsigned char mask=128;
   for(long m=0;m<8;m++,mask>>=1)
   {
    if (b&mask)
	{
     sBlock.vector_Tape.push_back((1<<15)|DELAY_ONE);
     sBlock.vector_Tape.push_back((0<<15)|DELAY_ONE);
	}
	else
	{
     sBlock.vector_Tape.push_back((1<<15)|DELAY_ZERO);
     sBlock.vector_Tape.push_back((0<<15)|DELAY_ZERO);
	}
   }
  }
  //добавляем паузу
  for(n=0;n<PAUSE_COUNTER;n++) sBlock.vector_Tape.push_back((1<<15)|DELAY_PAUSE);
  //анализируем данные блока
  sBlock.BlockType=BLOCK_TYPE_UNKNOW;
  sBlock.Addr=0;
  sBlock.Length=0;
  strcpy(sBlock.Name,"");
  if (length==19)//это заголовок
  {
   if (sBlock.vector_Data[0]==0)//размер стандартный
   {
	{
     //тип
	 unsigned char type=sBlock.vector_Data[1];
     if (type==0) sBlock.BlockType=BLOCK_TYPE_BASIC;
     if (type==1) sBlock.BlockType=BLOCK_TYPE_NUMBER_ARRAY;
     if (type==2) sBlock.BlockType=BLOCK_TYPE_SYMBOL_ARRAY;
     if (type==3) sBlock.BlockType=BLOCK_TYPE_CODE;
	 //имя
     for(n=0;n<10;n++) sBlock.Name[n]=sBlock.vector_Data[2+n];
	 sBlock.Name[10]=0;
	 //длина
     sBlock.Length=sBlock.vector_Data[13];
     sBlock.Length<<=8;
     sBlock.Length|=sBlock.vector_Data[12];
	 if (sBlock.BlockType==BLOCK_TYPE_CODE)
	 {
	  //адрес
      sBlock.Addr=sBlock.vector_Data[15];
      sBlock.Addr<<=8;
      sBlock.Addr|=sBlock.vector_Data[14];
	 }
	 if (sBlock.BlockType==BLOCK_TYPE_BASIC)
	 {
	  //адрес
      sBlock.Addr=sBlock.vector_Data[17];
      sBlock.Addr<<=8;
      sBlock.Addr|=sBlock.vector_Data[16];
	 }
	}
   }
  }
  vector_SBlock.push_back(sBlock);
 }
 fclose(file);
 return(true);
}

//----------------------------------------------------------------------------------------------------
//воспроизвести файл с данными
//----------------------------------------------------------------------------------------------------
void CTape::Start(long blockindex)
{
 BlockOffset=0;
 BlockIndex=blockindex;
 OutCounter=0;
 Out=0;
}
//----------------------------------------------------------------------------------------------------
//остановить воспроизведение
//----------------------------------------------------------------------------------------------------
void CTape::Stop(void)
{
 BlockOffset=-1;
}
//----------------------------------------------------------------------------------------------------
//получить состояние магнитофонного входа
//----------------------------------------------------------------------------------------------------
unsigned char CTape::GetTapeState(void)
{
 if (BlockIndex<0 || BlockOffset<0) return(Out);
 long block_size=vector_SBlock.size();
 if (block_size<=BlockIndex) return(Out);//запись закончилась
 if (OutCounter>0)
 {
  OutCounter--;
  return(Out);
 }
 //смена байта
 long size=vector_SBlock[BlockIndex].vector_Tape.size();
 if (size<=BlockOffset)//переходим к следующему блоку
 {
  BlockIndex++;
  BlockOffset=0;
  return(Out);
 }
 unsigned short block=vector_SBlock[BlockIndex].vector_Tape[BlockOffset];
 Out=(block>>15)&0x01;
 OutCounter=block&0x7FFF;
 BlockOffset++;
 return(Out);
}
//----------------------------------------------------------------------------------------------------
//получить указатель на вектор блоков
//----------------------------------------------------------------------------------------------------
vector<SBlock>* CTape::GetBlockVectorPtr(void)
{
 return(&vector_SBlock);
}

//----------------------------------------------------------------------------------------------------
//получить индекс текущего воспроизводимого блока
//----------------------------------------------------------------------------------------------------
long CTape::GetCurrentBlockIndex(void)
{
 return(BlockIndex);
}
//-Прочее--------------------------------------------------------------------

