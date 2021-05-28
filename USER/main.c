 /**************************************************************************
 ��  �� ���������
�Ա���ַ��https://shop119207236.taobao.com
**************************************************************************/
#include "sys.h"
/****************************ȫ�ֱ���*************************************/    
float Voltage;  															 //��ص�ѹ������صı���
float pitch,roll,yaw; 								  			 //ŷ����(��̬��)
short aacx,aacy,aacz;													 //���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;											 //������ԭʼ����
float UltrasonicWave_Distance;                 //���������
int   Encoder_Left,Encoder_Right;         		 //���ұ��������������
int 	Moto1=0,Moto2=0;												 //������������ո��������PWM
int 	Flag_Qian,Flag_Hou,Flag_Left,Flag_Right; //����ң����صı���
int		pulse1,pulse4;
u8    flag_UltrasonicWave=0;									 //�������Ƿ񳬳���ȫ�����־λ
u8    flag_fall=0;					 									 //ˤ����־λ
u8    key=0;								 									 //�����ļ�ֵ
u8    KEY_MODE=0;					 										 //ģʽ0,��ʾ����===HELLO BLIZ
																							 //ģʽ1,����ģʽ
																							 //ģʽ2,����ģʽ
																							 //ģʽ3,����ģʽ
u8 DIS_STATE=255;				 											 //����ʵ��ֻˢһ����Ļ�ı���			


int mode = 2;
#define pai 3.1415926

//char a[20]="";
//int pitch1,pitch2;;
//int j=0;
/***********************************************************************/

/****************************��λ������*********************************/   
unsigned char i;          										 //��������
unsigned char Send_Count; 										 //������Ҫ���͵����ݸ���
/**********************************************************************/

int main(void)	
{ 
	int pitch1,x=60,y=0,flag1 = 1,flag2 = 1;
//	float l = 119,h = 15,alf,rad,angle,d,d2,l2,ld;
	float data[61]={-87.85,
-86.76,
-85.62,
-84.44,
-83.21,
-81.91,
-80.55,
-79.12,
-77.6,
-75.99,
-74.27,
-72.42,
-70.44,
-68.28,
-65.94,
-63.38,
-60.56,
-57.45,
-53.98,
-50.11,
-45.78,
-40.92,
-35.49,
-29.44,
-22.77,
-15.55,
-7.89,
0,
7.89,
15.55,
22.77,
29.44,
35.49,
40.92,
45.78,
50.11,
53.98,
57.45,
60.56,
63.38,
65.94,
68.28,
70.44,
72.42,
74.27,
75.99,
77.6,
79.12,
80.55,
81.91,
83.21,
84.44,
85.62,
86.76,
87.85,
88.9,
89.92,
89.09,
88.13,
87.2,
86.29
};
	

	KEY_Init();                    //=====��ʼ���밴�����ӵ�IO
	LED_Init();
	delay_init();	    	           //=====��ʱ������ʼ��	

	NVIC_Configuration();					 //=====�ж����ȼ�����,���а��������е��ж����ȼ�������,��������һ�����޸ġ�
	Adc_Init();                    //=====��ʼ��ADC
	OLED_Init();                   //=====OLED��ʼ��
	OLED_Clear();									 //=====OLED����
	MPU_Init();					    			 //=====��ʼ��MPU6050
	mpu_dmp_init();								 //=====��ʼ��MPU6050��DMPģʽ					 
	TIM1_PWM_Init(999,359);   			 //=====��ʼ��PWM 10KHZ,�����������
	//MPU6050_EXTI_Init();					 //=====MPU6050 5ms��ʱ�жϳ�ʼ��

//  ʹ�ù�ʽ��ʱ����ʹ��
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
			OLED_ShowString(90,0," 1 ",12);
			break;
		case 2:
			KEY_MODE =2;
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			OLED_ShowString(90,0," 2 ",12);
			break;
		case 3:
			KEY_MODE =3;
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			OLED_ShowString(90,0," 3 ",12);
			break;
		default:
			break;
	}
	
//	OLED_ShowString(0,2,"Pitch:",12);
//	OLED_ShowString(0,4,"Roll :",12);
//	OLED_ShowString(0,6,"Yaw  :",12);
	
  while(1)	
	{
		/*�������ǲ鿴���ݵı仯,�鿴��̬��;�����Ҫ�����ٶ��Լ����ٶȿ������޸�***/
		mpu_dmp_get_data(&pitch,&roll,&yaw);			//�õ���̬�Ǽ�ŷ����
		
		switch (KEY_MODE)
		{
			case 1: 
				pulse1 = 300-(20.0/9.0)*pitch;	

				TIM_SetCompare1(TIM1,pulse1);
				break;
			case 2:
				pitch1 = (int)(pitch);
				pulse1 = 300-(20.0/9.0)*data[pitch1+30];	//��ʼΪ0
				TIM_SetCompare1(TIM1,pulse1);			
				break;
			case 3:
				x = 60;
				pulse1 = 300-(20.0/9.0)*x;
				pulse4 = 300-(20.0/9.0)*y;
				while(1)
				{
					//���1
					pulse1 = 300-(20.0/9.0)*x;
					if(x==-60)
						flag1 = 1;
					if(x==-60)
						flag1 = -1;
					x -= 2*flag1;
					
					//���2
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
