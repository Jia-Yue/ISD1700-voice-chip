#include <reg51.h>
#include <stdio.h>
#include <intrins.h>

typedef unsigned char uchar;
sbit TMDAT = P2^5;

void serial_initial(void);
void dmsec(unsigned int count);
void tmreset(void);
void tmstart(void);
unsigned char tmrtemp(void);


void Tem(void)
{
unsigned char last;
serial_initial();

dmsec(1);
tmstart();
dmsec(1);
last=tmrtemp();
printf("The current temperature:%bd\n",last);

}

void dmsec(unsigned int count)
{
unsigned int i;
while(count--)
{
for(i=0;i<125;i++){}
}
}

void tmreset(void)
{
unsigned int i;
TMDAT = 0;
i=103;
while(i>0) i--;
TMDAT = 1;
i=4;
while(i>0) i--;
}

void tmpre(void)
{
unsigned int i;
while(TMDAT);
while(~TMDAT);
i=4;
while(i>0) i--;
}

bit tmrbit(void)
{
unsigned int i;
bit dat;
TMDAT = 0; i++;
TMDAT = 1; i++; i++;
dat=TMDAT;
i=8;
while(i>0) i--;
return(dat);
}

unsigned char tmrbyte(void)
{
unsigned char i,j,dat;
dat=0;
for(i=1;i<=8;i++)
{
j=tmrbit();
dat=(j<<7)|(dat>>1);
}
return(dat);
}

void tmwbyte(unsigned char dat)
{
unsigned int i;
unsigned char j;
bit testb;
for(j=1;j<=8;j++)
{
testb=dat&0x01;
dat=dat>>1;
if(testb)
{
TMDAT=0;
i++; i++;
TMDAT=1;
i=8;
while(i>0) i--;
}
else
{
TMDAT=0;
i=8;
while(i>0) i--;
TMDAT=1;
i++; i++;
}
}
}

void tmstart(void)
{
tmreset();
tmpre();
dmsec(1);
tmwbyte(0xcc);
tmwbyte(0x44);
}

unsigned char tmrtemp(void)
{
unsigned char a,b,y1,y2,y3;
tmreset();
tmpre();
dmsec(1);
tmwbyte(0xcc);
tmwbyte(0xbe);
a=tmrbyte();
b=tmrbyte();
y1=a>>4;
y2=b<<4;
y3=y1|y2;
return(y3);
}

 void serial_initial(void)
 {
 SCON=0X52;					   //8051单片机串行口初始化（SCON=0101 0010）
 TMOD=0X20;					   /*设定定时器/计数器1的工作模式,TMOD=0010 0000，即GATE=0(只要TR1=1，定时器开通),C/T=0（定时器方式）,
	                                   M1=1 M0=0(模式2，8位自动重装)  ，中断INT0和INT1都是低电平触发*/
 TH1=0XFd;
 TR1=1;
 }
