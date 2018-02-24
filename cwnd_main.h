#ifndef CWND_MAIN
#define CWND_MAIN

#include "stdafx.h"
#include "cdialog_tape.h"

#include "video.h"
#include "z80.h"

class CWnd_Main:public CWnd
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CDialog_Tape cDialog_Tape;
  bool SpeedMode;//режим ускорения
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CWnd_Main(void);
  //-Деструктор класса-------------------------------------------------------
  ~CWnd_Main();
  //-Переменные класса-------------------------------------------------------
  //-Замещённые функции предка-----------------------------------------------
  //-Новые функции класса----------------------------------------------------
  //-Функции обработки сообщений класса--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy(void);
  afx_msg void OnPaint(void);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg BOOL OnEraseBkgnd(CDC *pDC);
  //-Новые функции класса----------------------------------------------------
 protected: 
  void Draw(void);//отрисовка
  bool LoadSNA(char *FileName);//загрузить файл SNA
  bool LoadZ80(char *FileName);//загрузить файл Z80
  bool SaveSNA(char *FileName);//сохранить файл SNA
  bool SaveZ80(char *FileName);//сохранить файл Z80
  //-Прочее------------------------------------------------------------------
};
#endif
