#include<reg51.h>
#define uint unsigned int
#define uchar unsigned char

/****���������0--9+ȫ�����****/
uchar code led[]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00
};

uchar a[6];//�������ʾ 
uchar b[6]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf};//�����λ��
/****�Ӻ�������****/
void delay_ms(uint z);//������ms����ʱ�Ӻ���
void init();//��ʼ������
void timechuli();//ʱ�䴦��
void key();//��������
void display();//��ʾ����

/****��������****/

uint num;  // ��¼����Ϊ��ı���
uchar hour=23;
uchar minite=59;
uchar second=56; 
uchar hour1=0;
uchar minite1=1;
uchar second1=0;

uchar k;
uchar flag;//����ʱ���4��ģʽ��������ģʽ
uchar S_flag;//��˸��־λ

/****λ����****/
sbit beep=P3^6; //������
sbit led0=P3^0; //������ʾ
sbit led1=P3^7; //���ӱ�ʱ��ʾ
sbit  flashflag=P3^1;

sbit key1=P1^0;//ģʽ�л���1����ģʽ 2ʱ��ʱ���� 3ʱ�ӷֵ��� 4����ʱ���� 5��ʱ�ӷֵ���
sbit key2=P1^1;//��1
sbit key3=P1^2;//��1
sbit key4=P1^3; //��λ��

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
case 0:  //ʱ����ʾģʽ
{
a[0]=led[hour/10];
a[1]=led[hour%10];
a[2]=led[minite/10];
a[3]=led[minite%10];
a[4]=led[second/10];
a[5]=led[second%10];
}
break;

   case 1:  // ʱ�� ʱ����ģʽ
{
if(S_flag==1)  //��־λΪ1								 
{
a[0]=led[hour/10];//����ʱ�ӵ�ʱ
a[1]=led[hour%10];
}
else
{
a[0]=led[11];//�ر���ʾ								
a[1]=led[11];
}
a[2]=led[minite/10];
a[3]=led[minite%10];
a[4]=led[second/10];
a[5]=led[second%10];
}
break;
case 2:  //ʱ�� �ֵ���ģʽ
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

   case 3:  // ���� ʱ����ģʽ
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

if(S_flag==1)// ���� �ֵ���ģʽ
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
    beep=0;//�رշ�����
    flag=0;//����ʱ����ʾģʽ
    S_flag=0;//��˸��־λ
    TMOD=0X01;//��ʱ��T0 ��ʽ1
    TH0=(65536-50000)/256;//��װ��ֵ
    TL0=(65536-50000)%256;
    EA=1;//�����ж�
    ET0=1;//������ʱ��T0
    TR0=1;
}



void time0() interrupt 1
{
TH0=(65536-50000)/256;//��װ��ֵ
TL0=(65536-50000)%256;
num++;
if(num==20)//1S��
{
num=0;
timechuli();//ʱ�䴦����
}
if(hour1==hour&&minite1==minite&&second<5)//����ʱ�䵽
{
 beep=!beep;
 led1=!led1;
}
else//��������ʱ�䵽�� �رշ�������ָʾ��
{
beep=0;
led1=0;
}

if((minite==0)&&(second==0)) //���㵽 
{
led0=!led0;
}
else  //�ر�LEDָʾ
 led0=0;

if(num%20==0) //1Sȡ��һ��
    S_flag=!S_flag;     //��˸��־λȡ��

for(k=0;k<6;k++)
{
P2=b[k];  //��λ��
P0=a[k];  //�Ͷ���									   
delay_ms(2);
P2=0xff;//��Ӱ
}
}


void key()
{
if(key1==0)//ģʽ�л�������
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
while(!key1) ; //�ȴ������ͷ�
}
if(flag!=0)								   
{
switch(flag)									
{
case 1:   //ʱ��ʱ����
{
if(key2==0)  //ʱ��ʱ��1
{
delay_ms(10);
if(key2==0)
{
if(hour<23) hour++;
else hour=0;
}
while(!key2);
}

if(key3==0)  //ʱ��ʱ��1
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

case 2:   //ʱ�ӷֵ���
{
if(key2==0)  //ʱ�ӷּ�1
{
delay_ms(10);
if(key2==0)
{
if(minite<59) minite++;
else minite=0;
}
while(!key2);
}

if(key3==0)  //ʱ�ӷּ�1
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

case 3:   //����ʱ����
{
if(key2==0)  //����ʱ��1
{
delay_ms(10);
if(key2==0)
{
if(hour1<23) hour1++;
else hour1=0;
}
while(!key2);
}

if(key3==0)  //����ʱ��1
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

case 4:   //���ӷֵ���
{
if(key2==0)  //���ӷּ�1
{
delay_ms(10);
if(key2==0)
{
if(minite1<59) minite1++;
else minite1=0;
}
while(!key2) ;
}

if(key3==0)  //���ӷּ�1
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
if(key4==0)//��λ��
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
//ʱ�����ߺ���
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