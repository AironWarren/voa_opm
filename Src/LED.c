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
void enablyChannel(uint8_t number_channel)
{
//----------------------------------------------------------------------------------------------------
	switch(number_channel){
		case 0x31:
		{
			//channel_1(); //выбор оптического канала
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,1); //светодиодная индикация
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,1);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,1);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,0);
			break;
		}
		case 0x32:
		{
			//channel_2();
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,1);	
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,0);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,1);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,1);
			break;
		}
		case 0x33:
		{
			//channel_3();
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,1);	
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,1);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,0);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,1);
			break;
		}
		case 0x34:
		{
			//channel_4();
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,0);	
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,1);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,1);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,1);
			break;
		}
	}
}