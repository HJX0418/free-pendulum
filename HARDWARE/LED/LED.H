#ifndef __LED_H
#define __LED_H
#include "sys.h"
 /**************************************************************************
 ��  �� ���������
�Ա���ַ��https://shop119207236.taobao.com
**************************************************************************/
//LED �˿ڶ���
#define LED PCout(14) // PC14
void LED_Init(void);  //��ʼ��
void Led_Flash(u16 time);
#endif