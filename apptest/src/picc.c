//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>

#include"public.h"
#define PICC_WAIT_TIME   10  //µ•Œª√Î

int Picc_Test(void) 
{
	BYTE snbuf[100],tmps[300],ch;
	int iret,i;

	APDU_SEND ApduSend;
	APDU_RESP ApduResp; 

LOOP:
	while(1)
	{
		memset(snbuf,0,sizeof(snbuf));
		memset(tmps,0,sizeof(tmps));

		Lib_LcdCls();
		Lib_LcdPrintxy(0,0*8,0x81,"  13.56Mhz Card   ");
		Lib_LcdPrintxy(0,2*8,0x01,"  1- CPU type A ");
		Lib_LcdPrintxy(0,4*8,0x01,"  2- M1 Card  ");
		Lib_LcdPrintxy(0,6*8,0x01,"  3- CPU type B ");
		Lib_LcdDrawBox(0,0,127,63);
		Lib_PiccClose();
		Lib_PiccOpen();
		ch=Lib_KbGetCh();
		if(ch==KEYCANCEL)  return 1; 

		switch(ch)
		{
		case '3':
			Lib_LcdClrLine(2*8,8*8);
			Lib_SetTimer(2, PICC_WAIT_TIME*10);
			while(1)
			{
				if (!Lib_KbCheck())
				{
					if (Lib_KbGetCh() == KEYCANCEL)
					{
						goto LOOP;
					}
				}

				iret=Lib_PiccCheck('B',0x00, snbuf); 
				if(iret)  
				{
					Lib_LcdPrintxy(0,3*8,0x01," Waiting Card... "); 
				}
				else
					break;

				if (!Lib_CheckTimer(2))
				{
					Lib_LcdClrLine(2*8,8*8);
					Lib_LcdPrintxy(0,4*8,0x01,"    No Card     "); 
					Lib_Beef(3, 1000);	 
					ch=Lib_KbGetCh();
					if(ch==KEYENTER) continue;
					goto LOOP;
				}
			}

			Lib_Beef(3, 100);
			ApduSend.Command[0]=0x00;
			ApduSend.Command[1]=0xa4;
			ApduSend.Command[2]=0x00;
			ApduSend.Command[3]=0x00;
			ApduSend.Lc=2;
			ApduSend.Le=256;
			memcpy(ApduSend.DataIn,"\x3f\x00",ApduSend.Lc);


			iret=Lib_PiccCommand(&ApduSend, &ApduResp);	  
			if (iret==0)
			{
				Lib_LcdClrLine(2*8,8*8);
				Lib_LcdPrintxy(0,2*8,0,"-------Success-------");
				Lib_LcdGotoxy(0,3*8);
				Lib_LcdSetFont(8,16,0);
				for(i=0;i<ApduResp.LenOut;i++)
					Lib_Lcdprintf("%02x ",ApduResp.DataOut[i]);
				Lib_Lcdprintf("%02x ",ApduResp.SWA);
				Lib_Lcdprintf("%02x ",ApduResp.SWB);
			}
			else 
			{
				Lib_LcdClrLine(2*8,8*8);
				Lib_LcdPrintxy(0,2*8,0,"-------Failing-------");
				Lib_LcdPrintxy(0,3*8,0,"[ERROR] %d",iret);
			}
			ch=Lib_KbGetCh();
			Lib_PiccClose();
			break;
		case '1':
			Lib_LcdClrLine(2*8,8*8);
			Lib_SetTimer(2, PICC_WAIT_TIME*10);
			while(1)
			{

				if (!Lib_KbCheck())
				{
					if (Lib_KbGetCh() == KEYCANCEL)
					{
						goto LOOP;
					}
				}

				iret=Lib_PiccCheck('A',0x00, snbuf); 
				if(iret)  
				{
					Lib_LcdPrintxy(0,3*8,0x01," Waiting Card... "); 
				}
				else
					break;

				if (!Lib_CheckTimer(2))
				{
					Lib_LcdClrLine(2*8,8*8);
					Lib_LcdPrintxy(0,4*8,0x01,"    N0 Card     "); 
					Lib_Beef(3, 1000);	 
					ch=Lib_KbGetCh();
					if(ch==KEYENTER) continue;
					goto LOOP;
				}
			}

			iret=Lib_PiccReset();
			if(iret)  
			{
				Lib_LcdClrLine(2*8,8*8);
				Lib_LcdPrintxy(0,3*8,0x01," CPU typeA Card "); 
				Lib_LcdPrintxy(0,5*8,0x01,"  Reset Err "); 
				Lib_Beef(3, 1000);   
				ch=Lib_KbGetCh();
				if(ch==KEYENTER) continue;
				goto LOOP ;
			}

			Lib_Beef(3, 100);
			ApduSend.Command[0]=0x00;  
			ApduSend.Command[1]=0xa4;  
			ApduSend.Command[2]=0x04;  
			ApduSend.Command[3]=0x00; 
			ApduSend.Lc=0x0e;          
			ApduSend.Le=256;           
			memcpy(ApduSend.DataIn,"1PAY.SYS.DDF02",14);

			iret=Lib_PiccCommand(&ApduSend, &ApduResp);	  
			if (iret==0)
			{
				Lib_LcdClrLine(2*8,8*8);
				Lib_LcdPrintxy(0,2*8,0,"-------Success-------");
				Lib_LcdGotoxy(0,3*8);
				Lib_LcdSetFont(8,16,0);
				for(i=0;i<ApduResp.LenOut;i++)
					Lib_Lcdprintf("%02x ",ApduResp.DataOut[i]);
				Lib_Lcdprintf("%02x ",ApduResp.SWA);
				Lib_Lcdprintf("%02x ",ApduResp.SWB);
			}
			else 
			{
				Lib_LcdClrLine(2*8,8*8);
				Lib_LcdPrintxy(0,2*8,0,"-------Failing-------");
				Lib_LcdPrintxy(0,3*8,0,"[ERROR] %d",iret);
			}
			ch=Lib_KbGetCh();
			Lib_PiccClose();

			break;
		case '2':
			Lib_LcdClrLine(2*8,8*8);
			Lib_SetTimer(2, PICC_WAIT_TIME*10);
			while(1)
			{

				if (!Lib_KbCheck())
				{
					if (Lib_KbGetCh() == KEYCANCEL)
					{
						goto LOOP;
					}
				}

				iret=Lib_PiccCheck('M',0x00, snbuf); 
				if(iret)  
				{
					Lib_LcdPrintxy(0,3*8,0x01," Waiting Card... "); 
				}
				else
					break;

				if (!Lib_CheckTimer(2))
				{
					Lib_LcdClrLine(2*8,8*8);
					Lib_LcdPrintxy(0,4*8,0x01,"    No Card     "); 
					Lib_Beef(3, 1000);	 
					ch=Lib_KbGetCh();
					if(ch==KEYENTER) continue;
					goto LOOP;
				}
			}

			Lib_Beef(3, 100);
			iret=Lib_PiccM1Authority('A',9,(BYTE *)"\xff\xff\xff\xff\xff\xff", &snbuf[1]);
			//iret=Lib_PiccM1Authority('A',9,(BYTE *)"\x00\x00\x00\x00\x00\x00", &snbuf[1]);
			if(iret)
			{
				Lib_LcdClrLine(2*8,8*8);
				Lib_LcdPrintxy(0,3*8,0x01,"   M1 Card OK  "); 
				Lib_KbGetCh();
				goto LOOP;
			}
			iret=Lib_PiccM1ReadBlock(9, tmps);
			if(iret==0)
			{
				Lib_LcdClrLine(2*8,8*8);
				Lib_LcdPrintxy(0,2*8,0,"--Read M1 Success--");
				Lib_LcdGotoxy(0,3*8);
				Lib_LcdSetFont(8,16,0);
				for(i=0;i<16;i++)
					Lib_Lcdprintf("%02x ",tmps[i]);	
				Lib_Beep();	 
				Lib_KbGetCh();
				Lib_PiccClose();
				goto LOOP;			
			}
			else 
			{
				Lib_LcdClrLine(2*8,8*8);
				Lib_LcdPrintxy(0,2*8,0,"--Read M1 Failing--");
				Lib_LcdPrintxy(0,3*8,0,"[ERROR] %d",iret);
				Lib_LcdDrawBox(0,0,127,63);
				Lib_KbGetCh();
				Lib_PiccClose();
				goto LOOP;
			}
			break;
		default : break;
		}
	}

	return 0;
}
////////////////////////////////////////////////////

int Picc_all_test(void) 
{
	BYTE snbuf[100],tmps[300],ch;
	int iret,i;

	memset(snbuf,0,sizeof(snbuf));
	memset(tmps,0,sizeof(tmps));

	Lib_LcdCls();
	vDispTitle("PICC TEST");
	Lib_PiccOpen();
	Lib_DelayMs(100);

	Lib_SetTimer(2, 60*10);
	while(1)
	{
		if (!Lib_KbCheck())
		{
			if (Lib_KbGetCh() == KEYCANCEL)
			{
				return -1;
			}
		}

		if (!Lib_CheckTimer(2))
		{
			Lib_LcdClrLine(2*8,8*8);
			Lib_LcdPrintxy(0,4*8,0x01,"    N0 Card     "); 
			Lib_Beef(3, 1000);	 
			Lib_DelayMs(1000);
			return -1;
		}

		Lib_LcdPrintxy(0,3*8,0x01," Waiting Card... "); 

		iret=Lib_PiccCheck('A',0x00, snbuf); 
		if(!iret)  
			break;

		iret=Lib_PiccCheck('B',0x00, snbuf); 
		if(!iret)  
			break;
	}

	Lib_LcdClrLine(2*8,8*8);
	Lib_LcdPrintxy(0,3*8,0x01,"  Read Card OK ");
	Lib_Beep();
	if (snbuf[0] == 4)
	{
		Lib_LcdPrintxy(0,5*8,0x01,"  SN:%02X%02X%02X%02X ", snbuf[1], snbuf[2],  snbuf[3], snbuf[4]); 
	}
	else
	{
		Lib_LcdPrintxy(0,5*8,0x00," SN: "); 
		Lib_LcdGotoxy(5*9, 5*8);
		Lib_LcdSetFont(8, 16, 0);
		for (i=0; i<snbuf[0]; i++)
		{
			Lib_Lcdprintf("%02X", snbuf[1+i]);
		}
		Api_WaitKey(3);
	}
	Api_WaitKey(3);

	return 0;
}
////////////////////////////////////////////////////