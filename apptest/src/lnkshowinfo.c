#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "public.h"  
//#include <main>

#define WLS_RST_TM    1
#define WLS_TM  3

int lnkshowinfo()
{
	int res1,res2,res3,res4;
	char juss[4], buss[4], liin[4];//,ajuh[4];
	unsigned char ucKey;
	unsigned char ucSerial;
	BYTE btFW;
	unsigned char ucVers;
	unsigned char ucDTTM;
	unsigned char ucTerm;
	int page=0;
	memset(juss, 0, sizeof(juss));
	memset(buss, 0, sizeof(buss));
	//memset(ajuh, 0, sizeof(ajuh));
	memset(liin, 0, sizeof(liin));
	res1=Lib_FileGetEnv("driver", juss);
	res2=Lib_FileGetEnv("driver", buss);
	//res4=Lib_FileGetEnv("driver", ajuh);
	res3=Lib_FileGetEnv("route", liin);
	while(1){
		Lib_LcdCls();
		Lib_KbFlush();
		DispTimer2();
		Lib_LcdPrintxy(0,0*8,0x80," tiPOS Lonestar info ");
		Lib_LcdClrLine(1*8, 8*8);
		Lib_LcdSetFont(8, 16, 0);
		if(page==0){
			Lib_LcdGotoxy(0,2*8);
			Lib_ReadSN(ucSerial);
			Lib_Lcdprintf("JU:%s, %d %d",juss,res1,strlen(juss));
			//ucVers='9';
			Lib_LcdGotoxy(0,3*8);
			//if () ucVers="1";
			Lib_Lcdprintf("BU:%s, %d %d",buss,res2,strlen(buss));
			Lib_LcdGotoxy(0,4*8);
			//Lib_GetDateTime(ucDTTM);
			Lib_Lcdprintf("LI:%s, %d %d",liin,res3,strlen(liin));
			Lib_LcdGotoxy(0,5*8);
			Lib_ReadTerminalType(ucTerm);
			//Lib_Lcdprintf("JU: %s, %d %d",ajuh,res4,strlen(ajuh));
			Lib_LcdGotoxy(0,6*8);
			Lib_Lcdprintf("SALE: %d",Lib_FileExist("SALE"));
		}
		else{
			Lib_LcdGotoxy(0,2*8); 
			Lib_Lcdprintf("  1-BAT 2-PICC");
		}
		
		while(1){
			DispTimer2();
			//DispWlsSignal(0, 2*8);
			DispBattery(128-11, 2*8);

			if(Lib_KbCheck()) continue;
			ucKey=Lib_KbGetCh();  
			if(ucKey>='0' && ucKey<='9') break;
			if(ucKey==KEYUP && page==1){
				page=0;
				Lib_SetTimer(WLS_TM, 0);   
				break;
			}
			if(ucKey==KEYDOWN && page==0){
				page=1;
				Lib_SetTimer(WLS_TM, 0);  
				break;
			}
			if(ucKey==KEYENTER){
				//page ^= 1;
				//Lib_SetTimer(WLS_TM, 0);   
				return 0;
				break;
			}
			if(ucKey==KEYCANCEL){
				Lib_KbFlush();
				return 0;
			}
		}
		if(!(ucKey>='0' && ucKey<='9')) continue;
		if(page==0){
			switch(ucKey){
			case '1':
				//BeepFun_Test();
				return 0;
				break;
			case '2':
				Test_KbFun(1);
				break;
			case '3':
				LCD_Test(); 
				break;
			case '4':
				CLK_Test();
				break;
			case '5': 
				Test_Wls();
				break;
			case '6':
				PRN_Test();	 
				break; 
			 
			}
		}
		else{
			switch(ucKey){
			case '1':
				Battery_Test();			
				break;
			case '2':
				Picc_Test();
				break; 
			}
						
		}
	}
	return 0;			
}

int GetSec(void)
{
	int sec;
	unsigned char font,buff[8];
	Lib_GetDateTime(buff);
	/*year=1900+(buff[0] >>4)*10+(buff[0] & 0x0f);
	if(buff[0]<=0x49) year+=100;
	buff[1]=(buff[1] >>4)*10+(buff[1] & 0x0f);
	buff[2]=(buff[2] >>4)*10+(buff[2] & 0x0f);
	buff[3]=(buff[3] >>4)*10+(buff[3] & 0x0f);
	buff[4]=(buff[4] >>4)*10+(buff[4] & 0x0f);*/
	buff[5]=(buff[5] >>4)*10+(buff[5] & 0x0f);
	sec=buff[5];
	//Lib_LcdPrintxy(0,1*8,0," %04d/%02d/%02d %02d:%02d:%02d ",year,buff[1],buff[2],buff[3],buff[4],buff[5]);
	//Lib_LcdPrintxy(0,1*8,0,"   %02d.%02d.%04d %02d:%02d  ",buff[2],buff[1],year,buff[3],buff[4]);
	return sec;
}
