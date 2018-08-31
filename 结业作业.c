#include<reg51.h>
#define uint unsigned int
#define uchar unsigned char

/****共阴数码管0--9+全灭段码****/
uchar code led[]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00
};

uchar a[6];//数码管显示 
uchar b[6]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf};//数码管位码
/****子函数声明****/
void delay_ms(uint z);//带参数ms级延时子函数
void init();//初始化函数
void timechuli();//时间处理
void key();//按键函数
void display();//显示函数

/****变量定义****/

uint num;  // 记录毫秒为秒的变量
uchar hour=23;
uchar minite=59;
uchar second=56; 
uchar hour1=0;
uchar minite1=1;
uchar second1=0;

uchar k;
uchar flag;//更新时间的4种模式加上正常模式
uchar S_flag;//闪烁标志位

/****位定义****/
sbit beep=P3^6; //蜂鸣器
sbit led0=P3^0; //整点显示
sbit led1=P3^7; //闹钟报时显示
sbit  flashflag=P3^1;

sbit key1=P1^0;//模式切换键1正常模式 2时钟时调整 3时钟分调整 4闹钟时调整 5闹时钟分调整
sbit key2=P1^1;//加1
sbit key3=P1^2;//减1
sbit key4=P1^3; //复位键

void main()
{
    init();
while(1)
{
key();
     display();
}
}

void display()
{
switch(flag)
{
case 0:  //时钟显示模式
{
a[0]=led[hour/10];
a[1]=led[hour%10];
a[2]=led[minite/10];
a[3]=led[minite%10];
a[4]=led[second/10];
a[5]=led[second%10];
}
break;

   case 1:  // 时钟 时调整模式
{
if(S_flag==1)  //标志位为1								 
{
a[0]=led[hour/10];//调整时钟的时
a[1]=led[hour%10];
}
else
{
a[0]=led[11];//关闭显示								
a[1]=led[11];
}
a[2]=led[minite/10];
a[3]=led[minite%10];
a[4]=led[second/10];
a[5]=led[second%10];
}
break;
case 2:  //时钟 分调整模式
{
a[0]=led[hour/10];
a[1]=led[hour%10];
if(S_flag==1)  
{
a[2]=led[minite/10];
a[3]=led[minite%10];
}
else
{
a[2]=led[11];
a[3]=led[11];
}
a[4]=led[second/10];
a[5]=led[second%10];
}
break;

   case 3:  // 闹钟 时调整模式
{
if(S_flag==1)
{
a[0]=led[hour1/10];
a[1]=led[hour1%10];
}
else
{
a[0]=led[11];
a[1]=led[11];
}

a[2]=led[minite1/10];
a[3]=led[minite1%10];

a[4]=led[11];
a[5]=led[11];
}
break;

case 4: 
{
a[0]=led[hour1/10];
a[1]=led[hour1%10];

if(S_flag==1)// 闹钟 分调整模式
{
a[2]=led[minite1/10];
a[3]=led[minite1%10];
}
else
{
a[2]=led[11];
a[3]=led[11];
}
a[4]=led[11]; 
a[5]=led[11]; ///
}
break;

}
}


void init()
{
    beep=0;//关闭蜂鸣器
    flag=0;//正常时钟显示模式
    S_flag=0;//闪烁标志位
    TMOD=0X01;//定时器T0 方式1
    TH0=(65536-50000)/256;//重装初值
    TL0=(65536-50000)%256;
    EA=1;//开总中断
    ET0=1;//开启定时器T0
    TR0=1;
}



void time0() interrupt 1
{
TH0=(65536-50000)/256;//重装初值
TL0=(65536-50000)%256;
num++;
if(num==20)//1S到
{
num=0;
timechuli();//时间处理函数
}
if(hour1==hour&&minite1==minite&&second<5)//闹钟时间到
{
 beep=!beep;
 led1=!led1;
}
else//闹钟提醒时间到后 关闭蜂鸣器和指示灯
{
beep=0;
led1=0;
}

if((minite==0)&&(second==0)) //整点到 
{
led0=!led0;
}
else  //关闭LED指示
 led0=0;

if(num%20==0) //1S取反一次
    S_flag=!S_flag;     //闪烁标志位取反

for(k=0;k<6;k++)
{
P2=b[k];  //送位码
P0=a[k];  //送段码									   
delay_ms(2);
P2=0xff;//消影
}
}


void key()
{
if(key1==0)//模式切换键按下
{
delay_ms(10);
if(key1==0)
{
  flag++;
  if(flag==5)
  {
flag=0;
  }
}
while(!key1) ; //等待按键释放
}
if(flag!=0)								   
{
switch(flag)									
{
case 1:   //时钟时调整
{
if(key2==0)  //时钟时加1
{
delay_ms(10);
if(key2==0)
{
if(hour<23) hour++;
else hour=0;
}
while(!key2);
}

if(key3==0)  //时钟时减1
{
delay_ms(10);
if(key3==0)
{
       if(hour> 0) hour--;
else hour=23;
}
while(!key3) ;
}
}  
break;

case 2:   //时钟分调整
{
if(key2==0)  //时钟分加1
{
delay_ms(10);
if(key2==0)
{
if(minite<59) minite++;
else minite=0;
}
while(!key2);
}

if(key3==0)  //时钟分减1
{
delay_ms(10);
if(key3==0)
{
       if(minite> 0)minite--;
else minite=59;
}
while(!key3);
}
}  
break;

case 3:   //闹钟时调整
{
if(key2==0)  //闹钟时加1
{
delay_ms(10);
if(key2==0)
{
if(hour1<23) hour1++;
else hour1=0;
}
while(!key2);
}

if(key3==0)  //闹钟时减1
{
delay_ms(10);
if(key3==0)
{
       if(hour1> 0) hour1--;
else hour1=23;
}
while(!key3);
}
}  
break;

case 4:   //闹钟分调整
{
if(key2==0)  //闹钟分加1
{
delay_ms(10);
if(key2==0)
{
if(minite1<59) minite1++;
else minite1=0;
}
while(!key2) ;
}

if(key3==0)  //闹钟分减1
{
delay_ms(10);
if(key3==0)
{
       if(minite1> 0)minite1--;
else minite1=59;
}
while(!key3);
}
}  
break;

}
}
if(key4==0)//复位键
{
delay_ms(10);
if(key4==0)
{
   flag=0;
hour=0;
minite=0;
second=0;
}
while(!key4);
}
}


void delay_ms(uint z)  
{
uint x,y;
for(x=z;x>0;x--)
for(y=110;y>0;y--);
}
//时间自走函数
void timechuli()
{
second++;
   if(second%2==0)
   flashflag=1;
   else flashflag=0;
   if(second==60)
   {
   second=0;
   minite++;
   }
   if( minite==60)
   {
    minite=0;
    hour++;
   }
   if(hour==24)
   hour=0;

}