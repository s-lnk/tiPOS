/***********************************************/
/*   Some public functions				       */
/*   For testing the P60 Run-time library      */
/*   Create by ZengYun at 6/5/2001             */
/***********************************************/
#include "josapi.h"
#include "wlsapi.h"

#ifndef _PUB_H
#define _PUB_H

#define MAC_ERR   (-7500)

#define LEFT_DISP       0   ///������ʾ
#define MIDDLE_DISP     1   ///������ʾ
#define RIGHT_DISP      2   ///������ʾ
#define NORMAL_DISP     0   ///������ʾ
#define REVERSE_DISP    1   ///������ʾ

#define ALLOW_ZERO_IN   0x80
#define BIG_IN          0x40
#define NUM_IN          0x20
#define ALPHA_IN        0x10
#define PASS_IN         0x08
#define CARRY_IN        0x04
#define AMOUNT_IN       0x02
#define ECHO_IN         0x01

#define TIMER_FOR_APILIB    5 


#define LINE1   0
#define LINE2   1
#define LINE3   2
#define LINE4   3
#define LINE5   4
#define LINE6   5
#define LINE7   6
#define LINE8   7
#define LINE9   9

#define NO_NEED_RESET	0
#define CHANNEL_SIM1	0
#define CHANNEL_SIM2	1

void DispTimer2(void);
void vDispTitle(char *mesg);
void vDisp2(uchar line,char *mesg);
void vDisp(uchar line,char *mesg);
uchar vMessage(char *mesg);
uint Ramdom(uint MaxNum);
uchar PressKey(void);

void ShowErr(char *msg);
unsigned char WaitEsc(unsigned short seconds);
void ok_beep(void); 
 
 void DispTitle(int mode);
 
void s_UartOpen(void);
void s_UartClose(void);
int ss_UartPrint(char *fmt,...);

 
int Test_KbFun(int mode);
int SYS_Test(void); 
int CLK_Test(void);
int LCD_Test(void);
int MEM_Test(void); 
int COM_Test(void);
int PRN_Test(void);  


#endif
