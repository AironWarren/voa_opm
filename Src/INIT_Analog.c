#include "main.h"
//----------------------------------------------------------------------------------------------------
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "usbd_cdc_if.h"
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
/*
*		Init Analog
*/
//----------------------------------------------------------------------------------------------------
void Init_Analog() //��������� ������, ������ ������ � ������ ���������� ������� � ����� ������ ��������� 
{
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_13,1);				//M1_ENN ���������� ��������������� � ����� ��������� ������ (����� �������������� ����� ���������, ����� ADC)
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,1);				//M1_A0
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_15,0);				//M1_A1
	PGA112(1); //��������� ����������������-��������� (��������� ��� gain)
	channel_1(); //����� ����������� ����� � ������� ����������� �����
	gain_1(); //����� gain � ��������� �������������� �� �����-�� �����
}
//----------------------------------------------------------------------------------------------------