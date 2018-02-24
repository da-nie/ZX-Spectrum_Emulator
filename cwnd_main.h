#ifndef CWND_MAIN
#define CWND_MAIN

#include "stdafx.h"
#include "cdialog_tape.h"

#include "video.h"
#include "z80.h"

class CWnd_Main:public CWnd
{
 protected:
  //-���������� ������-------------------------------------------------------
  CDialog_Tape cDialog_Tape;
  bool SpeedMode;//����� ���������
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CWnd_Main(void);
  //-���������� ������-------------------------------------------------------
  ~CWnd_Main();
  //-���������� ������-------------------------------------------------------
  //-���������� ������� ������-----------------------------------------------
  //-����� ������� ������----------------------------------------------------
  //-������� ��������� ��������� ������--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy(void);
  afx_msg void OnPaint(void);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg BOOL OnEraseBkgnd(CDC *pDC);
  //-����� ������� ������----------------------------------------------------
 protected: 
  void Draw(void);//���������
  bool LoadSNA(char *FileName);//��������� ���� SNA
  bool LoadZ80(char *FileName);//��������� ���� Z80
  bool SaveSNA(char *FileName);//��������� ���� SNA
  bool SaveZ80(char *FileName);//��������� ���� Z80
  //-������------------------------------------------------------------------
};
#endif
