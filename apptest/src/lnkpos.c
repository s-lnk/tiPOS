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

//#define CUR1	"RRR"
//#define CUR2	"RUB"
#define DIS1	30
#define DIS2	15
#define DIS3	5
#define NEAREST(number, multiple) (((number) + ((multiple) / 2)) / (multiple) * (multiple))

int LnkPosSale()
{
	//------SAVE Z DECLAR----
	int fileZHandle;
	char zData[480];
	int fileZSize;
	int fileZSeek;
	int zLen;
	//---------GLOBAL COUNTER-----
	char sum_global[8];
	char sum_day[8];
	/*char sum_globale[8];
	char sum_daye[8];
	char sum_globalr[8];
	char sum_dayr[8];*/
	long lng_sum_global;
	long lng_sum_day;
	//----------------------------
	int pilet_nr;
	long pilet_nr_long;
	char cpilet_nr[8];
	int allahindlus[4];
	int allah_id=0;
	allahindlus[0]=0;
	allahindlus[1]=DIS1;
	allahindlus[2]=DIS2;
	allahindlus[3]=DIS3;
	int posOnScreen=4;
	int currentID=0;
	int firstItem=0;
	char iJuh[4];
	char iBus[4];
	char iRou[4];
	char iPos[6];
	int i=0;
	int liini_nr;
	int liini_nr_s;
	char liini_nimi[128];
	int peatuste_arv;
	char CUR1[3]; //-------------------------------------CURRENCY HOLDER
	int hinna_umard;//-----------------------------------PRICE ROUND FOR DISCOUNT
	int peatus_st;
	int peatus_ni;
	int refresh_menu=0;
	int fileRHandle;
	int fileHandle;
	int fileSize;
	//char *fileData;
	uchar *fileLData;
	//char *fileRData;
	int fileRSize;
	const char delimiter[2]=";";
	char *token;
	int curTableID;
	int fileExist;
	int registerResult;
	int priceID;
	int realPrice;
	int realPVAT;
	int realWVAT;
	int realVVAT;
	int actualPrice;
	char datetime[15];
	char dtsave[14];
	unsigned char ucKey;
	int fnfrom;
	int fnto;
	Lib_LcdCls();
	fileExist=Lib_FileExist("z");
	if (fileExist<0) {
		//registerResult=LnkPosRegister();
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("REGISTREERI ENNEM!");
		BeepError();
		Lib_DelayMs(2000);
		return -1;
	} else {
		registerResult=0;
		//return -1;
	}
	if (registerResult<0) return -1;

	//LnkPrintStuff();
	//return 0;

	//---------------GET PARAMS------------
	memset(iJuh,0,sizeof(iJuh));
	memset(iBus,0,sizeof(iBus));
	memset(iRou,0,sizeof(iRou));
	memset(iPos,0,sizeof(iPos));
	memset(cpilet_nr,0,sizeof(cpilet_nr));
	memset(liini_nimi,0,sizeof(liini_nimi));
	memset(sum_day,0,sizeof(sum_day));
	memset(sum_global,0,sizeof(sum_global));
	Lib_FileGetEnv("JUHTNR", iJuh);
	Lib_FileGetEnv("BUSSNR", iBus);
	Lib_FileGetEnv("LIINNR", iRou);
	Lib_FileGetEnv("POSNR", iPos);
	Lib_FileGetEnv("PILETNR", cpilet_nr);
	Lib_FileGetEnv("DSUMEUR", sum_day);
	Lib_FileGetEnv("GSUMEUR", sum_global);
	//Lib_FileGetEnv("DSUMRUB", sum_dayr);
	//Lib_FileGetEnv("GSUMRUB", sum_globalr);

	pilet_nr=atoi(cpilet_nr);
	pilet_nr_long=atoi(cpilet_nr);
	lng_sum_day=atoi(sum_day);
	lng_sum_global=atoi(sum_global);
	//lng_sum_dr=atoi(sum_dayr);
	//lng_sum_gr=atoi(sum_globalr);

	//Lib_DelayMs(1000);
	fileRHandle=Lib_FileOpen(iRou,O_RDWR);
	if (fileRHandle<0) {
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("Viga faili avamisel %s %d",iRou,fileRHandle);
		Lib_DelayMs(5000);
		return -1;
	}
	fileRSize=Lib_FileSize(iRou);
	Lib_LcdGotoxy(0,2*8);
	Lib_FileRead(fileRHandle, fileLData, fileRSize);
	Lib_FileClose(fileRHandle);

	token = strtok(fileLData, delimiter);
	liini_nr=atoi(token); //-----------------------------------------LIINI_NR
	/*Lib_LcdGotoxy(0,0*8);
	Lib_Lcdprintf("L:*%d*",liini_nr);//,fileZSize,fileZSeek);
	Lib_DelayMs(1000);*/
	token = strtok(NULL, delimiter);
	liini_nr_s=atoi(token); //---------------------------------------LIINI_S_NR
	//Lib_LcdGotoxy(0,2*8);
	//Lib_Lcdprintf("liins %d %s//",liini_nr,token);//,fileZSize,fileZSeek);
	//Lib_DelayMs(2000);

	/*Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Ls:%d",liini_nr_s);//,fileZSize,fileZSeek);
	Lib_DelayMs(1000);*/
	token = strtok(NULL, delimiter);
	strcpy(liini_nimi,token);//--------------------------------------LIINI_NIMETUS
	//Lib_LcdGotoxy(0,2*8);
	//Lib_Lcdprintf("Nu");//,fileZSize,fileZSeek);
	//Lib_DelayMs(1000);
	token = strtok(NULL, delimiter);
	strcpy(CUR1,token);	//-----------------------------------LIINI_VALUUTA
	token = strtok(NULL, delimiter);
	hinna_umard=atoi(token);//---------------------------------------UMMARDUS
	/*Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Lu:%d",hinna_umard);//,fileZSize,fileZSeek);
	Lib_DelayMs(1000);*/
	token = strtok(NULL, delimiter);
	peatuste_arv=atoi(token);//--------------------------------------PEATUSTE_ARV
	/*Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("PA:%d",peatuste_arv);//,fileZSize,fileZSeek);
	Lib_DelayMs(1000);*/
	/*Lib_LcdGotoxy(0,6*8);
	Lib_Lcdprintf("READEDDD, read stops");
	Lib_DelayMs(3000);*/
	
	char *strPeatus[peatuste_arv][24];
	int intPrice1[(peatuste_arv*(peatuste_arv-1))/2];
	int intVAT1[(peatuste_arv*(peatuste_arv-1))/2];
	//Lib_LcdGotoxy(0,6*8);
	//Lib_Lcdprintf("GET ST %d ",i);
	/*if (strncmp(CUR1,"EUR",3)==0) {
		lng_sum_day=lng_sum_de;
		lng_sum_global=lng_sum_ge;
	} else {
		lng_sum_day=lng_sum_dr;
		lng_sum_global=lng_sum_gr;
	}*/

	//--------------GET STOPS--------------
	for (i;i<peatuste_arv;i++) {
		//Lib_LcdGotoxy(0,1*8);
		//Lib_Lcdprintf("GET ST %d",i);
		token = strtok(NULL, delimiter);
		strcpy(strPeatus[i],token);
		//Lib_LcdGotoxy(0,2*8);
		//Lib_Lcdprintf("P:%s",strPeatus[i]); //liin:%s (%s)",driver);
		//Lib_DelayMs(100);
	}
	//--------------GET PRICE CURRENCY1----


	Lib_LcdCls();
	i=0;
	for (i;i<(peatuste_arv*(peatuste_arv-1))/2;i++) {
		token = strtok(NULL, delimiter);
		intPrice1[i]=atoi(token);
		token = strtok(NULL, delimiter);
		intVAT1[i]=atoi(token);
		//Lib_LcdGotoxy(0,6*8);
		//Lib_Lcdprintf("H:%d %s(%d)",actualPrice,CUR1,intVAT1[i]); //liin:%s (%s)",driver);
		//Lib_DelayMs(100);
	}
	//---------------POS VIEW--------------		
	Lib_KbFlush();
	Lib_LcdSetFont(16, 16, 0);
	DispTimer2();
	//Lib_LcdGotoxy(0,6*8);
	//Lib_Lcdprintf("%d %s",(peatuste_arv*(peatuste_arv-1))/2,CUR1);
	Lib_LcdPrintxy(0,0*8,0x80,"JUHT:%4s LIIN:%4d.%1d",iJuh,liini_nr,liini_nr_s);
	//Lib_LcdPrintxy(0,0*8,0x80,"KASSA P:%d",peatuste_arv);
	Lib_KbFlush();
	peatus_st=0;
	peatus_ni=peatuste_arv-1;
	realPVAT=intVAT1[peatuste_arv-1];
	realPrice=intPrice1[peatuste_arv-1];
	actualPrice=realPrice;
	//realPVAT=intVAT1[(peatuste_arv*(peatuste_arv-1))/2];
	//Lib_LcdGotoxy(0,6*8);
	//Lib_Lcdprintf("<%d %d",actualPrice,realPVAT);

	//realPrice=intPrice1[i];
	//actualPrice=realPrice;
	Lib_LcdCls();
	Lib_LcdSetFont(16, 16, 0);
	Lib_LcdPrintxy(0,0*8,0x80,"JUHT:%4s LIIN:%4d.%1d",iJuh,liini_nr,liini_nr_s);
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("<%s",strPeatus[peatus_st]);
	Lib_LcdGotoxy(0,4*8);
	Lib_Lcdprintf(">%s",strPeatus[peatus_ni]);
	Lib_LcdGotoxy(0,6*8);
	//Lib_Lcdprintf("%d-%d: %d.%02d",peatus_st,peatus_ni,realPrice/100,realPrice%100); //liin:%s (%s)",driver);
	Lib_Lcdprintf("H %5d.%02d%s",actualPrice/100,actualPrice%100,CUR1);
	while(1) {
		DispTimer2();
		DispBattery(128-11, 2*8);
		if(Lib_KbCheck()) continue;
		ucKey=Lib_KbGetCh();  
		if(ucKey==KEYUP){
			//GET FROM STOP
			//---------------------------------------------------------------------------
			fnfrom=LnkGetStopByID();
			//---------------------------------------------------------------------------
			if (fnfrom<1) {
				posOnScreen=4;
				currentID=peatus_st;
				firstItem=peatus_st;
				if (peatus_st >= peatuste_arv-1) {
					firstItem=firstItem-1;
					posOnScreen=6;
				}
				Lib_KbFlush();
				Lib_LcdCls();
				Lib_LcdSetFont(16, 16, 0);
				DispTimer2();
				DispHeader();
				Lib_KbFlush();
				Lib_LcdGotoxy(0,2*8);
				Lib_Lcdprintf("Peatusest:");
				Lib_LcdGotoxy(0,4*8);
				Lib_Lcdprintf(" %s",strPeatus[firstItem]);
				Lib_LcdGotoxy(0,6*8);
				Lib_Lcdprintf(" %s",strPeatus[firstItem+1]);
				HighlightRouteItemBigFont(posOnScreen);
				while(1) {
					if(Lib_KbCheck()) continue;
					ucKey=Lib_KbGetCh(); 
					if(ucKey==KEYUP){
						if (posOnScreen==4 && currentID>0) {
							firstItem=firstItem-1;
							if (firstItem<0) firstItem=0;
							Lib_LcdGotoxy(0,4*8);
							Lib_Lcdprintf("              ");
							Lib_LcdGotoxy(0,6*8);
							Lib_Lcdprintf("              ");
							Lib_LcdGotoxy(0,4*8);
							Lib_Lcdprintf(" %s",strPeatus[firstItem]);
							Lib_LcdGotoxy(0,6*8);
							Lib_Lcdprintf(" %s",strPeatus[firstItem+1]);
						}
						posOnScreen=posOnScreen-2;
						currentID=currentID-1;
						if (currentID<0) currentID=0;
						if (posOnScreen<4) posOnScreen=4;
						HighlightRouteItemBigFont(posOnScreen);
						//Lib_LcdGotoxy(8*8,6*8);
						//Lib_Lcdprintf("%d-%d-%d",posOnScreen,currentID,firstItem);
					}
					if(ucKey==KEYDOWN){
						if (posOnScreen==6 && currentID<(peatuste_arv-1)) {
							firstItem=firstItem+1;
							Lib_LcdGotoxy(0,4*8);
							Lib_Lcdprintf("              ");
							Lib_LcdGotoxy(0,6*8);
							Lib_Lcdprintf("              ");
							Lib_LcdGotoxy(0,4*8);
							Lib_Lcdprintf(" %s",strPeatus[firstItem]);
							Lib_LcdGotoxy(0,6*8);
							Lib_Lcdprintf(" %s",strPeatus[firstItem+1]);	
						}
						currentID=currentID+1;
						posOnScreen=posOnScreen+2;
						if (currentID>(peatuste_arv-1)) currentID=(peatuste_arv-1);
						if (posOnScreen>6) posOnScreen=6;
						HighlightRouteItemBigFont(posOnScreen);
						//Lib_LcdGotoxy(8*8,6*8);
						//Lib_Lcdprintf("%d-%d-%d",posOnScreen,currentID,firstItem);
					}
					if(ucKey==KEYOK){
						Lib_LcdCls();
						Lib_LcdSetFont(16, 16, 0);
						DispTimer2();
						DispHeader();
						Lib_LcdGotoxy(0,2*8);
						peatus_st=currentID;
						Lib_Lcdprintf("Valitud %s %d",strPeatus[peatus_st],peatus_st);//,routeMain[currentID]);//,routeName[currentID]);
						Lib_DelayMs(1000);
						Lib_KbFlush();
						break;
					}
					if(ucKey==KEYCANCEL){
						Lib_KbFlush();
						break;
					}
	//---------------------------------------------------------------------------
				}
			} else {
				if (fnfrom>(peatuste_arv)) fnfrom=1;
				peatus_st=fnfrom-1;
			}
			refresh_menu=1;
		}
		if(ucKey==KEYDOWN){
			//GET TO STOP
//---------------------------------------------------------------------------
			fnto=LnkGetStopByID();
			//---------------------------------------------------------------------------
			if (fnto<1) {
				currentID=peatus_ni;
				firstItem=peatus_ni;
				posOnScreen=4;
				if (peatus_ni >= peatuste_arv-1) {
					firstItem=firstItem-1;
					posOnScreen=6;
				}
				Lib_KbFlush();
				Lib_LcdCls();
				Lib_LcdSetFont(16, 16, 0);
				DispTimer2();
				DispHeader();
				Lib_KbFlush();
				Lib_LcdGotoxy(0,2*8);
				Lib_Lcdprintf("Peatuseni:");
				Lib_LcdGotoxy(0,4*8);
				Lib_Lcdprintf(" %s",strPeatus[firstItem]);
				Lib_LcdGotoxy(0,6*8);
				Lib_Lcdprintf(" %s",strPeatus[firstItem+1]);
				HighlightRouteItemBigFont(posOnScreen);
				while(1) {
					if(Lib_KbCheck()) continue;
					ucKey=Lib_KbGetCh(); 
					if(ucKey==KEYUP){
						if (posOnScreen==4 && currentID>0) {
							firstItem=firstItem-1;
							if (firstItem<0) firstItem=0;
							Lib_LcdGotoxy(0,4*8);
							Lib_Lcdprintf("              ");
							Lib_LcdGotoxy(0,6*8);
							Lib_Lcdprintf("              ");
							Lib_LcdGotoxy(0,4*8);
							Lib_Lcdprintf(" %s",strPeatus[firstItem]);
							Lib_LcdGotoxy(0,6*8);
							Lib_Lcdprintf(" %s",strPeatus[firstItem+1]);
						}
						posOnScreen=posOnScreen-2;
						currentID=currentID-1;
						if (currentID<0) currentID=0;
						if (posOnScreen<4) posOnScreen=4;
						HighlightRouteItemBigFont(posOnScreen);
						//Lib_LcdGotoxy(8*8,6*8);
						//Lib_Lcdprintf("%d-%d-%d",posOnScreen,currentID,firstItem);
					}
					if(ucKey==KEYDOWN){
						if (posOnScreen==6 && currentID<(peatuste_arv-1)) {
							firstItem=firstItem+1;
							Lib_LcdGotoxy(0,4*8);
							Lib_Lcdprintf("              ");
							Lib_LcdGotoxy(0,6*8);
							Lib_Lcdprintf("              ");
							Lib_LcdGotoxy(0,4*8);
							Lib_Lcdprintf(" %s",strPeatus[firstItem]);
							Lib_LcdGotoxy(0,6*8);
							Lib_Lcdprintf(" %s",strPeatus[firstItem+1]);	
						}
						currentID=currentID+1;
						posOnScreen=posOnScreen+2;
						if (currentID>(peatuste_arv-1)) currentID=(peatuste_arv-1);
						if (posOnScreen>6) posOnScreen=6;
						HighlightRouteItemBigFont(posOnScreen);
						//Lib_LcdGotoxy(8*8,6*8);
						//Lib_Lcdprintf("%d-%d-%d",posOnScreen,currentID,firstItem);
					}
					if(ucKey==KEYOK){
						Lib_LcdCls();
						Lib_LcdSetFont(16, 16, 0);
						DispTimer2();
						DispHeader();
						Lib_LcdGotoxy(0,2*8);
						peatus_ni=currentID;
						Lib_Lcdprintf("Valitud %s %d",strPeatus[peatus_ni],peatus_ni);//,routeMain[currentID]);//,routeName[currentID]);
						Lib_DelayMs(1000);
						Lib_KbFlush();
						break;
					}
					if(ucKey==KEYCANCEL){
						Lib_KbFlush();
						break;
					}
	//---------------------------------------------------------------------------
				}
			} else {
				if (fnto>(peatuste_arv)) fnto=peatuste_arv;
				peatus_ni=fnto-1;
			}
			refresh_menu=1;
		}
		if(ucKey==KEYENTER){
			if (actualPrice>0) {
				//--------------------------------PRINT TICKET---------------------------
				Lib_LcdPrintxy(0,0*8,0x80,"JUHT:%4s LIIN:%4d.%1d",iJuh,liini_nr,liini_nr_s);
				Lib_PrnInit();
				Lib_PrnSetFont(16, 16, 0x33);
				if (allah_id<1) {
					Lib_PrnStr((BYTE*)"         PILET\n");
				} else {
					Lib_PrnStr((BYTE*)"      SOODUSPILET\n");
				}
				//fileZSize=Lib_FileGetEnv("SALESUM", sum_day);
				//Lib_PrnStr((BYTE*)"R1=%d\n",Lib_FileGetEnv("SALESUM", sum_day));
				//fileZSize=Lib_FileGetEnv("SALE1", sum_day);
				//Lib_PrnStr((BYTE*)"R2=%d\n",fileZSize);
				Lib_PrnSetFont(24,24,0x10);
				Lib_PrnStr((BYTE*)"          Nr. %06ld\n",pilet_nr_long);
				GetDTnosecToPrint(datetime);
				Lib_PrnStr((BYTE*)"       %s\n",datetime);
				Lib_PrnStr((BYTE*)"Liin:\n");
				Lib_PrnStr((BYTE*)"%d %s\n",liini_nr,liini_nimi);
				Lib_PrnStr((BYTE*)"Peatusest: %s\n",strPeatus[peatus_st]);
				Lib_PrnStr((BYTE*)"Peatuseni: %s\n",strPeatus[peatus_ni]);
				realWVAT=(actualPrice*100)/(100+realPVAT);
				realVVAT=actualPrice-realWVAT;
				Lib_PrnStr((BYTE*)"KM-ta:     %4d.%02d%s\n",realWVAT/100,realWVAT%100,CUR1);
				Lib_PrnStr((BYTE*)"KM %2d%%:    %4d.%02d%s\n",realPVAT,realVVAT/100,realVVAT%100,CUR1);
				Lib_PrnStr((BYTE*)"HIND:      %4d.%02d%s\n",actualPrice/100,actualPrice%100,CUR1);
				Lib_PrnSetFont(16,16,0x0);
				Lib_PrnStr((BYTE*)"Teid teenindas bussijuht koodiga %s, buss %s\n",iJuh,iBus);
				Lib_PrnSetFont(24,24,0x10);
				Lib_PrnStart();
				LnkPrintHeaderFooter();
				GetDTToZ(dtsave);
				//------------------SAVE Z ----------------------
				//                                                           KASSANR,PILETNT,KUUPKELL,JUHT,BUSS,LIIN,   LIINs,     PEATUSEST,          PEATUSENI,           VALUUTA,KM%,KMTA,KM,KMGA,ALLAH%
				memset(zData, '\0', sizeof(zData));
				fileZSize=Lib_FileSize("z");
				sprintf(zData,"%s;%ld;%s;%s;%s;%d;%d;%s;%s;%s;%d;%d;%d;%d;%d;\r",iPos,pilet_nr_long,dtsave,iJuh,iBus,liini_nr,liini_nr_s,strPeatus[peatus_st],strPeatus[peatus_ni],CUR1,realPVAT,realWVAT,realVVAT,actualPrice,allahindlus[allah_id]);
				fileZHandle=Lib_FileOpen("z", O_RDWR);
				fileZSeek=Lib_FileSeek(fileZHandle, 0, FILE_SEEK_END);
				zLen=strlen(zData);
				Lib_FileWrite(fileZHandle, zData, zLen);
				Lib_FileClose(fileZHandle);
				lng_sum_day=lng_sum_day+actualPrice;
				lng_sum_global=lng_sum_global+actualPrice;
				pilet_nr_long=pilet_nr_long+1;
				sprintf(cpilet_nr,"%ld",pilet_nr_long);
				sprintf(sum_day,"%ld",lng_sum_day);
				sprintf(sum_global,"%ld",lng_sum_global);
				Lib_FilePutEnv("PILETNR", cpilet_nr);					
				//uchar nm[8]; 
				//Lib_LcdGotoxy(0,2*8);
				//Lib_Lcdprintf("%d",Lib_FilePutEnv(sprintf("SALED%d",0), sum_day));
				//Lib_DelayMs(2000);
				//Lib_FilePutEnv(sprintf("SALEG%d",0), sum_global);
				//Lib_FilePutEnv(sprintf("SALEC%d",0), CUR1);
				//if (strcmp(CUR1,"EUR")==0) {
				if (strncmp(CUR1,"EUR",3)==0) {
					Lib_FilePutEnv("DSUMEUR", sum_day);
					Lib_FilePutEnv("GSUMEUR", sum_global);
				}
				/*if (strncmp(CUR1,"EUR",3)==0) {---------------------------------------THIS WAS A TRY FOR MULTICURRENCY!
					Lib_FilePutEnv("DSUMEUR", sum_day);
					Lib_FilePutEnv("GSUMEUR", sum_global);
				} else {
					Lib_FilePutEnv("DSUMRUB", sum_day);
					Lib_FilePutEnv("GSUMRUB", sum_global);
				}*/
				/*} else {
					Lib_FilePutEnv("SALESC2", sum_day);
					Lib_FilePutEnv("SALEGC2", sum_global);
				}*/
				//-----------------------------------------------
			} else {
				BeepError();
			}
		}
		if(ucKey==KEYOK){
			//------------------------------SET DISCOUNT---------------------------------
			allah_id=allah_id+1;
			if (allah_id>3) allah_id=0;
			Lib_LcdGotoxy(0,6*8);
			Lib_Lcdprintf("               ");
			Lib_LcdGotoxy(0,6*8);
			if (allah_id<1) {//FULLPRICE
				Lib_Beef(1,200);
				actualPrice=realPrice;
				//Lib_Lcdprintf("%d %d",realPrice,actualPrice);
				Lib_Lcdprintf("H %5d.%02d%s",actualPrice/100,actualPrice%100,CUR1);
			} else {//ROUNDED DISCOUNT
				Lib_Beef(6,500);
				//actualPrice=(realPrice/100*(100-allahindlus[allah_id])+(hinna_umard/2))/hinna_umard*hinna_umard;
				//actualPrice=((realPrice/100*(100-allahindlus[allah_id]))/hinna_umard*hinna_umard)+((realPrice%100*(100-allahindlus[allah_id])+hinna_umard/2)/hinna_umard*hinna_umard);
				actualPrice=(realPrice*(100-allahindlus[allah_id])/100+(hinna_umard/2))/hinna_umard*hinna_umard;
				//Lib_Lcdprintf("%d %d %d",realPrice/100,realPrice%100,realPrice);
				Lib_Lcdprintf("S%d %4d.%02d%s",allah_id,actualPrice/100,actualPrice%100,CUR1);
			}

			//Lib_KbFlush();
		}
		if(ucKey==KEYCANCEL){
			Lib_KbFlush();
		}
		if(ucKey==KEYMENU){
			Lib_KbFlush();
			//main();
			/*free(allahindlus);
			free(strPeatus);
			free(intPrice1);
			free(intVAT1);*/
			LnkShowMainMenu();
			return 0;
		}
		if (refresh_menu==1) {
			allah_id=0;
			refresh_menu=0;
			priceID=0;
			Lib_KbFlush();
			Lib_LcdCls();
			Lib_LcdSetFont(16, 16, 0);
			DispTimer2();
			DispHeader();
			Lib_LcdCls();
			Lib_LcdSetFont(16, 16, 0);
			Lib_LcdPrintxy(0,0*8,0x80,"JUHT:%4s LIIN:%4d.%1d",iJuh,liini_nr,liini_nr_s);
			Lib_LcdGotoxy(0,2*8);
			Lib_Lcdprintf("<%s",strPeatus[peatus_st]);
			Lib_LcdGotoxy(0,4*8);
			Lib_Lcdprintf(">%s",strPeatus[peatus_ni]);
			//Lib_LcdGotoxy(0,6*8);
			//Lib_Lcdprintf("%d-%d:",peatus_st,peatus_ni);
			allah_id=0;
			actualPrice=realPrice/100*(100-allahindlus[allah_id]);
			Lib_LcdGotoxy(0,6*8);
			Lib_Lcdprintf("               ");
			Lib_LcdGotoxy(0,6*8);
			Lib_Lcdprintf("H %5d.%02d%s",actualPrice/100,actualPrice%100,CUR1);
			if (peatus_st<peatus_ni) {
				i=0;
				for (i;i<(peatus_ni-1);i++) {
						priceID=priceID+(i+1);
				}
				priceID=priceID+peatus_st;
				//i = peatuste_arv-1;
				//for (i;i>(peatuste_arv-1-peatus_st);i--) {
				//	priceID=priceID+i;	
				//}
				//priceID=priceID+(peatus_ni-peatus_st-1);
				Lib_LcdGotoxy(0,6*8);
				Lib_Lcdprintf("              ");
				Lib_LcdGotoxy(0,6*8);
				realPVAT=intVAT1[priceID];
				realPrice=intPrice1[priceID];
				actualPrice=realPrice;
				Lib_Lcdprintf("H %5d.%02d%s",actualPrice/100,actualPrice%100,CUR1);
			}
			if (peatus_st>peatus_ni) {
				//i = peatuste_arv-1;
				//for (i;i>(peatuste_arv-1-peatus_ni);i--) {
				//	priceID=priceID+i;	
				//}
				//priceID=priceID+(peatus_st-peatus_ni-1);
				i=0;
				for (i;i<(peatus_st-1);i++) {
						priceID=priceID+(i+1);
				}
				priceID=priceID+peatus_ni;
				Lib_LcdGotoxy(0,6*8);
				Lib_Lcdprintf("              ");
				Lib_LcdGotoxy(0,6*8);
				realPVAT=intVAT1[priceID];
				realPrice=intPrice1[priceID];
				actualPrice=realPrice;
				Lib_Lcdprintf("H %5d.%02d%s",realPrice/100,realPrice%100,CUR1);
			}
			if (peatus_st==peatus_ni) {
				priceID=-1;
				Lib_LcdGotoxy(0,6*8);
				Lib_Lcdprintf("              ");
				Lib_LcdGotoxy(0,6*8);
				realPrice=0;
				actualPrice=realPrice;
				Lib_Lcdprintf("H %d.%02d%s",realPrice/100,realPrice%100,CUR1);
			}
		}
	}
	//-------------------------------------
}

int LnkGetStopByID()
{
	char peat[2];
	int peatus=0;
	int rslt=0;
	memset(peat,0,sizeof(peat));
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	Lib_LcdSetFont(8, 16, 0);
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Sisesta peatuse nr:");
	Lib_LcdGotoxy(0,5*8);
	rslt=Lib_KbGetStr((uchar*)peat,0,2,0x61|0x10,60);
	peatus=atoi(&peat);
	return peatus;
}

int LnkPosRegister()
{
	int respDrv=-1;
	int respBus=-1;
	int respRou=-1;
	char savebj[4];
	char savebs[4];
	uchar saveln;
	int save1,save2,save3;
	Lib_KbFlush();
	respDrv=TypeDrv();
	//if (respDrv>-1)  {
		Lib_KbFlush();
		respBus=TypeBus();
	//}
	//if (respBus>-1) {
		Lib_KbFlush();
		respRou=ChooseRoute2();
	//}

	int fileExist;
	int fileHandle;
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	if(respDrv>-1 && respBus>-1 && respRou>-1) {
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("Juht: %d",respDrv);
		Lib_LcdGotoxy(0,4*8);
		Lib_Lcdprintf("Buss: %d",respBus);
		Lib_LcdGotoxy(0,6*8);
		Lib_Lcdprintf("Liinif %d",respRou);
		BeepOK();
		Lib_DelayMs(1000);
		//---------------------SAVE JUHT------------
		memset(savebj,0,sizeof(savebj));
		sprintf(savebj,"%d",respDrv);
		save1=Lib_FilePutEnv("JUHTNR", savebj);//savedt);
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("Saved: %s %d",savebj,save1);
		
		//---------------------SAVE BUSS------------
		memset(savebs,0,sizeof(savebs));
		sprintf(savebs,"%d",respBus);
		save2=Lib_FilePutEnv("BUSSNR", savebs);
		Lib_LcdGotoxy(0,4*8);
		Lib_Lcdprintf("Saved: %s %d",savebs,save2);
		//--------------------SAVE LIIN--------------
		memset(savebs,0,sizeof(savebs));
		sprintf(saveln,"%d",respRou);
		save3=Lib_FilePutEnv("LIINNR", saveln);
		Lib_LcdGotoxy(0,6*8);
		Lib_Lcdprintf("Saved: %s %d",saveln,save3);
		//-------------------DONE---------------------
		Lib_DelayMs(1000);
		//LnkPrintRegister();
		fileExist=Lib_FileExist("z");
		if (fileExist<0) {
			fileHandle=Lib_FileOpen("z", O_CREATE);
			Lib_FileClose(fileHandle);
		}
	} else {
		Lib_Lcdprintf("Ei onnestunud registreerida(viga:%d.%d.%d)",respDrv,respBus,respRou); //liin:%s (%s)",driver);
		BeepError();
		Lib_DelayMs(3000);
		return -1;
	}
	return 0;
	
}

int TypeDrv() {
	char tmpdrv[4], drvtext[4];
	int drvres, drvlen;
	int drvint=-1;
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Sisesta juhi kood:");
	memset(tmpdrv,0,sizeof(tmpdrv));
	drvres=Lib_KbGetStr((uchar*)tmpdrv,1,4,0x61|0x10,60);
	if (drvres<-1009) {
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("Registreerimine ei onnestunud");
		Lib_DelayMs(1000);
		return -1;
	} else {
		Lib_KbFlush();
		drvlen=strlen(tmpdrv);
		memcpy(drvtext,tmpdrv,drvlen);
		drvint=atoi(tmpdrv);
	}
	return drvint; 
}

int TypeBus() {
	char tmpbus[4], bustext[4];
	int busres=-1;
	int buslen;
	int save2;
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Sisesta bussi kood:");
	memset(tmpbus,0,sizeof(tmpbus));
	busres=Lib_KbGetStr((uchar*)tmpbus,1,4,0x61|0x10,60);
	Lib_KbFlush();
	buslen=strlen(tmpbus);
	memcpy(bustext,tmpbus,buslen);
	if (busres<-1009) {
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("Registreerimine ei onnestunud");
		Lib_DelayMs(1000);
		return -1;
	} else {
		busres=atoi(tmpbus);
	}
	return busres;
}

int ChooseRoute2() {
	//declarations for route list viewer
	int posOnScreen=4;
	int routeres=-1;
	int currentID=0;
	int firstItem=0;
	int save3=-1;
	char routeFile;
	unsigned char ucKey;
	int resposne=-1;
	uchar liininr;
	uchar liininm;
	//----------------------------------
	int routeCnt=0;
	int i=0;
	int fileHandle;
	char fileName[2];
	int fileSize;
	int fileReadResult;
	int routeID;
	uchar *liiniF;
	uchar *fileData;
	const char delimiter[2] = ";";
	char *token;
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	Lib_KbFlush();
	routeCnt=CountRoutes();
	memset(fileData,0,sizeof(fileData));
	if (routeCnt<1) return -1;
	int routeMain[routeCnt];
	int routeSub[routeCnt];
	char routeName[routeCnt][255];
	Lib_LcdGotoxy(0,2*8);
	for (i;i<routeCnt;i++) {
		sprintf(fileName, "%d", i);
		fileSize=Lib_FileSize(fileName);
		//memset(liiniF, '\0', fileSize+1);
		fileHandle=Lib_FileOpen(fileName,O_RDWR);
		if (fileHandle<0) {
			Lib_LcdGotoxy(0,2*8);
			Lib_Lcdprintf("Viga faili %s avamisel (%d)%d-%d",fileName,fileHandle,i,routeCnt);
			Lib_DelayMs(2000);
			return -1;
		}
		Lib_Lcdprintf("Loen %d %d %d",i,fileSize,fileHandle);
		Lib_DelayMs(50);
		fileReadResult=Lib_FileRead(fileHandle, liiniF, fileSize);
		Lib_Lcdprintf("Loetud %d",fileReadResult);
		Lib_DelayMs(50);
		Lib_FileClose(fileHandle);	
		token = strtok(liiniF, delimiter); //Liini nr
		routeMain[i]=atoi(token);
		token = strtok(NULL, delimiter); // Liini s nr
		routeSub[i]=atoi(token);
		token = strtok(NULL, delimiter); //Liini nimetus
		strcpy(routeName[i],token);
	
	}
	//-------------------SHOW ROUTE LIST AND WAIT ENTRY-----------------------
	Lib_LcdCls();
	Lib_LcdSetFont(16, 16, 0);
	DispTimer2();
	DispHeader();
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Vali liin(%d):",routeCnt);
	Lib_LcdGotoxy(0,4*8);
	Lib_Lcdprintf(" %d.%d",routeMain[0],routeSub[0]);
	if (routeCnt>1) {
		Lib_LcdGotoxy(0,6*8);
		Lib_Lcdprintf(" %d.%d",routeMain[1],routeSub[1]);
	}
	HighlightRouteItemBigFont(posOnScreen);
	while(1) {
		if(Lib_KbCheck()) continue;
		ucKey=Lib_KbGetCh(); 
		if(ucKey==KEYUP){
			if (posOnScreen==4 && currentID>0) {
				firstItem=firstItem-1;
				if (firstItem<0) firstItem=0;
				Lib_LcdGotoxy(0,4*8);
				Lib_Lcdprintf(" %d.%d",routeMain[firstItem],routeSub[firstItem]);
				Lib_LcdGotoxy(0,6*8);
				Lib_Lcdprintf(" %d.%d",routeMain[firstItem+1],routeSub[firstItem+1]);	
			}
			posOnScreen=posOnScreen-2;
			currentID=currentID-1;
			if (currentID<0) currentID=0;
			if (posOnScreen<4) posOnScreen=4;
			HighlightRouteItemBigFont(posOnScreen);
		}
		if(ucKey==KEYDOWN){
			if (posOnScreen==6 && currentID<(routeCnt-1)) {
				firstItem=firstItem+1;
				Lib_LcdGotoxy(0,4*8);
				Lib_Lcdprintf(" %d.%d",routeMain[firstItem],routeSub[firstItem]);
				Lib_LcdGotoxy(0,6*8);
				Lib_Lcdprintf(" %d.%d",routeMain[firstItem+1],routeSub[firstItem+1]);	
			}
			currentID=currentID+1;
			posOnScreen=posOnScreen+2;
			if (currentID>(routeCnt-1)) currentID=(routeCnt-1);
			if (posOnScreen>6) posOnScreen=6;
			HighlightRouteItemBigFont(posOnScreen);
		}
		if(ucKey==KEYENTER){
			//sprintf(routeFile, "%d", currentID);
			//save3=Lib_FilePutEnv("LIINNR", routeFile);
			Lib_LcdCls();
			Lib_LcdSetFont(16, 16, 0);
			DispTimer2();
			DispHeader();
			routeres=currentID;
			//Lib_LcdGotoxy(0,2*8);
			//Lib_Lcdprintf("Valitud liinifail %d",routeres);//,routeMain[currentID]);//,routeName[currentID]);
			//Lib_DelayMs(4000);
			break;
		}
		if(ucKey==KEYCANCEL){
			Lib_KbFlush();
			return -1;
		}
	}
	return routeres;
}




int ChooseRoute() {
	//declarations for route list viewer
	int posOnScreen=4;
	int currentID=0;
	int firstItem=0;
	int save3=-1;
	char routeFile;
	unsigned char ucKey;
	int resposne=-1;
	uchar liininr;
	uchar liininm;
	//----------------------------------
	int routeCnt=0;
	int i=0;
	int fileHandle;
	char fileName[2];
	long fileSize;
	int fileReadResult;
	int routeID;
	uchar *fileData;
	const char delimiter[2] = ";";
	char *token;
	//memset(routeCnt,0,sizeof(routeCnt));
	//memset(liininr,0,sizeof(liininm));
	memset(fileName,0,sizeof(fileName));
	Lib_LcdCls();
	DispTimer2();
	DispHeader();
	Lib_KbFlush();
	routeCnt=CountRoutes();
	if (routeCnt<1) return -1;
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Vali liin(%d):",routeCnt);
	Lib_KbFlush();//---------------------------------------------------------------------------------KB
	Lib_DelayMs(1000);
	int routeMain[routeCnt];
	int routeSub[routeCnt];
	char routeName[routeCnt][255];
	Lib_KbFlush();//---------------------------------------------------------------------------------KB
	for (i;i<routeCnt;i++) {
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("SIIN %d",i);
		Lib_DelayMs(2000);

		sprintf(fileName, "%d", i);
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("SIIN FILE:%s",fileName);
		Lib_DelayMs(2000);
		//Lib_KbFlush();//---------------------------------------------------------------------------------KB
		Lib_LcdGotoxy(0,2*8);
		Lib_Lcdprintf("AVAN %s as (%d)%d-%d",fileName,fileHandle,i,routeCnt);
		Lib_DelayMs(200);
		fileSize=Lib_FileSize(fileName);	
		fileHandle=Lib_FileOpen(fileName,O_RDWR);
		if (fileHandle<0) {
			Lib_LcdGotoxy(0,2*8);
			Lib_Lcdprintf("Viga faili %s avamisel (%d)%d-%d",fileName,fileHandle,i,routeCnt);
			Lib_DelayMs(2000);
			return -1;
		}
		Lib_DelayMs(50);
		
		Lib_LcdGotoxy(0,4*8);
		Lib_Lcdprintf("Loen %s(%d), suurus %d",fileName,fileHandle,fileSize);
		fileReadResult=Lib_FileRead(fileHandle, fileData, fileSize);
		Lib_FileClose(fileHandle);
	/*Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Loetud      ");
	Lib_DelayMs(2000);*/

		token = strtok(fileData, delimiter);
		routeMain[i]=atoi(token);
	/*Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("token1");
	Lib_DelayMs(2000);*/

		token = strtok(NULL, delimiter);
		routeSub[i]=atoi(token);
	/*Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("token2");
	Lib_DelayMs(2000);*/

		token = strtok(NULL, delimiter);
		strcpy(routeName[i],token);
		Lib_LcdCls();
		DispTimer2();
		DispHeader();
		Lib_LcdGotoxy(0,4*8);
		Lib_Lcdprintf("Liin: %d.%d %s",routeMain[i],routeSub[i],routeName[i]);
		Lib_FileClose(fileHandle);
		Lib_DelayMs(50);
	}
	/*Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("LOOP END %d",routeCnt);
	Lib_DelayMs(2000);*/
	//-------------------SHOW ROUTE LIST AND WAIT ENTRY-----------------------
	Lib_LcdCls();
	Lib_LcdSetFont(16, 16, 0);
	DispTimer2();
	/*Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("timer");
	Lib_DelayMs(2000);*/

	DispHeader();
	/*Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("header");
	Lib_DelayMs(2000);*/
	//Lib_KbFlush();
		/*Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("flush");
	Lib_DelayMs(2000);*/
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Vali liin(%d):",routeCnt);
	Lib_LcdGotoxy(0,4*8);
	Lib_Lcdprintf(" %d.%d",routeMain[0],routeSub[0]);
	if (routeCnt>1) {
		Lib_LcdGotoxy(0,6*8);
		Lib_Lcdprintf(" %d.%d",routeMain[1],routeSub[1]);
	}
	HighlightRouteItemBigFont(posOnScreen);
	while(1) {
		if(Lib_KbCheck()) continue;
		ucKey=Lib_KbGetCh(); 
		if(ucKey==KEYUP){
			if (posOnScreen==4 && currentID>0) {
				firstItem=firstItem-1;
				if (firstItem<0) firstItem=0;
				Lib_LcdGotoxy(0,4*8);
				Lib_Lcdprintf(" %d.%d",routeMain[firstItem],routeSub[firstItem]);
				Lib_LcdGotoxy(0,6*8);
				Lib_Lcdprintf(" %d.%d",routeMain[firstItem+1],routeSub[firstItem+1]);	
			}
			posOnScreen=posOnScreen-2;
			currentID=currentID-1;
			if (currentID<0) currentID=0;
			if (posOnScreen<4) posOnScreen=4;
			HighlightRouteItemBigFont(posOnScreen);
			//Lib_LcdGotoxy(8*8,6*8);
			//Lib_Lcdprintf("%d-%d-%d",posOnScreen,currentID,firstItem);
			//break;
		}
		if(ucKey==KEYDOWN){
			if (posOnScreen==6 && currentID<(routeCnt-1)) {
				firstItem=firstItem+1;
				Lib_LcdGotoxy(0,4*8);
				Lib_Lcdprintf(" %d.%d",routeMain[firstItem],routeSub[firstItem]);
				Lib_LcdGotoxy(0,6*8);
				Lib_Lcdprintf(" %d.%d",routeMain[firstItem+1],routeSub[firstItem+1]);	
			}
			currentID=currentID+1;
			posOnScreen=posOnScreen+2;
			if (currentID>(routeCnt-1)) currentID=(routeCnt-1);
			if (posOnScreen>6) posOnScreen=6;
			HighlightRouteItemBigFont(posOnScreen);
			//Lib_LcdGotoxy(8*8,6*8);
			//Lib_Lcdprintf("%d-%d-%d",posOnScreen,currentID,firstItem);
			//break;
		}
		if(ucKey==KEYENTER){
			sprintf(routeFile, "%d", currentID);
			//sprintf(liininr,"%s",routeMain[currentID]);
			//sprintf(liininm,"%s",routeName[currentID]);
			save3=Lib_FilePutEnv("LIINNR", routeFile);
			//Lib_FilePutEnv("routeid", liininr);
			//Lib_FilePutEnv("routenm", liininm);
			Lib_LcdCls();
			Lib_LcdSetFont(16, 16, 0);
			DispTimer2();
			DispHeader();
			Lib_LcdGotoxy(0,2*8);
			Lib_Lcdprintf("Valitud liinifail %s",routeFile);//,routeMain[currentID]);//,routeName[currentID]);
			Lib_DelayMs(2000);
			break;
		}
		if(ucKey==KEYCANCEL){
			Lib_KbFlush();
			return -1;
		}
	}


	//---------------------------------------------------------------------------
	Lib_LcdGotoxy(0,2*8);
	//Lib_Lcdprintf("LIIN EDUKALT REGISTREERITUD");
	BeepOK();
	Lib_DelayMs(1000);
	return 0;
}

void HighlightRouteItemBigFont(int posOn) {
	Lib_LcdGotoxy(0,4*8);
	Lib_Lcdprintf(" ");
	Lib_LcdGotoxy(15*8,4*8);
	Lib_Lcdprintf(" ");
	Lib_LcdGotoxy(0,6*8);
	Lib_Lcdprintf(" ");
	Lib_LcdGotoxy(15*8,6*8);
	Lib_Lcdprintf(" ");

	Lib_LcdGotoxy(0,posOn*8);
	Lib_Lcdprintf(">");
	Lib_LcdGotoxy(15*8,posOn*8);
	Lib_Lcdprintf("<");
}

int GetDTToPrint(char *DTTM) {
	int year;
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
	return 0;
}

int GetDTnosecToPrint(char *DTTM) {
	int year;
	unsigned char font,buff[8];
	Lib_GetDateTime(buff);
	year=1900+(buff[0] >>4)*10+(buff[0] & 0x0f);
	if(buff[0]<=0x49) year+=100;
	buff[1]=(buff[1] >>4)*10+(buff[1] & 0x0f);
	buff[2]=(buff[2] >>4)*10+(buff[2] & 0x0f);
	buff[3]=(buff[3] >>4)*10+(buff[3] & 0x0f);
	buff[4]=(buff[4] >>4)*10+(buff[4] & 0x0f);
	buff[5]=(buff[5] >>4)*10+(buff[5] & 0x0f);
	sprintf(DTTM,"%02d.%02d.%04d %02d:%02d",buff[2],buff[1],year,buff[3],buff[4]);
	return 0;
}

int GetDTToZ(char *DTTM) {
	int year;
	unsigned char font,buff[8];
	Lib_GetDateTime(buff);
	year=1900+(buff[0] >>4)*10+(buff[0] & 0x0f);
	if(buff[0]<=0x49) year+=100;
	buff[1]=(buff[1] >>4)*10+(buff[1] & 0x0f);
	buff[2]=(buff[2] >>4)*10+(buff[2] & 0x0f);
	buff[3]=(buff[3] >>4)*10+(buff[3] & 0x0f);
	buff[4]=(buff[4] >>4)*10+(buff[4] & 0x0f);
	buff[5]=(buff[5] >>4)*10+(buff[5] & 0x0f);
	sprintf(DTTM,"%04d%02d%02d%02d%02d%02d",year,buff[1],buff[2],buff[3],buff[4],buff[5]);
	return 0;
}