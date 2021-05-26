 /**************************************************************************
 作  者 ：大鱼电子
淘宝地址：https://shop119207236.taobao.com
**************************************************************************/
#include "sys.h"
/****************************全局变量*************************************/    
float Voltage;  															 //电池电压采样相关的变量
float pitch,roll,yaw; 								  			 //欧拉角(姿态角)
short aacx,aacy,aacz;													 //加速度传感器原始数据
short gyrox,gyroy,gyroz;											 //陀螺仪原始数据
float UltrasonicWave_Distance;                 //超声波测距
int   Encoder_Left,Encoder_Right;         		 //左右编码器的脉冲计数
int 	Moto1=0,Moto2=0;												 //计算出来的最终赋给电机的PWM
int 	Flag_Qian,Flag_Hou,Flag_Left,Flag_Right; //蓝牙遥控相关的变量
int		pulse1,pulse4;
u8    flag_UltrasonicWave=0;									 //超声波是否超出安全距离标志位
u8    flag_fall=0;					 									 //摔倒标志位
u8    key=0;								 									 //按键的键值
u8    KEY_MODE=0;					 										 //模式0,显示名字===HELLO BLIZ
																							 //模式1,电量模式
																							 //模式2,表情模式
																							 //模式3,参数模式
u8 DIS_STATE=255;				 											 //用来实现只刷一次屏幕的变量			


int mode = 2;
#define pai 3.1415926

//char a[20]="";
//int pitch1,pitch2;;
//int j=0;
/***********************************************************************/

/****************************上位机变量*********************************/   
unsigned char i;          										 //计数变量
unsigned char Send_Count; 										 //串口需要发送的数据个数
/**********************************************************************/

int main(void)	
{ 
	int pitch1,x=60,y=0,flag1 = 1,flag2 = 1;
//	float l = 119,h = 15,alf,rad,angle,d,d2,l2,ld;
	float data[150]={-87.85,
-87.31,
-86.76,
-86.19,
-85.62,
-85.04,
-84.44,
-83.83,
-83.21,
-82.57,
-81.91,
-81.24,
-80.55,
-79.85,
-79.12,
-78.37,
-77.6,
-76.81,
-75.99,
-75.14,
-74.27,
-73.36,
-72.42,
-71.45,
-70.44,
-69.38,
-68.28,
-67.14,
-65.94,
-64.69,
-63.38,
-62.01,
-60.56,
-59.04,
-57.45,
-55.76,
-53.98,
-52.1,
-50.11,
-48.01,
-45.78,
-43.42,
-40.92,
-38.28,
-35.49,
-32.54,
-29.44,
-26.18,
-22.77,
-19.22,
-15.55,
-11.76,
-7.89,
-3.96,
0,
3.96,
7.89,
11.76,
15.55,
19.22,
22.77,
26.18,
29.44,
32.54,
35.49,
38.28,
40.92,
43.42,
45.78,
48.01,
50.11,
52.1,
53.98,
55.76,
57.45,
59.04,
60.56,
62.01,
63.38,
64.69,
65.94,
67.14,
68.28,
69.38,
70.44,
71.45,
72.42,
73.36,
74.27,
75.14,
75.99,
76.81,
77.6,
78.37,
79.12,
79.85,
80.55,
81.24,
81.91,
82.57,
83.21,
83.83,
84.44,
85.04,
85.62,
86.19,
86.76,
87.31,
87.85,
88.38,
88.9,
89.42,
89.92,
89.58,
89.09,
88.61,
88.13,
87.66,
87.2,
86.74,
86.29,
};
	

	KEY_Init();                    //=====初始化与按键连接的IO
	LED_Init();
	delay_init();	    	           //=====延时函数初始化	

	NVIC_Configuration();					 //=====中断优先级分组,其中包含了所有的中断优先级的配置,方便管理和一次性修改。
	Adc_Init();                    //=====初始化ADC
	OLED_Init();                   //=====OLED初始化
	OLED_Clear();									 //=====OLED清屏
	MPU_Init();					    			 //=====初始化MPU6050
	mpu_dmp_init();								 //=====初始化MPU6050的DMP模式					 
	TIM1_PWM_Init(999,359);   			 //=====初始化PWM 10KHZ,用于驱动电机
	//MPU6050_EXTI_Init();					 //=====MPU6050 5ms定时中断初始化

//  使用公式的时候再使用
//	angle = 180/pai;
//	rad = pai/180;
//	d = l+h;
//	l2=l*l;
//	d2=d*d;
//	ld=l*d;
	
	OLED_ShowString(0,0,"MODE:",12);
	
	switch (KEY_Scan())
	{
		case 1:
			KEY_MODE =1;
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			OLED_ShowString(90,0,"111111",12);
			break;
		case 2:
			KEY_MODE =2;
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			break;
		case 3:
			KEY_MODE =3;
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			break;
		default:
			break;
	}
	
	OLED_ShowString(0,2,"Pitch:",12);
	OLED_ShowString(0,4,"Roll :",12);
	OLED_ShowString(0,6,"Yaw  :",12);
	
  while(1)	
	{
		/*方便我们查看数据的变化,查看姿态角;如果需要看加速度以及角速度可自行修改***/
		mpu_dmp_get_data(&pitch,&roll,&yaw);			//得到姿态角即欧拉角
		
		switch (KEY_MODE)
		{
			case 1: 
				pulse1 = 300-(20.0/9.0)*pitch;	

				TIM_SetCompare1(TIM1,pulse1);
				break;
			case 2:
				pitch1 = (int)pitch;
				pulse1 = 300-(20.0/9.0)*data[pitch1+60];	//初始为0
				TIM_SetCompare1(TIM1,pulse1);			
				break;
			case 3:
				x = 60;
				pulse1 = 300-(20.0/9.0)*x;
				pulse4 = 300-(20.0/9.0)*y;
				while(1)
				{
					//舵机1
					pulse1 = 300-(20.0/9.0)*x;
					if(x==-60)
						flag1 = 1;
					if(x==-60)
						flag1 = -1;
					x -= 2*flag1;
					
					//舵机2
					pulse4 = 300-(20.0/9.0)*y;
					if(y == 60)
						flag2 = -1;
					if(y == -60)
						flag2 = 1;
					y += 2*flag2;
					
					
					TIM_SetCompare1(TIM1,pulse1);
					TIM_SetCompare4(TIM1,pulse4);
				}
			default:
				break;
		}
			

		

	}
}
