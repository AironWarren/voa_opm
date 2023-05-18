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
void channel_1() //����� 1-�� ����������� ������
{
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,0);				//FT1_A1 ������ � ������� �������� ��������� ������ �� ���������� ����
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,1);				//FT1_A8
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,0);				//FT1_B1
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,1);				//FT1_B8
}
//----------------------------------------------------------------------------------------------------
void channel_2() //����� 2-�� ����������� ������
{
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,1);				//FT1_A1
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,0);				//FT1_A8
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,0);				//FT1_B1
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,1);				//FT1_B8
}
//----------------------------------------------------------------------------------------------------
void channel_3() //����� 3-�� ����������� ������
{
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,0);				//FT1_A1
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,1);				//FT1_A8
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,1);				//FT1_B1
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,0);				//FT1_B8
}
//----------------------------------------------------------------------------------------------------
void channel_4() //����� 4-�� ����������� ������
{
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,1);				//FT1_A1
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,0);				//FT1_A8
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,1);				//FT1_B1
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,0);				//FT1_B8
}
//----------------------------------------------------------------------------------------------------