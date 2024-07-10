#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "public.h"  
#include "string.h"

#define WLS_TM  3
extern int Test_WlsAuTo(void);
extern int DispBattery(uchar x, uchar y);
extern int DispWlsSignal(uchar x, uchar y);
extern int AllTest_Battery(void);
extern int All_test_Print(void);
extern int ADM_GetRoutes3();

int LnkAdmin()
{
	int eodRes;
	int intCount=6;
	int iret;
	int page=0;
	int comres;
	int funcRes=0;
	int ZExists=0;
	unsigned char ucKey;
	char rcvData;
	int mxLen;
	uchar rcvdSChar[1];
	uchar ucWlsResetFlag;
	char ucMainList[intCount][15];
	strcpy(ucMainList[0],"1 SULGE PAEV  ");
	strcpy(ucMainList[1],"2 LAE LIINID  ");
	strcpy(ucMainList[2],"3 MAARA AEG   ");
	strcpy(ucMainList[3],"4 INFO: KASSA ");
	strcpy(ucMainList[4],"5 RESTART     ");
	strcpy(ucMainList[5],"6 DEV         ");
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
		/*case 2:
			Lib_LcdGotoxy(0,2*8);
			Lib_Lcdprintf(ucMainList[6]);
			Lib_LcdGotoxy(0,4*8);
			Lib_Lcdprintf(ucMainList[7]);
			Lib_LcdGotoxy(0,6*8);
			Lib_Lcdprintf(ucMainList[8]);
			break;*/
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
			if(ucKey==KEYCANCEL || ucKey==KEYMENU){
				Lib_KbFlush();
				return 0;
				break;
			}
		}
		if(!(ucKey>='0' && ucKey<='9')) continue;
		switch(ucKey){
			case '1':
				ZExists=Lib_FileExist("z");
				//ZExists=-1;
				if (ZExists<-1) {
					Lib_LcdCls();
					Lib_KbFlush();
					DispTimer2();
					DispHeader();
					Lib_LcdGotoxy(0,2*8);
					Lib_Lcdprintf("Ei saa sulgeda, alusta paev enne!");
					BeepError();
					Lib_DelayMs(2000);
					Lib_KbFlush();
				} else {
					eodRes=ADM_GetEOD3();
					Lib_LcdGotoxy(0,2*8);
					Lib_Lcdprintf("3");
					Lib_DelayMs(50);	
					if (eodRes<0) {
						Lib_LcdCls();
						Lib_KbFlush();
						DispTimer2();
						DispHeader();
						Lib_LcdGotoxy(0,2*8);
						Lib_Lcdprintf("VIGA EOD PROTSEDUURIS!");
						Lib_DelayMs(2000);
					}
				}
				Lib_LcdGotoxy(0,2*8);
				Lib_Lcdprintf("2");
				Lib_DelayMs(50);
				//Lib_KbFlush();
				LnkShowMainMenu();
				break;
			case '2':
				ZExists=Lib_FileExist("z");
				ZExists=-1;
				if (ZExists>-1) {
					Lib_LcdCls();
					Lib_KbFlush();
					DispTimer2();
					DispHeader();
					Lib_LcdGotoxy(0,2*8);
					Lib_Lcdprintf("Ei saa laadida, sulge paev enne!");
					Lib_DelayMs(2000);
					Lib_KbFlush();
				} else {
					funcRes=ADM_GetRoutes3();
					Lib_LcdCls();
					Lib_KbFlush();
					DispTimer2();
					DispHeader();
					Lib_LcdGotoxy(0,2*8);
					Lib_Lcdprintf("Laetud %d liini",funcRes);
					Lib_DelayMs(5000);
					Lib_KbFlush();
				}
				//Test_KbFun(1);
				break;
			case '3':
				LnkSetDTTM();
				//CountRoutes();
				//LCD_Test(); 
				//ADM_ReadRoutes2();
				break;
			case '4':
				//CLK_Test();
				//lnkshowinfo();
				break;
			case '5': 
				Lib_Reboot();
				//Lib_Reboot();
				//Test_Wls();
				break;
			case '6':
				//PRN_Test();	 
				//Lib_Reboot();
				break; 
			case '7':
				//CLK_Test();
				break;
			case '8': 
				//Test_Wls();
				break;
			case '9':
				//PRN_Test();	 
				break;
	}
	//return 0;
}

}


int LnkSetDTTM() {
	int setaeg=666;
	unsigned char newtime[7];
	uchar *settime[24];
	char tmpkuup[7];
	char kuup[7];
	char tmpkell[5];
	char kell[5];
	char zro[1]="0";
	//char dd[3];
	//char mm[3];
	//char yy[3];
	int rslt;
	int my;
	int mm;
	int md;
	int mh;
	int mmin;
	int mdhex;
	//Lib_LcdGotoxy(0,3*8);
	//Lib_Lcdprintf("Ootan serveri %d  ",elapseSec);
	memset(newtime,0,sizeof(newtime));
	memset(tmpkuup,0,sizeof(tmpkuup));
	memset(kell,0,sizeof(tmpkell));
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	Lib_LcdSetFont(8, 16, 0);
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Sisesta kuupaev (ddmmyy):");
	Lib_LcdGotoxy(0,5*8);
	rslt=Lib_KbGetStr((uchar*)kuup,6,6,0x61|0x10,60);
	//strncpy(newtime,"\x",2);
	//char *dd=substr(kuup,0,2);
	//char *mm=substr(kuup,2,4);
	//char *yy=substr(kuup,4,6);
	Lib_LcdCls();
	Lib_LcdSetFont(8, 16, 0);
	DispTimer2();
	DispHeader();
	Lib_LcdSetFont(8, 16, 0);
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Sisesta kellaaeg (hhmm):");
	Lib_LcdGotoxy(0,5*8);
	rslt=Lib_KbGetStr((uchar*)kell,4,4,0x61|0x10,60);
	
	//sprintf(newtime,"\\x%c%c\\x%c%c\\x%c%c\\x%c%c\\x%c%c\\x%c%c",kuup[4],kuup[5],kuup[2],kuup[3],kuup[0],kuup[1],kell[0],kell[1],kell[2],kell[3],zro[0],zro[0]);
	//sprintf(newtime,"\\x%c%c\\x%c%c\\x%c%c\\x%c%c\\x%c%c\\x%c%c",kuup[4],kuup[5],kuup[2],kuup[3],kuup[0],kuup[1],kell[0],kell[1],kell[2],kell[3],zro[0],zro[0]);
	md=atoi(&kuup[0])/10000;
	mm=atoi(&kuup[0])/100-md*100; 
	my=atoi(&kuup[0])-md*10000-mm*100;
	mh=atoi(&kell[0])/100;
	mmin=atoi(&kell[0])-mh*100; 	
	//sprintf(newtime,"%02d\\x%02d\\x%02d\\x%02d\\x%02d\\x%02d",my,mm,md,my,my,my);
	//sprintf(newtime,"%02d\\x%02d\\x%02x%02x%02x%02x",my,mm,md,my,my,my);
	//sprintf(settime,newtime);
	//setaeg=Lib_SetDateTime((unsigned char *)newtime);

	newtime[0]=(my/10)*16 + (my%10);
	newtime[1]=(mm/10)*16 + (mm%10);
	newtime[2]=(md/10)*16 + (md%10);
	newtime[3]=(mh/10)*16 + (mh%10);
	newtime[4]=(mmin/10)*16 + (mmin%10);
	newtime[5]=0 & 0xFF;
	//sprintf(newtime,"%c%c%c%c%c%c",my,mm,md,0,0,0);
	//////sprintf(newtime,"%d%d%d%d%d%d",((9/10)*16)+(9%10),((9/10)*16)+(9%10),((9/10)*16)+(9%10),((9/10)*16)+(9%10),((9/10)*16)+(9%10),((9/10)*16)+(9%10));
	setaeg=Lib_SetDateTime(newtime);//"\x09\x09\x09\x12\x00\x00");
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	if (setaeg<0) {
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("Viga kellaaja muutmisel: %d",setaeg);
		BeepError();
		Lib_DelayMs(2000);
	} else {
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("Kellaaeg muudetud.");
		BeepOK();
		Lib_DelayMs(1000);
	}
	
	return 0;
}

int ADM_GetEOD3() {
	char posPrint[10];
	char datetime[15];
	int posplen;
	char iPos[6];
	unsigned char rcvByte;
	char srvCMD[1];
	unsigned char rcvByteC;
	char srvCONF[1];
	char rcvData[7];
	char resData[7];
	int resLen;
	int comres;
	int fileHandle;
	int fileSize;
	uchar *fileData;
	int elapseSec=0;
	char *rcvHDR;
	char *rcvCMD;
	int recvLen;
	int bufSIZ;
	char *rcvSIZ;
	unsigned char kinnitus="+";
	int kinnitusLen;
	int i=0;
	kinnitusLen=sizeof(kinnitus);
	const char del[2]=">";
	comres=Lib_ComOpen(1,"115200,8,n,1");
	memset(rcvByte,0,sizeof(rcvByte));
	memset(rcvByteC,0,sizeof(rcvByteC));
	memset(srvCMD,0,sizeof(srvCMD));
	memset(srvCONF,0,sizeof(srvCONF));

	memset(iPos,0,sizeof(iPos));
	memset(posPrint,0,sizeof(posPrint));
	Lib_FileGetEnv("POSNR", iPos);
	memset(posPrint, '\0', sizeof(posPrint));
	sprintf(posPrint,"%s;",iPos);
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	Lib_LcdSetFont(8, 16, 0);
	//---------------------------WAITING FOR SERVER CONNECTION--------------------
	elapseSec=GetSec();
	Lib_LcdCls();
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Ootan serveri %d  ",elapseSec);
	Lib_ComReset(1);
	comres=Lib_ComRecvByte(1,&rcvByte,60000);
	if (comres<0) {
		Lib_ComReset(1);
		Lib_ComClose(1);
		elapseSec=GetSec();
		Lib_LcdCls();
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("Vastust ei ole, viga %d. Proovige uuesti!  ",comres);
		Lib_DelayMs(5000);
		return -1;
	}
	//----------------------------SERVER RESPONSE RECEIVED-----------------------
	sprintf(srvCMD,"EO%c",rcvByte);
	Lib_LcdCls();
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Serveri kask %c %s  ",rcvByte,srvCMD);
	Lib_DelayMs(1000);
	if (strstr(srvCMD,"EOD")==NULL) {
		Lib_ComReset(1);
		Lib_ComClose(1);
		elapseSec=GetSec();
		Lib_LcdCls();
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("Vale kask serverist. Proovige uuesti!  ");
		Lib_DelayMs(5000);
		return -1;
	}
	//---------------------GOT CONFIRM TO SEND EOD---------------------------
	Lib_LcdCls();
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("EOD kask serverist, valmistan andmed saatmiseks");
	Lib_DelayMs(1000);
	fileSize=Lib_FileSize("z");
	fileHandle=Lib_FileOpen("z", O_RDWR);
	if (fileHandle<0) {
		Lib_LcdCls();
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("Muugifail puudub, valjun %d",fileHandle);
		Lib_DelayMs(2000);
		return -1;
	}
	memset(fileData, '\0', fileSize);
	//----------------------SENDING POS ID---------------------------------
	Lib_LcdCls();
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Loen Z faili %d baiti %d",fileSize,comres);
	Lib_DelayMs(1000);
	comres=Lib_FileRead(fileHandle, fileData, fileSize);
	if (comres<0) {
		Lib_LcdCls();
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("Faili lugemise viga %d",comres);
		Lib_DelayMs(2000);
		return -1;
	}
	Lib_FileClose(fileHandle);
	Lib_DelayMs(1000);
	i=0;
	//-----------------------SENDING EOD DATA-----------------------------
	for(i;i<strlen(posPrint);i++) {
		Lib_LcdCls();
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("SAADAN KASSA NR");
		comres=Lib_ComSendByte(1,posPrint[i]);
		if (comres<0) {
			Lib_LcdCls();
			Lib_LcdGotoxy(0,3*8);
			Lib_Lcdprintf("Andmete1 edastamise viga %d, proovige uuesti",comres);
			Lib_DelayMs(2000);
			return -1;
		}
		//Lib_PrnInit();
		//Lib_PrnStr((BYTE*)"%c",posPrint[i]);
		//Lib_PrnStart();
		Lib_DelayMs(5);
	}
	i=0;
	for(i;i<fileSize;i++) {
		Lib_LcdCls();
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("SAADAN EOD: %c %d% d% ",fileData[i],i,fileSize);
		comres=Lib_ComSendByte(1,fileData[i]);
		if (comres<0) {
			Lib_LcdCls();
			Lib_LcdGotoxy(0,3*8);
			Lib_Lcdprintf("Andmete2 edastamise viga %d, proovige uuesti",comres);
			Lib_DelayMs(2000);
			return -1;
		}
		Lib_DelayMs(5);	
	}
	Lib_LcdCls();
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("EOD SAADETUD");
	Lib_DelayMs(1000);
	//-----------------------------CONFIRMING EOD SUCCESS-------------------------------------
	elapseSec=GetSec();
	Lib_ComReset(1);
	Lib_LcdCls();
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Saadan EOD kinnituse %d",elapseSec);
	Lib_DelayMs(1000);
	comres=Lib_ComSend(1,">",1);
	if (comres<0) {
		Lib_LcdCls();
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("Serverile kinnitus ei saadetud, viga %d, proovige uuesti",comres);
		Lib_DelayMs(2000);
		return -1;
	}
	//------------------------WAITING SERVER CONFIRMATION ON EOD OK------------------------
	elapseSec=GetSec();
	Lib_LcdCls();
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Ootan serveri %d kinnitust",elapseSec);
	Lib_ComReset(1);
	comres=Lib_ComRecvByte(1,&rcvByteC,60000);
	if (comres<0) {
		Lib_LcdCls();
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("Ei saanud kinnitus serverilt, viga %d, proovige uuesti",comres);
		Lib_DelayMs(2000);
		return -1;
	}	
	sprintf(srvCONF,"DO%c",rcvByteC);
	//--------------------------------RECEIVED SERVER RESPONSE--------------------------------
	Lib_LcdCls();
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Serveri vastus %c %s  ",rcvByteC,srvCONF);
	Lib_DelayMs(1000);
	if (strstr(srvCONF,"DOE")==NULL) {
		Lib_ComReset(1);
		Lib_ComClose(1);
		elapseSec=GetSec();
		Lib_LcdCls();
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("Vale vastus serverilt %c %d. Proovige uuesti!",rcvByteC,srvCONF);
		Lib_DelayMs(5000);
		return -1;
	}
	Lib_ComReset(1);
	Lib_ComClose(1);
	//-------------------------------------EOD DATA SENT SUCCESSFULLY--------------------------
	Lib_LcdCls();
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Serveri vastus OK! (%c %s)",rcvByteC,srvCONF);	
	Lib_DelayMs(2000);
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	Lib_LcdSetFont(16, 16, 0);
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("EOD tehtud");
	Lib_DelayMs(1000);
	//--------------------------------------PRINT REPORT AND EXIT------------------------------
	LnkPrintEOD();
	Lib_FilePutEnv("DSUMEUR", "0");
	Lib_FilePutEnv("DSUMRUB", "0");
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Aruanne trukitud");
	Lib_DelayMs(500);
	//comres=0;
	comres=Lib_FileRemove("z");
	if (comres<0) {
		elapseSec=GetSec();
		Lib_LcdCls();
		Lib_LcdGotoxy(0,3*8);
		Lib_Lcdprintf("Ei onnestunud tuhjendada Z fail, viga %d. KUTSUGE TEHNILINE ABI!",comres);
		Lib_DelayMs(10000);
		return -1;
	}
	//Lib_LcdGotoxy(0,2*8);
	//Lib_Lcdprintf("Fail kustutatud");
	//Lib_DelayMs(2000);
	return 0;
}

int ADM_GetRoutes3() {
	char rcvData[7];
	char *rcvHDR;
	char *rcvCMD;
	char rcvFNAME;
	char rcvFile;
	int comres;
	int recvLen;
	int bufSIZ;
	char *rcvSIZ;
	int elapseSec;
	int newFile=0;
	int fileHandle;
	int dlen;
	int *routesSaved;
	int *routeCount;
	char *fileName[2];
	const char del[2]=">";
	DeleteRoutes();
	routesSaved=0;
	comres=Lib_ComOpen(1,"115200,8,n,1");
	while(comres==0) {
		Lib_LcdCls();
		DispTimer2();
		DispHeader();
		Lib_LcdSetFont(8, 16, 0);
		Lib_LcdGotoxy(0,5*8);
		elapseSec=GetSec();
		Lib_Lcdprintf("Ootan %d sek.",elapseSec);
		comres=Lib_ComRecv(1,&rcvData,16,&recvLen,1000);
		if (recvLen>0) {
			Lib_LcdGotoxy(0,5*8);
			Lib_Lcdprintf("Sissetulev kask %d baiti",recvLen);
			Lib_DelayMs(1000);
			if (strstr(rcvData,">")==NULL)
			{
				Lib_ComReset(1);
			} else {
				rcvCMD=strtok(rcvData,del);
				rcvHDR=strtok(NULL,del);
				rcvSIZ=strtok(NULL,del);
				bufSIZ=atoi(rcvSIZ);
				Lib_LcdCls();
				Lib_LcdGotoxy(0,2*8);
				Lib_Lcdprintf("CMD: -%s-",rcvCMD);
				Lib_LcdGotoxy(0,3*8);
				Lib_Lcdprintf("BUF: -%d-",bufSIZ);
				if (strstr(rcvCMD,"R")!=NULL) { 
					//NEW ROUTE COMMAND
					routeCount=CountRoutes();
					Lib_LcdGotoxy(0,5*8);
					Lib_Lcdprintf("Laen liini %s suurusega %d baiti faili %d",rcvHDR,bufSIZ,routeCount);
					comres=Lib_ComSend(1,"S",1);
					comres=Lib_ComRecv(1,&rcvData,bufSIZ,&recvLen,10000);
					Lib_DelayMs(1000);
					if (recvLen>0) {
						sprintf(fileName, "%d", routeCount);
						fileHandle=Lib_FileOpen(fileName, O_CREATE);
						if (fileHandle==-8001) {
							fileHandle=Lib_FileRemove(fileName);
							Lib_LcdGotoxy(0,5*8);
							Lib_Lcdprintf("Recreate file %d",fileHandle);
							Lib_DelayMs(100);
							fileHandle=Lib_FileOpen(fileName, O_CREATE);
						}
						dlen=strlen(rcvData);
						Lib_FileWrite(fileHandle, rcvData, dlen);
						Lib_FileClose(fileHandle);
						routesSaved=routesSaved+1;
						Lib_LcdGotoxy(0,5*8);
						Lib_Lcdprintf("Liinifail %s salvestatud, jatkan",fileName);
						comres=Lib_ComSend(1,"R",1);
						Lib_DelayMs(500);
					} else {
						Lib_LcdGotoxy(0,5*8);
						Lib_Lcdprintf("Ei ole vastust");
						comres=Lib_ComSend(1,"N",1);
					}
					Lib_ComReset(1);
				} else if (strstr(rcvCMD,"E")!=NULL) {
					//END OF ROUTES COMMAND
					comres=Lib_ComSend(1,"E",1);
					Lib_LcdGotoxy(0,5*8);
					Lib_Lcdprintf("%d liini laetud, sulgen...",routesSaved);
					Lib_DelayMs(2000);
					Lib_ComReset(1);
					comres=Lib_ComClose(1);
					//LnkAdmin();
					//main();
					LnkShowMainMenu();
					//return 0;
					//comres=2;
					break;
				}
			}
		}
		if (comres==0) {
		} else if(comres==-6504) {
			//Lib_LcdGotoxy(0,5*8);
			//Lib_Lcdprintf("no data (-6504) %d",elapseSec);
			//Lib_DelayMs(1000);
			comres=0;
		} else {
			Lib_LcdGotoxy(0,5*8);
			Lib_Lcdprintf("RS232 vastus %05d",comres);
			Lib_DelayMs(1000);
			break;
		}
		if (Lib_KbCheck()==0) {
			//Lib_Lcdprintf("Key press exit");
			//Lib_DelayMs(1000);
			//Lib_ComClose(1);
			//comres=2;
			Lib_KbFlush();
			break;
		}
	} 
	Lib_LcdGotoxy(0,5*8);
	Lib_Lcdprintf("Will beep");
	Lib_DelayMs(1000);
	//Lib_Beef(0,500);
	Lib_LcdGotoxy(0,5*8);
	Lib_Lcdprintf("Will close");
	Lib_DelayMs(2000);
	comres=Lib_ComClose(1);
	Lib_LcdGotoxy(0,5*8);
	Lib_Lcdprintf("Will exit Sub   %d",comres);
	Lib_DelayMs(2000);	
	return 0;
}

int CountRoutes() {
	int result=0;
	char *fileName[2];
	Lib_LcdCls();
	Lib_LcdSetFont(8, 16, 0);
	for (result; result<100; result++) {
		sprintf(fileName, "%d", result);
		//Lib_LcdGotoxy(0,5*8);
		//Lib_Lcdprintf("checking %s",fileName);
		if (Lib_FileExist(fileName)<0) break;
		//Lib_DelayMs(50);
		//result=result+1;
	}
	//Lib_LcdGotoxy(0,5*8);
	//Lib_Lcdprintf("Kassas on %d liini",result);
	//Lib_DelayMs(100);
	return result;
}

int DeleteRoutes()
{
	Lib_LcdSetFont(8, 16, 0);
	int i=0;
	char *fileName[2];
	int fileHandle;
	for (i; i<100; i++) 
	{
		//Lib_LcdGotoxy(0,5*8);
		//Lib_Lcdprintf("Kustutan liinid",fileName,i);
		sprintf(fileName,"%d",i);
		Lib_LcdCls();
		//Lib_LcdGotoxy(0,5*8);
		//Lib_Lcdprintf("checking if exists %s (%d)",fileName,i);
		Lib_DelayMs(50);
		if (Lib_FileExist(fileName)<0) 
		{
			Lib_LcdSetFont(8, 16, 0);
			Lib_LcdCls();
			Lib_LcdGotoxy(0,5*8);
			Lib_Lcdprintf("%d liini kustutatud",i);
			Lib_DelayMs(100);
			break;
			//return i;
		} else {
			Lib_LcdSetFont(8, 16, 0);
			Lib_LcdCls();
			Lib_LcdGotoxy(0,5*8);
			Lib_Lcdprintf("Kustutan liini %d",i);
			fileHandle=Lib_FileRemove(fileName);
			Lib_DelayMs(100);
		}
	}
	return i;
}

int SaveToFile(char *fname, BYTE *data)
{
	int fileHandle;
	int fileHandle2;
	int dlen;
	dlen=strlen(data);
	fileHandle=Lib_FileOpen(fname,O_RDWR);
	Lib_FileWrite(fileHandle, data, dlen);
	Lib_LcdCls();
	Lib_LcdGotoxy(0,2*8);
	fileHandle2=Lib_FileClose(fileHandle);
	Lib_Lcdprintf("Write %s %d %d:%s",fname,fileHandle,fileHandle2,data);
	Lib_DelayMs(2000);
	
	return 0;
}

int ADM_GetEOD() {
	char posPrint[10];
	char datetime[15];
	int posplen;
	char iPos[6];
	char rcvData[7];
	char resData[7];
	int resLen;
	int comres;
	int fileHandle;
	int fileSize;
	char *fileData;
	int elapseSec=0;
	char *rcvHDR;
	char *rcvCMD;
	int recvLen;
	int bufSIZ;
	char *rcvSIZ;
	unsigned char kinnitus="+";
	int kinnitusLen;
	int i=0;
	kinnitusLen=sizeof(kinnitus);
	const char del[2]=">";
	comres=Lib_ComOpen(1,"115200,8,n,1");
	memset(iPos,0,sizeof(iPos));
	memset(posPrint,0,sizeof(posPrint));
	Lib_FileGetEnv("POSNR", iPos);
	memset(posPrint, '\0', sizeof(posPrint));
	sprintf(posPrint,"%s;",iPos);
	Lib_ComReset(1);
	while(comres==0) {
		Lib_LcdCls();
		DispTimer2();
		DispHeader();
		Lib_LcdSetFont(8, 16, 0);
		Lib_LcdGotoxy(0,5*8);
		elapseSec=GetSec();
		Lib_Lcdprintf("Ootan %d sek. (%s)%d",elapseSec,posPrint,strlen(posPrint));
		Lib_ComReset(1);
		comres=Lib_ComRecv(1,&rcvData,16,&recvLen,1000);
		if (recvLen>0) {
			Lib_LcdGotoxy(0,5*8);
			Lib_Lcdprintf("Sissetulev kask %d baiti",recvLen);
			Lib_DelayMs(1000);
			if (strstr(rcvData,">")==NULL)
			{
				Lib_ComReset(1);
			} else {
				rcvCMD=strtok(rcvData,del);
				rcvHDR=strtok(NULL,del);
				rcvSIZ=strtok(NULL,del);
				bufSIZ=atoi(rcvSIZ);
				Lib_ComReset(1);
				Lib_LcdCls();
				Lib_LcdGotoxy(0,2*8);
				Lib_Lcdprintf("CMD: -%s-",rcvCMD);
				Lib_LcdGotoxy(0,3*8);
				Lib_Lcdprintf("BUF: -%d-",bufSIZ);
				if (strstr(rcvCMD,"D")!=NULL) { 
					//EOD COMMAND - SENDING EOD
					Lib_LcdGotoxy(0,5*8);
					Lib_Lcdprintf("Saadan EOD");
					fileSize=Lib_FileSize("z");
					fileHandle=Lib_FileOpen("z", O_RDWR);
					if (fileHandle<0) {
						Lib_LcdGotoxy(0,5*8);
						Lib_Lcdprintf("Muugifail puudub, valjun %d",fileHandle);
						Lib_DelayMs(500);
						return 0;
						break;
					}
					comres=Lib_FileRead(fileHandle, fileData, fileSize);
					Lib_LcdGotoxy(0,5*8);
					Lib_Lcdprintf("Loen Z faili %d baiti",fileSize);
					Lib_DelayMs(3000);
					for(i;i<fileSize;i++) {
						Lib_LcdGotoxy(0,5*8);
						Lib_Lcdprintf("SAADAN: %s ",fileData[i]);
						comres=Lib_ComSendByte(1,fileData[i]);
						Lib_DelayMs(5000);	
					}
					return 0;
					//comres=Lib_ComSend(1,"1;",2);
					posplen=strlen(posPrint);
					Lib_Lcdprintf("Saadan %s ",posPrint);
					Lib_DelayMs(5000);
					elapseSec=GetSec();
					Lib_LcdGotoxy(0,5*8);
					Lib_Lcdprintf("VASTUS |%s|%d %d    ",posPrint,posplen,elapseSec);
					comres=Lib_ComSend(1,posPrint,posplen);
					elapseSec=GetSec();
					Lib_LcdGotoxy(0,5*8);
					Lib_Lcdprintf("HEADEROK |%s|%d %d    ",posPrint,posplen,elapseSec);
					comres=Lib_ComSend(1,fileData,fileSize);
					elapseSec=GetSec();
					Lib_LcdGotoxy(0,5*8);
					Lib_Lcdprintf("VASTUS |%s|%d %d X   ",posPrint,posplen,elapseSec);
					//Lib_ComReset(1);
					
					Lib_DelayMs(5000);
					Lib_FileClose(fileHandle);
					while(1) {
						elapseSec=GetSec();
						Lib_ComReset(1);
						comres=Lib_ComSend(1,">",1);
						Lib_LcdGotoxy(0,5*8);
						Lib_Lcdprintf("Saadan kinnituse %d ",elapseSec);
						Lib_ComReset(1);
						Lib_DelayMs(1000);
						comres=Lib_ComRecv(1,&rcvData,16,&recvLen,2000);
						if (recvLen>0) {
							Lib_LcdGotoxy(0,5*8);
							Lib_Lcdprintf("OK");
							Lib_DelayMs(1000);
							if (strstr(rcvData,"E>")==NULL) {
								Lib_ComReset(1);
							} else {
								break;
							}
						}
					}
					break;
				}
			}
		}
		if (comres==0) {
		} else if(comres==-6504) {
			comres=0;
		} else {
			Lib_LcdGotoxy(0,5*8);
			Lib_Lcdprintf("RS232 vastus %05d",comres);
			Lib_DelayMs(1000);
			break;
		}
		if (Lib_KbCheck()==0) {
			Lib_KbFlush();
			break;
		}
	}
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	Lib_LcdSetFont(16, 16, 0);
	Lib_LcdGotoxy(0,2*8);
	Lib_FilePutEnv("SALESUM", "0");
	Lib_Lcdprintf("EOD tehtud %d",comres);
	//LnkPrintEOD();
	comres=Lib_FileRemove("z");
	Lib_ComClose(1);
	Lib_DelayMs(2000);
	return 0;
}

int ADM_GetEOD2() {
	char posPrint[10];
	char datetime[15];
	int posplen;
	char iPos[6];
	char rcvData[7];
	char resData[7];
	int resLen;
	int comres;
	int fileHandle;
	int fileSize;
	uchar *fileData;
	int elapseSec=0;
	char *rcvHDR;
	char *rcvCMD;
	int recvLen;
	int bufSIZ;
	char *rcvSIZ;
	unsigned char kinnitus="+";
	int kinnitusLen;
	int i=0;
	kinnitusLen=sizeof(kinnitus);
	const char del[2]=">";
	comres=Lib_ComOpen(1,"115200,8,n,1");
	memset(iPos,0,sizeof(iPos));
	memset(posPrint,0,sizeof(posPrint));
	Lib_FileGetEnv("POSNR", iPos);
	memset(posPrint, '\0', sizeof(posPrint));
	sprintf(posPrint,"%s;",iPos);
	Lib_ComReset(1);
	while(comres==0) {
		Lib_LcdCls();
		DispTimer2();
		DispHeader();
		Lib_LcdSetFont(8, 16, 0);
		Lib_LcdGotoxy(0,5*8);
		elapseSec=GetSec();
		Lib_Lcdprintf("Ootan %d sek.  ",elapseSec);
		Lib_ComReset(1);
		comres=Lib_ComRecv(1,&rcvData,16,&recvLen,2000);
		if (recvLen>0) {
			Lib_LcdGotoxy(0,5*8);
			Lib_Lcdprintf("Sissetulev kask %d baiti",recvLen);
			Lib_DelayMs(1000);
			if (strstr(rcvData,">")==NULL)
			{
				Lib_ComReset(1);
			} else {
				rcvCMD=strtok(rcvData,del);
				rcvHDR=strtok(NULL,del);
				rcvSIZ=strtok(NULL,del);
				bufSIZ=atoi(rcvSIZ);
				Lib_ComReset(1);
				Lib_LcdCls();
				Lib_LcdGotoxy(0,2*8);
				Lib_Lcdprintf("CMD: -%s-",rcvCMD);
				Lib_LcdGotoxy(0,3*8);
				Lib_Lcdprintf("BUF: -%d-",bufSIZ);
				if (strstr(rcvCMD,"D")!=NULL) { 
					//EOD COMMAND - SENDING EOD
					Lib_LcdGotoxy(0,5*8);
					Lib_Lcdprintf("Saadan EOD");
					Lib_DelayMs(2000);
					fileSize=Lib_FileSize("z");
					fileHandle=Lib_FileOpen("z", O_RDWR);
					if (fileHandle<0) {
						Lib_LcdGotoxy(0,5*8);
						Lib_Lcdprintf("Muugifail puudub, valjun %d",fileHandle);
						Lib_DelayMs(500);
						return 0;
						break;
					}
					memset(fileData, '\0', fileSize);
					Lib_LcdGotoxy(0,5*8);
					Lib_Lcdprintf("Loen Z faili %d baiti %d",fileSize,comres);
					Lib_DelayMs(2000);
					comres=Lib_FileRead(fileHandle, fileData, fileSize);
					Lib_FileClose(fileHandle);
					Lib_DelayMs(2000);
					i=0;
					for(i;i<strlen(posPrint);i++) {
						Lib_LcdGotoxy(0,5*8);
						//Lib_Lcdprintf("SAADAN: %c %d/%d ",fileData[i],i,fileSize);
						//while(comres<0) {
							comres=Lib_ComSendByte(1,posPrint[i]);
						//}
						Lib_PrnInit();
						Lib_PrnStr((BYTE*)"%c",posPrint[i]);
						Lib_PrnStart();
						Lib_DelayMs(100);
						
					}
					i=0;
					comres=-1;
					for(i;i<fileSize;i++) {
						Lib_LcdGotoxy(0,5*8);
						Lib_Lcdprintf("SAADAN: %c %d% d% ",fileData[i],i,fileSize);
						//while(comres<0) {
							comres=Lib_ComSendByte(1,fileData[i]);
						//}
						//Lib_PrnInit();
						//Lib_PrnStr((BYTE*)"%4d) %c %d %d\n",i,fileData[i],fileData[i],fileSize);
						//Lib_PrnStart();
						Lib_DelayMs(1000);	
					}
					
					//comres=Lib_ComSend(1,"1;",2);
					//posplen=strlen(posPrint);
					//Lib_Lcdprintf("Saadan %s ",posPrint);
					//Lib_DelayMs(5000);
					//elapseSec=GetSec();
					//Lib_LcdGotoxy(0,5*8);
					//Lib_Lcdprintf("VASTUS |%s|%d %d    ",posPrint,posplen,elapseSec);
					//comres=Lib_ComSend(1,posPrint,posplen);
					//elapseSec=GetSec();
					//Lib_LcdGotoxy(0,5*8);
					//Lib_Lcdprintf("HEADEROK |%s|%d %d    ",posPrint,posplen,elapseSec);
					//comres=Lib_ComSend(1,fileData,fileSize);
					//elapseSec=GetSec();
					//Lib_LcdGotoxy(0,5*8);
					//Lib_Lcdprintf("VASTUS |%s|%d %d X   ",posPrint,posplen,elapseSec);
					//Lib_ComReset(1);
					
					//Lib_DelayMs(2000);
					
					while(1) {
						elapseSec=GetSec();
						Lib_ComReset(1);
						comres=Lib_ComSend(1,">",1);
						Lib_LcdGotoxy(0,5*8);
						Lib_Lcdprintf("Saadan kinnituse %d   ",elapseSec);
						//Lib_ComReset(1);
						Lib_DelayMs(1000);
						comres=Lib_ComRecv(1,&rcvData,16,&recvLen,2000);
						if (recvLen>0) {
							Lib_LcdGotoxy(0,5*8);
							Lib_Lcdprintf("Vastus serverilt             ");
							Lib_DelayMs(1000);
							if (strstr(rcvData,"E>")==NULL) {
								Lib_ComReset(1);
							} else {
								Lib_LcdGotoxy(0,5*8);
								Lib_Lcdprintf("Server kinnitas EOD OK! :)  ");
								Lib_DelayMs(1000);
								break;
							}
						}
					}
					Lib_LcdGotoxy(0,5*8);
					Lib_Lcdprintf("Valjusin kinnitustsuklist   ");
					Lib_DelayMs(2000);
					comres==0;
					break;
				}
			}
		}
		if (comres==0) {
		} else if(comres==-6504) {
			comres=0;
		} else {
			Lib_LcdGotoxy(0,5*8);
			Lib_Lcdprintf("RS232 vastus %05d",comres);
			Lib_DelayMs(1000);
			break;
		}
		if (Lib_KbCheck()==0) {
			//Lib_KbFlush();
			//break;
		}
	}
	Lib_LcdGotoxy(0,5*8);
	Lib_Lcdprintf("Valjusin peatsuklist   ");
	Lib_DelayMs(3000);
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	Lib_LcdSetFont(16, 16, 0);
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("EOD tehtud %d",comres);
	Lib_DelayMs(2000);
	LnkPrintEOD();
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("ARUANNE TRUKITUD");
	Lib_FilePutEnv("SALESUM", "0");
	comres=Lib_FileRemove("z");
	Lib_ComClose(1);
	Lib_DelayMs(2000);
	return 0;
}
