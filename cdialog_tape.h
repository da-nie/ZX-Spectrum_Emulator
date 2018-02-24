#ifndef CDIALOG_TAPE_H
#define CDIALOG_TAPE_H

#include "stdafx.h"

#include "ctape.h"

class CDialog_Tape:public CDialog
{
 protected:
  CTape cTape;
  bool TimerEnabled;//����� �� ������ ��������������
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CDialog_Tape(void);
  //-���������� ������-------------------------------------------------------
  ~CDialog_Tape();
  //-���������� ������-------------------------------------------------------
  //-���������� ������� ������-----------------------------------------------
  afx_msg void OnOK(void);
  afx_msg void OnClose(void);
  afx_msg void OnCancel(void);
  afx_msg BOOL OnInitDialog(void);
  //-����� ������� ������----------------------------------------------------
  unsigned char GetTapeState(void);//�������� ��������� �������������� �����
  //-������� ��������� ��������� ������--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnDestroy(void);
  afx_msg void OnCOmmand_Button_Start(void);
  afx_msg void OnCOmmand_Button_Stop(void);
  afx_msg void OnCOmmand_Button_Open(void);
  afx_msg void OnTimer(UINT nIDEvent);
 protected:
  //-����� ������� ������----------------------------------------------------
  //-������------------------------------------------------------------------  
};
//---------------------------------------------------------------------------
#endif