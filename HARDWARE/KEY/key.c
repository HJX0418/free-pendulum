#include "key.h"
/**************************************************************************
 ��  �� ���������
�Ա���ַ��https://shop119207236.taobao.com
**************************************************************************/
/**************************************************************************
�������ܣ�������ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void KEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_1;	           //�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
} 
/**************************************************************************
�������ܣ�����ɨ��
��ڲ�������
����  ֵ������״̬ 0���޶��� 1������ 
**************************************************************************/
int KEY_Scan(void)
{
	int key;
	while(1)
	{
		if(!(KEY&&KEY1&&KEY2))
		{
			delay_ms(5);					//����
			if(!(KEY&&KEY1&&KEY2)&&(2 == KEY+KEY1+KEY2))	//ֻ��һ��������ʱ��Ž���
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
