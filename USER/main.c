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
	float l = 104,h = 30,alf,rad,angle,d,d2,l2,ld;
	float data[55]={-69.72,
-68.04,
-66.29,
-64.44,
-62.51,
-60.49,
-58.35,
-56.11,
-53.75,
-51.25,
-48.61,
-45.82,
-42.88,
-39.76,
-36.48,
-33.02,
-29.38,
-25.58,
-21.60,
-17.48,
-13.24,
-8.88,
-4.46,
0.00,
4.46,
8.88,
13.24,
17.48,
21.60,
25.58,
29.38,
33.02,
36.48,
39.76,
42.88,
45.82,
48.61,
51.25,
53.75,
56.11,
58.35,
60.49,
62.51,
64.44,
66.28,
68.04,
69.72,
71.33,
72.88,
74.37,
75.80,
77.18,
78.52,
};
	
float data1[80]={9.46,
9.43,
9.32,
9.15,
8.9,
8.59,
8.21,
7.77,
7.28,
6.72,
6.11,
5.46,
4.76,
4.03,
3.26,
2.47,
1.66,
0.83,
0,
-0.83,
-1.66,
-2.47,
-3.26,
-4.03,
-4.76,
-5.46,
-6.11,
-6.72,
-7.28,
-7.77,
-8.21,
-8.59,
-8.9,
-9.15,
-9.32,
-9.43,
-9.46,
-9.43,
-9.32,
-9.15,
-8.9,
-8.59,
-8.21,
-7.77,
-7.28,
-6.72,
-6.11,
-5.46,
-4.76,
-4.03,
-3.26,
-2.47,
-1.66,
-0.83,
0,
0.83,
1.66,
2.47,
3.26,
4.03,
4.76,
5.46,
6.11,
6.72,
7.28,
7.77,
8.21,
8.59,
8.9,
9.15,
9.32,
9.43
};
float data2[80]={0,
0.82,
1.64,
2.44,
3.22,
3.98,
4.72,
5.41,
6.07,
6.68,
7.23,
7.74,
8.19,
8.57,
8.89,
9.14,
9.32,
9.43,
9.46,
9.43,
9.32,
9.14,
8.89,
8.57,
8.19,
7.74,
7.23,
6.68,
6.07,
5.41,
4.72,
3.98,
3.22,
2.44,
1.64,
0.82,
0,
-0.82,
-1.64,
-2.44,
-3.22,
-3.98,
-4.72,
-5.41,
-6.07,
-6.68,
-7.23,
-7.74,
-8.19,
-8.57,
-8.89,
-9.14,
-9.32,
-9.43,
-9.46,
-9.43,
-9.32,
-9.14,
-8.89,
-8.57,
-8.19,
-7.74,
-7.23,
-6.68,
-6.07,
-5.41,
-4.72,
-3.98,
-3.22,
-2.44,
-1.64,
-0.82
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

  
	angle = 180/pai;
	rad = pai/180;
	d = l+h;
	l2=l*l;
	d2=d*d;
	ld=l*d;
	
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
				pulse1 = 300-(20.0/9.0)*data[pitch1+26];	
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
			
				break;
			default:
				break;
		}
			

		

	}
}