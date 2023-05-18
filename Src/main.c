/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2021 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
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
/* USER CODE BEGIN Includes */
#define rx_buffer_size 500
#define MYADDR_1310 0x08040000
#define MYADDR_1550 0x08080000
#define MYADDR_COEFF_1310 0x08018002
#define MYADDR_COEFF_1550 0x08020000
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
//----------------------------------------------------------------------------------------------------	
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
//----------------------------------------------------------------------------------------------------
void Init_Analog();
//----------------------------------------------------------------------------------------------------
int index_1=0;	

float dBm_in=-60;
float dBm_out=-60;

float lt6306=0;
float lt6306_temp=-60.0;

uint8_t wavelength_1310_cal = 0x30;
uint8_t wavelength_1550_cal = 0x30;

uint16_t rx_buffer_len;
char str_rx[rx_buffer_size]={0,};

uint8_t firstByteWait=1;

uint8_t str_in_tx[15];
uint8_t str_out_tx[15];
uint8_t str_error[7];

uint8_t value_pga_112[2];
//uint8_t value_dac[2];
uint16_t value;
uint8_t value_adc[2];

uint16_t adc_value=0;
uint32_t adc_value_32=0;
uint16_t dac_data=0;

uint8_t pga112_data=0;
uint8_t calib = 0x01;

//float dBm = 0.0;
float dBm[4] = {0.0, 0.0, 0.0, 0.0};
float mV;
uint8_t status_key;
uint8_t status_key_1;
uint8_t N_Rez;
//uint8_t number_channel = 0x31;
//uint8_t lenght = 0x31;
//uint8_t gain = 0x31;

uint8_t test_gein = 0;

uint8_t number_channel[4] = {0x30,0x30,0x30,0x30};
uint8_t lenght[4] = {0x31,0x31,0x31,0x31};

uint8_t number_channel_1 = 0x30;
uint8_t lenght_1 = 0x30;
float dBm_1 = 0.0;

uint8_t dBm_in_tx[5];
uint8_t dBm_out_tx[5];
uint16_t temp;
//uint8_t gainPGA = 1;
uint8_t count=0;
extern uint8_t gainPGA;
uint16_t mom = 0;

uint16_t voa_1310[301];// = {0xD70F, 0xD4EF, 0xD33D, 0xD1BF, 0xD065, 0xCF2F, 0xCE0F, 0xCD09, 0xCC0B, 0xCB1F, 0xCA2F, 0xC95D, 0xC881, 0xC7BD, 0xC6F3, 0xC63D, 0xC57F, 0xC4CF, 0xC41F, 0xC37D, 0xC2D9, 0xC23D, 0xC19F, 0xC06F, 0xBFDD, 0xBF4D, 0xBEBD, 0xBE2F, 0xBDAD, 0xBD1F, 0xBC9F, 0xBC1F, 0xBB9F, 0xBB1F, 0xBA9F, 0xBA2D, 0xB9AF, 0xB93F, 0xB8CD, 0xB85D, 0xB7ED, 0xB77D, 0xB70F, 0xB69F, 0xB639, 0xB5CF, 0xB56D, 0xB4FF, 0xB49D, 0xB42F, 0xB3CD, 0xB36D, 0xB30D, 0xB2AD, 0xB24D, 0xB1ED, 0xB18D, 0xB12F, 0xB0CF, 0xB06F, 0xB00F, 0xAFBD, 0xAF5F, 0xAF0D, 0xAEAF, 0xAE5D, 0xADFF, 0xADAD, 0xAD4F, 0xACFD, 0xACAB, 0xAC51, 0xAC05, 0xABAF, 0xAB5F, 0xAB0F, 0xAABF, 0xAA6F, 0xAA1F, 0xA9CF, 0xA97F, 0xA92F, 0xA8DF, 0xA88F, 0xA83F, 0xA7EF, 0xA79F, 0xA74F, 0xA6FF, 0xA6AF, 0xA66D, 0xA61F, 0xA5DD, 0xA5B0, 0xA58F, 0xA53F, 0xA4FD, 0xA4AF, 0xA46D, 0xA41F, 0xA3DD, 0xA39D, 0xA34F, 0xA30D, 0xA2CB, 0xA287, 0xA24B, 0xA201, 0xA1C7, 0xA189, 0xA14D, 0xA119, 0xA0C7, 0xA087, 0xA047, 0xA007, 0x9FC7, 0x9F87, 0x9F47, 0x9F07, 0x9ED3, 0x9E97, 0x9E57, 0x9E17, 0x9DD7, 0x9D97, 0x9D63, 0x9D27, 0x9CF5, 0x9CB7, 0x9C87, 0x9C85, 0x9C47, 0x9C07, 0x9BC7, 0x9B95, 0x9B57, 0x9B25, 0x9AE7, 0x9AB5, 0x9A77, 0x9A43, 0x9A07, 0x99D5, 0x9997, 0x9965, 0x9927, 0x98F5, 0x98B7, 0x9885, 0x9847, 0x9815, 0x97E5, 0x97A7, 0x9775, 0x9737, 0x9705, 0x96C7, 0x9695, 0x9657, 0x9625, 0x95E7, 0x95B7, 0x9587, 0x9555, 0x9525, 0x94EF, 0x94BF, 0x9497, 0x9495, 0x9465, 0x9435, 0x93F7, 0x93C7, 0x9395, 0x9365, 0x9335, 0x9305, 0x92D5, 0x92A5, 0x9275, 0x9237, 0x9207, 0x91D7, 0x91A7, 0x9177, 0x9147, 0x9115, 0x90E5, 0x90B5, 0x9085, 0x9055, 0x9025, 0x8FF5, 0x8FC5, 0x8F97, 0x8F67, 0x8F37, 0x8F07, 0x8ED7, 0x8EA7, 0x8E77, 0x8E47, 0x8E17, 0x8DE7, 0x8DB7, 0x8D87, 0x8D57, 0x8D27, 0x8CF7, 0x8CC7, 0x8C97, 0x8C67, 0x8C37, 0x8C07, 0x8BD7, 0x8BA7, 0x8B77, 0x8B47, 0x8B17, 0x8AED, 0x8AC5, 0x8A97, 0x8A67, 0x8A37, 0x8A07, 0x89D7, 0x89A7, 0x8977, 0x8947, 0x8917, 0x88E7, 0x88B7, 0x8895, 0x8865, 0x8837, 0x8807, 0x87D7, 0x87A7, 0x8785, 0x8757, 0x8727, 0x86F5, 0x86C5, 0x8695, 0x8667, 0x8637, 0x8607, 0x85D7, 0x85A9, 0x8581, 0x8555, 0x8527, 0x8505, 0x84D5, 0x84A7, 0x8477, 0x8445, 0x8417, 0x83F3, 0x83C1, 0x838F, 0x8361, 0x8335, 0x8305, 0x82D1, 0x82A3, 0x8273, 0x824D, 0x8225, 0x81EB, 0x81BD, 0x8187, 0x8157, 0x8137, 0x8105, 0x80D3, 0x80A3, 0x8071, 0x8041, 0x8007, 0x7FE1, 0x7FB1, 0x7F85, 0x7F47, 0x7F1D, 0x7EF3, 0x7ECF, 0x7E95, 0x7E5D, 0x7E2F, 0x7DF3, 0x7DC1, 0x7D93, 0x7D65, 0x7D3B, 0x7D05, 0x7CD5, 0x7CA3, 0x7C75, 0x7C45};
//uint16_t voa_1310[301];
uint16_t voa_1550[301];// = {0xDD9B, 0xDBAD, 0xD9FD, 0xD87F, 0xD71F, 0xD5DD, 0xD4AD, 0xD39B, 0xD289, 0xD18D, 0xD09F, 0xCFB1, 0xCED5, 0xCDFD, 0xCD2D, 0xCC5F, 0xCB9D, 0xCADF, 0xCA1F, 0xC96D, 0xC8BB, 0xC80F, 0xC76B, 0xC61F, 0xC58D, 0xC4EB, 0xC44F, 0xC3BD, 0xC32B, 0xC29F, 0xC20F, 0xC17F, 0xC0F9, 0xC06F, 0xBFED, 0xBF5F, 0xBEDF, 0xBE5F, 0xBDDF, 0xBD5F, 0xBCDF, 0xBC65, 0xBBEF, 0xBB7D, 0xBB0D, 0xBA9D, 0xBA2B, 0xB9AF, 0xB93F, 0xB93D, 0xB907, 0xB8A5, 0xB845, 0xB7E7, 0xB785, 0xB725, 0xB6C5, 0xB667, 0xB607, 0xB5A7, 0xB547, 0xB4E7, 0xB491, 0xB437, 0xB3DF, 0xB385, 0xB327, 0xB2D5, 0xB277, 0xB225, 0xB1C7, 0xB175, 0xB117, 0xB0C7, 0xB073, 0xB01F, 0xAFC7, 0xAF77, 0xAF67, 0xAF1D, 0xAECD, 0xAE7D, 0xAE2D, 0xADDD, 0xAD8D, 0xAD3D, 0xACED, 0xAC9D, 0xAC4F, 0xABFF, 0xABAF, 0xAB5F, 0xAB0F, 0xAAEA, 0xAAC3, 0xAA7F, 0xAA2F, 0xA9DF, 0xA98F, 0xA949, 0xA8FF, 0xA8AF, 0xA869, 0xA81F, 0xA7CF, 0xA77F, 0xA735, 0xA6EF, 0xA6AD, 0xA65F, 0xA61D, 0xA5CF, 0xA58D, 0xA53F, 0xA4FD, 0xA4AF, 0xA46D, 0xA41F, 0xA3DD, 0xA38F, 0xA34D, 0xA2FF, 0xA2BF, 0xA27D, 0xA23B, 0xA1EF, 0xA1AD, 0xA16B, 0xA11F, 0xA0DD, 0xA0CF, 0xA0CD, 0xA089, 0xA03F, 0x9FFD, 0x9FBB, 0x9F79, 0x9F2F, 0x9EEF, 0x9EAD, 0x9E69, 0x9E1F, 0x9DDF, 0x9D9D, 0x9D5D, 0x9D0F, 0x9CCF, 0x9C8D, 0x9C4D, 0x9C0D, 0x9BCD, 0x9B7F, 0x9B3F, 0x9AFF, 0x9ABD, 0x9A6F, 0x9A2F, 0x99EF, 0x99AF, 0x996F, 0x992D, 0x98ED, 0x98A7, 0x986D, 0x982D, 0x97ED, 0x97AD, 0x976B, 0x972D, 0x96DF, 0x96DD, 0x969D, 0x965D, 0x961D, 0x95DD, 0x959D, 0x955D, 0x9515, 0x94DB, 0x949D, 0x945D, 0x941D, 0x93DD, 0x939D, 0x935D, 0x931D, 0x92DD, 0x929D, 0x925D, 0x921D, 0x91DD, 0x919D, 0x915D, 0x911D, 0x90DD, 0x909D, 0x905B, 0x901D, 0x8FDD, 0x8F9D, 0x8F5D, 0x8F19, 0x8EDF, 0x8E9F, 0x8E5F, 0x8E1F, 0x8DDF, 0x8D9F, 0x8D5F, 0x8D1F, 0x8CDF, 0x8C9D, 0x8C5D, 0x8C1D, 0x8BDD, 0x8B9D, 0x8B5D, 0x8B1D, 0x8ADD, 0x8A9D, 0x8A5D, 0x8A1D, 0x89DD, 0x899D, 0x895F, 0x891F, 0x88DF, 0x889F, 0x885F, 0x881F, 0x87DF, 0x879F, 0x875F, 0x871F, 0x86DB, 0x8699, 0x865F, 0x861D, 0x85DF, 0x85A9, 0x856D, 0x852D, 0x84ED, 0x84AD, 0x846F, 0x842F, 0x83EF, 0x83BB, 0x8373, 0x833D, 0x82FD, 0x82BF, 0x827F, 0x823F, 0x81FF, 0x81CD, 0x818D, 0x814D, 0x810F, 0x80CF, 0x808D, 0x804F, 0x800D, 0x7FDD, 0x7F9B, 0x7F59, 0x7F1F, 0x7EE9, 0x7EAB, 0x7E6B, 0x7E2D, 0x7DF1, 0x7DAF, 0x7D7B, 0x7D3D, 0x7CFB, 0x7CBD, 0x7C8D, 0x7C4D, 0x7C0D, 0x7BCB, 0x7B79, 0x7B3D, 0x7AFD, 0x7ABD, 0x7A8B, 0x7A4D, 0x79FF, 0x79CF, 0x797D, 0x7947, 0x790D, 0x78D7, 0x789F, 0x7869, 0x782D, 0x77EF, 0x77A9, 0x776B, 0x7729, 0x76EF};

uint8_t Len = 0x30;
uint8_t Ch = 0x30;
uint8_t in_or_out = 0x30;
float Cof = 0;
uint8_t str_tx_coeff[87];
uint8_t str_rez_coeff[40];

uint8_t Coeff_in_1310_1_gui[20]; //было 4
uint8_t Coeff_in_1310_2_gui[20];
uint8_t Coeff_in_1310_3_gui[20];
uint8_t Coeff_in_1310_4_gui[20];

uint8_t Coeff_in_1550_1_gui[20];
uint8_t Coeff_in_1550_2_gui[20];
uint8_t Coeff_in_1550_3_gui[20];
uint8_t Coeff_in_1550_4_gui[20];

uint8_t Coeff_out_1310_1_gui[20]; //было 4
uint8_t Coeff_out_1310_2_gui[20];
uint8_t Coeff_out_1310_3_gui[20];
uint8_t Coeff_out_1310_4_gui[20];

uint8_t Coeff_out_1550_1_gui[20];
uint8_t Coeff_out_1550_2_gui[20];
uint8_t Coeff_out_1550_3_gui[20];
uint8_t Coeff_out_1550_4_gui[20];

uint8_t Coeff_REZ_0_gui[5];
uint8_t Coeff_REZ_1_gui[5];
uint8_t Coeff_REZ_2_gui[5];
uint8_t Coeff_REZ_3_gui[5];


/*float Coeff_in_1310_1;
float Coeff_out_1310_1;
float Coeff_in_1310_2;
float Coeff_out_1310_2;
float Coeff_in_1310_3;
float Coeff_out_1310_3;
float Coeff_in_1310_4;
float Coeff_out_1310_4;*/

float Coeff_1310[8];

/*float Coeff_in_1550_1;
float Coeff_out_1550_1;
float Coeff_in_1550_2;
float Coeff_out_1550_2;
float Coeff_in_1550_3;
float Coeff_out_1550_3;
float Coeff_in_1550_4;
float Coeff_out_1550_4;*/

float Coeff_1550[8];

float Coeff_REZ[4];

uint16_t Coeff_1310_test = 0;

uint16_t Coeff_1550_test = 0;

uint16_t Coeff_Rez_test = 0;

uint8_t calib;

uint8_t mode = 0x30;

float Out_AOP = 0.0; 

float delta = 0;

extern HAL_UART_StateTypeDef status;

static FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t page_error = 0;
int main(void)
 {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI4_Init();
  MX_SPI5_Init();
  MX_USART2_UART_Init();
	
	int addres_1310 = 0;
	int addres_1550 = 0;
	int addres_COEFF_1310 = 0;
	int addres_COEFF_1550 = 0;
	int addres_COEFF_REZ = 0;

	addres_1310 = MYADDR_1310;
	addres_1550 = MYADDR_1550;
	addres_COEFF_1310 = MYADDR_COEFF_1310;
	addres_COEFF_1550 = MYADDR_COEFF_1550;
	
	/*
	HAL_FLASH_Unlock();
	
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector = 5;
	EraseInitStruct.NbSectors = 1;
	HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);

	for(uint16_t i = 0; i<301; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_1310,voa_1310[i]);
		addres_1310 = addres_1310 +2;
	}
	
	HAL_FLASH_Lock();
	*/
	
	//======================================================================================
	for(uint16_t i = 0; i<301; i ++)
	{
		voa_1310[i] = *(uint16_t*)addres_1310;
		addres_1310 = addres_1310 + 2;
	}
	
	for(uint16_t i = 0; i<301; i ++)
	{
		voa_1550[i] = *(uint16_t*)addres_1550;
		addres_1550 = addres_1550 + 2;
	}
	
	//======================================================================================
	
	for(uint8_t i = 0; i < 8; i++)
	{
		Coeff_1310_test = (*(uint16_t*)addres_COEFF_1310);
		HAL_Delay(10);
		if((Coeff_1310_test & 0x8000) == 0x8000)
		{
			Coeff_1310_test = (~Coeff_1310_test);
			Coeff_1310[i] = ((float)Coeff_1310_test)/(-10);
		}
		else
			Coeff_1310[i] = (float)(Coeff_1310_test)/(10);
		
		addres_COEFF_1310 = addres_COEFF_1310 + 2;
	}
	
	for(uint8_t i = 0; i < 8; i++)
	{
		Coeff_1550_test = (*(uint16_t*)addres_COEFF_1550);
		HAL_Delay(10);
		if((Coeff_1550_test & 0x8000) == 0x8000)
		{
			Coeff_1550_test = (~Coeff_1550_test);
			Coeff_1550[i] = ((float)Coeff_1550_test)/(-10);
		}
		else
			Coeff_1550[i] = (float)(Coeff_1550_test)/(10);
		
		addres_COEFF_1550 = addres_COEFF_1550 + 2;
	}

	for(uint8_t i = 0; i < 4; i++)
	{
		Coeff_Rez_test = (*(uint16_t*)addres_COEFF_1550);
		HAL_Delay(10);
		Coeff_REZ[i] = (float)(Coeff_Rez_test)/(10);
		addres_COEFF_1550 = addres_COEFF_1550 + 2;
	}

	//======================================================================================
	/*
	addres_1310 = 0;
	addres_1550 = 0;
	addres_COEFF_1310 = 0;
	addres_COEFF_1550 = 0;

	addres_1310 = MYADDR_1310;
	addres_1550 = MYADDR_1550;
	addres_COEFF_1310 = MYADDR_COEFF_1310;
	addres_COEFF_1550 = MYADDR_COEFF_1550;
	
	Coeff_in_1310_1 = (float)(*(uint16_t*)addres_COEFF_1310)/10;
	addres_COEFF_1310 = addres_COEFF_1310 + 2;
	Coeff_out_1310_1 = (float)(*(uint16_t*)addres_COEFF_1310)/10;
	addres_COEFF_1310 = addres_COEFF_1310 + 2;
	
	Coeff_in_1310_2 = (float)(*(uint16_t*)addres_COEFF_1310)/10;
	addres_COEFF_1310 = addres_COEFF_1310 + 2;
	Coeff_out_1310_2 = (float)(*(uint16_t*)addres_COEFF_1310)/10;
	addres_COEFF_1310 = addres_COEFF_1310 + 2;
	
	Coeff_in_1310_3 = (float)(*(uint16_t*)addres_COEFF_1310)/10;
	addres_COEFF_1310 = addres_COEFF_1310 + 2;
	Coeff_out_1310_3 = (float)(*(uint16_t*)addres_COEFF_1310)/10;
	addres_COEFF_1310 = addres_COEFF_1310 + 2;
	
	Coeff_in_1310_4 = (float)(*(uint16_t*)addres_COEFF_1310)/10;
	addres_COEFF_1310 = addres_COEFF_1310 + 2;
	Coeff_out_1310_4 = (float)(*(uint16_t*)addres_COEFF_1310)/10;

	
	Coeff_in_1550_1 = (float)(*(uint16_t*)addres_COEFF_1550)/10;
	addres_COEFF_1550 = addres_COEFF_1550 + 2;
	Coeff_out_1550_1 = (float)(*(uint16_t*)addres_COEFF_1550)/10;
	addres_COEFF_1550 = addres_COEFF_1550 + 2;
	
	Coeff_in_1550_2 = (float)(*(uint16_t*)addres_COEFF_1550)/10;
	addres_COEFF_1550 = addres_COEFF_1550 + 2;
	Coeff_out_1550_2 = (float)(*(uint16_t*)addres_COEFF_1550)/10;
	addres_COEFF_1550 = addres_COEFF_1550 + 2;
	
	Coeff_in_1550_3 = (float)(*(uint16_t*)addres_COEFF_1550)/10;
	addres_COEFF_1550 = addres_COEFF_1550 + 2;
	Coeff_out_1550_3 = (float)(*(uint16_t*)addres_COEFF_1550)/10;
	addres_COEFF_1550 = addres_COEFF_1550 + 2;
	
	Coeff_in_1550_4 = (float)(*(uint16_t*)addres_COEFF_1550)/10;
	addres_COEFF_1550 = addres_COEFF_1550 + 2;
	Coeff_out_1550_4 = (float)(*(uint16_t*)addres_COEFF_1550)/10;
	*/
	addres_1310 = 0;
	addres_1550 = 0;
	addres_COEFF_1310 = 0;
	addres_COEFF_1550 = 0;

	addres_1310 = MYADDR_1310;
	addres_1550 = MYADDR_1550;
	addres_COEFF_1310 = MYADDR_COEFF_1310;
	addres_COEFF_1550 = MYADDR_COEFF_1550;
	
	//----------------------------------------------------------------------------------------------------------------------
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	//----------------------------------------------------------------------------------------------------------------------
	HAL_UART_Receive_IT(&huart2, str_rx, rx_buffer_size);
	//----------------------------------------------------------------------------------------------------
	Init_Analog();	
	//----------------------------------------------------------------------------------------------------
  adc_value = adc_read();	
	//----------------------------------------------------------------------------------------------------
	dac_write(dBm[0],lenght[0]);
	//----------------------------------------------------------------------------------------------------
	enablyChannel(number_channel[0]);
	enablyChannel(number_channel[1]);
	enablyChannel(number_channel[2]);
	enablyChannel(number_channel[3]);
	//----------------------------------------------------------------------------------------------------
	//table_and_0_point();
	//
	//PGA112(1);
	//gainPGA = 1;
	//gain_2();
	PGA112(17);//*2
	gainPGA = 2;
	//channel_1();
	//----------------------------------------------------------------------------------------------------
	while (1)
  {
		if(status == HAL_BUSY)
		{
			HAL_UART_DeInit(&huart2);
			HAL_UART_Init(&huart2);
			__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	
		}
		//----------------------------------------------------------------------------------------------------
		if(firstByteWait==2) //флаг о том что пришла команда с ПК
		{
			HAL_Delay(10);			
			firstByteWait=1;
			count=0;
			if(calib == 0x02)
			{
				calib = 0x01;
				//number_channel_1 = str_rx[6];
				//lenght_1 = str_rx[7];
					
				switch(number_channel_1)
				{
					case 0x31:
						channel_1();
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,0);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,1);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,1);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,1);
						dac_write(dBm[0],lenght_1);
						break;
					case 0x32:
						channel_2();
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,0);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,1);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,1);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,1);
						dac_write(dBm[1],lenght_1);
						break;
					case 0x33:
						channel_3();
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,0);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,1);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,1);						
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,1);
						dac_write(dBm[2],lenght_1);
						break;
					case 0x34:
						channel_4();
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,0);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,1);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,1);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,1);
						dac_write(dBm[3],lenght_1);
						break;
				}
				//----------------------------------------------------------------------
			/*	switch(lenght_1)
				{
					case 0x31:
						switch(number_channel_1)
						{
							case 0x31:
								Coeff_1310[0] = 0;
								Coeff_1310[1] = 0;
								break;
							case 0x32:
								Coeff_1310[2] = 0;
								Coeff_1310[3] = 0;
								break;
							case 0x33:
								Coeff_1310[4] = 0;
								Coeff_1310[5] = 0;
								break;
							case 0x34:
								Coeff_1310[6] = 0;
								Coeff_1310[7] = 0;
							break;
						}
						break;
					case 0x32:
						switch(number_channel_1)
						{
							case 0x31:
								Coeff_1550[0] = 0;
								Coeff_1550[1] = 0;
								break;
							case 0x32:
								Coeff_1550[2] = 0;
								Coeff_1550[3] = 0;
								break;
							case 0x33:
								Coeff_1550[4] = 0;
								Coeff_1550[5] = 0;
								break;
							case 0x34:
								Coeff_1550[6] = 0;
								Coeff_1550[7] = 0;
								break;
						}
						break;
				}
				
				for(int j=0;j<1000;j++)
				{
					//HAL_Delay(100);
					read_dBm(number_channel_1,lenght_1,dBm_1);
				}
				
				switch(lenght_1)
				{
					case 0x31:
						switch(number_channel_1)
						{
							case 0x31:
							{
								Coeff_1310[0] = - dBm_in;
								Coeff_1310[1] = dBm_out + 3;
							}
							break;
							case 0x32:
							{
								Coeff_1310[2] = - dBm_in;
								Coeff_1310[3] = - dBm_out + 2.5;
							}
							break;
							case 0x33:
							{
								Coeff_1310[4] = - dBm_in;
								Coeff_1310[5] = - dBm_out + 2.5;
							}
							break;
							case 0x34:
							{
								Coeff_1310[6] = - dBm_in;
								Coeff_1310[7] = - dBm_out + 3;
							}
							break;
						}
						break;
					case 0x32:
						switch(number_channel_1)
						{
							case 0x31:
							{
								Coeff_1550[0] = - dBm_in;
								Coeff_1550[1] = - dBm_out;
							}
							break;
							case 0x32:
							{
								Coeff_1550[2] = - dBm_in;
								Coeff_1550[3] = - dBm_out;
							}
							break;
							case 0x33:
							{
								Coeff_1550[4] = - dBm_in;
								Coeff_1550[5] = - dBm_out;
							}
							break;
							case 0x34:
							{
								Coeff_1550[6] = - dBm_in;
								Coeff_1550[7] = - dBm_out;
							}
							break;
						}
						break;
				}
				
				HAL_FLASH_Unlock();
				
				switch(lenght_1)
				{
					case 0x31:
					{
						EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
						EraseInitStruct.Sector = 3;
						EraseInitStruct.NbSectors = 1;
						HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);
						
						for(uint8_t i = 0; i<8; i++)
						{
							if(Coeff_1310[i]*10 < 0)
								Coeff_1310_test = ~((uint32_t)(Coeff_1310[i]*(-10)));
							else
								Coeff_1310_test = (uint32_t)(Coeff_1310[i]*10);
							
							HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1310,Coeff_1310_test);
							addres_COEFF_1310 = addres_COEFF_1310 +2;
						}
					}
					break;
					case 0x32:
					{
						EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
						EraseInitStruct.Sector = 4;
						EraseInitStruct.NbSectors = 1;
						HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);
							
						for(uint8_t i = 0; i<8; i++)
						{
							if(Coeff_1550[i]*10 < 0)
								Coeff_1550_test = ~((uint32_t)(Coeff_1550[i]*(-10)));
							else
								Coeff_1550_test = (uint32_t)(Coeff_1550[i]*10);
							
							HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,Coeff_1550_test);
							addres_COEFF_1550 = addres_COEFF_1550 +2;
						}
					}
					break;
				}
				HAL_FLASH_Lock();	*/
				//----------------------------------------------------------------------
				addres_1310 = 0;
				addres_1550 = 0;
				addres_COEFF_1310 = 0;
				addres_COEFF_1550 = 0;

				addres_1310 = MYADDR_1310;
				addres_1550 = MYADDR_1550;
				addres_COEFF_1310 = MYADDR_COEFF_1310;
				addres_COEFF_1550 = MYADDR_COEFF_1550;
				
				calib = 0;
				
				if(number_channel_1 == 0x31)
				{
					table_and_0_point(lenght_1,number_channel_1);
				}
				number_channel_1 = 0x30;
				
				for(int i =0; i< 15; i++)
				{
					str_rx[i] = 0;
				}
			}
			if(str_rx[0]=='c' && str_rx[1]=='m' && str_rx[2]=='d')
			{
				number_channel_1 = str_rx[3];
				enablyChannel(number_channel_1);
				switch(number_channel_1)
				{
					case 0x31:
						channel_1();
						break;
					case 0x32:
						channel_2();
						break;
					case 0x33:
						channel_3();
						break;
					case 0x34:
						channel_4();
						break;
				}
				lenght_1 = str_rx[4];
				sscanf((char *)(str_rx+5),"%f", &dBm_1);
				dac_write(dBm_1,lenght_1);
				
				for(int i =0; i< 21; i++)
				{
					str_rx[i] = 0;
				}
			}
			//=================================================================================================
			if(strncmp(str_rx,"record",6) == 0)
			{
				Ch = str_rx[6];
				Len = str_rx[7];
				in_or_out = str_rx[8];
				sscanf((char*)(str_rx + 9),"%f",&Cof);
				HAL_FLASH_Unlock();
				switch(Len)
				{
					case 0x31:
					{
						if(in_or_out == 0x31)
						{
							switch(Ch)
							{
								case 0x31:
									Coeff_1310[0] = Cof;
									break;
								case 0x32:
									Coeff_1310[2] = Cof;
									break;
								case 0x33:
									Coeff_1310[4] = Cof;
									break;
								case 0x34:
									Coeff_1310[6] = Cof;
									break;
							}
						}
						if(in_or_out == 0x32)
						{
							switch(Ch)
							{
								case 0x31:
									Coeff_1310[1] = Cof;
									break;
								case 0x32:
									Coeff_1310[3] = Cof;
									break;
								case 0x33:
									Coeff_1310[5] = Cof;
									break;
								case 0x34:
									Coeff_1310[7] = Cof;
									break;
							}
						}
						
							
						EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
						EraseInitStruct.Sector = 3;
						EraseInitStruct.NbSectors = 1;
						HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);
						
						for(uint8_t i = 0; i<8; i++)
						{
							if(Coeff_1310[i]*10 < 0)
								Coeff_1310_test = ~((uint32_t)(Coeff_1310[i]*(-10)));
							else
								Coeff_1310_test = (uint32_t)(Coeff_1310[i]*10);
							
							HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1310,Coeff_1310_test);
							addres_COEFF_1310 = addres_COEFF_1310 +2;
						}
						/*
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1310,Coeff_in_1310_1_test);
						addres_COEFF_1310 = addres_COEFF_1310 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1310,(uint32_t)(Coeff_out_1310_1*10));
						addres_COEFF_1310 = addres_COEFF_1310 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1310,(uint32_t)(Coeff_in_1310_2*10));
						addres_COEFF_1310 = addres_COEFF_1310 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1310,(uint32_t)(Coeff_out_1310_2*10));
						addres_COEFF_1310 = addres_COEFF_1310 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1310,(uint32_t)(Coeff_in_1310_3*10));
						addres_COEFF_1310 = addres_COEFF_1310 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1310,(uint32_t)(Coeff_out_1310_3*10));
						addres_COEFF_1310 = addres_COEFF_1310 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1310,(uint32_t)(Coeff_in_1310_4*10));
						addres_COEFF_1310 = addres_COEFF_1310 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1310,(uint32_t)(Coeff_out_1310_4*10));
						*/
						break;
					}
					case 0x32:
					{
						if(in_or_out == 0x31)
						{
							switch(Ch)
							{
								case 0x31:
									Coeff_1550[0] = Cof;
									break;
								case 0x32:
									Coeff_1550[2] = Cof;
									break;
								case 0x33:
									Coeff_1550[4] = Cof;
									break;
								case 0x34:
									Coeff_1550[6] = Cof;
									break;
							}
						}
						if(in_or_out == 0x32)
						{
							switch(Ch)
							{
								case 0x31:
									Coeff_1550[1] = Cof;
									break;
								case 0x32:
									Coeff_1550[3] = Cof;
									break;
								case 0x33:
									Coeff_1550[5] = Cof;
									break;
								case 0x34:
									Coeff_1550[7] = Cof;
									break;
							}
						}
						if(in_or_out == 0x33)
						{
							switch(Ch)
							{
								case 0x31:
									Coeff_REZ[0] = Cof;
									break;
								case 0x32:
									Coeff_REZ[1] = Cof;
									break;
								case 0x33:
									Coeff_REZ[2] = Cof;
									break;
								case 0x34:
									Coeff_REZ[3] = Cof;
									break;
							}
						}
						EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
						EraseInitStruct.Sector = 4;
						EraseInitStruct.NbSectors = 1;
						HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);
							
						for(uint8_t i = 0; i<8; i++)
						{
							if(Coeff_1550[i]*10 < 0)
								Coeff_1550_test = ~((uint32_t)(Coeff_1550[i]*(-10)));
							else
								Coeff_1550_test = (uint32_t)(Coeff_1550[i]*10);
							
							HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,Coeff_1550_test);
							addres_COEFF_1550 = addres_COEFF_1550 +2;
						}
						
						for(uint8_t i = 0; i<4; i++)
						{
							Coeff_Rez_test = (uint32_t)(Coeff_REZ[i]*10);
							
							HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,Coeff_Rez_test);
							addres_COEFF_1550 = addres_COEFF_1550 +2;
						}
						/*
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,(uint32_t)(Coeff_in_1550_1*10));
						addres_COEFF_1550 = addres_COEFF_1550 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,(uint32_t)(Coeff_out_1550_1*10));
						addres_COEFF_1550 = addres_COEFF_1550 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,(uint32_t)(Coeff_in_1550_2*10));
						addres_COEFF_1550 = addres_COEFF_1550 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,(uint32_t)(Coeff_out_1550_2*10));
						addres_COEFF_1550 = addres_COEFF_1550 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,(uint32_t)(Coeff_in_1550_3*10));
						addres_COEFF_1550 = addres_COEFF_1550 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,(uint32_t)(Coeff_out_1550_3*10));
						addres_COEFF_1550 = addres_COEFF_1550 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,(uint32_t)(Coeff_in_1550_4*10));
						addres_COEFF_1550 = addres_COEFF_1550 +2;
						HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_COEFF_1550,(uint32_t)(Coeff_out_1550_4*10));
						*/
						break;
					}
				}	

				HAL_FLASH_Lock();
				addres_1310 = 0;
				addres_1550 = 0;
				addres_COEFF_1310 = 0;
				addres_COEFF_1550 = 0;

				addres_1310 = MYADDR_1310;
				addres_1550 = MYADDR_1550;
				addres_COEFF_1310 = MYADDR_COEFF_1310;
				addres_COEFF_1550 = MYADDR_COEFF_1550;
				
				for(int i =0; i< 21; i++)
				{
					str_rx[i] = 0;
				}
			}
			//=================================================================================================
			if(strncmp(str_rx,"read coeff",10) == 0)
			{
				str_tx_coeff[0] = 'C';
				str_tx_coeff[1] = 'o';
				str_tx_coeff[2] = 'e';
				str_tx_coeff[3] = 'f';
				str_tx_coeff[4] = 'f';
				
				sprintf ((char *)Coeff_in_1310_1_gui,"%f",Coeff_1310[0]);
				sprintf ((char *)Coeff_in_1310_2_gui,"%f",Coeff_1310[2]);
				sprintf ((char *)Coeff_in_1310_3_gui,"%f",Coeff_1310[4]);
				sprintf ((char *)Coeff_in_1310_4_gui,"%f",Coeff_1310[6]);
				
				sprintf ((char *)Coeff_in_1550_1_gui,"%f",Coeff_1550[0]);
				sprintf ((char *)Coeff_in_1550_2_gui,"%f",Coeff_1550[2]);
				sprintf ((char *)Coeff_in_1550_3_gui,"%f",Coeff_1550[4]);
				sprintf ((char *)Coeff_in_1550_4_gui,"%f",Coeff_1550[6]);
				
				sprintf ((char *)Coeff_out_1310_1_gui,"%f",Coeff_1310[1]);
				sprintf ((char *)Coeff_out_1310_2_gui,"%f",Coeff_1310[3]);
				sprintf ((char *)Coeff_out_1310_3_gui,"%f",Coeff_1310[5]);
				sprintf ((char *)Coeff_out_1310_4_gui,"%f",Coeff_1310[7]);
				
				sprintf ((char *)Coeff_out_1550_1_gui,"%f",Coeff_1550[1]);
				sprintf ((char *)Coeff_out_1550_2_gui,"%f",Coeff_1550[3]);
				sprintf ((char *)Coeff_out_1550_3_gui,"%f",Coeff_1550[5]);
				sprintf ((char *)Coeff_out_1550_4_gui,"%f",Coeff_1550[7]);
				
				str_tx_coeff[5] = ' ';
				str_tx_coeff[6] = Coeff_in_1310_1_gui[0];
				str_tx_coeff[7] = Coeff_in_1310_1_gui[1];
				str_tx_coeff[8] = Coeff_in_1310_1_gui[2];
				str_tx_coeff[9] = Coeff_in_1310_1_gui[3];
				str_tx_coeff[10] = ' ';
				str_tx_coeff[11] = Coeff_in_1310_2_gui[0];
				str_tx_coeff[12] = Coeff_in_1310_2_gui[1];
				str_tx_coeff[13] = Coeff_in_1310_2_gui[2];
				str_tx_coeff[14] = Coeff_in_1310_2_gui[3];
				str_tx_coeff[15] = ' ';
				str_tx_coeff[16] = Coeff_in_1310_3_gui[0];
				str_tx_coeff[17] = Coeff_in_1310_3_gui[1];
				str_tx_coeff[18] = Coeff_in_1310_3_gui[2];
				str_tx_coeff[19] = Coeff_in_1310_3_gui[3];
				str_tx_coeff[20] = ' ';
				str_tx_coeff[21] = Coeff_in_1310_4_gui[0];
				str_tx_coeff[22] = Coeff_in_1310_4_gui[1];
				str_tx_coeff[23] = Coeff_in_1310_4_gui[2];
				str_tx_coeff[24] = Coeff_in_1310_4_gui[3];
				
				str_tx_coeff[25] = ' ';
				str_tx_coeff[26] = Coeff_in_1550_1_gui[0];
				str_tx_coeff[27] = Coeff_in_1550_1_gui[1];
				str_tx_coeff[28] = Coeff_in_1550_1_gui[2];
				str_tx_coeff[29] = Coeff_in_1550_1_gui[3];
				str_tx_coeff[30] = ' ';
				str_tx_coeff[31] = Coeff_in_1550_2_gui[0];
				str_tx_coeff[32] = Coeff_in_1550_2_gui[1];
				str_tx_coeff[33] = Coeff_in_1550_2_gui[2];
				str_tx_coeff[34] = Coeff_in_1550_2_gui[3];
				str_tx_coeff[35] = ' ';
				str_tx_coeff[36] = Coeff_in_1550_3_gui[0];
				str_tx_coeff[37] = Coeff_in_1550_3_gui[1];
				str_tx_coeff[38] = Coeff_in_1550_3_gui[2];
				str_tx_coeff[39] = Coeff_in_1550_3_gui[3];
				str_tx_coeff[40] = ' ';
				str_tx_coeff[41] = Coeff_in_1550_4_gui[0];
				str_tx_coeff[42] = Coeff_in_1550_4_gui[1];
				str_tx_coeff[43] = Coeff_in_1550_4_gui[2];
				str_tx_coeff[44] = Coeff_in_1550_4_gui[3];
				
				str_tx_coeff[45] = ' ';
				str_tx_coeff[46] = Coeff_out_1310_1_gui[0];
				str_tx_coeff[47] = Coeff_out_1310_1_gui[1];
				str_tx_coeff[48] = Coeff_out_1310_1_gui[2];
				str_tx_coeff[49] = Coeff_out_1310_1_gui[3];
				str_tx_coeff[50] = ' ';
				str_tx_coeff[51] = Coeff_out_1310_2_gui[0];
				str_tx_coeff[52] = Coeff_out_1310_2_gui[1];
				str_tx_coeff[53] = Coeff_out_1310_2_gui[2];
				str_tx_coeff[54] = Coeff_out_1310_2_gui[3];
				str_tx_coeff[55] = ' ';
				str_tx_coeff[56] = Coeff_out_1310_3_gui[0];
				str_tx_coeff[57] = Coeff_out_1310_3_gui[1];
				str_tx_coeff[58] = Coeff_out_1310_3_gui[2];
				str_tx_coeff[59] = Coeff_out_1310_3_gui[3];
				str_tx_coeff[60] = ' ';
				str_tx_coeff[61] = Coeff_out_1310_4_gui[0];
				str_tx_coeff[62] = Coeff_out_1310_4_gui[1];
				str_tx_coeff[63] = Coeff_out_1310_4_gui[2];
				str_tx_coeff[64] = Coeff_out_1310_4_gui[3];
				
				str_tx_coeff[65] = ' ';
				str_tx_coeff[66] = Coeff_out_1550_1_gui[0];
				str_tx_coeff[67] = Coeff_out_1550_1_gui[1];
				str_tx_coeff[68] = Coeff_out_1550_1_gui[2];
				str_tx_coeff[69] = Coeff_out_1550_1_gui[3];
				str_tx_coeff[70] = ' ';
				str_tx_coeff[71] = Coeff_out_1550_2_gui[0];
				str_tx_coeff[72] = Coeff_out_1550_2_gui[1];
				str_tx_coeff[73] = Coeff_out_1550_2_gui[2];
				str_tx_coeff[74] = Coeff_out_1550_2_gui[3];
				str_tx_coeff[75] = ' ';
				str_tx_coeff[76] = Coeff_out_1550_3_gui[0];
				str_tx_coeff[77] = Coeff_out_1550_3_gui[1];
				str_tx_coeff[78] = Coeff_out_1550_3_gui[2];
				str_tx_coeff[79] = Coeff_out_1550_3_gui[3];
				str_tx_coeff[80] = ' ';
				str_tx_coeff[81] = Coeff_out_1550_4_gui[0];
				str_tx_coeff[82] = Coeff_out_1550_4_gui[1];
				str_tx_coeff[83] = Coeff_out_1550_4_gui[2];
				str_tx_coeff[84] = Coeff_out_1550_4_gui[3];
	
				str_tx_coeff[85]= '\r';		
				str_tx_coeff[86]= '\n';	
					
				//HAL_Delay(1000);
				
				CDC_Transmit_FS((unsigned char*)str_tx_coeff, 87);
				//HAL_UART_Transmit(&huart2, (uint8_t*)str_tx_coeff, 45,100);
				
				for(int i =0; i< 26; i++)
				{
					str_rx[i] = 0;
				}
			}
			//=================================================================================================
			if(strncmp(str_rx,"mode",4) == 0)
			{
				dBm_1 = 0;
				dac_write(dBm_1,lenght_1);
				//===============================================
				mode = str_rx[4];
			}
			if(strncmp(str_rx,"Out AOP", 7) == 0)
			{
				sscanf((char *)(str_rx+7),"%f", &Out_AOP);
			}
			
			if(strncmp(str_rx,"read R", 6) == 0)
			{
				str_rez_coeff[0] = 'R';
				str_rez_coeff[1] = 'E';
				str_rez_coeff[2] = 'Z';
				
				sprintf ((char *)Coeff_REZ_0_gui,"%f",Coeff_REZ[0]);
				sprintf ((char *)Coeff_REZ_1_gui,"%f",Coeff_REZ[1]);
				sprintf ((char *)Coeff_REZ_2_gui,"%f",Coeff_REZ[2]);
				sprintf ((char *)Coeff_REZ_3_gui,"%f",Coeff_REZ[3]);
				
				str_rez_coeff[3] = ' ';
				str_rez_coeff[4] = Coeff_REZ_0_gui[0];
				str_rez_coeff[5] = Coeff_REZ_0_gui[1];
				str_rez_coeff[6] = Coeff_REZ_0_gui[2];
				str_rez_coeff[7] = Coeff_REZ_0_gui[3];
				str_rez_coeff[8] = Coeff_REZ_0_gui[4];
				
				str_rez_coeff[9] = ' ';
				str_rez_coeff[10] =  Coeff_REZ_1_gui[0];
				str_rez_coeff[11] = Coeff_REZ_1_gui[1];
				str_rez_coeff[12] = Coeff_REZ_1_gui[2];
				str_rez_coeff[13] = Coeff_REZ_1_gui[3];
				str_rez_coeff[14] = Coeff_REZ_1_gui[4];
				
				str_rez_coeff[15] = ' ';
				str_rez_coeff[16] = Coeff_REZ_2_gui[0];
				str_rez_coeff[17] = Coeff_REZ_2_gui[1];
				str_rez_coeff[18] = Coeff_REZ_2_gui[2];
				str_rez_coeff[19] = Coeff_REZ_2_gui[3];
				str_rez_coeff[20] = Coeff_REZ_2_gui[4];
				
				str_rez_coeff[21] = ' ';
				str_rez_coeff[22] = Coeff_REZ_3_gui[0];
				str_rez_coeff[23] = Coeff_REZ_3_gui[1];
				str_rez_coeff[24] = Coeff_REZ_3_gui[2];
				str_rez_coeff[25] = Coeff_REZ_3_gui[3];
				str_rez_coeff[26] = Coeff_REZ_3_gui[4];
				
				str_rez_coeff[27]= '\r';		
				str_rez_coeff[28]= '\n';	
				
				CDC_Transmit_FS((unsigned char*)str_rez_coeff, 29);
				
				for(int i =0; i< 10; i++)
				{
					str_rx[i] = 0;
				}
			}
		}	
		//----------------------------------------------------------------------------------------------------
		/*
		switch (test_gein)
		{
			case 0x31:
				gain_1();
				break;
			case 0x32:
				gain_2();
				break;
			case 0x33:
				gain_3();
				break;
			case 0x34:
				gain_4();
				break;
			case 0x35:
				gain_5();
				break;
		}
		test_gein = 0;
		
		adc_value = adc_read();	//получение значения с ADC
		mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000)); //мощность на выходе 
	*/
		//----------------------------------------------------------------------------------------------------
		if(mode == 0x30)
		{
			if(number_channel_1 != 0x30)
			{
				for(int i=0;i<10;i++)
				{
					read_dBm(number_channel_1,lenght_1,dBm_1);
				}
				sprintf ((char *)dBm_in_tx,"%f",dBm_in);
				sprintf ((char *)dBm_out_tx,"%f",dBm_out);	

				//передача данных
			
				str_in_tx[0]= 'c';
				str_in_tx[1]= 'm';
				str_in_tx[2]= 'd';
				str_in_tx[3]= ' ';
				str_in_tx[4]= number_channel_1;	
				str_in_tx[5]= ' ';
				str_in_tx[6]= 'i';
				str_in_tx[7]= '=';	
				str_in_tx[8]= dBm_in_tx[0];				
				str_in_tx[9]= dBm_in_tx[1];	
				str_in_tx[10]= dBm_in_tx[2];
				str_in_tx[11]= dBm_in_tx[3];
				str_in_tx[12]= dBm_in_tx[4];	
				str_in_tx[13]= '\r';		
				str_in_tx[14]= '\n';	
				
				CDC_Transmit_FS((unsigned char*)str_in_tx, 15);
				HAL_UART_Transmit(&huart2, (uint8_t*)str_in_tx, 15,100);
				
				str_out_tx[0]= 'c';
				str_out_tx[1]= 'm';
				str_out_tx[2]= 'd';
				str_out_tx[3]= ' ';
				str_out_tx[4]= number_channel_1;
				str_out_tx[5]= ' ';
				str_out_tx[6]= 'o';
				str_out_tx[7]= '=';	
				str_out_tx[8]= dBm_out_tx[0];				
				str_out_tx[9]= dBm_out_tx[1];	
				str_out_tx[10]= dBm_out_tx[2];
				str_out_tx[11]= dBm_out_tx[3];
				str_out_tx[12]= dBm_out_tx[4];		
				str_out_tx[13]= '\r';		
				str_out_tx[14]= '\n';	
					
				
				CDC_Transmit_FS((unsigned char*)str_out_tx, 15);
				HAL_UART_Transmit(&huart2, (uint8_t*)str_in_tx, 15,100);
			}
			else
			{
				channel_1();
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,1);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,1);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,1);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,1);	
			}
		}
		else
		{
			if(number_channel_1 != 0x30)
			{
				//===============================================
				for(int i=0;i<10;i++)
				{
					read_dBm(number_channel_1,lenght_1,dBm_1);
				}
				//===============================================
				sprintf ((char *)dBm_in_tx,"%f",dBm_in);
				sprintf ((char *)dBm_out_tx,"%f",dBm_out);
				//===============================================
				if(dBm_in < Out_AOP)
				{
					str_error[0] = 'E';
					str_error[1] = 'r';
					str_error[2] = 'r';
					str_error[3] = 'o';
					str_error[4] = 'r';
					str_error[5] = '\r';
					str_error[6] = '\n';
					
					CDC_Transmit_FS((unsigned char*)str_error, 7);
					HAL_UART_Transmit(&huart2, (uint8_t*)str_error, 7,100);
				}
				else
				{
					
					delta = fabs(fabs(Out_AOP) - fabs(dBm_out));					
					
					if(delta > 0.1)
					{
						if(Out_AOP < dBm_out)
						{
							dBm_1 += delta;
						}
						else
						{
							dBm_1 -= delta;
							if(dBm_1 < 0)
								dBm_1 = 0;
						}
						dac_write(dBm_1,lenght_1);
					}	
				}
				
				//передача данных
			
				str_in_tx[0]= 'c';
				str_in_tx[1]= 'm';
				str_in_tx[2]= 'd';
				str_in_tx[3]= ' ';
				str_in_tx[4]= number_channel_1;	
				str_in_tx[5]= ' ';
				str_in_tx[6]= 'i';
				str_in_tx[7]= '=';	
				str_in_tx[8]= dBm_in_tx[0];				
				str_in_tx[9]= dBm_in_tx[1];	
				str_in_tx[10]= dBm_in_tx[2];
				str_in_tx[11]= dBm_in_tx[3];
				str_in_tx[12]= dBm_in_tx[4];	
				str_in_tx[13]= '\r';		
				str_in_tx[14]= '\n';	
				
				CDC_Transmit_FS((unsigned char*)str_in_tx, 15);
				HAL_UART_Transmit(&huart2, (uint8_t*)str_in_tx, 15,100);
				
				str_out_tx[0]= 'c';
				str_out_tx[1]= 'm';
				str_out_tx[2]= 'd';
				str_out_tx[3]= ' ';
				str_out_tx[4]= number_channel_1;
				str_out_tx[5]= ' ';
				str_out_tx[6]= 'o';
				str_out_tx[7]= '=';	
				str_out_tx[8]= dBm_out_tx[0];				
				str_out_tx[9]= dBm_out_tx[1];	
				str_out_tx[10]= dBm_out_tx[2];
				str_out_tx[11]= dBm_out_tx[3];
				str_out_tx[12]= dBm_out_tx[4];		
				str_out_tx[13]= '\r';		
				str_out_tx[14]= '\n';	
					
				
				CDC_Transmit_FS((unsigned char*)str_out_tx, 15);
				HAL_UART_Transmit(&huart2, (uint8_t*)str_in_tx, 15,100);
			}
			else
			{
				if(dBm_1 != 0)
				{
					dBm_1 = 0;
					dac_write(dBm_1,lenght_1);
				}
				channel_1();
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,1);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,1);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,1);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,1);
			}
		}
	}
}
/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
