#include <REG51.h>
#include <stdio.h>
#include <intrins.h>

sbit PLY=P3^4;
sbit Rec=P3^5;

extern unsigned char std_L,std_H,endd_L,endd_H; 

extern void ISD_SET_PLAY(unsigned char Saddl,Saddh,Eaddl,Eaddh);
extern void ISD_SET_Rec(unsigned char Saddl,Saddh,Eaddl,Eaddh);
extern void ISD_SET_Erase(unsigned char Saddl,Saddh,Eaddl,Eaddh);
extern void delay10(void);
extern int k;

void SPI_shaoxie(void)
{
	PLY=1;
	PLY=0;
	for(k=4;k>0;k--)
	{;}	  
	PLY=1;	              
  TL0=0xFF;
  delay10();
	ISD_SET_PLAY(0,0,3,0);     //听到提示音后播放完毕	SE1
	delay10();

	ISD_SET_Erase(std_L,std_H,endd_L,endd_H);    
  delay10();
		
	ISD_SET_PLAY(3,0,7,0);      //听到提示音后擦除完毕 SE2
	delay10();
	
  Rec=1;
	Rec=0;
	for(k=4;k>0;k--)
	{;}	
	TL1=0xFF; 
	Rec=1;
	delay10();		
	ISD_SET_PLAY(7,0,11,0);     //听到提示音录音结束 SE3
  delay10();
	
	PLY=1;
	PLY=0;                      //跳至播放中断
	for(k=4;k>0;k--)
	{;}	
	TL0=0xFF;			//听到提示音后第二次播放完毕 SE4
	PLY=1;	               
	delay10();
	ISD_SET_PLAY(12,0,15,0); 
	delay10();
}

void AllPlay(void)
{
	TL0=0xFE;
  PLY=0; 
  for(k=4;k>0;k--)
    {;}
  ISD_SET_PLAY(0x10,0,0xEF,0x01);
  delay10();				
  PLY=1;
  ISD_SET_PLAY(12,0,15,0);
  delay10();
}
	


