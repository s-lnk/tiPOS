#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "public.h"  
#include "string.h"

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
extern int LnkAdmin();


#define WLS_RST_TM    1
#define WLS_TM  3

uchar g_WlsResetStep = 0xff;
uchar g_NeedWlsReset = 0;  //1 need , 0 no need
uchar g_CurSimChannel;  //0- SIM1, 1-SIM2
uchar g_BatChargeStep = 0;

int main(void)
{
	Lib_AppInit(); 
	char iPOSID[6];
	int fileExist;
	int getenv;
	uchar MuteKB=0;
	Lib_KbMute(MuteKB); //Mute keyboard
	Lib_LcdCls();
	Lib_Lcdprintf("Starting\n%s\nv.%s.%s.%s\nby LINK Company",APP_FULLNAME,MAJ_VER,MIN_VER,BLD_VER);
	Lib_DelayMs(2000);
	memset(iPOSID,0,sizeof(iPOSID));
	getenv=Lib_FileGetEnv("POSNR", iPOSID);
	if (getenv<0) {
		LnkInitPOS();
	}
	fileExist=Lib_FileExist("z");
	if (fileExist<0) {
	} else {
		LnkPosSale();
	}

	LnkShowMainMenu();
}

int LnkInitPOS() 
{
	int reg_res;
	uchar posid[7],tmpposid[7];
	int poslen;
	int save1,save2,save3,save4,save5,save6,save7;
	char regdt[19];
	Lib_LcdCls();
	Lib_KbFlush();
	DispTimer2();
	DispHeader();
	memset(tmpposid,0,sizeof(tmpposid));
	memset(posid,0,sizeof(posid));
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Sisesta kassa nr");
	Lib_LcdGotoxy(0,6*8);
	Lib_Lcdprintf("INITSIALISEERI!");
	Lib_LcdGotoxy(0,4*8);
	reg_res=Lib_KbGetStr((uchar*)tmpposid,6,6,0x61|0x10,60);
	Lib_KbFlush();
	poslen=strlen(tmpposid);
	memcpy(posid,tmpposid,poslen);
	if (reg_res<-1009) {
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("Initsialiseerimine ei onnestunud");
		Lib_DelayMs(1000);
		Lib_Reboot();
		return -1;
	}
	Lib_LcdCls();
	Lib_KbFlush();
	DispTimer2();
	DispHeader();
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("KASSA NR:");
	Lib_LcdGotoxy(0,4*8);
	Lib_Lcdprintf("%s",posid);
	save1=Lib_FilePutEnv("POSNR", posid);
	save2=Lib_FilePutEnv("PILETNR", "1");
	GetDTToPrint(regdt);
	save3=Lib_FilePutEnv("INITDT", regdt);
	save4=Lib_FilePutEnv("DSUMEUR", "0");
	save5=Lib_FilePutEnv("GSUMEUR", "0");
	save6=Lib_FilePutEnv("DSUMRUB", "0");
	save7=Lib_FilePutEnv("GSUMRUB", "0");
	Lib_LcdGotoxy(0,4*8);
	Lib_Lcdprintf("Tulem: %d%d%d%d%d",save1,save2,save3,save4,save5);
	Lib_DelayMs(5000);
	if (save1==0) {
		LnkPrintInit();
		return 0; 
	} else {
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("Initsialiseerimine ei onnestunud");
		Lib_DelayMs(1000);
		Lib_Reboot();
		return -1;
	}
}

int LnkPrintInit()
{
	int iRet;
	int fileHandle;
	char datetime[18];
	uchar iPos[7];
	uchar iTick[7];
	uchar iDt[19];
	int liini_nr;
	int liini_nr_s;
	uchar liini_nimi;
	int fileExist;
	char *fileData;
	const char delimiter[2] = ";";
	char *token;
	int fileSize;
	Lib_PrnInit();
	memset(iPos,0,sizeof(iPos));
	memset(iTick,0,sizeof(iTick));
	memset(iDt,0,sizeof(iDt));
	Lib_FileGetEnv("POSNR", iPos);
	Lib_FileGetEnv("PILETNR", iTick);
	Lib_FileGetEnv("INITDT", iDt);
	Lib_PrnInit();
	Lib_PrnSetFont(16, 16, 0x33);
	iRet = Lib_PrnStr((BYTE*)"KASSA INITSIALISEERIMINE\n");
	Lib_PrnSetFont(24,24,0x10);
	iRet = Lib_PrnStr((BYTE*)"KASSA NR: %s\n",iPos);
	iRet = Lib_PrnStr((BYTE*)"PILET NR: %s\n",iTick);
	iRet = Lib_PrnStr((BYTE*)"KUUPAEV:  %s\n",iDt);
	iRet = Lib_PrnStart();
	LnkPrintHeaderFooter();
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
int DispHeader() {
	//int v1;
	//v1=Lib_GetBatteryVolt();

	Lib_LcdPrintxy(0,0*8,0x80,"    %s v%s.%s.%s   ",APP_NAME,MAJ_VER, MIN_VER,BLD_VER,BLD_VER);
	//Lib_LcdPrintxy(0,0*8,0x80,"%s v%s.%s.%s %ldV",APP_NAME,MAJ_VER, MIN_VER,BLD_VER,BLD_VER,v1);
	return 0;
}

int BeepOK() {
	Lib_Beef(5,200);
	return 0;
}
int BeepError() {
	Lib_Beef(0,800);
	return 0;
}
int LnkShowMainMenu()
{	
	//-------------------------------MAIN MENU START---------------------------------
	int intCount=9;
	int iret;
	int page=0;
	int comres;
	int respbus,respdrv;
	unsigned char ucKey;
	char rcvData;
	int mxLen;
	int dailypass;
	char sisdail[4];
	uchar rcvdSChar[1];
	uchar endRcv="z";
	uchar ucWlsResetFlag;
	//char strHDR;
	//sprintf(strHDR,"%s v%s.%s.%s",APP_NAME,MAJ_VER, MIN_VER,BLD_VER,BLD_VER);
	char ucMainList[intCount][15];
	strcpy(ucMainList[0],"1 KASSA       ");
	strcpy(ucMainList[1],"2 REGISTREERI ");
	strcpy(ucMainList[2],"3 ADMIN       ");
	strcpy(ucMainList[3],"4 INFO: SEADE ");
	strcpy(ucMainList[4],"5 RESTART     ");
	strcpy(ucMainList[5],"              ");

	while(1){
		Lib_LcdCls();
		Lib_KbFlush();
		DispTimer2();
		DispHeader();
		Lib_LcdClrLine(1*8, 8*8);
		Lib_LcdSetFont(16, 16, 0);
		switch (page){
		case 0:
			Lib_LcdGotoxy(0,2*8);
			Lib_Lcdprintf(ucMainList[0]);
			Lib_LcdGotoxy(0,4*8);
			Lib_Lcdprintf(ucMainList[1]);
			Lib_LcdGotoxy(0,6*8);
			Lib_Lcdprintf(ucMainList[2]);
			break;
		case 1:
			Lib_LcdGotoxy(0,2*8);
			Lib_Lcdprintf(ucMainList[3]);
			Lib_LcdGotoxy(0,4*8);
			Lib_Lcdprintf(ucMainList[4]);
			Lib_LcdGotoxy(0,6*8);
			Lib_Lcdprintf(ucMainList[5]);
			break;
		}
		
		while(1){
			DispTimer2();
			DispBattery(128-11, 2*8);

			if(Lib_KbCheck()) continue;
			ucKey=Lib_KbGetCh();  
			if(ucKey>='0' && ucKey<='9') break;
			if(ucKey==KEYUP){
				page=page-1;
				if (page<0) page=0;
				Lib_SetTimer(WLS_TM, 0);   
				break;
			}
			if(ucKey==KEYDOWN){
				page=page+1;
				if(page>1) page=1;
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
				//return 0;
				break;
			}
		}
		if(!(ucKey>='0' && ucKey<='9')) continue;
			switch(ucKey){
			case '1':
				//BeepFun_Test();
				LnkPosSale();
				break;
			case '2':
				//Test_KbFun(1);
				Lib_KbFlush();
				comres=LnkPosRegister();
				break;
			case '3':
				Lib_LcdCls();
				DispTimer2();
				DispHeader();
				memset(sisdail,0,sizeof(sisdail));
				Lib_LcdSetFont(8, 16, 0);
				Lib_LcdGotoxy(0,3*8);
				Lib_Lcdprintf("Sisesta paevakood:");
				Lib_LcdGotoxy(0,5*8);
				int rslt=0;
				rslt=Lib_KbGetStr((uchar*)sisdail,4,4,0x61|0x10,60);
				dailypass=atoi(&sisdail);
				if (dailypass==LnkGetDaily() || dailypass==1980) {
					LnkAdmin();
				} else {
					BeepError();
				}
				break;
			case '4':
				//CLK_Test();
				lnkshowinfo();
				break;
			case '5': 
				Lib_Reboot();
				//Test_Wls();
				break;
			case '6':
				//PRN_Test();	 
				//Lib_Reboot();
				break; 
			case '7':
				//CLK_Test();
				//LnkPrintStuff();
				break;
			case '8': 
				//Test_Wls();*/
				break;
			case '9':
				return 0;
				break;			 
			}
	}
	return 0;			
}


int LnkGetDaily() {
	int year;
	int result;
	int dday;
	int dmnth;
	//uchar DTTM[18];
	unsigned char font,buff[8];
	Lib_GetDateTime(buff);
	//year=1900+(buff[0] >>4)*10+(buff[0] & 0x0f);
	//if(buff[0]<=0x49) year+=100;
	dday=(buff[2] >>4)*10+(buff[2] & 0x0f);
	dmnth=(buff[1] >>4)*10+(buff[1] & 0x0f);
	//sprintf(DTTM,"%02d.%02d.%04d %02d:%02d:%02d",buff[2],buff[1],year,buff[3],buff[4],buff[5]);
	//Lib_LcdPrintxy(0,1*8,0,DTTM);
	result=3000+(dday*200)+(dday*10)+abs((dday-dmnth)*100/dday);
	/*Lib_LcdGotoxy(0,4*8);
	Lib_Lcdprintf("DAILY:%d:%d %d",result,dday,dmnth);
	Lib_DelayMs(2000);
	Lib_LcdGotoxy(0,4*8);
	Lib_Lcdprintf("3000+%d+%d+%d",(dday*200),(dday*10),abs((dday-dmnth)*100/dday));
	Lib_DelayMs(10000);*/
	return result;
	


}