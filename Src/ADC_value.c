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
extern uint16_t value;
extern uint8_t value_adc[2];
//----------------------------------------------------------------------------------------------------
/*
*		ADC READ 16 bit spi2 cs pin I0, return 16 bit value ADC
*/
//----------------------------------------------------------------------------------------------------
uint16_t adc_read (void)
{
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,0);						  					// CS down (разрешаем работй SPI с микросхемой ADC)
  HAL_SPI_Receive_IT(&hspi2, (uint8_t*) value_adc, 2);					// read adc
	HAL_Delay(50);																								// задержка
  value = ((value_adc[0] & 0xFF) << 8) | (value_adc[1] & 0xFF);	// convertation data
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,1);												// CS up (закрываем работу SPI)
	return value;	
}
//----------------------------------------------------------------------------------------------------