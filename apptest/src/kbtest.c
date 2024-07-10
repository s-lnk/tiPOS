#include "public.h"

 
void getstring(unsigned char ucKey, char *buf)
{
	switch(ucKey){
		  case KEY1:
			  strcpy(buf," KEY: 1      ");
			  break;
		  case KEY2:
			  strcpy(buf," KEY: 2      ");
			  break;
		  case KEY3:
			  strcpy(buf," KEY: 3      ");
			  break;
		  case KEY4:
			  strcpy(buf," KEY: 4      ");
			  break;
		  case KEY5:
			  strcpy(buf," KEY: 5      ");
			  break;
		  case KEY6:
			  strcpy(buf," KEY: 6      ");
			  break;
		  case KEY7:
			  strcpy(buf," KEY: 7      ");
			  break;
		  case KEY8:
			  strcpy(buf," KEY: 8      ");
			  break;
		  case KEY9:
			  strcpy(buf," KEY: 9      ");
			  break;
		  case KEY0:
			  strcpy(buf," KEY: 0      ");
			  break;
		  case KEY00:
			  strcpy(buf," KEY: 00     ");
			  break;
		  case KEYALPHA:
			  strcpy(buf," KEY: ALPHA  ");
			  break;
		  case KEYENTER:
			  strcpy(buf," KEY: ENTER  ");
			  break;
		  case KEYCLEAR:
			  strcpy(buf," KEY: CLEAR  ");
			  break;
		  case KEYMENU:
			  strcpy(buf," KEY: MENU   ");
			  break;
		  case KEYFN:
			  strcpy(buf," KEY: FUNCT  ");
			  break;
		  case KEYUP:
			  strcpy(buf," KEY: UP     ");
			  break;
		  case KEYDOWN:
			  strcpy(buf," KEY: DOWN   ");
			  break;
		  case KEYBACKSPACE:
			  strcpy(buf," KEY: <--    ");
			  break;

		  case  KEYDOT:
			  strcpy(buf," KEY: .     ");
			  break;

		  case  KEYOK:
			  strcpy(buf," KEY: OK    ");
			  break;
	}
}



/*
int KB_Test(void)
{	
	int i,j;
	uchar ch;
	unsigned char ucKey,buf[20]; 
 
	Lib_LcdClrLine(2*8,8*8);
	Lib_Lcdprintf("KB TEST:"); 
	while(1){	
		ucKey=Lib_KbGetCh();
		if(ucKey==KEYCANCEL)
		{ 
			return 0; 
		}
		memset(buf,0,sizeof(buf));
		getstring(ucKey,(char *)buf);
		Lib_LcdGotoxy(0,6*8);
		vDisp2(3,(char *)buf); 
	}
}
*/



int Test_KbFun(int mode)
{
	int i,j;
	uchar TestAll[256],ch;
	unsigned char ucKey,buf[30];

	if(mode)
		Lib_LcdPrintxy(0,0,0x81,"    KB  TEST    ");
	for(i=0;i<256;i++) TestAll[i]=0;
	Lib_LcdClrLine(2*8,63);
	Lib_LcdPrintxy(0,2*8,1," Pls Press:");
	Lib_LcdDrawBox(0,0,127,63);

	while(1){
		ucKey=Lib_KbGetCh();
		if(ucKey==KEYCANCEL){
			j=0;
			for(i=0;i<256;i++) if(TestAll[i]) j++;
			if(j==17) return 0;
			Lib_LcdClrLine(2*8,63);
			Lib_LcdGotoxy(0,2*8);
			Lib_Lcdprintf(" NOT FINISH!\n");
			Lib_Lcdprintf(" CANCEL-EXIT\n");
			Lib_Lcdprintf(" ENTER -AGAIN\n");
			Lib_LcdDrawBox(0,0,127,63);
			ch=Lib_KbGetCh();
			if(ch==KEYCANCEL) return 1;

			Lib_LcdClrLine(2*8,63);
			Lib_LcdPrintxy(0,2*8,1," Pls Press:");
			Lib_LcdDrawBox(0,0,127,63);
			continue;
		}
		memset(buf,0,sizeof(buf));
		getstring(ucKey,(char *)buf);
		Lib_LcdPrintxy(0,6*8,1,(char *)buf);
		TestAll[ucKey]=1;
		Lib_LcdDrawBox(0,0,127,63);
	}
}
