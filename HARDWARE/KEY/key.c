#include "key.h"
/**************************************************************************
 作  者 ：大鱼电子
淘宝地址：https://shop119207236.taobao.com
**************************************************************************/
/**************************************************************************
函数功能：按键初始化
入口参数：无
返回  值：无 
**************************************************************************/
void KEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_1;	           //端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
} 
/**************************************************************************
函数功能：按键扫描
入口参数：无
返回  值：按键状态 0：无动作 1：单击 
**************************************************************************/
int KEY_Scan(void)
{
	int key;
	while(1)
	{
		if(!(KEY&&KEY1&&KEY2))
		{
			delay_ms(5);					//消抖
			if(!(KEY&&KEY1&&KEY2)&&(2 == KEY+KEY1+KEY2))	//只有一个按键的时候才进入
			{
				if(!KEY)
					key = 1;
				else if(!KEY1)
					key = 2;
				else if(!KEY2)
					key = 3;
				while(!(KEY&&KEY1&&KEY2));
				break;
			}			
		}
	}
	return key;
}
