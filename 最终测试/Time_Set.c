#include <reg52.h>
#include <stdio.h>
#include <absacc.h>
#include <intrins.h>
#define fosc 		11.0592				//????????11.0592MHz
#define adj	-17							  //???????,???????

void UART_initial(unsigned int x);
void send(unsigned char x);
signed int receiv();
void wait_bit();
extern void delay(unsigned int t);
extern void serial_initial(void);
unsigned char address_judge(unsigned char d);
void address_assign(void);


sbit TXD1=P2^7;
sbit RXD1=P2^6;
sbit test=P2^2;

signed int read_buf;
extern unsigned char std_L,std_H,endd_L,endd_H;
unsigned char x; 
extern unsigned char i,j;
unsigned char d[2];
signed int read_buf;
bit RIx,TIx,error; 
unsigned char thx,tlx;
unsigned char address=0x10,DH=0;
unsigned char puterror;


	

void UART_initial(unsigned int x)
{
	TR0=0;									
	TMOD=(TMOD & 0XF0) | 0x01;				
	x=-(((1000000/x)/2)*fosc/12+adj);		
	TH0=x/256;
	TL0=x%256;
	thx=TH0;
	tlx=TL0;
	RXD1=1;								
}

void send(unsigned char x)
{		unsigned char i;
		TXD1=0;				  				
		wait_bit();							
		for(i=0;i<=7;i++)
		  {
			TXD1=x & 0x01;       
			x=x>>1;               
			wait_bit();						
		  }
		TXD1=1;				  				
		wait_bit();							
		TIx=1;								
}
void wait_bit()
{
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);						
		TR0=0;
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);						
		TR0=0;
}
signed int receiv()
{		unsigned char temp,i,x;
		while(RXD1);					
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					
		TR0=0;
		test=1;
		temp=(char)RXD1;
		test=0;
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);						
		TR0=0;						
		if(temp==1) return(-1);			

		for(i=0;i<=7;i++)
		{
			TH0=thx;TL0=tlx;TF0=0;TR0=1;
			while(!TF0);					
			TR0=0;
			test=1;
			temp=RXD1;
			test=0;
			x=x>>1;
			if(RXD1) x=x | 0x80;
			TH0=thx;TL0=tlx;TF0=0;TR0=1;
			while(!TF0);					
			TR0=0;		  
		}

		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					
		TR0=0;
		test=1;
		temp=RXD1;						
		test=0;
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);						
		TR0=0;			   					
		if(temp!=1) return(-1);
		RIx=1;						   
		return(x);
}

unsigned char address_judge(unsigned char d)
{
	unsigned char j;	
	
	if(d>=0x00&&d<=0x09)
	{
	address=0x10;
	j=d-0x00;
  for(j;j>0;j--)
	address=address+0x08;	
	}	
	
	else if(d>=0x10&&d<=0x19)
	  { 
		  address=0x60;
		  j=d-0x10;
	    for(j;j>0;j--)
      address=address+0x08;
		}
	
  else if(d>=0x20&&d<=0x29)
		{ 
			address=0xB0; 
			j=d-0x20;
	    for(j;j>0;j--)
      address=address+0x08;
		}
	
	else if(d>=0x30&&d<=0x39)
		{ 
			address=0x00;
			j=d-0x30;
      for(j;j>0;j--)
   	  address=address+0x08;
			DH=0x01;
		}
	
  else if(d>=0x40&&d<=0x49)
		{ 
			address=0x50;
			j=d-0x40;
	    for(j;j>0;j--)
      address=address+0x08;
			DH=0x01;
		}
	
  else if(d>=0x50&&d<=0x60)
		{ 
			address=0xA0;
			j=d-0x50;
	    for(j;j>0;j--)
      address=address+0x08;
			DH=0x01;
		}
  
	else {printf("地址输入不合法 \n"); puterror=1;}
	
	return(address);
}

void address_assign(void)
{
	std_L=address_judge(d[0]);
	std_H=DH;
	delay(300);
	endd_L=address_judge(d[1])-0x01;
	endd_H=DH;
}



void Time_Set (void)
{
	serial_initial();
	UART_initial(9600);	
puterror=1;	
while(puterror)
{
	puterror=0;
  for(j=0;j<=1;j++)
		{
			read_buf=receiv();
			RIx=0;
			d[j]=read_buf;
			delay(200);		
		nop_();
			if(d[j]=='\0') d[j]=0x00;
		}	
		address_assign();
		if(d[0]>d[1])
		{puterror=1;
		printf("地址输入顺序错误 \n");	}
		
		send(puterror);
			TIx=0;
			_nop_();
}
		printf("赋值完毕 \n");	
}

