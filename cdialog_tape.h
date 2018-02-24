#ifndef CDIALOG_TAPE_H
#define CDIALOG_TAPE_H

#include "stdafx.h"

#include "ctape.h"

class CDialog_Tape:public CDialog
{
 protected:
  CTape cTape;
  bool TimerEnabled;//может ли таймер использоваться
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CDialog_Tape(void);
  //-Деструктор класса-------------------------------------------------------
  ~CDialog_Tape();
  //-Переменные класса-------------------------------------------------------
  //-Замещённые функции предка-----------------------------------------------
  afx_msg void OnOK(void);
  afx_msg void OnClose(void);
  afx_msg void OnCancel(void);
  afx_msg BOOL OnInitDialog(void);
  //-Новые функции класса----------------------------------------------------
  unsigned char GetTapeState(void);//получить состояние магнитофонного входа
  //-Функции обработки сообщений класса--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnDestroy(void);
  afx_msg void OnCOmmand_Button_Start(void);
  afx_msg void OnCOmmand_Button_Stop(void);
  afx_msg void OnCOmmand_Button_Open(void);
  afx_msg void OnTimer(UINT nIDEvent);
 protected:
  //-Новые функции класса----------------------------------------------------
  //-Прочее------------------------------------------------------------------  
};
//---------------------------------------------------------------------------
#endif