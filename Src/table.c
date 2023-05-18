#include "main.h"
//----------------------------------------------------------------------------------------------------
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "usbd_cdc_if.h"
#include <math.h>
#include <stdio.h>
//----------------------------------------------------------------------------------------------------

#define rx_buffer_size 500
#define MYADDR_1310 0x08040000
#define MYADDR_1550 0x08080000

uint16_t wave_1310_value_calibrate = 0x0EFF;
uint16_t wave_1550_value_calibrate = 0x0EFF;

uint16_t wave_1310_inCoefficient = 0;
uint16_t wave_1310_outCoefficient = 0;

uint16_t wave_1550_inCoefficient = 0;
uint16_t wave_1550_outCoefficient = 0;

extern uint8_t wavelength_1310_cal;
extern uint8_t wavelength_1550_cal;

extern char str_rx[rx_buffer_size];

extern uint8_t value_dac[2];
uint16_t value_dac_const = 0;

double dBm_out_temp = 0;

float temp_dBm_in = 0;
float temp_dBm_out = 0;

extern float lt6306;
extern float lt6306_temp;

extern float dBm;

extern float dBm_in;
extern float dBm_out;

extern uint8_t str_in_tx[15];
extern uint8_t str_out_tx[15];

extern uint8_t dBm_in_tx[5];

extern uint16_t temp;

int index_2=0;

static FLASH_EraseInitTypeDef EraseInitStruct;

extern uint32_t page_error;

//МБ создать отдельный буфер?
extern uint16_t voa_1310[301];
extern uint16_t voa_1550[301];

uint16_t coeff_voa_1310 = 0;
uint16_t coeff_voa_1550 = 0;

void table_and_0_point(uint8_t lenght,uint8_t number_channel)
{
	/*if(lenght == 0x31)
	{
		for(uint16_t i = 0; i<301; i ++)
		{
			voa_1310[i] = 0xFFFF;
		}
	}
	else
	{
		for(uint16_t i = 0; i<301; i ++)
		{
			voa_1550[i] = 0xFFFF;
		}
	}*/
	//--------------------------------------------------------------------------------------------------------
	dBm = 0;
	temp_dBm_in = 0;
	temp_dBm_out = 0;
	dBm_out_temp = 0;
	//--------------------------------------------------------------------------------------------------------
	str_rx[1]=0xFF;																								//1111 1111 1111 1111 запись в DAC
	str_rx[2]=0xFF;
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off (отключаем очистку DAC до 0)
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);												// Разрешаем запись в DAC
	value_dac[0] = (str_rx[1]) & 0xFF;														// convertation data
	value_dac[1] = 	str_rx[2] & 0xFF;															// помещаем в него максимальное число 
	HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC
	HAL_Delay(50);																								// задержка
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);												// запрещаем запись в DAC
	//--------------------------------------------------------------------------------------------------------
	for(int i=0;i<10;i++)
	{
		read_dBm(number_channel,lenght,dBm);												// получаем значение входной мощности 
	}
	//--------------------------------------------------------------------------------------------------------
	temp_dBm_in =((float)((int8_t)(dBm_in*10)))/10;								// пересчет (перведущее значение)
	//--------------------------------------------------------------------------------------------------------
	str_rx[1]=0xE0;																								//делаем тоже самое что описанно выше 
	str_rx[2]=0xFF;																								//1111 1111 1110 0000 запись в DAC
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);	
	value_dac[0] = (str_rx[1]) & 0xFF;														// convertation data
	value_dac[1] =  str_rx[2] & 0xFF;
	HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);	
	//--------------------------------------------------------------------------------------------------------
	for(int i=0;i<10;i++)
	{
		read_dBm(number_channel,lenght,dBm);												// получаем значение входной мощности
	}
	//--------------------------------------------------------------------------------------------------------
	
	//Функция fabs() позволяет вычислить абсолютное значение по модулю x 
	//и возвращает значение типа double (число с плавающей точкой). 
	
	 
	
	while((temp_dBm_in-((float)((int8_t)(dBm_in*10)))/10)<=fabs(0.05)) //берем разницу между max и меньшим затуханием и сравниваем с 0.05
	//крутимся в цикле пока разница между приведущим и текущим значение не будет больше 0.05
	//ДЛЯ ПОИСКА 0 точки на графике
	{
		//если условие выполняется, то записываем полученную токущее мощность делаем приведущей 
		temp_dBm_in = ((float)((int8_t)(dBm_in*10)))/10; 
		str_rx[2]-=0x10;																//1111 1111 - 0001 0000
		if(str_rx[2]<=0x0F)															//1110 1111 < 0000 1111 когда условие выполниться 
		{
			str_rx[1]-=0x01;															//1110 0000 - 0000 0001 
			str_rx[2]=0xFF;                               //1111 1111
		}
		//производим теже самые маниипуляции с DAC 
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);	
		value_dac[0] = (str_rx[1]) & 0xFF;														// convertation data
		value_dac[1] = str_rx[2] & 0xFF;
		HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);	
		//--------------------------------------------------------------------------------------------------------
		for(int i=0;i<10;i++)
		{
			read_dBm(number_channel,lenght,dBm);
		}		
	}
	//когда вышли из цикла, получили значение dBm_in у которого, разница от придедущего значения больше чем на 0.05
	//и получили комбинацию на DAC при котором антенюатор делает такую разницу между входными значениями 
	sprintf ((char *)dBm_in_tx,"%f",dBm_in);
  
	str_in_tx[0]= value_dac[0];	
	str_in_tx[1]= value_dac[1];
		
	//CDC_Transmit_FS((unsigned char*)str_in_tx, 2);	
	
	//используется для получения выходной мощности с 0-ой точки
	temp_dBm_out=((float)((int8_t)(dBm_out*10)))/10;
	dBm_out_temp = dBm_out_temp+dBm_out; //dBm_out_temp = 0
	//--------------------
	
	//кусок для получения падения мощности на антенюаторе (программа для колибровки на ПК есть?)	
	
	while(index_2 < 300) 
	{
		read_dBm(number_channel,lenght,dBm);
		while((((float)((int32_t)(dBm_out_temp*10000)))/10000)>(temp_dBm_out-0.1))
		{	
			temp	= (str_rx[1] & 0xFF)<<8 |(str_rx[2] & 0xFF); //значения str_rx получены при рачетах в приведущем цикле 
			// это работает только в режиме калибровки
			temp-=0x02; // отнимаем двойку потому что идем с шагом 3?
			str_rx[1] = (temp & 0xFF00)>>8;
			str_rx[2] = temp & 0x00FF;

			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);	
			value_dac[0] = (str_rx[1]) & 0xFF;														// convertation data
			value_dac[1] = str_rx[2] & 0xFF;															
			HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC
			HAL_Delay(50);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);	
			HAL_Delay(50);
			
			dBm_out_temp=0;
			
			for(int i=0;i<3;i++)
			{
				read_dBm(number_channel,lenght,dBm);
				dBm_out_temp = dBm_out_temp+dBm_out;
			}
			
			dBm_out_temp=dBm_out_temp/3; //шаг
		}
		
		temp_dBm_out = ((float)((int32_t)(dBm_out_temp*10000)))/10000;
		str_in_tx[0]= value_dac[0];	
		str_in_tx[1]= value_dac[1];
		
		
		index_2 = ((int)(dBm*10)); //переводим значение dBm в индекс, для поиска табличного значения
		
		value_dac_const = ((value_dac[0] & 0x00FF)<<8) + (value_dac[1] & 0x00FF);
		
		switch(lenght)
		{
			case 0x31:
			{
				voa_1310[index_2] = value_dac_const;// + coeff_voa_1310;	
				
				/*if(abs(abs((int)voa_1310[index_2 - 1]) - abs((int)voa_1310[index_2 - 1])) > 0x64)
				{
					voa_1310[index_2] = voa_1310[index_2 - 1] -0x64;
				}*/
				
				break;
			}
			case 0x32:
			{
				voa_1550[index_2] = value_dac_const;// + coeff_voa_1550;
				
				/*if(abs(abs((int)voa_1310[index_2 - 1]) - abs((int)voa_1310[index_2 - 1])) > 0x64)
				{
					voa_1310[index_2] = voa_1310[index_2 - 1] -0x64;
				}*/
				
				break;
			}
			default:break;
		}
		
		
		dBm+=0.1;
	
		CDC_Transmit_FS((unsigned char*)str_in_tx, 2);			
	}
	coeff_voa_1310 = 0;
	coeff_voa_1550 = 0;
	
	
	int addres_1310_E = 0;
	int addres_1550_E = 0;

	addres_1310_E = MYADDR_1310;
	addres_1550_E = MYADDR_1550;
	
	HAL_FLASH_Unlock();
	
	switch(lenght)
	{
		case 0x31:
		{
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
			EraseInitStruct.Sector = 5;
			EraseInitStruct.NbSectors = 1;
			HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);
			break;
		}
		case 0x32:
		{
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
			EraseInitStruct.Sector = 6;
			EraseInitStruct.NbSectors = 1;
			HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);
			break;
		}
	}
	
	for(uint16_t i = 0; i<301; i++)
	{
		switch(lenght)
		{
			case 0x31:
			{
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_1310_E,voa_1310[i]);
				addres_1310_E = addres_1310_E +2;
				break;
			}
			case 0x32:
			{	
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_1550_E,voa_1550[i]);
				addres_1550_E = addres_1550_E +2;
				break;
			}
			default:break;
		}
	}
	
	HAL_FLASH_Lock();
}
//======================================================================================================================================
void Calib_1310(uint8_t lenght,uint8_t number_channel)
{
	wave_1310_value_calibrate = 0x0EFF;
	dBm = 0;
	
	for(int i=0;i<10;i++)
	{
		read_dBm(number_channel,lenght,dBm);												// получаем значение входной мощности
	}
	
	lt6306_temp=lt6306;
	
	while(lt6306-lt6306_temp >= 0 || wavelength_1310_cal==0x31)
	{
		wavelength_1310_cal=0x30;
		
		wave_1310_value_calibrate = (str_rx[1] & 0xFF)<<8 |(str_rx[2] & 0xFF);
		wave_1310_value_calibrate-=0x2;
		
		str_rx[1] = (wave_1310_value_calibrate & 0xFF00)>>8;
		str_rx[2] = wave_1310_value_calibrate & 0x00FF;
		
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);	
		value_dac[0] = (str_rx[1]) & 0xFF;														// convertation data
		value_dac[1] = str_rx[2] & 0xFF;															
		HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);	
		HAL_Delay(50);
		
		HAL_Delay(1500);
		
		if(strncmp(str_rx,"cmd lt=", 7) == 0)
		{
			sscanf((char *)(str_rx+7),"%f", &lt6306);
			lt6306=roundf(lt6306*10)/10;
			memset(str_rx,0,sizeof(str_rx));
		}
		
		if(lt6306_temp-lt6306<=-0.09)
		{
			lt6306_temp=roundf(lt6306*10)/10;
		}
		
		wave_1310_value_calibrate = (str_rx[1] & 0xFF)<<8 |(str_rx[2] & 0xFF);
		wave_1310_value_calibrate-=0x2;
		
		str_rx[1] = (wave_1310_value_calibrate & 0xFF00)>>8;
		str_rx[2] = wave_1310_value_calibrate & 0x00FF;
		
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);	
		value_dac[0] = (str_rx[1]) & 0xFF;														// convertation data
		value_dac[1] = str_rx[2] & 0xFF;															
		HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);	
		HAL_Delay(50);
		
		HAL_Delay(1500);
		if(strncmp(str_rx,"cmd lt=", 7) == 0)
		{
			sscanf((char *)(str_rx+7),"%f", &lt6306);
			lt6306=roundf(lt6306*10)/10;
			memset(str_rx,0,sizeof(str_rx));
		}			
	}
	
	HAL_Delay(1500);

	for(int i=0;i<5;i++)
	{
		HAL_Delay(200);
		
		for(int i=0;i<10;i++)
		{
			read_dBm(number_channel,lenght,dBm);												// получаем значение входной мощности
		}
					
		if(strncmp(str_rx,"cmd lt=", 7) == 0)
		{
			sscanf((char *)(str_rx+7),"%f", &lt6306);
			lt6306=roundf(lt6306*10)/10;
			memset(str_rx,0,sizeof(str_rx));
		}
	}
	
	wave_1310_inCoefficient = 0-dBm_in;
	wave_1310_outCoefficient = dBm_out-lt6306;
	
	for(int i=0;i<10;i++)
	{
		read_dBm(number_channel,lenght,dBm);												// получаем значение входной мощности
	}
	
	while(index_2 < 300) 
	{
		if(strncmp(str_rx,"cmd lt=", 7) == 0)
		{
			sscanf((char *)(str_rx+7),"%f", &lt6306);
			lt6306=roundf(lt6306*10)/10;
			memset(str_rx,0,sizeof(str_rx));
		}
		
		wave_1310_value_calibrate = (str_rx[1] & 0xFF)<<8 |(str_rx[2] & 0xFF);
		
		if(lt6306_temp-lt6306>=0.09)
		{
			voa_1310[index_2] = wave_1310_value_calibrate;
			char mes[22]={'p'};
			CDC_Transmit_FS((uint8_t*)mes, 22);

			index_2++;
			lt6306_temp=roundf(lt6306*10)/10;
		}
					
		wave_1310_value_calibrate = (str_rx[1] & 0xFF)<<8 |(str_rx[2] & 0xFF);
		wave_1310_value_calibrate-=0x2;
		
		str_rx[1] = (wave_1310_value_calibrate & 0xFF00)>>8;
		str_rx[2] = wave_1310_value_calibrate & 0x00FF;
		
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);	
		value_dac[0] = (str_rx[1]) & 0xFF;														// convertation data
		value_dac[1] = str_rx[2] & 0xFF;															
		HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);	
		HAL_Delay(50);
		
		HAL_Delay(1500);
		if(strncmp(str_rx,"cmd lt=", 7) == 0)
		{
			sscanf((char *)(str_rx+7),"%f", &lt6306);
			lt6306=roundf(lt6306*10)/10;
			memset(str_rx,0,sizeof(str_rx));
		}
	}
	char mes[22]={'l'};
	CDC_Transmit_FS((uint8_t*)mes, 22);
	
	int addres_1310_E = 0;
	int addres_1550_E = 0;

	addres_1310_E = MYADDR_1310;
	addres_1550_E = MYADDR_1550;
	
	HAL_FLASH_Unlock();
	
	switch(lenght)
	{
		case 0x31:
		{
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
			EraseInitStruct.Sector = 5;
			EraseInitStruct.NbSectors = 1;
			HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);
			break;
		}
		case 0x32:
		{
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
			EraseInitStruct.Sector = 7;
			EraseInitStruct.NbSectors = 1;
			HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);
			break;
		}
	}

	for(uint16_t i = 0; i<301; i++)
	{
		switch(lenght)
		{
			case 0x31:
			{
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_1310_E,voa_1310[i]);
				addres_1310_E = addres_1310_E +2;
				break;
			}
			case 0x32:
			{	
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_1550_E,voa_1550[i]);
				addres_1550_E = addres_1550_E +2;
				break;
			}
			default:break;
		}
	}
	
	HAL_FLASH_Lock();
}

//======================================================================================================================================
void Calib_1550(uint8_t lenght,uint8_t number_channel)
{
	wave_1550_value_calibrate = 0x0EFF;
	dBm = 0;
	
	for(int i=0;i<10;i++)
	{
		read_dBm(number_channel,lenght,dBm);												// получаем значение входной мощности
	}
	
	lt6306_temp=lt6306;
	
	while(lt6306-lt6306_temp >= 0 || wavelength_1550_cal == 0x31)
	{
		wavelength_1550_cal = 0x30;
		
		wave_1550_value_calibrate = (str_rx[1] & 0xFF)<<8 |(str_rx[2] & 0xFF);
		wave_1550_value_calibrate -= 0x2;
		
		str_rx[1] = (wave_1550_value_calibrate & 0xFF00)>>8;
		str_rx[2] = wave_1550_value_calibrate & 0x00FF;
		
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);	
		value_dac[0] = (str_rx[1]) & 0xFF;														// convertation data
		value_dac[1] = str_rx[2] & 0xFF;															
		HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);	
		HAL_Delay(50);
		
		HAL_Delay(1500);
		
		if(strncmp(str_rx,"cmd lt=", 7) == 0)
		{
			sscanf((char *)(str_rx+7),"%f", &lt6306);
			lt6306=roundf(lt6306*10)/10;
			memset(str_rx,0,sizeof(str_rx));
		}
		
		if(lt6306_temp-lt6306<=-0.09)
		{
			lt6306_temp=roundf(lt6306*10)/10;
		}
		
		wave_1550_value_calibrate = (str_rx[1] & 0xFF)<<8 |(str_rx[2] & 0xFF);
		wave_1550_value_calibrate -= 0x2;
		
		str_rx[1] = (wave_1550_value_calibrate & 0xFF00)>>8;
		str_rx[2] = wave_1550_value_calibrate & 0x00FF;
		
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);	
		value_dac[0] = (str_rx[1]) & 0xFF;														// convertation data
		value_dac[1] = str_rx[2] & 0xFF;															
		HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);	
		HAL_Delay(50);
		
		HAL_Delay(1500);
		if(strncmp(str_rx,"cmd lt=", 7) == 0)
		{
			sscanf((char *)(str_rx+7),"%f", &lt6306);
			lt6306=roundf(lt6306*10)/10;
			memset(str_rx,0,sizeof(str_rx));
		}			
	}
	
	HAL_Delay(1500);

	for(int i=0;i<5;i++)
	{
		HAL_Delay(200);
		
		for(int i=0;i<10;i++)
		{
			read_dBm(number_channel,lenght,dBm);												// получаем значение входной мощности
		}
					
		if(strncmp(str_rx,"cmd lt=", 7) == 0)
		{
			sscanf((char *)(str_rx+7),"%f", &lt6306);
			lt6306=roundf(lt6306*10)/10;
			memset(str_rx,0,sizeof(str_rx));
		}
	}
	
	wave_1550_inCoefficient = 0-dBm_in;
	wave_1550_outCoefficient = dBm_out-lt6306;
	
	for(int i=0;i<10;i++)
	{
		read_dBm(number_channel,lenght,dBm);												// получаем значение входной мощности
	}
	
	while(index_2 < 300) 
	{
		if(strncmp(str_rx,"cmd lt=", 7) == 0)
		{
			sscanf((char *)(str_rx+7),"%f", &lt6306);
			lt6306=roundf(lt6306*10)/10;
			memset(str_rx,0,sizeof(str_rx));
		}
		
		wave_1550_value_calibrate = (str_rx[1] & 0xFF)<<8 |(str_rx[2] & 0xFF);
		
		if(lt6306_temp-lt6306>=0.09)
		{
			voa_1310[index_2] = wave_1310_value_calibrate;
			char mes[22]={'p'};
			CDC_Transmit_FS((uint8_t*)mes, 22);

			index_2++;
			lt6306_temp=roundf(lt6306*10)/10;
		}
					
		wave_1550_value_calibrate = (str_rx[1] & 0xFF)<<8 |(str_rx[2] & 0xFF);
		wave_1550_value_calibrate-=0x2;
		
		str_rx[1] = (wave_1550_value_calibrate & 0xFF00)>>8;
		str_rx[2] = wave_1550_value_calibrate & 0x00FF;
		
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,1);												// clr off
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,0);	
		value_dac[0] = (str_rx[1]) & 0xFF;														// convertation data
		value_dac[1] = str_rx[2] & 0xFF;															
		HAL_SPI_Transmit_IT(&hspi4, value_dac, 2);										// write data DAC
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,1);	
		HAL_Delay(50);
		
		HAL_Delay(1500);
		if(strncmp(str_rx,"cmd lt=", 7) == 0)
		{
			sscanf((char *)(str_rx+7),"%f", &lt6306);
			lt6306=roundf(lt6306*10)/10;
			memset(str_rx,0,sizeof(str_rx));
		}
	}
	
	char mes[22]={'l'};
	CDC_Transmit_FS((uint8_t*)mes, 22);
	
	int addres_1310_E = 0;
	int addres_1550_E = 0;

	addres_1310_E = MYADDR_1310;
	addres_1550_E = MYADDR_1550;
	
	HAL_FLASH_Unlock();
	
	switch(lenght)
	{
		case 0x31:
		{
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
			EraseInitStruct.Sector = 5;
			EraseInitStruct.NbSectors = 1;
			HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);
			break;
		}
		case 0x32:
		{
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
			EraseInitStruct.Sector = 7;
			EraseInitStruct.NbSectors = 1;
			HAL_FLASHEx_Erase(&EraseInitStruct,&page_error);
			break;
		}
	}

	for(uint16_t i = 0; i<301; i++)
	{
		switch(lenght)
		{
			case 0x31:
			{
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_1310_E,voa_1310[i]);
				addres_1310_E = addres_1310_E +2;
				break;
			}
			case 0x32:
			{	
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addres_1550_E,voa_1550[i]);
				addres_1550_E = addres_1550_E +2;
				break;
			}
			default:break;
		}
	}
	
	HAL_FLASH_Lock();
}