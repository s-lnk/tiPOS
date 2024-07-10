#include "public.h"
#include "batlogo.h"
#include "wlsapi.h"
#include "batAndSingLogo.h"

#define WLS_TM  3
void ShowVer(void);
int TimerEventTest(void);
int ReadTime(void);
int FileTest(void);
void TimerSet_TimerCheck_test_5(void);

extern int PackageMachine_WlsInit();

unsigned char kbflush_getkey(void);

extern uchar g_NeedWlsReset;

int FileTest(void)
{

	int i, fd;
    unsigned char buff[300];
    
	Lib_LcdCls();
	Lib_Lcdprintf("Create File...");
	Lib_FileRemove("test.dat");
    fd = Lib_FileOpen("test.dat", O_CREATE);
    if (fd < 0)
	{
		Lib_Lcdprintf("\nError");
		Lib_KbGetCh();
		return 0;
	}
	Lib_Lcdprintf("\nOk");

    Lib_KbGetCh();

    memset(buff,0,sizeof(buff));
	for(i=0;i<100;i++)
	{
		Lib_LcdCls();
		Lib_LcdPrintxy(0,0,0,"open i=%d  ",i);

		fd = Lib_FileOpen("test.dat", O_RDWR);
		if (fd < 0)
		{
			Lib_LcdPrintxy(0,4*8,0,"Err   i=%d  ",i);
			Lib_KbGetCh();
			return -1;
		}
		Lib_LcdPrintxy(0,1*8,0,"seek i=%d  ",i);
		Lib_FileSeek(fd, (long)i *sizeof(buff), FILE_SEEK_SET);
		Lib_LcdPrintxy(0,2*8,0,"write i=%d %d",i,sizeof(buff));
		Lib_FileWrite(fd, buff, sizeof(buff));
		Lib_LcdPrintxy(0,3*8,0,"close i=%d  ",i);
		Lib_FileClose(fd);
		Lib_LcdPrintxy(0,4*8,0,"end   i=%d  ",i);
	}
	Lib_LcdPrintxy(0,6*8,0,"OK   i=%d  ",i);

    Lib_KbGetCh();

    return 0;


}

int MEM_Test(void)
{
     
     return 0;
}


int LCD_Test(void)
{
	uchar ret,mode,i,a,x,y;
	char chkey; 

	Lib_LcdCls();
	Lib_LcdPrintxy(0,0*8,0x81,"    LCD  Test   ");
	Lib_LcdGotoxy(0, 2*8);
	Lib_LcdSetFont(8, 16, 0);
	Lib_Lcdprintf("abcdefghijklmnopqrstu");
	Lib_Lcdprintf("vwxyz!@#$%&*()_-+=[]{");
	Lib_Lcdprintf("},;.<>|\\/?1234567890A");
	Lib_Lcdprintf("BCDEFGHIJKLMNOPQRSTUV");
	Lib_Lcdprintf("WXWXYZ");
	a = WaitEsc(1);
	if (a) return 1; 
	
	Lib_LcdClrLine(2*8,8*8);
	Lib_LcdGotoxy(0, 2*8);
	Lib_LcdSetFont(16, 16, 0);
	Lib_Lcdprintf("abcdefghijklmnop");
	Lib_Lcdprintf("qrstuvwxyz!@#$%&");
	Lib_Lcdprintf("*()_-+=[]{},;.<>");
	a = WaitEsc(1);
	if (a) return 1;
	
	Lib_LcdClrLine(2*8,8*8);
	Lib_LcdGotoxy(0, 2*8);
	Lib_Lcdprintf("|\\/?1234567890AB");
	Lib_Lcdprintf("CDEFGHIJKLMNOPQR");
	Lib_Lcdprintf("STUVWXYZ");
	a = WaitEsc(1);
	if (a) return 1; 
	
 /*
	Lib_LcdClrLine(2*8,7*9);
	Lib_LcdPrintxy(0,7*8,0,"    TECHNOLOGY  ");
	Lib_LcdSetGray(58);
	Lib_LcdPrintxy(25,4*8,1,"High Gray");
	a = WaitEsc(1);
	if (a) return 1;


	Lib_LcdSetGray(38);
	Lib_LcdPrintxy(25,4*8,1,"Low Gray");
	a = WaitEsc(1);
	if (a) return 1;
	Lib_LcdSetGray(48);

  */

	Lib_LcdClrLine(2*8,7*9);
	Lib_LcdSetBackLight(0);
	Lib_LcdPrintxy(0,4*8,1," Close BackLight ");
	a = WaitEsc(1);
	if (a) return 1;  

	Lib_LcdSetBackLight(1);
	Lib_LcdPrintxy(0,4*8,1," Open BackLight  ");
	a = WaitEsc(1);
	if (a) return 1;

	Lib_LcdClrLine(2*8,7*9);
	Lib_LcdPrintxy(0,4*8,1,"    FULL LCD    ");
	for(x=0;x<128;x++)
		for(y=0;y<64;y++)
			Lib_LcdDrawPlot(x,y,1);
	a = WaitEsc(1);
	if (a) return 1;


    Lib_LcdCls();
	Lib_LcdPrintxy(0,4*8,1,"  LCD TEST OK  ");
	ok_beep();
	WaitEsc(1); 
	
	return 0;
}

int CLK_Test(void)
{
	int i;
	unsigned char ucRet,sec, ucBuf[20],old_time[7], oldsec, newsec;
	int iret;
	Lib_LcdCls();
	Lib_LcdPrintxy(0,0,0x81,"  Clock  Test   ");
	Lib_LcdGotoxy(0,2*8);
	Lib_LcdSetFont(16, 16, 0);
	Lib_Lcdprintf("Clock test...");

	sec = 0;
	iret = Lib_GetDateTime(old_time);
	if (iret)
	{
		Lib_LcdPrintxy(0,4*8,1," Read Time Err1");
		ShowErr("Clock test Error");
		return 1;
	}

	Lib_DelayMs(1000); //  1秒后 是否有变化
	Lib_DelayMs(50);
	memset(ucBuf,0,sizeof(ucBuf));
	iret = Lib_GetDateTime(ucBuf);
	if (iret)
	{
		Lib_LcdPrintxy(0,4*8,1," Read Time Err2");
		ShowErr("Clock test Error");
		return 2;
	}

	Lib_LcdClrLine(2*8, 8*8);
	oldsec = bcd_to_byte(old_time[5]);
	newsec = bcd_to_byte(ucBuf[5]);


	if (newsec >= oldsec)
	{
		sec = newsec - oldsec;
	}
	else if (newsec < oldsec)
	{
		sec = (60-oldsec) + newsec;
	}

	//Lib_LcdPrintxy(0,2*8,1," %d:%d ", oldsec, newsec);
	if (sec == 0)
	{
		ShowErr("Clock test Err1");
		return 3;
	}
	else if(sec > 3)
	{//大于3秒, OS 中如果电池检测中断中提示有一个1S的提示信息 
		ShowErr("Clock test Err2");
		return 1;		
	}
	else
	{
		Lib_LcdPrintxy(0,4*8,1,"Clock Test OK");
		Lib_Beep();
		Lib_DelayMs(500);
		return 0;
	}
}


/*
int CLK_Test(void)
{
	int i;
	unsigned char ucRet,ucBuf[20],old_time[7];
	int iret;
	Lib_LcdCls();
	Lib_LcdPrintxy(0,0,0x81,"  Clock  Test   ");
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Clock test...");
	Lib_GetDateTime(old_time);
	iret=Lib_SetDateTime((unsigned char *)"\x99\x12\x31\x23\x59\x58");
	if(iret){
		ShowErr("Clock test Error1");
		return 1;
	}
	memset(ucBuf,0,sizeof(ucBuf));
	Lib_GetDateTime(ucBuf);
	if(memcmp(ucBuf,"\x99\x12\x31\x23\x59\x58",5)){
		ShowErr("Clock test Error2");
		return 1;
	}
	Lib_SetDateTime((unsigned char *)"\x99\x12\x31\x23\x59\x59");
	for(i=0;i<100;i++){
		memset(ucBuf,0,sizeof(ucBuf));
		Lib_GetDateTime(ucBuf);
		if(!memcmp(ucBuf,"\x00\x01\x01\x00\x00\x00\x06",7)) break;
		Lib_DelayMs(100);
	}
	if(i==100){
		ShowErr("Clock test Error3");
		return 1;
	}
	if(!old_time[0])
	{
		Lib_SetDateTime((unsigned char *)"\x02\x09\x01\x00\x00\x00");
		Lib_LcdPrintxy(0,4*8,1,"Clock Proofread OK");
	}
	else
	{
		Lib_SetDateTime(old_time);
		Lib_LcdPrintxy(0,4*8,1,"Clock Test OK");
	}
	Lib_Beep();
	Lib_DelayMs(500);
	return 0;
}
*/

int SYS_Test(void)
{
	
	unsigned char ret,key,flag;
	unsigned char dispm=0;
	int i;
	unsigned char desout[8],deskey[8];

	while(1){
		dispm = 0;
		Lib_LcdCls();
		Lib_LcdPrintxy(0,0,0x81,"   OTHER TEST   ");
		if(dispm==0){
			Lib_LcdPrintxy(0,2*8,1," 1- Read Time");
			Lib_LcdPrintxy(0,4*8,1," 2- File Test");
			Lib_LcdPrintxy(0,6*8,1," 3- Beep Test");
		}

		key=Lib_KbGetCh();
		switch(key){
		case KEYCANCEL:
			return 0;
		case '1':
			ReadTime();
			break;
		case '2':
			FileTest();
			break;
		case '3': 
			BeepTest();
			break;
		default:
			if(dispm==0) dispm=1;
			else dispm=0;
			break;
          }
     }
}


//电池五个等级
#define BATTERY_LEVEL5     (8200) //满格
#define BATTERY_LEVEL4     (7500)
#define BATTERY_LEVEL3     (7000)
#define BATTERY_LEVEL2     (6500)
#define BATTERY_LEVEL1     (6000) //最低显示电池
 

void Disp_BatteryLogo(int battery_volt)
{
	int ibv;
	ibv=battery_volt;

	Lib_LcdGotoxy(4*8,4*8);
	if(ibv>=BATTERY_LEVEL5)
	{
		Lib_LcdDrawLogo(g_Display_bat7);
	}
	else if(ibv>=BATTERY_LEVEL4)
	{
		Lib_LcdDrawLogo(g_Display_bat5);
	}
	else if(ibv>=BATTERY_LEVEL3)
	{
		Lib_LcdDrawLogo(g_Display_bat4);
	}
	else if(ibv>=BATTERY_LEVEL2)
	{
		Lib_LcdDrawLogo(g_Display_bat2);
	}
	else 
	{
		Lib_LcdDrawLogo(g_Display_bat0);
	} 
}

int g_BackupV;
int g_BackupCnt = 0;
extern uchar g_BatChargeStep;

int DispBattery(uchar x, uchar y)
{	
	int iret=0;
	int v1,v2,v3,v4,v5,v6;
	int ibv;
	int allv, i;

	Lib_LcdGotoxy(x, y);	
	if(Lib_GetBatChargeStatus() == BAT_CHARGE_ING)
	{
		if (!Lib_CheckTimer(TIMER_FOR_APILIB))
		{
			if (g_BatChargeStep==0)
			{
				Lib_SetTimer(TIMER_FOR_APILIB, 4);
				g_BatChargeStep = 1;
				Lib_LcdDrawLogo(g_Display_b_cap0);
			}
			else if (g_BatChargeStep==1)
			{
				Lib_SetTimer(TIMER_FOR_APILIB, 4);
				g_BatChargeStep = 2;
				Lib_LcdDrawLogo(g_Display_b_cap1);

			}
			else if (g_BatChargeStep==2)
			{
				Lib_SetTimer(TIMER_FOR_APILIB, 4);
				g_BatChargeStep = 3;
				Lib_LcdDrawLogo(g_Display_b_cap3);

			}
			else{
				Lib_SetTimer(TIMER_FOR_APILIB, 4);
				g_BatChargeStep = 0;
				Lib_LcdDrawLogo(g_Display_b_cap5);

			}
		}
		else
		{
			if (g_BatChargeStep==0)
			{
				Lib_LcdDrawLogo(g_Display_b_cap0);
			}
			else if (g_BatChargeStep==1)
			{
				Lib_LcdDrawLogo(g_Display_b_cap1);

			}
			else if (g_BatChargeStep==2)
			{
				Lib_LcdDrawLogo(g_Display_b_cap3);

			}
			else{
				Lib_LcdDrawLogo(g_Display_b_cap5);
			}
		}

	}
	else
	{
		v1=Lib_GetBatteryVolt();
		if (v1 != g_BackupV)
		{
			g_BackupCnt++;
			if (g_BackupCnt++ > 100)
			{
				g_BackupCnt = 0;
				if (g_BackupV > v1)
					g_BackupV = v1;   
			}
		}
		else
			g_BackupV = v1;

		Lib_LcdGotoxy(x, y);	
		if(g_BackupV>=BATTERY_LEVEL5)
		{//满格显示
			Lib_LcdDrawLogo(g_Display_b_cap5);
		}
		else if(g_BackupV>=BATTERY_LEVEL4)
		{
			Lib_LcdDrawLogo(g_Display_b_cap4);
		}
		else if(g_BackupV>=BATTERY_LEVEL3)
		{
			Lib_LcdDrawLogo(g_Display_b_cap3);
		}
		else if(g_BackupV>=BATTERY_LEVEL4)
		{
			Lib_LcdDrawLogo(g_Display_b_cap2);
		}
		else if(g_BackupV>=BATTERY_LEVEL1)
		{
			Lib_LcdDrawLogo(g_Display_b_cap1);
		}
		else 
		{
			Lib_LcdDrawLogo(g_Display_b_cap0);
		} 
	}


	return 0; 
	
}

 
int Test_BatteryFun(void)
{ 
	uchar kb;
	int iret=0, cnt;
	int v1,v2,v3,v4,v5,v6,backupV5;

	Lib_LcdCls();
	Lib_LcdGotoxy(0,0);
	(void)Lib_LcdSetFont(16, 16, 0);
	Lib_LcdPrintxy(0,0,0x81,"  Battery Test  ");
	Lib_LcdClrLine(2*8,63);
	//Lib_LcdPrintxy(0,2*8,0x01,"Current Voltage:");


	backupV5 = Lib_GetBatteryVolt();
	cnt = 0;
	while(1)
	{
		v1=Lib_GetBatteryVolt();
		if (v1 != backupV5)
		{
			cnt++;
			if (cnt++ > 100)
			{
				cnt = 0;
				if (backupV5 > v1)
					v2 = v1;  //取小的，更接近实际
				else
					v2 = backupV5;
			}
			else
				v2 = backupV5;
		}
		else
			v2 = backupV5;

        v3=v2/1000;
        v4=(v2%1000) / 100;  
		v5=(v2%1000%100) / 10;  
		//v6=(v2%1000%100)  % 10; 
		
		Lib_LcdPrintxy(0,2*8,0x01,"     %d.%d%d V ", v3,v4,v5); 
		//Lib_LcdPrintxy(0,2*8,0x01,"     %d MV   ", v1);

		Disp_BatteryLogo(v2);

		Lib_LcdDrawBox(0,15,127,63);
		if(0==Lib_KbCheck())
		{
			if(Lib_KbGetCh()==KEYCANCEL) return 0; 
		}
		Lib_DelayMs(100);
	} 
	return 0; 
}


int AllTest_Battery(void)
{ 
	uchar kb;
	int iret=0;
	int v1,v2,v3,v4,v5,v6;

	Lib_LcdCls();
	Lib_LcdGotoxy(0,0);
	(void)Lib_LcdSetFont(16, 16, 0);
	Lib_LcdPrintxy(0,0,0x81,"  Battery Test  ");
	Lib_LcdClrLine(2*8,63);

	v2=Lib_GetBatteryVolt();

    v3=v2/1000;
    v4=(v2%1000) / 100;  
	v5=(v2%1000%100) / 10;  
	//v6=(v2%1000%100)  % 10;   
		
	 
	if (v2 >= BATTERY_LEVEL1)
	{
		Lib_LcdPrintxy(0,2*8,0x01,"     %d.%d%d V ", v3,v4,v5); 
		Disp_BatteryLogo(v2);
		WaitEsc(2);
		Lib_LcdDrawBox(0,15,127,63);
		return 0;
	}
	else if (v1 < BATTERY_LEVEL1)
	{
		Lib_LcdPrintxy(0,2*8,0x01,"     %d.%d%d V ", v3,v4,v5); 
		Lib_LcdPrintxy(0,4*8,0x01," Battery Failer ");
		Lib_LcdPrintxy(0,6*8,0x01,"   Too Low! ");
		Lib_Beef(3, 300);
		WaitEsc(10);
		Lib_LcdDrawBox(0,15,127,63);
		return -1;
	}
}


int ReadTime(void)
{
	int i;
	unsigned char ucRet,ucBuf[20]; 

	memset(ucBuf,0,sizeof(ucBuf));	
	Lib_LcdCls();
	Lib_LcdPrintxy(0,0,0x81,"  Current Time  ");
	while(1){
		Lib_GetDateTime(ucBuf);
		Lib_LcdPrintxy(0,2*8,1,"   20%02x-%02x-%02x",ucBuf[0],ucBuf[1],ucBuf[2]);
		Lib_LcdPrintxy(0,4*8,1,"    %02x:%02x:%02x",ucBuf[3],ucBuf[4],ucBuf[5]);
		switch(ucBuf[6]){
			case 1:
				Lib_LcdPrintxy(0,6*8,1," MONDAY   ");
				break;
			case 2:
				Lib_LcdPrintxy(0,6*8,1," TUESDAY  ");
				break;
			case 3:
				Lib_LcdPrintxy(0,6*8,1," WEDNESDAY");
				break;
			case 4:
				Lib_LcdPrintxy(0,6*8,1," THURSDAY ");
				break;
			case 5:
				Lib_LcdPrintxy(0,6*8,1," FRIDAY   ");
				break;
			case 6:
				Lib_LcdPrintxy(0,6*8,1," SATURDAY ");
				break;
			case 7:
				Lib_LcdPrintxy(0,6*8,1," SUNDAY   ");
				break;
			default:
				Lib_LcdPrintxy(0,6*8,1,"        ");
				break;
		}
		if(!Lib_KbCheck()) break;
	}
	Lib_KbGetCh();
	return 0;
}

int BeepTest(void)
{
	int i;
	Lib_LcdCls();
	Lib_Lcdprintf("BEEP TEST...");
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
	return 0;
}


void ShowVer(void)
{	
	 
}
 
int PackageMachine_Lcd(void)
{
	return LCD_Test();
}

int Battery_Test(void)
{

}

int DispWlsSignal(uchar x, uchar y)
{
	int iRet2, signal;
	static uchar simOK = 0;
	static uchar checkSimCnt = 0;		

	Lib_LcdGotoxy(x, y);	

	if (g_NeedWlsReset != NO_NEED_RESET)
	{
		Lib_LcdDrawLogo(g_Display_s_No); 
		return 0;
	}
	

	if (!Lib_CheckTimer(WLS_TM))
	{
		if (simOK == 0)
		{
			if (checkSimCnt < 5)
			{//检查10s
				checkSimCnt++;
				iRet2 = Wls_CheckSim();
				if (iRet2)
				{
					Lib_LcdDrawLogo(g_Display_s_NoSIM); 
					//Lib_LcdSetIcon(ICON_SIGNAL, 1);
					Lib_SetTimer(WLS_TM, 2*10);  //1s
					return -1;
				}
				else
				{
					simOK = 1;
				}
			}
			else
			{
				Lib_LcdDrawLogo(g_Display_s_NoSIM); 
				//Lib_LcdSetIcon(ICON_SIGNAL, 1);
				return -1;
			}

			return -1;
		}


		signal = -1;
		iRet2 = Wls_CheckSignal(&signal); 
		if (!iRet2)
		{
			if (signal == SIGNAL_VERY_STRONG)
			{ 
				Lib_LcdDrawLogo(g_Display_s_veryStrong);
			}
			else if (signal == SIGNAL_STRONG)
			{
				Lib_LcdDrawLogo(g_Display_s_Strong); 
			}
			else if (signal == SIGNAL_NORMAL)
			{ 
				Lib_LcdDrawLogo(g_Display_s_Normal); 
			}
			else if (signal == SIGNAL_WEAK)
			{ 
				Lib_LcdDrawLogo(g_Display_s_Seak);  
			}
			else if (signal == SIGNAL_VERY_WEAK)
			{ 
				Lib_LcdDrawLogo(g_Display_s_verySeak); 
			}
			else
			{ 
				Lib_LcdDrawLogo(g_Display_s_No); 
			} 
		}
		else
		{
			Lib_LcdDrawLogo(g_Display_s_No); 
		}

		Lib_SetTimer(WLS_TM, 5*10);  //5s
		
	}

	return 0;
}

 




