#include "public.h"
#include "wlsapi.h"
#include "josapi.h"
#include <stdio.h>
#include <string.h>

#define RET_OK		 0
#define RET_ERR		(-1)
#define ERR			(-1)

#define WLS_TM  3

uchar sendDataBuf[2048];
uchar recvDataBuf[2048];

int   cursimno=0;

extern uchar g_NeedWlsReset;
extern void DisplayPrintResult(BYTE col,BYTE row,BYTE mode, int iResult);
int Test_Wls(void); 
 

//*********************************************************************************
void DispWlsTestErr(int iRet, int mode)
{
	Lib_LcdClrLine(2*8, 8*8);
	switch(iRet)
	{      
	case WLS_OK:
		return;
		//Lib_LcdPrintxy(0 , 4*8, 0x01, "Success  ");	
		break;
	case WLS_ERR: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Operate Err     "); 
		break;  
	case WLS_NORSP: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "No Resp        "); 
	    break; 
	case WLS_RSPERR: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Return Err     "); 
	    break; 
	case WLS_NOSIM: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "No SIM Card    "); 
		break; 
	case WLS_NEEDPIN: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Need SIM PIN  "); 
		break; 
	case WLS_NEEDPUK: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Need SIM PUK   "); 
	    break; 
	case WLS_SIMBLOCKED: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "SIM Card Locked "); 
	    break; 
	case WLS_SIMERR: 
		Lib_LcdPrintxy(0 , 2*8, 0x01, "No SIM or PIN "); 
		break; 
	case WLS_PINERR: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "PIN Err " ); 
		break; 
	case WLS_PARAMERR: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Input Para Err " ); 
		break; 
	case WLS_USERORPWDERR: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "UserName&Pwd Err" ); 
	    break; 
	case WLS_SNLTOOWEAK: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Singnal Too Weak" ); 
	    break; 
	case WLS_LINKCLOSED: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Module Offline" ); 
		break; 
	case WLS_LINKOPENED: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Module  Online" ); 
		break; 
	case WLS_LINKOPENING: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Logoning Net" ); 
	    break;
	case WLS_TCPCLOSED: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "TCP  Closed" ); 
	    break; 
	case WLS_TCPOPENED: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "TCP  Opened" ); 
	    break; 
	case WLS_TCPOPENING: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "TCP  Openning" ); 
		break; 
	case WLS_ATTACHED: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Module Attached" ); 
		break;

	case WLS_DETTACHED: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Module Dettached" ); 
	    break; 
	case WLS_UNKNOWNTYPE: 
		Lib_LcdPrintxy(0 , 2*8, 0x01, "Type Unknown" ); 
	    break; 
	case WLS_DIALERR: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Attached Refused" ); 
	    break; 
	case WLS_TCPSENDERR: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "TCP Send Fail " ); 
		break; 
	case WLS_TCPCONNECTOVERTIME: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "TCP CON Timeout" ); 
		break; 
	case WLS_PORTERR: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "COM Port Err" ); 
		break;
	case WLS_PORTINUSE: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "COM Port inuse" ); 
	    break; 
	case WLS_DETECTERR: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Detect Module Err" ); 
	    break;

	case WLS_SOCKETOVERRUN: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "TCP SocKetNo Over" ); 
	    break; 

	case WLS_TIMEOUT:
		Lib_LcdPrintxy(0 , 4*8, 0x01, "TimeOut" ); 
		break;

	case WLS_OTHERR: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, " The Other Err" );
	    break;

	case WLS_NOTSUPPORT: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, " Dont Support" );
		break;

	case WLS_NoUNReadMessage: 
		Lib_LcdPrintxy(0 , 4*8, 0x01, "No MSG inside" );
		break;
		
	default:
		Lib_LcdPrintxy(0 , 4*8, 0x01, " Unkown Err" );
	    break;
	}
	Lib_Beef(3, 200);
	
	if (!mode)
		Lib_DelayMs(500);
	else
		Lib_KbGetCh();
}

int PackageMachine_WlsInit()
{
	int iRet;
	uchar ch;

	Lib_LcdClrLine(0*8,8*8); 
	vDispTitle("Wireless Init");
	Lib_LcdClrLine(2*8,8*8); 
	Lib_LcdPrintxy(0 , 3*8, 0x01, "  Init..." );
	iRet = Wls_Init();
	Lib_LcdClrLine(2*8,8*8);
	if (!iRet)
	{ 
		Lib_LcdPrintxy(0 , 3*8, 0x01, "   Init OK  " ); 
	}
	else
	{
		Lib_LcdPrintxy(0 , 3*8, 0x01, "   Init Error  " ); 
		Lib_LcdPrintxy(0 , 5*8, 0x01, " Ret Code:%d ", iRet); 
	}

	Lib_DelayMs(500);
	return iRet;
}

int Test_WlsInit(int mode)
{
	int iRet;
	uchar ch;
	int i, okCnt;
	
	Lib_LcdClrLine(0*8,8*8); 
	vDispTitle("Wireless Init");
	Lib_LcdClrLine(2*8,8*8); 
	Lib_LcdPrintxy(0 , 3*8, 0x01, " Init..." );
	
	for (i=0; i<10; i++)
	{
		if (!Lib_KbCheck())
		{
			if (Lib_KbGetCh() == KEYCANCEL)
			{
				return -1;
			}
		}
		iRet = Wls_Init();
		if (!iRet)
		{
			break;
		}
	}

	Lib_LcdClrLine(2*8,8*8);
	if (!iRet)
	{ 
		Lib_LcdPrintxy(0 , 3*8, 0x01, "   Init OK  " ); 
	}
	else
	{
		DispWlsTestErr(iRet, mode); 
	}
	
	while (1)
	{

		if (!mode)
		{
			Lib_DelayMs(500);
			ch = KEYENTER;
		}
		else
			ch = Lib_KbGetCh();

		if (ch==KEYCANCEL || ch==KEYENTER)
		{
			break;
		}
	}
	return iRet;
}

int conv_buf(uchar * pp)
{
	uchar *p;

	p = pp;
	while (*p != NULL)
	{
		if (*p=='O' && *(p+1)=='K')
		{
			*p = NULL;
		}
		p++;
	}

	return 0;
}

int Test_WlsCheckSimAndSignal(int mode, uchar flag)   
{//0: 始终动态显示信号值，1：如果大于normal就退出，用于自动进入拨号流程
	int signal;
	int iRet1, iRet2;
	uchar ch, *p;
	uchar buf[20];
	ushort len;
	int cnt;

	Lib_LcdClrLine(0*8,8*8); 
	vDispTitle("SIM & Signal");

	Lib_SetTimer(WLS_TM, 10*10);
	while (1)
	{
		Lib_LcdPrintxy(0 , 2*8, 0x01, " Check SIM Card..." ); 
		iRet1 = Wls_CheckSim();
		Lib_LcdClrLine(2*8,8*8);
		if (!iRet1)
		{ 
			Lib_LcdPrintxy(0, 2*8, 0x01, "  SIM Card OK  "); 
			break;
		}
		else if (iRet1 == WLS_RSPERR)
		{
			continue;
		}
		else
		{
			DispWlsTestErr(iRet1, mode); 
			goto LOOPSIM;
		}

		if (!Lib_CheckTimer(WLS_TM))
		{
			DispWlsTestErr(iRet1, mode); 
			goto LOOPSIM;
		}

		if (!Lib_KbCheck())
		{
			if (Lib_KbGetCh()==KEYCANCEL)
				goto LOOPSIM;
		}
	}

	signal = -1;

	cnt = 0;
	if (!flag)
		Lib_SetTimer(WLS_TM, 1*10);
	else 
		Lib_SetTimer(WLS_TM, 60*10);

	Lib_LcdPrintxy(0 , 4*8, 0x01, " Check Signal..." );
	while (1)
	{
		if (!Lib_KbCheck())
		{
			if (Lib_KbGetCh()==KEYCANCEL)
				break;
		}

		memset(buf, 0, sizeof(buf));
		iRet1 = Wls_ExecuteCmd("at+csq\r",7, buf, 18, &len, 1000);
		if (strlen(buf))
		{
			p = buf;
			p = strstr(p, "+CSQ:");
			p += 6;
			conv_buf(p);
		}
		
		iRet2 = Wls_CheckSignal(&signal); 

		if (flag != 0)
		{
			if (iRet2==0 && signal<=SIGNAL_NORMAL)
			{
				break;
			}

			if (!Lib_CheckTimer(WLS_TM))
			{
				break;
			}
		}

		//Lib_LcdPrintxy(0 , 6*8, 0x01, "    %02d ", cnt++);

		Lib_LcdClrLine(4*8,8*8);
		Lib_LcdPrintxy(0 , 4*8, 0x01, "Signal:%s", p);
		if (signal == SIGNAL_VERY_STRONG)
		{ 
			Lib_LcdPrintxy(0 , 6*8, 0x01, "Very Strong" ); 
		}
		else if (signal == SIGNAL_STRONG)
		{
			Lib_LcdPrintxy(0 , 6*8, 0x01, "  Strong" ); 
		}
		else if (signal == SIGNAL_NORMAL)
		{ 
			Lib_LcdPrintxy(0 , 6*8, 0x01, "  Normal" ); 
		}
		else if (signal == SIGNAL_WEAK)
		{ 
			Lib_LcdPrintxy(0 , 6*8, 0x01, "  Weak" ); 
		}
		else if (signal == SIGNAL_VERY_WEAK)
		{ 
			Lib_LcdPrintxy(0 , 6*8, 0x01, " Very Weak" ); 
		}
		else
		{ 
			Lib_LcdPrintxy(0 , 6*8, 0x01, " No Signal" ); 
		} 

		if (!flag)
		{//动态显示信号值
			while (1)
			{
				if (!Lib_CheckTimer(WLS_TM))
				{
					Lib_SetTimer(WLS_TM, 1*10);	
					break;
				}

				if (!Lib_KbCheck())
				{
					if (Lib_KbGetCh()==KEYCANCEL)
						goto LOOPSIM;
				}
			}

		}

	}




LOOPSIM:
 	while (1)
	{
		if (!mode)
		{
			Lib_DelayMs(500);
			ch = KEYENTER;
		}
		else
			ch = Lib_KbGetCh();

		if (ch==KEYCANCEL || ch==KEYENTER)
		{
			break;
		}
	}

	if (iRet1!=0 || iRet2!=0)
		return RET_ERR;
	else
		return RET_OK;
}

int Test_WlsDail(int mode)
{
	int iRet;
	uchar ch;
	uchar wlsType;
	uchar moduleHwVer[20], moduleSwVer[20], boardVer[20], driverVer[20];
	uchar dialNum[30], uerID[60], userPassWord[30];
	
	Lib_LcdClrLine(0*8,8*8); 
	vDispTitle("Dail & Logon");

	Lib_LcdClrLine(2*8,8*8); 
	Lib_LcdPrintxy(0, 3*8 , 0x01," Check Net..."); 

 	iRet = Wls_CheckDial(); 
 	if (!iRet)
 	{
 		Lib_LcdClrLine(2*8,8*8);
  
 		Lib_LcdPrintxy(0 , 3*8, 0x01, " Module Online" );
  
 	}
 	else
	{
		memset(dialNum, 0, sizeof(dialNum));
		memset(uerID, 0, sizeof(uerID));
		memset(userPassWord, 0, sizeof(userPassWord));
		Lib_FileGetEnv("DIALNUM", dialNum);
		Lib_FileGetEnv("USERID", uerID);
		Lib_FileGetEnv("USERPWD", userPassWord);

		//iRet = Wls_GetVerInfo(&wlsType, moduleHwVer, moduleSwVer, boardVer, driverVer);
		//if (iRet)
		//{
		//	DispWlsTestErr(iRet, 1);
		//	return;
		//}

		if (!strlen(dialNum))
		{
 
			memcpy(dialNum, (uchar *)"CMNET", strlen((uchar *)"CMNET"));
	  
		}
		
		if (!strlen(uerID) || !strlen(userPassWord))
		{
	 	 
			memcpy(uerID, (uchar *)"", strlen((uchar *)""));
			memcpy(userPassWord, (uchar *)"", strlen((uchar *)""));
		 
		}

		Lib_LcdClrLine(2*8,8*8);
		Lib_LcdPrintxy(0 , 3*8, 0x01, "Dailing... " ); 
		iRet = Wls_InputUidPwd(uerID, userPassWord);
		if (iRet)
		{
			DispWlsTestErr(iRet, mode);
		}

		iRet = Wls_Dial(dialNum);
		if (!iRet)
		{
			Lib_LcdClrLine(2*8,8*8);
			Lib_LcdPrintxy(0 , 3*8, 0x01, " Dail OK! " );
 
		}
		else
		{
			DispWlsTestErr(iRet, mode);
		}   
	}
	
	while (1)
	{
		if (!mode)
		{
			Lib_DelayMs(500);
			ch = KEYENTER;
		}
		else
			ch = Lib_KbGetCh();

		if (ch==KEYCANCEL || ch==KEYENTER)
		{
			break;
		}
	}

	return iRet;
}

int Test_WlsTcpConnect(int mode)
{
	uchar tmpBuf[30],ipBuf[30],portBuf[30]; 	
	int iRet;
	uchar ch;
	
	Lib_LcdClrLine(0*8,8*8); 
	vDispTitle("Connect Server");
	Lib_LcdClrLine(2*8,8*8);
	Lib_LcdPrintxy(0 , 3*8, 0x01, " Check TCP Connect..." ); 
	iRet = Wls_CheckTcpLink();
	
	Lib_LcdClrLine(2*8,8*8);
	if (!iRet)
	{ 
		Lib_LcdPrintxy(0 , 3*8, 0x01, " TCP Connected " ); 
	}
	else
	{
		memset(ipBuf, 0, sizeof(ipBuf));
		memset(portBuf, 0, sizeof(portBuf));
		Lib_FileGetEnv("IP", ipBuf);
		Lib_FileGetEnv("PORT", portBuf); 
		
		Lib_LcdClrLine(2*8,8*8);
		Lib_LcdPrintxy(0 , 3*8, 0x01, " Connect... " ); 
		iRet = Wls_TcpConnect((uchar *)ipBuf, (uchar *)portBuf);  
		//iRet = Wls_TcpConnect((uchar *)"119.122.165.187", (uchar *)"8500");      
		if (!iRet)
		{
			Lib_LcdClrLine(2*8,8*8); 
			Lib_LcdPrintxy(0 , 3*8, 0x01, " TCP Connect" ); 
			Lib_LcdPrintxy(0 , 5*8, 0x01, " Success" ); 
		}
		else if (WLS_PARAMERR == iRet)
		{
			Lib_LcdClrLine(2*8,8*8); 
			Lib_LcdPrintxy(0 , 2*8, 0x01, "  Connect Error!" ); 
			Lib_LcdPrintxy(0 , 4*8, 0x01, " Pls check server " ); 
			Lib_LcdPrintxy(0 , 6*8, 0x01, "   IP and Port" ); 
			Lib_Beef(3, 200);
			if (!mode)
				Lib_DelayMs(500);
			else
				Lib_KbGetCh();
		}
		else
		{
			DispWlsTestErr(iRet, mode);
		}
		
	}
	
	while (1)
	{
		if (!mode)
		{
			Lib_DelayMs(500);
			ch = KEYENTER;
		}
		else
			ch = Lib_KbGetCh();

		if (ch==KEYCANCEL || ch==KEYENTER)
		{
			break;
		}
	}

	return iRet;
}

int Test_WlsSendData(int mode, ushort sendLen)
{
	int i, iRet;
	ushort prxlen;
	uchar ch, add;
	uchar sendRecvOkFlag;
	ushort timeout;

	sendRecvOkFlag = 0;
	Lib_LcdClrLine(0*8,8*8); 
	vDispTitle("Send & Recv");

	Lib_LcdClrLine(2*8,8*8);
	memset(sendDataBuf, 0, sizeof(sendDataBuf));
	memset(recvDataBuf, 0, sizeof(recvDataBuf));

	for (i=0,add=0; i<sendLen; i++)
	{
		sendDataBuf[i] = i%10+'0';//add++;
	}
	
 
	Lib_LcdPrintxy(0 , 3*8, 0x01, "TCP Send Data..." ); 
	iRet= Wls_TcpSend(sendDataBuf, sendLen);
	Lib_LcdClrLine(2*8,8*8);
	if (!iRet)
	{ 
		Lib_LcdPrintxy(0 , 3*8, 0x01, "TCP Send OK" ); 
	}
	else
	{
		DispWlsTestErr(iRet, mode);
		return -1;
	}

	int tlen;
	tlen = 0; 
	Lib_LcdClrLine(2*8,8*8);
	timeout = 300;
	Lib_SetTimer(WLS_TM, 10);
	if (iRet == RET_OK)
	{
		while (timeout)
		{
			if (!Lib_KbCheck())
			{
				ch = Lib_KbGetCh();
				if (ch == KEYCANCEL)
				{
					sendRecvOkFlag = 0;
					break;
				}
			}

			if (!Lib_CheckTimer(WLS_TM))
			{
				timeout--;
				Lib_SetTimer(WLS_TM, 10);
			}

			prxlen = sendLen; 
			Lib_LcdPrintxy(0 ,3*8, 0x01, " Recving...%03d ", timeout); 
			Lib_LcdPrintxy(0, 7*8, 0x00," tlen=%d   ", tlen);
			iRet=Wls_TcpRecv(recvDataBuf+tlen, &prxlen, 500);
			if(iRet != 0 )
			{
				Lib_KbFlush(); 
				Lib_LcdPrintxy(0 , 3*8, 0x01, "TCP Recv Failure" ); 
				Lib_KbGetCh();
				break;
			}
			if (prxlen>0) 
			{
				tlen += prxlen;
				if(tlen >= sendLen) break;
			}
		}
		
		if (!iRet && tlen>=sendLen)
		{
			Lib_LcdClrLine(2*8,8*8); 
			Lib_LcdPrintxy(0 , 3*8, 0x01, "RecvData Complete" ); 
			//Lib_LcdPrintxy(0, 7*8, 0x00,"recvLen=%d   ", prxlen);
			if (!memcmp(recvDataBuf, sendDataBuf, sendLen))
			{
				Lib_LcdPrintxy(0 , 3*8, 0x01, "Recv Data Success" );
				Lib_LcdPrintxy(0, 7*8, 0x00," prxlen=%d ", tlen);
				sendRecvOkFlag = 1;
			}
			else
			{
				Lib_LcdPrintxy(0 , 3*8, 0x01, "Send&Recv Data Err" );
				Lib_LcdPrintxy(0, 7*8, 0x00," prxlen=%d ", tlen);				
				Lib_KbGetCh();
				sendRecvOkFlag = 0;
			}
		}
		else if (!iRet && prxlen!=0)
		{
			Lib_LcdClrLine(3*8,8*8);
			Lib_LcdPrintxy(0 , 3*8, 0x01, "Recv part Data" );
			Lib_LcdPrintxy(0, 7*8, 0x00," recvLen=%d  ", prxlen);
			sendRecvOkFlag = 0;
		}
		else
		{
			Lib_LcdClrLine(3*8,8*8);
			Lib_LcdPrintxy(0 , 3*8, 0x01, " Recv Failer  " ); 	
			Lib_LcdPrintxy(0, 7*8, 0x00," Wls_TcpRecv=%d  ", iRet);
			sendRecvOkFlag = 0;
		}
		
	} 
	while (1)
	{
		if (!mode)
		{
			Lib_DelayMs(500);
			ch = KEYENTER;
		}
		else
			ch = Lib_KbGetCh();

		if (ch==KEYCANCEL || ch==KEYENTER)
		{
			break;
		}
	}

	if (sendRecvOkFlag == 1)
		return 0;
	else
		return -1;
}





int Test_WlsRecvData(int mode)
{
	int i, iRet;
	ushort prxlen;
	uchar ch, add; 
	
	int tlen;
	tlen = 0;
	
	ushort sendLen=1024; 
	
	memset(sendDataBuf, 0, sizeof(sendDataBuf)); 
	memset(recvDataBuf, 0, sizeof(recvDataBuf)); 
	for (i=0,add=0; i<sendLen; i++)
	{
		sendDataBuf[i] = i%10+'0';//add++;
	}
	
	
	
	Lib_LcdClrLine(2*8,8*8);
	
	//if (iRet == RET_OK)
	{
		while (1)
		{
			prxlen = sendLen;
 
			Lib_LcdPrintxy(0 ,3*8, 0x01, "TCP Recving..." );
 
			Lib_LcdPrintxy(0, 7*8, 0x00,"tlen=%d   ", tlen);
			iRet=Wls_TcpRecv(recvDataBuf+tlen, &prxlen, 5000);
			if(iRet != 0 )
			{
				Lib_KbFlush();
 
				Lib_LcdPrintxy(0 , 3*8, 0x01, "TCP Recv Timeout" );
 
				Lib_KbGetCh();
				break;
			}
			if (prxlen>0) 
			{
				tlen += prxlen;
				if(tlen >= sendLen) break;
			}
			if (!Lib_KbCheck())
			{
				ch = Lib_KbGetCh();
				if (ch == KEYCANCEL) break;
			}
		}
		
		if (!iRet && tlen>=sendLen)
		{
			Lib_LcdClrLine(2*8,8*8); 
			Lib_LcdPrintxy(0 , 3*8, 0x01, "RecvData Complete" );
 
			//Lib_LcdPrintxy(0, 7*8, 0x00,"recvLen=%d   ", prxlen);
			if (!memcmp(recvDataBuf, sendDataBuf, sendLen))
			{ 
				Lib_LcdPrintxy(0 , 3*8, 0x01, "RecvData Success" ); 
				Lib_LcdPrintxy(0, 7*8, 0x00,"prxlen=%d  ", tlen);
			}
			else
			{ 
				Lib_LcdPrintxy(0 , 3*8, 0x01, "Send&RecvData Err" ); 
				Lib_LcdPrintxy(0, 7*8, 0x00,"prxlen=%d  ", tlen);
				Lib_LcdPrintxy(0, 6*8, 0x00,"%s ", recvDataBuf);
				
				Lib_KbGetCh();
				
			}
		}
		else if (!iRet && prxlen!=0)
		{
 
			Lib_LcdPrintxy(0 , 3*8, 0x01, "Recv part Data" );
 
			Lib_LcdPrintxy(0, 7*8, 0x00,"recvLen=%d   ", prxlen);
		}
		else
		{
 
			Lib_LcdPrintxy(0 , 3*8, 0x01, " Recv Failer " );
 
			Lib_LcdPrintxy(0, 7*8, 0x00,"Wls_TcpRecv=%d   ", iRet);
		}
		
	}
	
	while (1)
	{
		if (!mode)
		{
			Lib_DelayMs(500);
			ch = KEYENTER;
		}
		else
			ch = Lib_KbGetCh();

		if (ch==KEYCANCEL || ch==KEYENTER)
		{
			break;
		}
	}

	return iRet;
}

int Test_WlsTcpClose(int mode)
{
	int iRet;
	uchar ch;

	Lib_LcdClrLine(2*8, 8*8);	
 
	Lib_LcdPrintxy(0 , 3*8, 0x01, "Closing TCP " );
 
	
	
	//iRet = Wls_CheckTcpLink();
	//if (iRet == WLS_TCPCLOSED)
	//{
	//	Lib_LcdClrLine(2*8, 8*8);	
 
	//	Lib_LcdPrintxy(0 , 2*8, 0x01, "TCP Status: Closed " );
 	
	//	
	//}
	//else if (iRet == 0)
	{
		iRet = Wls_TcpClose();
		Lib_LcdClrLine(2*8, 8*8);
		if (!iRet)
		{
			Lib_LcdClrLine(2*8, 8*8);	
 
			Lib_LcdPrintxy(0 , 2*8, 0x01, "TCP Close OK!" );
 
			
		}
		else
		{
			DispWlsTestErr(iRet, mode);
		}
	}
	//else
	//{
	//	DispWlsTestErr(iRet, mode);
	//}
	
	while (1)
	{
		if (!mode)
		{
			Lib_DelayMs(500);
			ch = KEYENTER;
		}
		else
			ch = Lib_KbGetCh();

		if (ch==KEYCANCEL || ch==KEYENTER)
		{
			break;
		}
	}

	return iRet;
}

int Test_WlsNetClose(int mode)
{
	int iRet;
	uchar ch;

	Lib_LcdClrLine(2*8, 8*8);
 
	Lib_LcdPrintxy(0 , 3*8, 0x01, "Logoff NET..." );
 
	

	//iRet = Wls_CheckNetLink();
	//if (!iRet)
	{
		iRet = Wls_NetClose();
		
		if (!iRet)
		{
			Lib_LcdClrLine(2*8, 8*8);	
 
			Lib_LcdPrintxy(0 , 3*8, 0x01, "Logoff NET OK " );
 
			
		}
		else
		{
			DispWlsTestErr(iRet, mode);
		}
	}
	//else
	//{
	//	Lib_LcdClrLine(2*8, 8*8);	
 
	//	Lib_LcdPrintxy(0 , 3*8, 0x01, " Net Status: Logoff " );
 
	//	
	//}
	
	while (1)
	{
		if (!mode)
		{
			Lib_DelayMs(500);
			ch = KEYENTER;
		}
		else
			ch = Lib_KbGetCh();

		if (ch==KEYCANCEL || ch==KEYENTER)
		{
			break;
		}
	}

	return iRet;
}

int Test_WlsAuTo(void)
{
	int iRet;
	int flag;
	uchar ch;

	Wls_SelectSim(0);
	flag = 0;
	if (Test_WlsInit(0))
	{		
		flag = ERR;
 
		Lib_LcdPrintxy(0 , 6*8, 0x01, " <GO ON? YES/NO>" );
 
		
		ch = Lib_KbGetCh();
		if (ch == KEYCANCEL)	
			return RET_ERR;
	}

	if(Test_WlsCheckSimAndSignal(0, 1))
	{
		flag = ERR;
 
		Lib_LcdPrintxy(0 , 6*8, 0x01, " <GO ON? YES/NO>" );
 
		ch = Lib_KbGetCh();
		if (ch == KEYCANCEL)	
			return RET_ERR;
	}

	//断开上一次链接
	Wls_TcpClose();

	if(Test_WlsDail(0))
	{
		flag = ERR;
 
		Lib_LcdPrintxy(0 , 6*8, 0x01, " <GO ON? YES/NO>" );
 
		ch = Lib_KbGetCh();
		return RET_ERR;

	}
	return 0;

	if(Test_WlsTcpConnect(0))
	{
		flag = ERR;
 
		Lib_LcdPrintxy(0 , 6*8, 0x01, " <GO ON? YES/NO>" );
 
		ch = Lib_KbGetCh();
		if (ch == KEYCANCEL)	
			return RET_ERR;

	}

	if(Test_WlsSendData(0, 1000))
	{
		flag = ERR;
 
		Lib_LcdPrintxy(0 , 6*8, 0x01, " <GO ON? YES/NO>" );
 
		ch = Lib_KbGetCh();
		if (ch == KEYCANCEL)	
			return RET_ERR;
	}
	else 
		flag = 0;

 
	if(Test_WlsTcpClose(0))
	{
// 		flag = ERR;
 
//		Lib_LcdPrintxy(0 , 6*8, 0x01, " <GO ON? YES/NO>" );
 
// 		ch = Kb_GetKey();
// 		if (ch == KEYCANCEL)	
// 			return RET_ERR;
	}

	if(Test_WlsNetClose(0))
	{
// 		flag = ERR;
 
//		Lib_LcdPrintxy(0 , 6*8, 0x01, " <GO ON? YES/NO>" );
 
// 		ch = Kb_GetKey();
// 		if (ch == KEYCANCEL)	
// 			return RET_ERR;
	}
	
	return 	flag;
}

//------------------------------------------------
int Test_Wls(void)
{
	uchar ch, keyNo;
	int iRet;
	ushort sendLen;
	uchar bcdtime[8];
	uchar buff[1024];
	ushort rsplen;
	//uchar sendcmd[100];

	char ATcmdUSSD_MenuMOVISTAR[]= "AT+CUSD=1,\"*201#\",15\r"; 


	cursimno=0;
	
    while(1)
    {
        Lib_LcdCls();
        Lib_LcdPrintxy(0,0,0x81,"  Wireless Test  ");
        Lib_LcdPrintxy(0,2*8,0,"1-Init   2-Sim&Signal");
        Lib_LcdPrintxy(0,3*8,0,"3-Dial   4-Connect");
        Lib_LcdPrintxy(0,4*8,0,"5-SendRxd  6-CloseTcp");
        Lib_LcdPrintxy(0,5*8,0,"7-CloseNet 8-Reset  ");
        Lib_LcdPrintxy(0,6*8,0,"9-Select Sim No. ");
		Lib_LcdPrintxy(0,7*8,0,"0-Auto Test  00-Setup"); 

        //Lib_LcdPrintxy(0,6*8,0,"9-Version");
		//Lib_LcdPrintxy(0,7*8,0,"0-AUTO ");

		Lib_KbFlush();
        ch = Lib_KbGetCh();
        if(ch == KEYCANCEL)  return 0;

        switch(ch)
        {
        case KEY1:
			Wls_SelectSim(cursimno);
			Test_WlsInit(1);
            break;

		case KEY2:
			Test_WlsCheckSimAndSignal(1, 0);
            break;

		case KEY3:
			Test_WlsDail(1);
			break;

		case KEY4:
			Test_WlsTcpConnect(1);
			break;

		case KEY5:
LOOP:
			Lib_LcdClrLine(2*8,8*8);
			Lib_LcdPrintxy(0 , 2*8, 0x01, "Select Data Len:" );  
			Lib_LcdPrintxy(0,4*8, 0x00,"1- 100,  2- 400");
			Lib_LcdPrintxy(0,5*8, 0x00,"3- 512,  4- 800");
			Lib_LcdPrintxy(0,6*8, 0x00,"5- 1000, 6- 1024");
			keyNo = Lib_KbGetCh();
			if (keyNo==KEY1)
			{
				sendLen = 100;
			}
			else if (keyNo==KEY2)
			{
				sendLen = 400;
			}
			else if (keyNo==KEY3)
			{
				sendLen = 512;
			}
			else if (keyNo==KEY4)
			{
				sendLen = 800;
			}
			else if (keyNo==KEY5)
			{
				sendLen = 1000;
			}
			else if (keyNo==KEY6)
			{
				sendLen = 1024;
			}
			else if (keyNo==KEYCANCEL)
			{
				break;
			}
			else
				goto LOOP;

			Test_WlsSendData(1, sendLen);
			
			break;


		case KEY6:
			Test_WlsTcpClose(1);
			break;

		case KEY7:	
			Test_WlsNetClose(1);
			break;
			
		case KEY8:
			Lib_LcdClrLine(2*8, 8*8);
			Lib_LcdPrintxy(0 , 3*8, 0x01, " Rst Module..." ); 
			Wls_SelectSim(cursimno);
			Wls_Reset();
			Lib_LcdPrintxy(0 , 3*8, 0x01, " Rst Module ok" ); 
			while (1)
			{
				ch = Lib_KbGetCh();
				if (ch==KEYCANCEL || ch==KEYENTER)
				{
					break;
				}
			}
			break;

		case KEY9:
			//DispWlsVer();
			Lib_LcdClrLine(2*8,8*8); 
			Lib_LcdPrintxy(0 , 2*8, 0x01, " Select SIM No. " ); 
			Lib_LcdPrintxy(0,4*8, 0x00,"1-SIM1");
			Lib_LcdPrintxy(0,5*8, 0x00,"2-SIM2");  
			keyNo = Lib_KbGetCh();
			if (keyNo==KEY1)
			{
				cursimno=0;
			} 
			else if (keyNo==KEY2)
			{
				cursimno=1;
			}  
			break;

		case KEY0:

			/*
           	Lib_LcdClrLine(2*8, 8*8);
			Lib_LcdPrintxy(0 , 3*8, 0x01, "Get Date&Time " ); 
			memset(bcdtime,0,sizeof(bcdtime));
			memcpy(bcdtime,"\x10\x08\x23\x09\x00\x00",6);
			Wls_SetTime(bcdtime); 
			memset(bcdtime,0,sizeof(bcdtime));
		    Wls_GetTime(bcdtime);
			Lib_LcdPrintxy(0 , 3*8, 0x01, "%02x/%02x/%02x,%02x:%02x:%02x",bcdtime[0],bcdtime[1],bcdtime[2],bcdtime[3],bcdtime[4],bcdtime[5]); 
			while (1)
			{
				ch = Lib_KbGetCh();
				if (ch==KEYCANCEL || ch==KEYENTER)
				{
					break;
				}
			}*/


			
			iRet = Test_WlsAuTo();

			Lib_LcdClrLine(2*8, 8*8);
			if (!iRet)
			{ 
				Lib_LcdPrintxy(0 , 4*8, 0x01, " Wrieless Sucess" ); 
				Lib_Beep();
			}
			else
			{ 
				Lib_LcdPrintxy(0 , 4*8, 0x01, "Wrieless Failure" );  
				Lib_Beep();
				Lib_Beep();
			}

			while (1)
			{
				Lib_KbFlush();
				ch = Lib_KbGetCh();
				if (ch==KEYCANCEL || ch==KEYENTER)
				{
					break;
				}
			}
			
			break;

		case KEY00:
			SetGPRS();
			break;

		case KEYDOWN:
			DispWlsVer();
			//My_ReadSignal();
			break;

		case KEYUP:
			My_DailNum();
			break;

		case KEYOK: 
			Test_Wls_AT();
			break;

		case KEYMENU:
			Wls_Update_G610();
			break; 

		case KEYCLEAR:
			Lib_LcdClrLine(0*8,8*8); 
			vDispTitle("Other test");
			Lib_LcdPrintxy(0,2*8, 0x01,"1- LBS");
			keyNo = Lib_KbGetCh();
			if(keyNo==KEY1)
			{
				Test_WlsGetLocation(1);
			}	
			break;

        default : break;
        }
    }
    return 0;	
}

int DispWlsVer(void)
{
	uchar wlsType;
	uchar moduleHwVer[33], moduleSwVer[33], boardVer[33], driverVer[33];
	int iRet;
	uchar ch;

	Lib_LcdCls();
 
	Lib_LcdPrintxy(0 , 0*8, 0x01, "    Version    " );
	Lib_LcdPrintxy(0 , 3*8, 0x01, " Waiting... " );
 	

	//iRet = Wls_Init();
	//	
	//Lib_LcdClrLine(2*8,8*8);
	//if (iRet)
	//{
	//	DispWlsTestErr(iRet, 1);
	//	return;
	//}

	iRet = Wls_GetVerInfo(&wlsType, moduleHwVer, moduleSwVer, boardVer, driverVer);
	if (iRet)
	{
		DispWlsTestErr(iRet, 1);
		return;
	}
	Lib_LcdClrLine(2*8, 8*8);
	Lib_LcdSetFont(8, 16, 0);
 
	
	if (!strlen(boardVer))
	{
		Lib_Lcdprintf(" boardVer: NULL\n");
	}
	else
	{
		Lib_Lcdprintf(" boardVer: %s\n", boardVer);
	}

	if (!strlen(driverVer))
	{
		Lib_Lcdprintf(" driverVer: NULL\n");
	}
	else
	{
		Lib_Lcdprintf(" driverVer: %s\n", driverVer);
	}

	if (!strlen(moduleHwVer))
	{
		Lib_Lcdprintf(" moduleHwVer: NULL\n");
	}
	else
	{
		Lib_Lcdprintf(" moduleHwVer: %s\n", moduleHwVer);
	}

	if (!strlen(moduleSwVer))
	{
		Lib_Lcdprintf(" moduleSwVer: NULL\n");
	}
	else
	{
		Lib_Lcdprintf(" moduleSwVer: %s\n", moduleSwVer);
	}

	while (1)
	{
		ch = Lib_KbGetCh();
		if (ch==KEYCANCEL || ch==KEYENTER)
		{
			break;
		}
	}

}

#define DEBUG_COM	1  
#define AT_COM		5


uchar mybuf[32*1024];

int Wls_Update_G610(void)
{
	int iRet;
	uchar keyNo, ch;
	int len;

	Lib_LcdCls();
	Lib_LcdPrintxy(0,0,0x81,"   G610 Update   ");

	Lib_LcdSetAttr(0);
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Open PC COM...");
	Lib_LcdGotoxy(0,4*8);
	iRet = Lib_ComOpen(DEBUG_COM, "115200,8,n,1");
	if(iRet != 0x00)
	{
		Lib_Beep();
		Lib_LcdClrLine(2*8,7*9);
		Lib_LcdPrintxy(0,4*8,0x00,"Open PC COM failed:%d",iRet);
		Lib_KbGetCh();
		return;
	}
	
	iRet = Lib_ComOpen(AT_COM, "115200,8,n,1");	//CPU to Module
	if(iRet != 0x00)
	{
		Lib_Beep();
		Lib_LcdClrLine(2*8,7*9);
		Lib_LcdPrintxy(0,4*8,0x00,"Open AT_COM failed:%d",iRet);
		Lib_KbGetCh();
		return;
	}
	
	Lib_LcdClrLine(2*8,7*9);
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf("Any key start...");
	Lib_KbGetCh();

	Lib_LcdClrLine(2*8,7*9);
	Lib_Lcdprintf(" power on...");
	//power on
	Lib_WlsIoCtl(WLS_RESERVE1_IO, WLS_IO_HIGH);  //初始为高（POWER_ON脚）
	Lib_DelayMs(10);
	Lib_WlsIoCtl(WLS_RST_IO, WLS_IO_LOW);   //一直保持为低（RESET_IN脚）
	Lib_WlsIoCtl(WLS_PWREN_IO, WLS_IO_HIGH);  //初始为高（POWER_ON脚）
	//Lib_DelayMs(100);

	Lib_LcdClrLine(2*8,7*9);
	Lib_LcdPrintxy(0, 3*8, 0x01, " Waiting ...");
	
	while(1)
	{
		iRet = Lib_KbCheck();
		if(iRet == 0x00)
		{
			ch = Lib_KbGetCh();
			if(ch == KEYCANCEL)	break;
		}
		
		iRet = Lib_ComRecv(AT_COM, mybuf, 16*1024, &len, 1);
		if (len)
		{
			Lib_ComSend(DEBUG_COM, mybuf, len);
		}
		
		iRet = Lib_ComRecv(DEBUG_COM, mybuf, 16*1024, &len, 1);
		if (len)
		{
			Lib_ComSend(AT_COM, mybuf, len);
		}
	}	
}
 
int Test_Wls_AT(void)
{
	int iRet;
	uchar keyNo, ch;



	Lib_LcdCls();
	Lib_LcdPrintxy(0,0,0x81,"   AT CMD Test   ");

	Lib_LcdClrLine(2*8,7*9);
	Lib_LcdGotoxy(0,3*8);
	Lib_Lcdprintf(" Pls Waiting ...");

	Lib_LcdSetAttr(0);
	Lib_LcdGotoxy(0,2*8);
	Lib_Lcdprintf("Open DEBUG_COM...");
	Lib_LcdGotoxy(0,4*8);
	iRet = Lib_ComOpen(DEBUG_COM, "115200,8,n,1");
	if(iRet != 0x00)
	{
		Lib_Beep();
		Lib_LcdClrLine(2*8,7*9);
		Lib_LcdPrintxy(0,4*8,0x00,"Open DEBUG_COM failed:%d",iRet);
		Lib_KbGetCh();
		return;
	}

	iRet = Lib_ComOpen(AT_COM, "115200,8,n,1");	//CPU to Module
	if(iRet != 0x00)
	{
		Lib_Beep();
		Lib_LcdClrLine(2*8,7*9);
		Lib_LcdPrintxy(0,4*8,0x00,"Open AT_COM failed:%d",iRet);
		Lib_KbGetCh();
		return;
	}

	Lib_LcdClrLine(2*8,7*9);
	Lib_LcdPrintxy(0, 3*8, 0x01, " Waiting at ...");

	while(1)
	{
		iRet = Lib_KbCheck();
		if(iRet == 0x00)
		{
			ch = Lib_KbGetCh();
			if(ch == KEYCANCEL)	break;
		}

		iRet = Lib_ComRecvByte(AT_COM, &ch, 1);
		if(iRet == 0x00)
			Lib_ComSendByte(DEBUG_COM, ch);

		iRet = Lib_ComRecvByte(DEBUG_COM, &ch, 1);
		if(iRet == 0x00)
		{
			iRet=Lib_ComSendByte(AT_COM, ch);
			if(iRet!=0)
			{
				Lib_Beep();
			}
		}
	}	
}
 
int My_DailNum()
{
	uchar cmdString[50];
	uchar ch, rsp[50];
	int iRet;
	int rsplen, ilen;
	char tmps[300], tel_no[25]; 
	ushort uslen;

	memset(tel_no,0,sizeof(tel_no));
	Lib_LcdCls();
	Lib_LcdPrintxy(0, 0, 0x81, " Dail Telephone  ");

	//iRet = Lib_FileGetEnv("电话一",tel_no);
	iRet = -1;
	if(iRet!=0)
	{
		Lib_LcdClrLine(2*8,7*9);
		Lib_LcdPrintxy(0,2*8,1,"Input Telephone Number:");
		Lib_LcdGotoxy(0, 6*8);   
		memset(tmps,0,sizeof(tmps));
		if (Lib_KbGetStr((uchar*)tmps ,0, 20,0x61|0x10,120)) return 1;
		ilen=strlen(tmps);
		memcpy(tel_no,tmps,ilen); 
	}

	Lib_LcdClrLine(2*8, 8*8);
	//Lib_Lcdprintf("正在拨号,请等待...\n<取消>键挂断");
	Lib_Lcdprintf("Dail...\n<CANCEL> Exit");

	memset(rsp, 0, sizeof(rsp));
	sprintf(cmdString, "atd%s;\r", tel_no);
	iRet = Wls_ExecuteCmd(cmdString, strlen(cmdString), rsp, 20,&uslen, 1000);

	while (1)
	{
		if (!Lib_KbCheck())
		{
			ch = Lib_KbGetCh();
			if (ch==KEYCANCEL)
			{
				iRet = Wls_ExecuteCmd("ath\r", 4, rsp, 10, &uslen, 1000);
				Lib_Lcdprintf("Hang Up!\n");
				Lib_KbGetCh();
				return;
			}
		}

	}

}


//设置IP地址
int get_ip_addr(uchar x, uchar y,char *ip_addr)
{
	int pos = 0;
	unsigned char ch;
	unsigned char disp_buf[2];
	unsigned char tmp_buf[20];
	
	strcpy((char*)tmp_buf, "___.___.___.___");
	Lib_LcdGotoxy(x, y*8);
	Lib_Lcdprintf((char*)tmp_buf);
	
	while (1){
		Lib_LcdGotoxy((uchar)(x + pos * 8), (uchar) y*8);
		Lib_LcdSetAttr(1);
		Lib_Lcdprintf(" ");
		Lib_LcdSetAttr(0);
		
		ch = Lib_KbGetCh();
		if (ch == KEYCANCEL)
			return -1;
		
		if ((ch >= KEY0) && (ch <= KEY9) && (pos < 15)){
			Lib_LcdGotoxy((uchar)(x + pos * 8), y*8);
			disp_buf[0] = ch;
			disp_buf[1] = 0;
			Lib_Lcdprintf((char*)disp_buf);
			
			tmp_buf[pos++] = ch;
			
			if ((pos == 3) || (pos == 7) || (pos == 11))
				pos ++;
		}
		else if ((ch == KEYENTER) && (pos == 15)){
			strcpy((char*)ip_addr,(char*) tmp_buf);
			return 0;
		}
		else if ((ch == KEYCLEAR) && (pos > 0)){
			Lib_LcdGotoxy((uchar)(x + pos * 8), y*8);
			Lib_Lcdprintf("_");
			
			pos --;
			if ((pos == 3) || (pos == 7) || (pos == 11))
				pos --;
		}
	}
}


void SetGPRS(void)
{
	uchar buf[50], buf1[50];
	uchar ucRet, ucKey, bIfRenew;
	int iTmpInt;
	uchar dialNum[30], uerID[60], userPassWord[30];
	uchar ip[30], port[60];

	Lib_LcdCls();
	vDispTitle("GPRS Setting");

	Lib_FileGetEnv("DIALNUM", dialNum);
	Lib_FileGetEnv("USERID", uerID);
	Lib_FileGetEnv("USERPWD", userPassWord);
	Lib_FileGetEnv("IP", ip);
	Lib_FileGetEnv("PORT", port);
	
	Lib_KbFlush();
	Lib_LcdCls();
	vDispTitle("APN Setting");
	Api_DispString(dialNum, LINE3, LEFT_DISP, NORMAL_DISP);
	memset(buf, 0x00, sizeof(buf));
	Lib_LcdGotoxy(58,5*8);
	ucRet = GetStr_m1000(buf, ECHO_IN + ALPHA_IN, 0, 20); 
	if(ucRet == 0x00 && strcmp(dialNum, buf) != 0 && strlen(buf) != 0)
	{
		strcpy(dialNum, buf);
		Lib_FilePutEnv("DIALNUM", dialNum);
	}

	Lib_KbFlush();
	Lib_LcdCls();
	vDispTitle("APN user name");
	Api_DispString(uerID, LINE3, LEFT_DISP, NORMAL_DISP);
	memset(buf, 0x00, sizeof(buf));
	Lib_LcdGotoxy(58,5*8);
	ucRet = GetStr_m1000(buf, ECHO_IN + ALPHA_IN, 0, 20); 
	if(ucRet == 0x00 && strcmp(uerID, buf) != 0)
	{
		strcpy(uerID, buf);
		Lib_FilePutEnv("USERID", uerID);
	}

	Lib_LcdCls();
	vDispTitle("user password");
	Api_DispString("Pls enter: ", LINE3, LEFT_DISP, NORMAL_DISP);
	Lib_LcdGotoxy(58,5*8);
	ucRet = GetStr_m1000(buf, ECHO_IN + ALPHA_IN, 0, 20);
	if(ucRet == 0x00 && strcmp(userPassWord, buf) != 0)
	{
		strcpy(userPassWord, buf);
		Lib_FilePutEnv("USERPWD", userPassWord);
	}

	Lib_LcdCls();
	vDispTitle("Server IP");
	Api_DispString((ip), LINE3, MIDDLE_DISP, NORMAL_DISP);
	if(get_ip_addr(0, 6, buf) == 0) 
	{
		strcpy(ip, buf);
		Lib_FilePutEnv("IP", ip);
	}

	Lib_LcdCls();
	vDispTitle("Server Port");
	Api_DispString((port), LINE3, MIDDLE_DISP, NORMAL_DISP);
	strcpy(buf, port);
	Lib_LcdGotoxy(58,5*8);
	ucRet = GetStr_m1000(buf, NUM_IN + ECHO_IN, 0, 10);
	if(ucRet == 0) 
	{
		strcpy(port, buf);
		Lib_FilePutEnv("PORT", port);
	}


	return;
} 




int Test_WlsGetLocation(int mode)
{
	int iRet;
	uchar buf[2*1024], ch;

	Lib_LcdClrLine(0*8,8*8); 
	vDispTitle("LBS");

	Lib_LcdClrLine(2*8,8*8); 
	Lib_LcdPrintxy(0, 3*8 , 0x01," Pls Wait..."); 

	memset(buf, 0, sizeof(buf));
	iRet = Wls_GetLocation(1, buf, 30*1000); 
	if (!iRet)
	{
		Lib_PrnInit();
		Lib_PrnStr((BYTE*)"\n\n");
		Lib_PrnSetFont(16, 16, 0x33);
		Lib_PrnStr("Location Info:\n");
		Lib_PrnSetFont(16, 16, 0x00);
		Lib_PrnStr("%s \n", buf);
		Lib_PrnStr((BYTE*)"\n\n\n\n\n\n\n\n\n");
		Lib_PrnStart();
		//if (iRet < 0)
		//{
		//	Lib_LcdClrLine(0*8,8*8); 
		//	vDispTitle("Print Info");
		//	DisplayPrintResult(10, 2*8, 1, iRet);
		//}

		Lib_LcdClrLine(0*8,8*8); 
		vDispTitle("Location Info");
		Lib_LcdClrLine(2*8,8*8);
		Lib_LcdSetFont(8, 16, 0);
		buf[21*6-1] = 0;
		Lib_Lcdprintf("%s", buf);
	 }
	 else
		DispWlsTestErr(iRet, 1);


	while (1)
	{
		if (!mode)
		{
			Lib_DelayMs(500);
			ch = KEYENTER;
		}
		else
			ch = Lib_KbGetCh();

		if (ch==KEYCANCEL || ch==KEYENTER)
		{
			break;
		}
	}

	return iRet;
}
 

