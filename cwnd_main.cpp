#include "cwnd_main.h"
#include "main_ROM.h"
 
extern unsigned char ROM[16384];

extern SVideo_DisplaySetting sVideo_DisplaySetting;
extern unsigned char Z80_Memory[65536];

extern unsigned char Z80_ScreenIRQ;
extern unsigned char Z80_TapeInState;
extern unsigned char Z80_KempstonInState;
extern unsigned char Z80_AttributesPort;

extern UZ80_Reg uZ80_Reg;
extern UZ80_A_Reg uZ80_A_Reg;
extern UZ80_SU uZ80_SU;

extern unsigned char Z80_IFF1;
extern unsigned char Z80_IFF2;
extern unsigned char Z80_IM;
extern unsigned char Z80_IRQ;
extern unsigned char Z80_MachineCycle;
extern unsigned char Z80_BorderColor;

//-Функции обработки сообщений класса----------------------------------------
BEGIN_MESSAGE_MAP(CWnd_Main,CWnd)
 ON_WM_PAINT()
 ON_WM_CREATE()
 ON_WM_DESTROY()
 ON_WM_TIMER() 
 ON_WM_LBUTTONDOWN()
 ON_WM_RBUTTONDOWN()
 ON_WM_RBUTTONUP()
 ON_WM_LBUTTONUP()
 ON_WM_MOUSEMOVE()
 ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
//-Конструктор класса--------------------------------------------------------
CWnd_Main::CWnd_Main(void)
{
 //устанавливаем параметры экрана
 sVideo_DisplaySetting.ScreenWidth=256+BORDER_LEFT_RIGHT*8*2;
 sVideo_DisplaySetting.ScreenHeight=192+BORDER_TOP_BOTTOM*8*2;
 sVideo_DisplaySetting.LineWidth=256+BORDER_LEFT_RIGHT*8*2;
 sVideo_DisplaySetting.VPtr=new unsigned char[sVideo_DisplaySetting.ScreenWidth*sVideo_DisplaySetting.ScreenHeight*4];
 sVideo_DisplaySetting.DVPtr=sVideo_DisplaySetting.LineWidth*4; 
 Z80_Reset();
 //записываем ПЗУ
 FILE *file=fopen("zx48-original.bin","wb");
 for(long n=0;n<16384;n++) 
 {
  Z80_Memory[n]=ROM[n];
  unsigned char b=ROM[n];
  fwrite(&b,1,1,file);
 }
 fclose(file);
 SpeedMode=false;
}
//-Деструктор класса---------------------------------------------------------
CWnd_Main::~CWnd_Main()
{
 delete[](sVideo_DisplaySetting.VPtr);
}
//-Замещённые функции предка-------------------------------------------------
//-Новые функции класса------------------------------------------------------
//-Функции обработки сообщений класса----------------------------------------
afx_msg void CWnd_Main::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 CWnd::OnCreate(lpCreateStruct); 
 //создаём окно магнитофона 
 cDialog_Tape.Create((LPSTR)IDD_DIALOG_TAPE,this);
 cDialog_Tape.ShowWindow(SW_SHOW);

 timeBeginPeriod(1);
 SetTimer(ID_TIMER_MAIN,1000/60,NULL);//30 Гц
}
afx_msg void CWnd_Main::OnDestroy(void)
{ 
 timeEndPeriod(1);
 KillTimer(ID_TIMER_MAIN);
 CWnd::OnDestroy();
}
afx_msg void CWnd_Main::OnPaint(void)
{ 
 CPaintDC dc(this);
 BITMAPINFOHEADER bmih;
 bmih.biSize=sizeof(BITMAPINFOHEADER);
 bmih.biWidth=sVideo_DisplaySetting.ScreenWidth;
 bmih.biHeight=-sVideo_DisplaySetting.ScreenHeight;
 bmih.biPlanes=1;
 bmih.biBitCount=32;
 bmih.biCompression=BI_RGB;
 bmih.biSizeImage=0;
 bmih.biXPelsPerMeter=300;
 bmih.biYPelsPerMeter=300;
 bmih.biClrUsed=0;
 bmih.biClrImportant=0;
 RGBQUAD rgbq;
 BITMAPINFO info;
 rgbq.rgbBlue=1;
 rgbq.rgbGreen=0;
 rgbq.rgbRed=0;
 rgbq.rgbReserved=0;
 info.bmiHeader=bmih;
 info.bmiColors[0]=rgbq;
 StretchDIBits(dc.m_hDC,0,0,sVideo_DisplaySetting.ScreenWidth*2,sVideo_DisplaySetting.ScreenHeight*2,0,0,sVideo_DisplaySetting.ScreenWidth,sVideo_DisplaySetting.ScreenHeight,sVideo_DisplaySetting.VPtr,&info,DIB_RGB_COLORS,SRCCOPY);
 CWnd::OnPaint();
}
afx_msg void CWnd_Main::OnTimer(UINT nIDEvent)
{
 if (nIDEvent!=ID_TIMER_MAIN) 
 {
  CWnd::OnTimer(nIDEvent);
  return;
 }
 Draw();
}
afx_msg BOOL CWnd_Main::OnEraseBkgnd(CDC *pDC)
{
 return(TRUE);
}
//-Новые функции класса------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//отрисовка
//----------------------------------------------------------------------------------------------------
void CWnd_Main::Draw(void)
{ 
 long n;
 unsigned short Table[40][2]=
 {
	 {'A',KB_A},
	 {'B',KB_B},
	 {'C',KB_C},
	 {'D',KB_D},
	 {'E',KB_E},
	 {'F',KB_F},
	 {'G',KB_G},
	 {'H',KB_H},
	 {'I',KB_I},
	 {'J',KB_J},
	 {'K',KB_K},
	 {'L',KB_L},
	 {'M',KB_M},
	 {'N',KB_N},
	 {'O',KB_O},
	 {'P',KB_P},
	 {'Q',KB_Q},
	 {'R',KB_R},
	 {'S',KB_S},
	 {'T',KB_T},
	 {'U',KB_U},
	 {'V',KB_V},
	 {'W',KB_W},
	 {'X',KB_X},
	 {'Y',KB_Y},
	 {'Z',KB_Z},
	 {'0',KB_0},
	 {'1',KB_1},
	 {'2',KB_2},
	 {'3',KB_3},
	 {'4',KB_4},
	 {'5',KB_5},
	 {'6',KB_6},
	 {'7',KB_7},
	 {'8',KB_8},
	 {'9',KB_9},
  	 {VK_RETURN,KB_ENT},
	 {VK_MENU,KB_CS},
	 {VK_SPACE,KB_SP},
	 {VK_SHIFT,KB_SS}
 };

 unsigned short CursorTable[5][3]=
 {
 	 {VK_LEFT,KB_6,1},
	 {VK_RIGHT,KB_7,0},
	 {VK_UP,KB_9,3},
	 {VK_DOWN,KB_8,2},
	 {VK_CONTROL,KB_0,4}
 };

 //отрисовываем экран 
 long tick_point=224;//в строке должно быть 224 такта 
 //делаем развёртку
 unsigned char *z80_vptr=Z80_Memory+16384;//адрес видеопамяти
 unsigned char *z80_aptr=Z80_Memory+16384+6144;//адрес атрибутов видеопамяти
 unsigned char *v_ptr=sVideo_DisplaySetting.VPtr;
 unsigned long s;

 if (this==GetActiveWindow())
 {
  if (GetAsyncKeyState(VK_F1)&32768) SpeedMode=true;
  if (GetAsyncKeyState(VK_F2)&32768) SpeedMode=false;
  if (GetAsyncKeyState(VK_F12)&32768) Z80_Reset();
  //if (GetAsyncKeyState(VK_F5)&32768) SaveSNA("snapshot.sna");
  //if (GetAsyncKeyState(VK_F9)&32768) LoadSNA("snapshot.sna");
  if (GetAsyncKeyState(VK_F5)&32768) SaveZ80("snapshot.z80");
  if (GetAsyncKeyState(VK_F9)&32768) LoadZ80("snapshot.z80");
  //сканируем клавиатуру
  for(s=0;s<40;s++)
  {
   if (GetAsyncKeyState(Table[s][0])&32768) Z80_SetKey(Table[s][1]);
                                       else Z80_ResetKey(Table[s][1]);
  }
  Z80_KempstonInState=0;
  for(s=0;s<5;s++)
  {
   if (GetAsyncKeyState(CursorTable[s][0])&32768) 
   {
    Z80_SetKey(CursorTable[s][1]);
    Z80_KempstonInState|=(1<<CursorTable[s][2]);
   }
  }
 }
 //выполняем эмуляцию процессора без обновления экрана
 if (SpeedMode==true)
 {
  for(s=0;s<20;s++)
  {
   Z80_ScreenIRQ=1;
   Z80_Run();
   Z80_ScreenIRQ=0;
   for(n=0;n<tick_point*312;n++)
   {	   
    Z80_TapeInState=cDialog_Tape.GetTapeState();//состояние входа магнитофона
    if (Z80_MachineCycle==0) 
    {
     Z80_Run();
    }
    else Z80_MachineCycle--;
   }
  }
 }
 //выполняем эмуляцию процессора с обновлением экрана
 Z80_ScreenIRQ=1;
 Z80_Run();
 Z80_ScreenIRQ=0;
 for(unsigned long y=0;y<312;y++)
 {
  //выполняем эмуляцию процессора
  for(n=0;n<tick_point;n++)
  {	   
   Z80_TapeInState=cDialog_Tape.GetTapeState();//состояние входа магнитофона
   if (Z80_MachineCycle==0) 
   {
    Z80_Run();
   }
   else Z80_MachineCycle--;
  }
  if (y<60-BORDER_TOP_BOTTOM*8) continue;//внеэкранная область
  if (y>=60+192+BORDER_TOP_BOTTOM*8) continue;//внеэкранная область
  for(unsigned long x=0;x<32+BORDER_LEFT_RIGHT*2;x++)
  {
   unsigned char b=Z80_BorderColor&0x01;
   unsigned char g=(Z80_BorderColor&0x04)>>2;
   unsigned char r=(Z80_BorderColor&0x02)>>1;
   r*=192;
   g*=192;
   b*=192;
   if ((y<60 || y>=192+60) || (x<BORDER_LEFT_RIGHT || x>=32+BORDER_LEFT_RIGHT))//бордюр
   {
    for(unsigned char p=0;p<8;p++)
	{
     *v_ptr=b;
     v_ptr++;
     *v_ptr=g;
     v_ptr++;
     *v_ptr=r;
     v_ptr++;
     *v_ptr=0;
     v_ptr++;
	}
    continue;
   }
   //выводим изображение
   unsigned long xs=x-BORDER_LEFT_RIGHT;
   unsigned long y_local=y-60;
   //определяем номер знакоместа по высоте делением на 8
   unsigned long ys=y_local>>3;
   //считываем аттрибут и определяем цвет
   unsigned char attr=*(z80_aptr+xs+(ys<<5));
   Z80_AttributesPort=attr;
   unsigned char ink=attr&0x07;
   unsigned char paper=(attr>>3)&0x07;
   unsigned char bright=(attr>>6)&0x01;
   unsigned char flash=(attr>>7)&0x01;
   //определяем адрес видеопамяти
   unsigned char *out_ptr=z80_vptr+2048*(ys>>3)+xs+(ys&0x07)*32+256*(y_local&0x07);   
   //считываем байт   
   unsigned char byte=*out_ptr;
   unsigned char mask=128;
   for(unsigned char p=0;p<8;p++,mask>>=1)
   {
    if (byte&mask)//точка цвета ink
    {
     b=ink&0x01; 
	 g=(ink&0x04)>>2;
	 r=(ink&0x02)>>1;
    }
    else//точка цвета paper
    {
     b=paper&0x01;
     g=(paper&0x04)>>2;
     r=(paper&0x02)>>1;
    }
    if (bright)
    {
     r*=255;
     g*=255;
     b*=255;
    }
    else
    {
     r*=192;
     g*=192;
     b*=192;
    }
    *v_ptr=b;
    v_ptr++;
    *v_ptr=g;
    v_ptr++;
    *v_ptr=r;
    v_ptr++;
    *v_ptr=0;
    v_ptr++;
   }
  }
 }
 InvalidateRect(NULL,FALSE);
}
//----------------------------------------------------------------------------------------------------
//загрузить файл SNA
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::LoadSNA(char *FileName)
{
 FILE *file=fopen(FileName,"rb");
 if (file==NULL) return(false);
 #pragma pack(1)
 struct SSNA
 {
  unsigned char I;
  unsigned char L_A;
  unsigned char H_A;
  unsigned char E_A;
  unsigned char D_A;
  unsigned char C_A;
  unsigned char B_A;
  unsigned char F_A;
  unsigned char A_A;
  unsigned char L;
  unsigned char H;
  unsigned char E;
  unsigned char D;
  unsigned char C;
  unsigned char B;
  unsigned short IY;
  unsigned short IX;
  unsigned char IFF2;
  unsigned char R;
  unsigned char F;
  unsigned char A;
  unsigned short SP;
  unsigned char IM;
  unsigned char Border;
 } sSNA;
 if (fread(&sSNA,1,sizeof(SSNA),file)!=sizeof(SSNA))
 {
  #pragma pack()
  fclose(file);
  return(false);
 }
 #pragma pack()
 //копируем регистры
 Z80_I=sSNA.I;
 Z80_L_=sSNA.L_A;
 Z80_H_=sSNA.H_A;
 Z80_E_=sSNA.E_A;
 Z80_D_=sSNA.D_A;
 Z80_C_=sSNA.C_A;
 Z80_B_=sSNA.B_A;
 Z80_F_=sSNA.F_A;
 Z80_A_=sSNA.A_A;

 Z80_L=sSNA.L;
 Z80_H=sSNA.H;
 Z80_E=sSNA.E;
 Z80_D=sSNA.D;
 Z80_C=sSNA.C;
 Z80_B=sSNA.B;

 Z80_IX=sSNA.IX;
 Z80_IY=sSNA.IY;
 Z80_IFF2=(sSNA.IFF2>>2)&0x01;
 Z80_R=sSNA.R;
 Z80_F=sSNA.F;
 Z80_A=sSNA.A;
 Z80_SP=sSNA.SP;
 Z80_IM=sSNA.IM; 
 Z80_IFF1=Z80_IFF2;
 //загружаем образ
 unsigned long addr=16384;
 while(1)
 {
  unsigned char b;
  if (fread(&b,1,1,file)<=0) break;
  Z80_Memory[addr]=b;
  addr++;
 }
 fclose(file);
 RETN();
 Z80_MEMPTR=0;
 Z80_RR7=0;
 return(true);
}
//----------------------------------------------------------------------------------------------------
//загрузить файл Z80
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::LoadZ80(char *FileName)
{ 
 FILE *file=fopen(FileName,"rb");
 if (file==NULL) return(false);
 #pragma pack(1)
 struct SZ80
 { 
  unsigned char A;
  unsigned char F;
  unsigned short BC;
  unsigned short HL;
  unsigned short PC;
  unsigned short SP;
  unsigned char I;
  unsigned char R;
  unsigned char Mode;
  unsigned short DE;
  unsigned short BC_A;
  unsigned short DE_A;
  unsigned short HL_A;
  unsigned char A_A;
  unsigned char F_A;
  unsigned short IY;
  unsigned short IX;
  unsigned char Interrupt;
  unsigned char IFF2;
  unsigned char IMode;
 } sZ80;
 if (fread(&sZ80,1,sizeof(sZ80),file)!=sizeof(SZ80))
 {
  #pragma pack()
  fclose(file);
  return(false);
 }
 #pragma pack()
 //копируем регистры
 if (sZ80.Mode==0xff) sZ80.Mode=1;

 Z80_A=sZ80.A;
 Z80_F=sZ80.F;
 Z80_BC=sZ80.BC;
 Z80_DE=sZ80.DE;
 Z80_HL=sZ80.HL;
 Z80_PC=sZ80.PC;
 Z80_SP=sZ80.SP;
 Z80_I=sZ80.I;
 Z80_R=(sZ80.R&0x7f)|((sZ80.Mode&0x01)<<7);
 Z80_IX=sZ80.IX;
 Z80_IY=sZ80.IY;
 Z80_A_=sZ80.A_A;
 Z80_F_=sZ80.F_A;
 Z80_BC_=sZ80.BC_A;
 Z80_DE_=sZ80.DE_A;
 Z80_HL_=sZ80.HL_A;
 Z80_IFF2=sZ80.IFF2;
 Z80_IFF1=Z80_IFF2;
 Z80_IM=sZ80.IMode&0x03;
 if (sZ80.Interrupt==0)
 {
  Z80_IFF1=0;
  Z80_IFF2=0;
 }
 Z80_BorderColor=(sZ80.Mode>>1)&0x7;
 unsigned long addr=16384;
 if (sZ80.Mode&(1<<5))
 {  
  while(addr<65536)
  {
   unsigned char b;
   if (fread(&b,1,1,file)<=0) break;
   if (b!=0xED)
   {
    Z80_Memory[addr]=b;
	addr++;
	continue;
   }
   unsigned char c;
   if (fread(&c,1,1,file)<=0) break;
   if (c!=0xED)
   {
    if (addr>=65536) break;
    Z80_Memory[addr]=b;
    addr++;
    if (addr>=65536) break;
    Z80_Memory[addr]=c;
    addr++;
	continue;
   }
   unsigned char xx;
   unsigned char yy;
   if (fread(&xx,1,1,file)<=0) break;
   if (fread(&yy,1,1,file)<=0) break;
   while(xx>0 && addr<65536)
   {
    Z80_Memory[addr]=yy;
    xx--;
    addr++;	  
   }   
  }
 }
 else
 {
  while(1)
  {
   unsigned char b;
   if (fread(&b,1,1,file)<=0) break;
   Z80_Memory[addr]=b;
   addr++;
  }
 }
 fclose(file);
 return(true);
}
//----------------------------------------------------------------------------------------------------
//сохранить файл SNA
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::SaveSNA(char *FileName)
{
 FILE *file=fopen(FileName,"wb");
 if (file==NULL) return(false);
 PUSH(Z80_PC);
 #pragma pack(1)
 struct SSNA
 {
  unsigned char I;
  unsigned char L_A;
  unsigned char H_A;
  unsigned char E_A;
  unsigned char D_A;
  unsigned char C_A;
  unsigned char B_A;
  unsigned char F_A;
  unsigned char A_A;
  unsigned char L;
  unsigned char H;
  unsigned char E;
  unsigned char D;
  unsigned char C;
  unsigned char B;
  unsigned short IY;
  unsigned short IX;
  unsigned char IFF2;
  unsigned char R;
  unsigned char F;
  unsigned char A;
  unsigned short SP;
  unsigned char IM;
  unsigned char Border;
 } sSNA;
 //копируем регистры
 sSNA.I=Z80_I;
 sSNA.L_A=Z80_L_;
 sSNA.H_A=Z80_H_;
 sSNA.E_A=Z80_E_;
 sSNA.D_A=Z80_D_;
 sSNA.C_A=Z80_C_;
 sSNA.B_A=Z80_B_;
 sSNA.F_A=Z80_F_;
 sSNA.A_A=Z80_A_;

 sSNA.L=Z80_L;
 sSNA.H=Z80_H;
 sSNA.E=Z80_E;
 sSNA.D=Z80_D;
 sSNA.C=Z80_C;
 sSNA.B=Z80_B;

 sSNA.IX=Z80_IX;
 sSNA.IY=Z80_IY;
 sSNA.IFF2=Z80_IFF2<<2;
 sSNA.R=Z80_R;
 sSNA.F=Z80_F;
 sSNA.A=Z80_A;
 sSNA.SP=Z80_SP;
 sSNA.IM=Z80_IM;
 sSNA.Border=0;

 if (fwrite(&sSNA,1,sizeof(SSNA),file)!=sizeof(SSNA))
 {
  #pragma pack()
  fclose(file);
  RETN();
  return(false);
 }
 #pragma pack()
 //записываем образ
 unsigned long addr=16384;
 while(addr<65536)
 {
  unsigned char b=Z80_Memory[addr];
  if (fwrite(&b,1,1,file)<=0) break;
  addr++;
 }
 fclose(file);
 RETN();
 return(true);
}
//----------------------------------------------------------------------------------------------------
//сохранить файл Z80
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::SaveZ80(char *FileName)
{
 FILE *file=fopen(FileName,"wb");
 if (file==NULL) return(false);
 #pragma pack(1)
 struct SZ80
 { 
  unsigned char A;
  unsigned char F;
  unsigned short BC;
  unsigned short HL;
  unsigned short PC;
  unsigned short SP;
  unsigned char I;
  unsigned char R;
  unsigned char Mode;
  unsigned short DE;
  unsigned short BC_A;
  unsigned short DE_A;
  unsigned short HL_A;
  unsigned char A_A;
  unsigned char F_A;
  unsigned short IY;
  unsigned short IX;
  unsigned char Interrupt;
  unsigned char IFF2;
  unsigned char IMode;
 } sZ80;
 //копируем регистры
 sZ80.A=Z80_A;
 sZ80.F=Z80_F;
 sZ80.BC=Z80_BC;
 sZ80.DE=Z80_DE;
 sZ80.HL=Z80_HL;
 sZ80.PC=Z80_PC;
 sZ80.SP=Z80_SP;
 sZ80.I=Z80_I;
 sZ80.R=Z80_R&0x7F;
 sZ80.Mode=(Z80_BorderColor<<1)|((Z80_R>>7)&0x01);
 sZ80.IX=Z80_IX;
 sZ80.IY=Z80_IY;
 sZ80.A_A=Z80_A;
 sZ80.F_A=Z80_F_;
 sZ80.BC_A=Z80_BC_;
 sZ80.DE_A=Z80_DE_;
 sZ80.HL_A=Z80_HL_;
 sZ80.IFF2=Z80_IFF2;
 sZ80.IMode=Z80_IM;
 sZ80.Interrupt=Z80_IRQ;
 if (fwrite(&sZ80,1,sizeof(SZ80),file)!=sizeof(SZ80))
 {
  #pragma pack()
  fclose(file);
  return(false);
 }
 #pragma pack()
 //записываем образ
 unsigned long addr=16384;
 while(addr<65536)
 {
  unsigned char b=Z80_Memory[addr];
  if (fwrite(&b,1,1,file)<=0) break;
  addr++;
 }
 fclose(file);
 return(true);
}

//-Прочее--------------------------------------------------------------------

