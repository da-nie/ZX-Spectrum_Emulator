#include "cdialog_tape.h"

//-Функции обработки сообщений класса----------------------------------------
BEGIN_MESSAGE_MAP(CDialog_Tape,CDialog)
 ON_WM_DESTROY()
 ON_WM_HSCROLL()
 ON_WM_TIMER()
 ON_COMMAND(IDC_BUTTON_TAPE_START,OnCOmmand_Button_Start)
 ON_COMMAND(IDC_BUTTON_TAPE_STOP,OnCOmmand_Button_Stop)
 ON_COMMAND(IDC_BUTTON_TAPE_OPEN,OnCOmmand_Button_Open)
END_MESSAGE_MAP()
//-Конструктор класса--------------------------------------------------------
CDialog_Tape::CDialog_Tape(void)
{
}
//-Деструктор класса---------------------------------------------------------
CDialog_Tape::~CDialog_Tape()
{
}
//-Замещённые функции предка-------------------------------------------------
afx_msg void CDialog_Tape::OnOK(void)
{
}
afx_msg void CDialog_Tape::OnClose(void)
{
 return;
 //ShowWindow(SW_HIDE);
}
afx_msg void CDialog_Tape::OnCancel(void)
{
 return;
 //ShowWindow(SW_HIDE);
}
afx_msg BOOL CDialog_Tape::OnInitDialog(void)
{
 SetTimer(ID_TIMER_TAPE,100,NULL);
 ((CButton*)GetDlgItem(IDC_BUTTON_TAPE_START))->EnableWindow(TRUE);
 ((CButton*)GetDlgItem(IDC_BUTTON_TAPE_STOP))->EnableWindow(FALSE);
 TimerEnabled=false;
 return(CDialog::OnInitDialog());
}
//-Новые функции класса------------------------------------------------------
//-Функции обработки сообщений класса----------------------------------------
afx_msg void CDialog_Tape::OnDestroy(void)
{
 KillTimer(ID_TIMER_TAPE);
 CDialog::OnDestroy();
}
afx_msg void CDialog_Tape::OnCOmmand_Button_Start(void)
{
 long block=((CListBox*)GetDlgItem(IDC_LIST_TAPE_BLOCK))->GetCurSel();
 cTape.Start(block);
 ((CListBox*)GetDlgItem(IDC_LIST_TAPE_BLOCK))->EnableWindow(FALSE);
 ((CButton*)GetDlgItem(IDC_BUTTON_TAPE_START))->EnableWindow(FALSE);
 ((CButton*)GetDlgItem(IDC_BUTTON_TAPE_STOP))->EnableWindow(TRUE);
 TimerEnabled=true;
}
afx_msg void CDialog_Tape::OnCOmmand_Button_Stop(void)
{
 cTape.Stop();
 ((CListBox*)GetDlgItem(IDC_LIST_TAPE_BLOCK))->EnableWindow(TRUE);
 ((CButton*)GetDlgItem(IDC_BUTTON_TAPE_START))->EnableWindow(TRUE);
 ((CButton*)GetDlgItem(IDC_BUTTON_TAPE_STOP))->EnableWindow(FALSE);
 TimerEnabled=false;
}
afx_msg void CDialog_Tape::OnCOmmand_Button_Open(void)
{
 char Path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,Path);
 
 CFileDialog cFileDialog(TRUE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"*.tap|*.tap||",this);
 cFileDialog.m_ofn.lpstrInitialDir=Path;
 char Title[256];
 strcpy(Title,"Выбор tap-файла");
 cFileDialog.m_ofn.lpstrTitle=Title;
 if (cFileDialog.DoModal()!=IDOK) return;
 string file_name=cFileDialog.GetFileName();
 cTape.LoadTAP(file_name.c_str());
 //заполняем список
 ((CListBox*)GetDlgItem(IDC_LIST_TAPE_BLOCK))->ResetContent();
 vector<SBlock> *vector_SBlock=cTape.GetBlockVectorPtr();
 long block=vector_SBlock->size();
 for(long n=0;n<block;n++)
 {
  char name[255];
  char type[255];
  strcpy(type,"");
  SBlock *sBlock_Ptr=&(*vector_SBlock)[n];
  if (sBlock_Ptr->BlockType==BLOCK_TYPE_BASIC) strcpy(type,"Programm:");
  if (sBlock_Ptr->BlockType==BLOCK_TYPE_CODE) strcpy(type,"Bytes:");
  if (sBlock_Ptr->BlockType==BLOCK_TYPE_SYMBOL_ARRAY) strcpy(type,"Array of symbol:");
  if (sBlock_Ptr->BlockType==BLOCK_TYPE_NUMBER_ARRAY) strcpy(type,"Array of number:");
  sprintf(name,"Блок [%04i] %s:%s Адрес:%i Размер:%i",n+1,type,sBlock_Ptr->Name,sBlock_Ptr->Addr,sBlock_Ptr->Length);
  ((CListBox*)GetDlgItem(IDC_LIST_TAPE_BLOCK))->AddString(name);
 } 
 block=cTape.GetCurrentBlockIndex();
 ((CListBox*)GetDlgItem(IDC_LIST_TAPE_BLOCK))->SetCurSel(block);
}
afx_msg void CDialog_Tape::OnTimer(UINT nIDEvent)
{
 if (nIDEvent!=ID_TIMER_TAPE) 
 {
  CDialog::OnTimer(nIDEvent);
  return;
 }
 if (TimerEnabled==false) return;
 long block=cTape.GetCurrentBlockIndex();
 ((CListBox*)GetDlgItem(IDC_LIST_TAPE_BLOCK))->SetCurSel(block);
}

//-Новые функции класса------------------------------------------------------

//----------------------------------------------------------------------------------------------------
//получить состояние магнитофонного входа
//----------------------------------------------------------------------------------------------------
unsigned char CDialog_Tape::GetTapeState(void)
{
 return(cTape.GetTapeState());
}

//-Прочее--------------------------------------------------------------------
