#include "public.h" 

int COM_Test(void)
{
	unsigned short i,j;
	unsigned char buf[100];
	unsigned char ch,ret,cmd,comno;
	unsigned char dispm=0;
	int iret;
     
     
     Lib_LcdCls();
     Lib_LcdPrintxy(0,0,0x81,"    COM TEST    ");
     Lib_LcdPrintxy(0,2*8,1,"1-COM1(115200)");
     Lib_LcdPrintxy(0,4*8,1,"2-COM2(115200)");	 
     ch=Lib_KbGetCh(); 
     if(ch=='2')  comno=COM2;
     else         comno=COM1; 

     Lib_LcdClrLine(2*8,7*8);
     Lib_LcdPrintxy(0,2*8,1,"1-NONE");
     Lib_LcdPrintxy(0,4*8,1,"2-EVEN");
     Lib_LcdPrintxy(0,6*8,1,"3-ODD");
     cmd=Lib_KbGetCh();
     if(cmd=='2')
	     iret=Lib_ComOpen(comno,"115200,8,e,1");
     else if(cmd=='3')
	     iret=Lib_ComOpen(comno,"115200,8,o,1");
     else
          iret=Lib_ComOpen(comno,"115200,8,n,1");

     Lib_LcdGotoxy(0,2*8);
     Lib_LcdClrLine(2*8,7*8);
     Lib_LcdPrintxy(0,6*8,1,"open(115200)");
     if(iret){
	     Lib_LcdClrLine(2*8,7*8);
	     Lib_LcdPrintxy(0,2*8,1,"Open Error!");
	     Lib_KbGetCh();
	     return 0;
     }
     while(1){
	     iret=Lib_ComRecvByte(comno,&ch,50);
	     if(iret==0){
		     Lib_Lcdprintf("%02x",ch);
		     Lib_ComSendByte(comno,ch);
	     }				
	     if(!Lib_KbCheck()){
		     ch=Lib_KbGetCh();
		     if(ch==KEYCANCEL){
		          Lib_ComClose(comno);
		          return 0;
		     }
		     else Lib_Lcdprintf("key=%02x",ch);
	     }
     }     
	return 0;
}


 