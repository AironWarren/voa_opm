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
*		DAC WRITE 16 bit spi4 cs pin E4, clr pin E3 
*/
//----------------------------------------------------------------------------------------------------
uint8_t value_dac[2];
extern int index_1;	
extern const uint16_t voa_1310[301];
extern const uint16_t voa_1550[301];
//----------------------------------------------------------------------------------------------------
void dac_write(float dBm, uint8_t lenght) //настраиваем уровень затухания
{
//----------------------------------------------------------------------------------------------------
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);												// cs down
	index_1=((int)(dBm*10)); //переводим значение dBm в индекс, для поиска табличного значения
	switch(lenght)
	{
		case 0x31: //1310
		{
			value_dac[0]=(voa_1310[index_1]&0xFF00)>>8;
			value_dac[1]=voa_1310[index_1]&0x00FF;
			break;
		}
		case 0x32: //1550
		{
			value_dac[0]=(voa_1550[index_1]&0xFF00)>>8;
			value_dac[1]=voa_1550[index_1]&0x00FF;
			break;
		}	
		default:break;
	}

  HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC (отправляем знаяение в DAC для настройки затухания)
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);												// cs up
}