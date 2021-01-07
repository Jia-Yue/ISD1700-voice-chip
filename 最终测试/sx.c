#include <REG51.h>
#include <stdio.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int

unsigned char bdata SR0_L;
unsigned char bdata SR0_H;
unsigned char bdata SR1;

int i,j,k;
unsigned char APCL=0,APCH=0;
unsigned char std_L,std_H,endd_L,endd_H;    //设置的开始录音地址与结束录音地址
sbit CMD=SR0_L^0;
sbit FULL=SR0_L^1;
sbit PU=SR0_L^2;
sbit EOM=SR0_L^3;
sbit INTT=SR0_L^4;
sbit RDY=SR1^0;
sbit ERASE=SR1^1;
sbit PLAY=SR1^2;
sbit REC=SR1^3; 

unsigned char ISD_SendData(unsigned char dat);
void ISD_PU(void);
void ISD_Rd_Status(void);
void ISD_WR_APC2(unsigned char apcdatl,apcdath);
void ISD_SET_PLAY(unsigned char Saddl,Saddh,Eaddl,Eaddh);
void ISD_SET_Rec(unsigned char Saddl,Saddh,Eaddl,Eaddh);
void ISD_SET_Erase(unsigned char Saddl,Saddh,Eaddl,Eaddh);

sbit SS=P1^4;
sbit SCK=P1^7;
sbit MOSI=P1^5;
sbit MISO=P1^6;
sbit PLY=P3^4;
sbit Rec=P3^5;
sbit sx=P1^3;
sbit stop=P1^0;
sbit sxcx=P1^2;
sbit allplay=P1^1;

void Cpu_Init(void);
void ISD_Init(void);
void delay(unsigned int t);
void delay10(void);
void EAUP(void);
extern void SPI_shaoxie(void);
extern void AllPlay(void); //首字母大写



void Cpu_Init(void)
{
	P0=P1=P2=P3=0xff;
	TMOD=0x01;
	EA=0;
	
}

void ISD_Init(void)
{
	uchar i=2;
	SS=1;
	SCK=1;
	MOSI=0;
	do
	{
		ISD_PU();
		delay(50);
		ISD_Rd_Status();
	}while(CMD||(!PU));
	ISD_WR_APC2(0x40,0x04);
	
	do
	{
		ISD_Rd_Status();
	}while(RDY==0);
	do
	{
		delay(300);
		delay(300);
		i--;
	}while(i>0);
}

void EAUP(void)
{	
	TMOD=0x66;
	TL0=TH0=0xFF;
	TL1=TH1=0xFF;
	EA=1;
	ET0=1;
	ET1=1;
	TR0=1;
	TR1=1;
}
unsigned char ISD_SendData(unsigned char dat)
{
	unsigned char i,j,BUF_ISD=dat;
	SCK=1;
	SS=0;
	for(j=4;j>0;j--)
	{;}

  for(i=0;i<8;i++)
		{
			SCK=0;
			for(j=2;j>0;j--)
			{;}
			if(BUF_ISD&0x01)
				MOSI=1;
			else
				MOSI=0;
			BUF_ISD>>=1;
			if(MISO)
				BUF_ISD|=0x80;
			SCK=1;
			for(j=6;j>0;j--)
			{;}
		}
		MOSI=0;
			return(BUF_ISD);
}

void ISD_Rd_Status(void)
{
	unsigned char i;
	ISD_SendData(0x05);
	ISD_SendData(0x00);
	ISD_SendData(0x00);
	SS=1;
	for(i=2;i>0;i--)
	{;}
	SR0_L=ISD_SendData(0x05);
	SR0_H=ISD_SendData(0x00);
	SR1=ISD_SendData(0x00);
	SS=1;
}

void ISD_PU(void)
{
	ISD_SendData(0x01);
	ISD_SendData(0x00);
	SS=1;
}

void ISD_WR_APC2(unsigned char apcdatl,apcdath)
{
	ISD_SendData(0x65);
	ISD_SendData(apcdatl);
	ISD_SendData(apcdath);
	SS=1;
}


void ISD_SET_PLAY(unsigned char Saddl,Saddh,Eaddl,Eaddh)
{
	ISD_SendData(0x80);
	ISD_SendData(0x00);
	ISD_SendData(Saddl);		
	ISD_SendData(Saddh);
  ISD_SendData(Eaddl); 
	ISD_SendData(Eaddh);
	ISD_SendData(0x00);
	SS=1;
	ISD_Rd_Status();

	while(PLAY&&(!CMD))
	{
		ISD_Rd_Status();
		if(stop==0)
		{
			ISD_SendData(0x02);
    	ISD_SendData(0x00);	
			SS=1; 
			delay10();
		}	
	}

}

void ISD_SET_Rec(unsigned char Saddl,Saddh,Eaddl,Eaddh)
{
	ISD_SendData(0x81);
	ISD_SendData(0x00);
	ISD_SendData(Saddl);
	ISD_SendData(Saddh);
	ISD_SendData(Eaddl);
	ISD_SendData(Eaddh);
	ISD_SendData(0x00);
	SS=1;
	ISD_Rd_Status();
	while(REC&&(!CMD))
	{
		ISD_Rd_Status();
		if(stop==0)
		{
			ISD_SendData(0x02);
    	ISD_SendData(0x00);	
			SS=1; 
			delay10();
		}	
	}
}

void ISD_SET_Erase(unsigned char Saddl,Saddh,Eaddl,Eaddh)
{
  ISD_SendData(0x82);
  ISD_SendData(0x00);
  ISD_SendData(Saddl);
	ISD_SendData(Saddh);
	ISD_SendData(Eaddl);
	ISD_SendData(Eaddh);
	ISD_SendData(0x00);
	SS=1;
	ISD_Rd_Status();
	while(ERASE)
	{
		ISD_Rd_Status();
	}
}

void delay(unsigned int t)
{
	for(;t>0;t--)
	{
		
		TH0=0xfc;
		
		TL0=0x18;
		TR0=1;
		while(TF0!=1)
    {;}
		TF0=0;
		TR0=0;
		}
}
void delay10(void)
{
	for(i=5;i>0;i--)
	for(j=4;j>0;j--)
	for(k=248;k>0;k--)
	{;}	
}

void SPI_PLY(void) interrupt 1
{
	ISD_SET_PLAY(std_L,std_H,endd_L,endd_H);	
}

void SPI_REC(void) interrupt 3
{
	ISD_SET_Rec(std_L,std_H,endd_L,endd_H);
}
	