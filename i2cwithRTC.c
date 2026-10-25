#include<p18f4520.h>
#include "LCD.h"

void RTC_init(void);
void RTC_write(void);
void start(void);
void stop(void);
void i2c_data(unsigned char);
void i2c_read(void);
void read_time(void);
void display(void);
void check(void);

unsigned char x, arr[8],time[8],sec,min,hr;
int i;

void main()
{
unsigned char str[20]="RTC INTERFACE";
lcd_init();
RTC_init();
lcd_string(str);
RTC_write();
while(1)
{
read_time();
}
}
void RTC_init(void)
{
TRISC=0X18;
SSPCON1=0X28;
SSPSTAT=0XC0;
PIE1bits.SSPIE=1;
SSPADD=0X31;
}
void RTC_write(void)
{
//sec
start();
i2c_data(0XD0);//slave address
i2c_data(0X00);//word add(sec)
PIR1bits.SSPIF=0;
i2c_data(0X00);//data
PIR1bits.SSPIF=0;
stop();


//minutes
start();
i2c_data(0XD0);//slave address
i2c_data(0X01);//word add(sec)
PIR1bits.SSPIF=0;
i2c_data(0X45);//data
PIR1bits.SSPIF=0;
stop();

//hr
start();
i2c_data(0XD0);//slave address
i2c_data(0X02);//word add(sec)
PIR1bits.SSPIF=0;
i2c_data(0X13);//data
PIR1bits.SSPIF=0;
stop();
}
void start(void)
{
PIR1bits.SSPIF=0;
SSPCON2bits.SEN=1;
while(PIR1bits.SSPIF==0);
PIR1bits.SSPIF=0;
}
void i2c_data(unsigned char x)
{
SSPBUF=x;
while(PIR1bits.SSPIF==0);
PIR1bits.SSPIF=0;
}
void stop(void)
{
PIR1bits.SSPIF=0;
SSPCON2bits.PEN=1;
while(PIR1bits.SSPIF==0);
PIR1bits.SSPIF=0;
}
void read_time(void)
{
start();
i2c_data(0XD0);
i2c_data(0X00);
stop();
i2c_read();
display();
}
void i2c_read(void)
{
start();
SSPBUF=0XD1;
check();
for(i=0;i<7;i++)
{
PIR1bits.SSPIF=0;
SSPCON2bits.RCEN=1;
while(PIR1bits.SSPIF==0);
PIR1bits.SSPIF=0;
x=SSPBUF;
arr[i]=x;
SSPCON2bits.ACKDT=0;
SSPCON2bits.ACKEN=1;
while(PIR1bits.SSPIF==0);
PIR1bits.SSPIF=0;
}
PIR1bits.SSPIF=0;
SSPCON2bits.RCEN=1;
while(PIR1bits.SSPIF==0);
PIR1bits.SSPIF=0;
x=SSPBUF;
arr[i]=x;
SSPCON2bits.ACKDT=1;
SSPCON2bits.ACKEN=1;
while(PIR1bits.SSPIF==0);
PIR1bits.SSPIF=0;
stop();
}
void check(void)
{
int a;
PIR1bits.SSPIF=0;
while(PIR1bits.SSPIF==0);
PIR1bits.SSPIF=0;
a=SSPBUF;
}
void display(void)
{
sec=arr[0];
min=arr[1];
hr=arr[2];
time[0]=((hr&0XF0)>>4)+'0';
time[1]=(hr&0X0F)+'0';
time[2]=':';
time[3]=((min&0XF0)>>4)+'0';
time[4]=(min&0X0F)+'0';
time[5]=':';
time[6]=((sec&0XF0)>>4)+'0';
time[7]=(sec&0X0F)+'0';
lcd_cmd(0XC0);
for(i=0;i<8;i++)
{
lcd_data(time[i]);
}
}

