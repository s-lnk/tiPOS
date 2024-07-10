#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "public.h"  
#include "string.h"

extern int Test_WlsAuTo(void);
extern int DispBattery(uchar x, uchar y);
extern int DispWlsSignal(uchar x, uchar y);
extern int AllTest_Battery(void);
extern int All_test_Print(void);

int LnkPrintHeaderFooter()
{
	char datetime[18];
	char iPos[6];
	memset(iPos,0,sizeof(iPos));
	GetDTToPrint(datetime);
	Lib_FileGetEnv("POSNR", iPos);
	Lib_PrnInit();
	Lib_PrnSetFont(24,24,0x10);
	Lib_PrnStr((BYTE*)"********************************\n");
	Lib_PrnStr((BYTE*)"%s       %s\n",datetime,iPos);
	Lib_PrnStr((BYTE*)"      website\n");
	Lib_PrnStr((BYTE*)"\n\n\n\n");
	Lib_PrnSetFont(16, 16, 0x33);
	Lib_PrnStr((BYTE*)"     header 0\n");
	Lib_PrnSetFont(24,24,0x10);
	Lib_PrnStr((BYTE*)"Header row 1\n");
	Lib_PrnStr((BYTE*)"Header row 2\n");

	Lib_PrnStart();
	return 0;
}

int LnkPrintEOD()
{
	//--------------GLOBAL COUNTERS AND REPORT
	uchar iPos[7];
	uchar prkuup[17];
	uchar cMuukP1[8];
	uchar cMuukG1[8];
	uchar cMuukP2[8];
	uchar cMuukG2[8];
	uchar cPilArv[6];
	uchar inkuup[19];
	long iPaevaSumma;
	long iUldSumma;
	long iPaevaSumma2;
	long iUldSumma2;
	int iPilArv;
	int getRes;
	int i;
 	memset(iPos,0,sizeof(iPos));
	memset(cMuukP1,0,sizeof(cMuukP1));
	memset(cMuukG1,0,sizeof(cMuukG1));
	memset(cMuukP2,0,sizeof(cMuukP2));
	memset(cMuukG2,0,sizeof(cMuukG2));
	memset(cPilArv,0,sizeof(cPilArv));
	memset(inkuup,0,sizeof(inkuup));
	memset(prkuup,0,sizeof(prkuup));
	Lib_FileGetEnv("POSNR", iPos);
	Lib_FileGetEnv("DSUMEUR", cMuukP1);
	Lib_FileGetEnv("GSUMEUR", cMuukG1);
	Lib_FileGetEnv("DSUMRUB", cMuukP2);
	Lib_FileGetEnv("GSUMRUB", cMuukG2);
	Lib_FileGetEnv("PILETNR", cPilArv);
	Lib_FileGetEnv("INITDT", inkuup);
	iPaevaSumma=atol(cMuukP1);
	iUldSumma=atol(cMuukG1);
	iPaevaSumma2=atol(cMuukP2);
	iUldSumma2=atol(cMuukG2);
	iPilArv=atoi(cPilArv);
	iPilArv=iPilArv-1;
	//----------------------------------------

	//Lib_LcdSetBackLight(2);
	GetDTnosecToPrint(prkuup);
	//Lib_LcdCls();
	//Lib_LcdGotoxy(0,2*8);
	//Lib_Lcdprintf("%s %s=%d %s",iPos,cMuukG,iUldSumma,cPilArv);
	//Lib_DelayMs(15000);
	Lib_PrnInit();
	Lib_PrnSetFont(16, 16, 0x33);
	Lib_PrnStr((BYTE*)"    VAHETUSE ARUANNE\n");//Max len = 22 for font 16x16x33
	Lib_PrnSetFont(24,24,0x10);
	Lib_PrnStr((BYTE*)"Kassa nr: %s\n",iPos);
	Lib_PrnStr((BYTE*)"Aeg:      %s\n",prkuup);
	Lib_PrnStr((BYTE*)"Summa:    %5d.%02d EUR\n",iPaevaSumma/100,iPaevaSumma%100);
	Lib_PrnStr((BYTE*)"Summa:    %5d.%02d RUB\n",iPaevaSumma2/100,iPaevaSumma2%100);
	Lib_PrnStr((BYTE*)"********************************\n");
	Lib_PrnStr((BYTE*)"Init. kuup.: %s\n",inkuup);
	Lib_PrnStr((BYTE*)"PILETEID KOKKU: %12d TK.\n",iPilArv);
	Lib_PrnStr((BYTE*)"SUMMA KOKKU:    %9d.%02d EUR\n",iUldSumma/100,iUldSumma%100);
	Lib_PrnStr((BYTE*)"SUMMA KOKKU:    %9d.%02d RUB\n",iUldSumma2/100,iUldSumma2%100);


	Lib_PrnSetFont(16,16,0x0);
	Lib_PrnStr((BYTE*)"tiPOS Lonestar v.1.6.614 LINK Company Ltd.\n");
	Lib_PrnSetFont(24,24,0x10);
	Lib_PrnStart();
	LnkPrintHeaderFooter();
	//Lib_LcdSetBackLight(1);
}




int LnkPrintRegister()
{
	int iRet;
	int fileHandle;
	char datetime[18];
	uchar iJuh[4];
	uchar iBus[4];
	uchar iRou[4];
	int liini_nr;
	int liini_nr_s;
	uchar liini_nimi;
	int fileExist;
	char *fileData;
	const char delimiter[2] = ";";
	char *token;
	int fileSize;
	Lib_PrnInit();
	memset(iJuh,0,sizeof(iJuh));
	memset(iBus,0,sizeof(iBus));
	memset(iRou,0,sizeof(iRou));
	Lib_FileGetEnv("JUHTNR", iJuh);
	Lib_FileGetEnv("BUSSNR", iBus);
	Lib_FileGetEnv("LIINNR", iRou);
	/*fileHandle=Lib_FileOpen(iRou,O_RDWR);
	if (fileHandle<0) {
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("Viga faili avamisel");
		Lib_DelayMs(2000);
		return -1;
	}
	fileSize=Lib_FileSize(iRou);
	Lib_LcdGotoxy(0,2*8);
	Lib_FileRead(fileHandle, fileData, fileSize);
	token = strtok(fileData, delimiter);
	liini_nr=atoi(token);
	token = strtok(NULL, delimiter);
	liini_nr_s=atoi(token);
	token = strtok(NULL, delimiter);
	strcpy(liini_nimi,token);
*/
	Lib_PrnSetFont(16, 16, 0x33);
	fileExist=Lib_FileExist("z");
	if (fileExist<0) {
		iRet = Lib_PrnStr((BYTE*)"     PAEVA ALGUS\n");
	} else {
		iRet = Lib_PrnStr((BYTE*)" LIINI REGISTREERIMINE\n");
	}
	Lib_PrnSetFont(24,24,0x10);
	iRet = Lib_PrnStr((BYTE*)"JUHT %s\n",iJuh);
	iRet = Lib_PrnStr((BYTE*)"BUSS %s\n",iBus);
	////iRet = Lib_PrnStr((BYTE*)"LIIN %d.%d (%s)\n",liini_nr,liini_nr_s,iRou);
	////iRet = Lib_PrnStr((BYTE*)"    %s\n",liini_nimi);
	iRet = Lib_PrnStr((BYTE*)"LIIN %s\n",iRou);
	iRet = Lib_PrnStart();
	LnkPrintHeaderFooter();
	return 0;
}


int GetDT(char *DTTM) {
	int year;
	//uchar DTTM[18];
	unsigned char font,buff[8];
	Lib_GetDateTime(buff);
	year=1900+(buff[0] >>4)*10+(buff[0] & 0x0f);
	if(buff[0]<=0x49) year+=100;
	buff[1]=(buff[1] >>4)*10+(buff[1] & 0x0f);
	buff[2]=(buff[2] >>4)*10+(buff[2] & 0x0f);
	buff[3]=(buff[3] >>4)*10+(buff[3] & 0x0f);
	buff[4]=(buff[4] >>4)*10+(buff[4] & 0x0f);
	buff[5]=(buff[5] >>4)*10+(buff[5] & 0x0f);
	sprintf(DTTM,"%02d.%02d.%04d %02d:%02d:%02d",buff[2],buff[1],year,buff[3],buff[4],buff[5]);
	Lib_LcdPrintxy(0,1*8,0,DTTM);
	Lib_DelayMs(5000);
	return 0;
	//Lib_LcdPrintxy(0,1*8,0," %04d/%02d/%02d %02d:%02d:%02d ",year,buff[1],buff[2],buff[3],buff[4],buff[5]);
	//Lib_LcdPrintxy(0,1*8,0,"   %02d.%02d.%04d %02d:%02d  ",buff[2],buff[1],year,buff[3],buff[4]);
	
}

int LnkPrintStuff() {
	char datetime[18];
	char iPos[6];
	int alfa=0;
	int v1;
	int prnres=0;
	memset(iPos,0,sizeof(iPos));
	Lib_FileGetEnv("POSNR", iPos);
	while(1) {
		v1=Lib_GetBatteryVolt();
		GetDTToPrint(datetime);
		alfa++;
		Lib_LcdSetBackLight('2');
		Lib_KbFlush();
		Lib_LcdCls();
		Lib_LcdSetFont(16, 16, 0);
		DispTimer2();
		DispHeader();
		Lib_KbFlush();
		
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("TEST PRINT %d",alfa);
		Lib_LcdGotoxy(0,4*8);
		Lib_Lcdprintf("VOLT %ld",v1);
		if (prnres==0) {
			Lib_LcdGotoxy(0,6*8);
			Lib_Lcdprintf("PRINTER %d",prnres);
			Lib_DelayMs(5000);
			Lib_PrnInit();
			Lib_PrnSetFont(24,24,0x10);
			Lib_PrnStr((BYTE*)"TESTPRINT %d\n",alfa);
			Lib_PrnStr((BYTE*)"BATTERY %ld volt\n",v1);
			Lib_PrnStr((BYTE*)"PRENTER RESP %d\n",prnres);
			Lib_PrnStr((BYTE*)"      LINE 1\n");
			Lib_PrnStr((BYTE*)"      LINE 2\n");
			Lib_PrnStr((BYTE*)"      LINE 3\n");
			Lib_PrnStr((BYTE*)"      LINE 4\n");
			Lib_PrnStr((BYTE*)"      LINE 5\n");
			Lib_PrnStr((BYTE*)"********************************\n");
			Lib_PrnStr((BYTE*)"%s       %s\n",datetime,iPos);
			Lib_PrnStr((BYTE*)"      footer 1\n");
			Lib_PrnStr((BYTE*)"\n\n\n\n");
			Lib_PrnSetFont(16, 16, 0x33);
			Lib_PrnStr((BYTE*)"     header 1\n");
			Lib_PrnSetFont(24,24,0x10);
			Lib_PrnStr((BYTE*)"header 2\n");
			Lib_PrnStr((BYTE*)"header 3\n");
			prnres=Lib_PrnStart();
		}
		Lib_LcdSetBackLight('0');
		Lib_DelayMs(5000);
	}
	return 0;
}