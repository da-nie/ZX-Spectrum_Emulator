#include "z80.h"
#include "z80_common.h"
 
/*include Z80_OpCodes*/
#include "Z80_OpCode_base.h"
#include "Z80_OpCode_cb.h"
#include "Z80_OpCode_dd.h"
#include "Z80_OpCode_ed.h"
#include "Z80_OpCode_fd.h"
#include "Z80_OpCode_ddcb.h"
#include "Z80_OpCode_fdcb.h"

extern unsigned char Z80_HalfcarryAddTable[8]; 
extern unsigned char Z80_HalfcarrySubTable[8]; 
extern unsigned char Z80_OverflowAddTable[8]; 
extern unsigned char Z80_OverflowSubTable[8];
/*the S, Z, 5 and 3 bits of the index*/
extern unsigned char Z80_SZ53Table[0x100]; 
/*OR the above two tables together*/
extern unsigned char Z80_SZ53pTable[0x100]; 
/*the parity of the lookup value*/
extern unsigned char Z80_ParityTable[0x100]; 
/*table for daa, contains af*/
extern unsigned char Z80_DAATab[0x1000];

extern unsigned char Z80_IFF1;
extern unsigned char Z80_IFF2;
extern unsigned char Z80_IM;
extern unsigned char Z80_Halt;
 
extern unsigned char Z80_Temp8;
extern unsigned short Z80_Temp16;
 
extern unsigned char Z80_OpCode;
extern unsigned char Z80_Prefix;
extern unsigned char Z80_NoInterrupt;
extern unsigned char Z80_MachineCycle;

extern void (*Z80_OpCode_base [0x100])(void);
extern void (*Z80_OpCode_cb[0x100])(void);
extern void (*Z80_OpCode_dd[0x100])(void);
extern void (*Z80_OpCode_ddcb[0x100])(void);
extern void (*Z80_OpCode_ed [0x100])(void);
extern void (*Z80_OpCode_fd[0x100])(void);
extern void (*Z80_OpCode_fdcb[0x100])(void);

extern unsigned char Z80_Memory[65536];

extern unsigned short Z80_VectorNMI;
extern unsigned char Z80_IRQ;

extern unsigned char Z80_ScreenIRQ;
extern unsigned char Z80_BorderColor;

extern UZ80_Reg uZ80_Reg;
extern UZ80_A_Reg uZ80_A_Reg;
extern UZ80_SU uZ80_SU;

void Z80_Reset(void)
{
 Z80_AF=0;
 Z80_BC=0;
 Z80_DE=0;
 Z80_HL=0;
 Z80_IX=0;
 Z80_IY=0;
 Z80_SP=0;
 Z80_PC=0;
 Z80_IFF1=Z80_IFF2=0;
 Z80_IM=0;
 Z80_MEMPTR=0;
 return;
}
 
void Z80_Run(void)
{
 if (Z80_IFF1==0)
 {
  Z80_IRQ=0;
 }
  
 if (Z80_IRQ==1)
 {
  if (Z80_ScreenIRQ==1)
  {
   if (Z80_IM==0)
   {
    Z80_IFF1=0;
    Z80_IFF2=0;
    RST(0x38);
    Z80_Halt=0;
   }
   else 
   if (Z80_IM==1)
   {
    Z80_IFF1=0;
    Z80_IFF2=0;
    RST(0x38);
    Z80_Halt=0;
   }
   else 
   if (Z80_IM==2)
   {
    Z80_IFF1=0;
    Z80_IFF2=0;
    Z80_VectorNMI=((Z80_I*256)+255);
    Z80_VectorNMI=READ_WORD(Z80_VectorNMI);
    RST(Z80_VectorNMI);
    Z80_Halt=0;
   }
  }
 }
  
 if (Z80_IFF1==1)
 {
  Z80_IRQ=1;
 }
  
 if (Z80_Halt==0)
 {
  Z80_OpCode=NEXT_BYTE();
  (*Z80_OpCode_base[Z80_OpCode])();
  Z80_R++;
   
  if (Z80_Prefix==0xCB)
  {
   Z80_OpCode=NEXT_BYTE();
   (*Z80_OpCode_cb[Z80_OpCode])();
   Z80_Prefix=0;
   Z80_NoInterrupt=0;
   Z80_MachineCycle+=4;
   Z80_R++;
  }
  if (Z80_Prefix==0xED)
  {
   Z80_OpCode=NEXT_BYTE();
   (*Z80_OpCode_ed[Z80_OpCode])();
   Z80_Prefix=0;
   Z80_NoInterrupt=0;
   Z80_MachineCycle+=4;
   Z80_R++;
  }
  if (Z80_Prefix==0xDD)
  {
   Z80_OpCode=NEXT_BYTE();
   Z80_R++;
   if (Z80_OpCode==0xCB)
   {
    uZ80_SU.u=NEXT_BYTE();
    Z80_OpCode=NEXT_BYTE();
    (*Z80_OpCode_ddcb[Z80_OpCode])();
    Z80_Prefix=0;
    Z80_NoInterrupt=0;
    Z80_MachineCycle+=4;
   }
   else
   {
    (*Z80_OpCode_dd[Z80_OpCode])();
    Z80_Prefix=0;
    Z80_NoInterrupt=0;
    Z80_MachineCycle+=4;
   }
  }
  if (Z80_Prefix==0xFD)
  {
   Z80_OpCode=NEXT_BYTE();
   Z80_R++;
   if (Z80_OpCode==0xCB)
   {
    uZ80_SU.u=NEXT_BYTE();
    Z80_OpCode=NEXT_BYTE();
    (*Z80_OpCode_fdcb[Z80_OpCode])();
    Z80_Prefix=0;
    Z80_NoInterrupt=0;
    Z80_MachineCycle+=4;
   }
   else
   {
    (*Z80_OpCode_fd[Z80_OpCode])();
    Z80_Prefix=0;
    Z80_NoInterrupt=0;
    Z80_MachineCycle+=4;
   }
  }
 }
}
 
 
