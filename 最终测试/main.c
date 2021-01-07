#include <REG51.h>
#include <stdio.h>
#include <intrins.h>

extern unsigned char std_L,std_H,endd_L,endd_H;
sbit sx=P1^3;
sbit stop=P1^0;
sbit sxcx=P1^2;
sbit allplay=P1^1;

extern void Cpu_Init(void);
extern void ISD_Init(void);
extern void delay10(void);
extern void EAUP(void);
extern void SPI_shaoxie(void);
extern void AllPlay(void);
extern void Tem(void);
extern void ISD_SET_PLAY(unsigned char Saddl,Saddh,Eaddl,Eaddh);
extern void Time_Set(void);

void main(void)
{	
	Tem();
	Time_Set();
	Cpu_Init();
	ISD_Init();
	EAUP();

	while(1)
	{ 
		  
		ISD_SET_PLAY(12,0,15,0);
		delay10();
		if(sx==0)                  //S7开关
		{
 		SPI_shaoxie();
			while(sx)
			{
				if(sxcx==0)             //S8按键
					SPI_shaoxie();
			}

		}				
	  if(allplay==0)	               //S9按键

		{AllPlay();}		
	}	
}