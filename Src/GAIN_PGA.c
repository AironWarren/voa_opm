#include "main.h"
//----------------------------------------------------------------------------------------------------
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "usbd_cdc_if.h"
//----------------------------------------------------------------------------------------------------

extern uint8_t gain;
extern uint8_t value_pga_112[2];


void gain_1() //установка усиления с помошью первого усилительного каскада 
{
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_13,1);				//M1_ENN
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,1);				//M1_A0
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_15,0);				//M1_A1
	gain=0x31;
}

void gain_2()
{
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_13,1);				//M1_ENN
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,0);				//M1_A0
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_15,1);				//M1_A1
	gain=0x32;
}

void gain_3()
{
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_13,1);				//M1_ENN
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,1);				//M1_A0
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_15,1);				//M1_A1
	gain=0x33;
}


void gain_5()
{
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_13,1);				//M1_ENN
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,0);				//M1_A0
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_15,0);				//M1_A1
	gain=0x35;
}

void gain_4() //тут на выход вообще ничего не проходит 
{
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_13,0);				//M1_ENN
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,0);				//M1_A0
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_15,0);				//M1_A1
	gain=0x34;
}

/*
*		PGA112-Drift Programmable Gain Amplifier for CH1
*		GAIN 1 - 0x01		(1)
*		GAIN 2 - 0x11		(17) 
*		GAIN 4 - 0x21		(33)
*		GAIN 8 - 0x31		(49)
*		GAIN 16 - 041		(65)
*		GAIN 32 - 0x51	(81)
*   GAIN 64 - 0x61	(97)
*		GAIN 128 - 0x71	(113)
*/
void PGA112 (char value) //устанавление усиления с помошью программируемого усилителя
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);									// cs down (сигнал NS)
	value_pga_112[0] = 0x2A;																// command to write (команда записи)
	value_pga_112[1] = value;																// value gain (уровень)
  HAL_SPI_Transmit_IT(&hspi1, value_pga_112, 2);					// write data in PGA112 (отправляем уровень)
	HAL_Delay(50);																					//ждем 50 мс
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,1);									// cs up (закрываем канал общени)
}
