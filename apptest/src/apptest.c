#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "public.h"  


#define APP_NAME		"tiPOS"
#define APP_FULLNAME	"tiPOS Lonestar"
#define MAJ_VER			"1"
#define MIN_VER			"6"
#define BLD_VER			"614"


const APP_MSG App_Msg={
	"tiPOS Lonestar", 
	"1.1",
	0,
	0,
	0,
	"",
	"Test",
	"", 
	0,
	""
};


extern int Test_WlsAuTo(void);
extern int DispBattery(uchar x, uchar y);
extern int DispWlsSignal(uchar x, uchar y);
extern int AllTest_Battery(void);
extern int All_test_Print(void);


#define WLS_RST_TM    1
#define WLS_TM  3

uchar g_WlsResetStep = 0xff;
uchar g_NeedWlsReset = 0;  //1 need , 0 no need
uchar g_CurSimChannel;  //0- SIM1, 1-SIM2

uchar g_BatChargeStep = 0;
int testmenu(void)
{
	int iret;
	int page=0;
	unsigned char ucKey;
	uchar ucWlsResetFlag;

	
	Lib_AppInit(); 
	 
 
    /*if(Lib_FileExist("EnvFile") < 0)
    {
		//д╛хо
		Lib_FilePutEnv("DIALNUM", "CMNET");
		Lib_FilePutEnv("USERID", "card");
		Lib_FilePutEnv("USERPWD", "card");
		Lib_FilePutEnv("IP", "");
		Lib_FilePutEnv("PORT", "9005");
    }*/	

	Lib_LcdCls();
	Lib_Lcdprintf("Starting\n%s\nv.%s.%s.%s",APP_FULLNAME,MAJ_VER,MIN_VER,BLD_VER);
	/*g_CurSimChannel = CHANNEL_SIM1;
	Wls_SelectSim(g_CurSimChannel);
	Wls_Reset();
	if (Wls_Init() != 0)
	{
		if (Wls_Init() != 0)
		{
			Lib_LcdCls();
			Lib_Lcdprintf("\n\n Reset error!!!");
			Lib_KbGetCh();
		}
	}*/
	//Lib_LcdCls();
	//Lib_Lcdprintf("\nReset Wireless OK");
	Lib_DelayMs(1500);

	while(1){
		Lib_LcdCls();
		Lib_KbFlush();
		DispTimer2();
		//int batt=Lib_GetBatteryVolt();
		uchar chrbt;
		chrbt=Lib_GetBatteryVolt();
		//uchar BattPwr= static_cast<ostringstream*>( &(ostringstream() << batt) )->str();
		Lib_LcdPrintxy(0,0*8,0x80,"%s v%s.%s.%s %s",APP_NAME,MAJ_VER, MIN_VER,BLD_VER,BLD_VER);
		Lib_LcdClrLine(1*8, 8*8);
		Lib_LcdSetFont(16, 16, 0);
		if(page==0){
			Lib_LcdGotoxy(0,2*8);
			Lib_Lcdprintf("  1-BEEP 2-KB  ");
			Lib_LcdGotoxy(0,4*8);
			Lib_Lcdprintf("  3-LCD  4-RTC ");
			Lib_LcdGotoxy(0,6*8);
			Lib_Lcdprintf("  5-WLS  6-PRN ");
		}
		else{
			Lib_LcdGotoxy(0,2*8); 
			Lib_Lcdprintf("  1-BAT 2-PICC");
		}
		
		while(1){
			DispTimer2();
			DispWlsSignal(0, 2*8);
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
				page ^= 1;
				Lib_SetTimer(WLS_TM, 0);   
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
				BeepFun_Test();
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

int BeepFun_Test(void)
{
	Lib_LcdClrLine(2*8,7*9);
	Lib_LcdPrintxy(0,2*8,1," Beep Test...");
	Lib_Beef(7,200);
	Lib_DelayMs(300);
	Lib_Beef(1,200);
	Lib_DelayMs(300);
	Lib_Beef(2,200);
	Lib_DelayMs(300);
	Lib_Beef(3,200);
	Lib_DelayMs(300);
	Lib_Beef(4,200);
	Lib_DelayMs(300);
	Lib_Beef(5,200);
	Lib_DelayMs(300);
	Lib_Beef(6,200);
	Lib_DelayMs(300);
	Lib_LcdClrLine(2*8,7*9); 
	Lib_LcdPrintxy(0,4*8,1,"  Beep OK ");
	Lib_LcdDrawBox(0,15,127,63);
	Lib_DelayMs(1000);
	return 0;
}


