////////////////////////////////////////////
//程序名：51单片机INT1测量脉冲信号周期
//作者：szy0syz
//时间：2010.7.19  
/////////////////////////////////////////////

#include unsigned char const dofly[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//显示以十六进制显示在LED上
int temp,i;
sbit PP=P3^3;

void delay(unsigned int cnt)
{
    while(--cnt);
}

void display()
{
    for(i=0;i<100;i++)
    {
        temp=TH1/16;  //高十位
        P0=dofly[temp];
        P2=0;
        delay(300);
        temp=TH1%16; //高个位
        P0=dofly[temp];
        P2=1;
        delay(300);
        temp=TL1/16; //低个位
        P0=dofly[temp];
        P2=2;
        delay(300);
        temp=TL1%16; //低十位
        P0=dofly[temp];
        P2=3;
        delay(300);
    }
}

/*------------------------------------------------
                    主程序
------------------------------------------------*/
main()
{
    TMOD |= 0x10;//开T1的门控位，选择T1工作模式位01(16bit)
    EA=1;        //全局中断开
    EX1=1;       //外部中断1开
    IT1=1;       //边沿触发，IT1=0表示电平触发
    TR1=0;	     //T1定时器关闭
    ET0=0;       //定时器0中断打开
    TR0=0;       //定时器0开关打开
    TH0=0xec;	 //给定初值，这里使用定时器最大值从0开始计数一直到65535溢出
    TL0=0x77;    //中断定时为1ms  
    TL1=0;
    TH1=0;
    while(1)
    {
        ;
    }
}

/*------------------------------------------------
                 外部中断程序
------------------------------------------------*/
void ISR_Key(void) interrupt 2 using 1
{
	delay(20);
	if(PP==0)
	{
		TR1=~TR1;
		if(TR1==0) 
		{
			display();
			TL1=0;
			TH1=0;
		}
	}
}
void Timer0_isr(void) interrupt 1 using 1
{
    PP=~PP;
    TH0=0xec;	  //给定初值，这里使用定时器最大值从0开始计数一直到65535溢出
    TL0=0x77;     //中断定时为1ms  
}