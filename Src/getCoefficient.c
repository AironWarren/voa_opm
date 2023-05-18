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
#define kilo 1000
//extern uint8_t lenght;
extern uint8_t gain;
/*extern float Coeff_in_1310_1;
extern float Coeff_out_1310_1;
extern float Coeff_in_1310_2;
extern float Coeff_out_1310_2;
extern float Coeff_in_1310_3;
extern float Coeff_out_1310_3;
extern float Coeff_in_1310_4;
extern float Coeff_out_1310_4;

extern float Coeff_in_1550_1;
extern float Coeff_out_1550_1;
extern float Coeff_in_1550_2;
extern float Coeff_out_1550_2;
extern float Coeff_in_1550_3;
extern float Coeff_out_1550_3;
extern float Coeff_in_1550_4;
extern float Coeff_out_1550_4;*/

extern float Coeff_1310[8];

extern float Coeff_1550[8];

extern float Coeff_REZ[4];

uint32_t R_86 = 1000000;
uint32_t R_2 = 1000;
//----------------------------------------------------------------------------------------------------
static float Cu(float R_3)
{
	float K = 0;
	
	K = ((R_2 + (R_3 * kilo)) * R_86)/(R_2 + (R_3 * kilo) + R_86);
	
	return K;
}
//----------------------------------------------------------------------------------------------------
double getCoefficientR(uint8_t gain) //получение коэффициента усиления с первичного каскада (скорее всего коэффициенты рассчитаны из схемы операционного усилителя?)
	//приэтом в 35 добавлен максимальный коэффициент усиления программируемого усилителя 128 * коэф каскада
{
	switch (gain)
	{
		case 0x31:
		{
			
			//return 9;
			return Cu(Coeff_REZ[0]);
			//return Cu(2.0);
			//return 1;
		}
		case 0x32:
		{
			//return 51;
			return Cu(Coeff_REZ[1]);
			//return 9;
		}
		case 0x33:
		{
			return Cu(Coeff_REZ[2]);
			//return 247;
			//return 45;
		}
		case 0x34:
		{
			return Cu(Coeff_REZ[3]);
			//return 950;
			//return 240;
		}
		case 0x35:
		{
			return 101500;
			//return 10150;
		}
	}
}

/*
*	 Coefficient lenght
*
*  0x31 - 1310        			
*  0x32 - 1550			 
*/
//----------------------------------------------------------------------------------------------------
double getCoefficientLenght(uint8_t lenght) //получение коэффициента рабочий длинный волны ?
{
	switch(lenght)
	{
		case 0x31: //1310
		case 0x33: //1310
		{
			return 26;
		}
		case 0x32://1550
		{
			return 28;
		}
	}
}
//----------------------------------------------------------------------------------------------------
float getCoefficientChannelIn(uint8_t number_channel, uint8_t lenght) //получение коэффициента номера оптического канала (исходя из рабочий длинны волны)
{
	switch(number_channel){
		case 0x31:
		{
			//==============================================================================================
			//production DWDM
			/*if(lenght==0x31)
			{
					//return Coeff_in_1310_1 - 3.1;//2;
					return Coeff_1310[0] - 4.5;//2;
			}
			if(lenght==0x32)
			{
					//return Coeff_in_1550_1 - 1.5;
					return Coeff_1550[0] - 1.5;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[0] - 4.0;//2;
			}*/
			
			//==============================================================================================
			//opm LR
			/*if(lenght==0x31)
			{
					return Coeff_1310[0] + 1.0;//2;
			}
			if(lenght==0x32)
			{
					return Coeff_1550[0] + 0.6;//2;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[0] + 1.8;//2;
			}*/
			
			//==============================================================================================
			//opm is our
			/*
			if(lenght==0x31)
			{
					if(gain == 0x35)
						return Coeff_1310[0] + 8.5;//7.5;
					else
						return Coeff_1310[0] - 2.0;//2;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[0] + 11.0;//7.5;
				else
					return Coeff_1550[0] + 0.6;//2;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[0] - 1.6;//2;
			}
			*/
			//==============================================================================================
			//New VOP-OPM silver
			if(lenght==0x31)
			{
				if(gain == 0x35)
					return Coeff_1310[0] + 12.7;//2;
				if(gain == 0x34)
					return Coeff_1310[0] + 5.7;//2;
				else
					return Coeff_1310[0] + 2.7;//2;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[0] + 11.0;
				if(gain == 0x34)
					return Coeff_1550[0] + 4.0;
				else
					return Coeff_1550[0] + 2.0;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[0] + 3.5;//2;
			}
		}
		case 0x32:
		{
			//==============================================================================================
			//production DWDM
			/*if(lenght==0x31)
			{
					//return Coeff_in_1310_2 - 0.3;
					return Coeff_1310[2] - 0.3;
			}
			if(lenght==0x32)
			{
					//return Coeff_in_1550_2 - 0.6;
					return Coeff_1550[2] - 0.6;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[2] + 0.0;//2;
			}*/
			
			//==============================================================================================
			//opm LR
			/*
			if(lenght==0x31)
			{
					return Coeff_1310[2] + 1;
			}
			if(lenght==0x32)
			{
					return Coeff_1550[2] + 1.0;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[2] + 2.0;//2; // pri kalibrovki bilo [0]
			}*/
			
			//==============================================================================================
			//opm is our
			/*
			if(lenght==0x31)
			{
				if(gain == 0x35)
						return Coeff_1310[2] + 9.5;//7.5;
				else
						return Coeff_1310[2] - 0.4;//2;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[2] + 10.0;//7.5;
				else
					return Coeff_1550[2] - 0.6;//2;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[2] + 0.4;//2;
			}
			*/
			//==============================================================================================
			//New VOP-OPM silver
			if(lenght==0x31)
			{
				if(gain == 0x35)
					return Coeff_1310[2] + 15.0;
				if(gain == 0x34)
					return Coeff_1310[2] + 8.0;
				else
					return Coeff_1310[2] + 5.0;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[2] + 12.0;
				if(gain == 0x34)
					return Coeff_1550[2] + 5.0;
				else
					return Coeff_1550[2] + 3;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[2] + 4.2;//2;
			}
		}
		case 0x33:
		{
			//==============================================================================================
			//production DWDM
			/*if(lenght==0x31)
			{
					//return Coeff_in_1310_3;
					return Coeff_1310[4];
			}
			if(lenght==0x32)
			{
					//return Coeff_in_1550_3 + 0.3;
					return Coeff_1550[4] + 0.3;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[4];//2;
			}*/
			
			//==============================================================================================
			//opm LR
			/*
			if(lenght==0x31)
			{
					return Coeff_1310[4]+1.0;
			}
			if(lenght==0x32)
			{
					return Coeff_1550[4] - 0.0;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[4] + 1.5;//2;
			}
			*/
			
			//==============================================================================================
			//opm is our
			/*
			if(lenght==0x31)
			{
				if(gain == 0x35)
						return Coeff_1310[4] + 9.5;//7.5;
				else
						return Coeff_1310[4] - 0.0;//2;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[4] + 10.0;//7.5;
				else
					return Coeff_1550[4] - 0.6;//2;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[4] + 0.6;//2;
			}
			*/
			//==============================================================================================
			//New VOP-OPM silver
			if(lenght==0x31)
			{
				if(gain == 0x35)
					return Coeff_1310[4] + 13.1;
				if(gain == 0x34)
					return Coeff_1310[4] + 6.2;
				else
					return Coeff_1310[4] + 3.1;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[4] + 11.0;
				if(gain == 0x34)
					return Coeff_1550[4] + 4.0;
				if(gain == 0x33)
					return Coeff_1550[4] + 2.8;
				else
					return Coeff_1550[4] + 1.8;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[4] + 3.7;//2;
			}
		}
		case 0x34:
		{
			//==============================================================================================
			//production DWDM
			/*if(lenght==0x31)
			{
					//return Coeff_in_1310_4 + 0.4;
					return Coeff_1310[6] + 0.4;
			}
			if(lenght==0x32)
			{
					//return Coeff_in_1550_4 + 0.9;
					return Coeff_1550[6] + 0.4;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[6] + 1.0;//2;
			}*/
			
			//==============================================================================================
			//opm LR
			/*
			if(lenght==0x31)
			{
					return Coeff_1310[6] + 2;
			}
			if(lenght==0x32)
			{
					return Coeff_1550[6] + 1.4;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[6] + 2.3;//2;
			}
			*/
			
			//==============================================================================================
			//opm is our
			/*
			if(lenght==0x31)
			{
				if(gain == 0x35)
						return Coeff_1310[6] + 9.5;//7.5;
				else
						return Coeff_1310[6] + 0.4;//2;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[6] + 9.0;//7.5;
				else
					return Coeff_1550[6] - 1.0;//2;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[6] + 0.3;//2;
			}
			*/
			//==============================================================================================
			//New VOP-OPM silver
			if(lenght==0x31)
			{
				if(gain == 0x35)
					return Coeff_1310[6] + 12.5;
				if(gain == 0x34)
					return Coeff_1310[6] + 4.5;
				else
					return Coeff_1310[6] + 1.5;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[6] + 11.0;
				if(gain == 0x34)
					return Coeff_1550[6] + 4.0;
				else
					return Coeff_1550[6] + 1.3;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[6] + 1.8;//2;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------
float getCoefficientChannelOut(uint8_t number_channel,uint8_t lenght) //получение коэффициента выходного канала исходя из выбранного входного канала и длинны волны
{
	switch(number_channel)
	{
		case 0x31:
		{
			//==============================================================================================
			//production DWDM
			/*if(lenght==0x31)
			{
				//return Coeff_out_1310_1 - 0.6;//7.5;
				return Coeff_1310[1] - 0.6;//7.5;
			}
			if(lenght==0x32)
			{
				//return Coeff_out_1550_1 + 0;
				return Coeff_1550[1] + 0;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[1] + 0.4;//2;
			}*/
			
			//==============================================================================================
			//opm LR production
			/*
			if(lenght==0x31)
			{
					return Coeff_1310[1] + 2.0;//7.5;
			}
			if(lenght==0x32)
			{
					return Coeff_1550[1] + 1.7;//7.5;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[1] + 1.7;//2; //pri kalibr bilo [0]
			}
			*/
			
			//==============================================================================================
			//opm is our
			/*
			if(lenght==0x31)
			{
				if(gain == 0x35)
						return Coeff_1310[1] - 14.0;//7.5;
				else
						return Coeff_1310[1] - 4.3;//7.5;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[1] - 10.0;//7.5;
				else
					return Coeff_1550[1] + 0.0;//7.5;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[1] - 3.2;//2;
			}
			*/
			//==============================================================================================
			//New VOP-OPM silver
			if(lenght==0x31)
			{
				if(gain == 0x35)
					return Coeff_1310[1] - 9.0;//7.5;
				if(gain == 0x34)
					return Coeff_1310[1] - 1.8;//7.5;
				else
					return Coeff_1310[1] + 1.0;//7.5;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[1] - 8.3;
				if(gain == 0x34)
					return Coeff_1550[1] - 1.0;
				else
					return Coeff_1550[1] + 2.0;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[1] + 1.0;//2;
			}
		}
		case 0x32:
		{
			//==============================================================================================
			//production DWDM
			/*if(lenght==0x31)
			{
					//return Coeff_out_1310_2 + 0.4;
					return Coeff_1310[3] + 0.4;
			}
			if(lenght==0x32)
			{
					//return Coeff_out_1550_2 + 1.2;
					return Coeff_1550[3] + 1.2;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[3] + 1.2;//2;
			}*/
			
			//==============================================================================================
			//opm LR production
			/*if(lenght==0x31)
			{
					return Coeff_1310[3] + 1.8;
			}
			if(lenght==0x32)
			{
					return Coeff_1550[3] + 1.0;//2;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[3] + 1.6;//2;
			}*/
			
			//==============================================================================================
			//opm is our
			/*
			if(lenght==0x31)
			{
				if(gain == 0x35)
						return Coeff_1310[3] - 12.5;//7.5;
				else
						return Coeff_1310[3] - 2.7;//7.5;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[3] - 12.0;//7.5;
				else
					return Coeff_1550[3] - 1.9;//7.5;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[3] - 1.8;//2;
			}
			*/
			//==============================================================================================
			//New VOP-OPM silver
			if(lenght==0x31)
			{
				if(gain == 0x35)
					return Coeff_1310[3] - 9.0;
				if(gain == 0x34)
					return Coeff_1310[3] - 2.0;
				else
					return Coeff_1310[3] + 1.0;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[3] - 8.3;
				if(gain == 0x34)
					return Coeff_1550[3] - 1.0;
				else
					return Coeff_1550[3] + 1.6;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[3] + 1.4;//2;
			}
		}
		case 0x33:
		{
			//==============================================================================================
			//production DWDM
			/*if(lenght==0x31)
			{
					//return Coeff_out_1310_3 + 1.5;
					return Coeff_1310[5] + 1.5;
			}
			if(lenght==0x32)
			{
					//return Coeff_out_1550_3 + 1.2;
					return Coeff_1550[5] + 1.2;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[5] + 2.3;
			}*/
			
			//==============================================================================================
			//opm LR production
			/*if(lenght==0x31)
			{
					return Coeff_1310[5] + 2;
			}
			if(lenght==0x32)
			{
					return Coeff_1550[5] + 1.7;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[5] + 1.8;//2;
			}*/
			
			//==============================================================================================
			//opm is our
			/*
			if(lenght==0x31)
			{
				if(gain == 0x35)
						return Coeff_1310[5] - 13.0;//7.5;
				else
						return Coeff_1310[5] - 3.0;//7.5;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[5] - 12.0;//7.5;
				else
					return Coeff_1550[5] - 1.7;//7.5;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[5] - 2.0;//2;
			}
			*/
			//==============================================================================================
			//New VOP-OPM silver
			if(lenght==0x31)
			{
				if(gain == 0x35)
					return Coeff_1310[5] - 8.6;
				if(gain == 0x34)
					return Coeff_1310[5] - 1.6;
				else
					return Coeff_1310[5] + 1.0;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[5] - 8.3;
				if(gain == 0x34)
					return Coeff_1550[5] - 1.0;
				if(gain == 0x33)
					return Coeff_1550[5] + 1.0;
				else
					return Coeff_1550[5] + 2.0;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[5] + 1.3;
			}
		}
		case 0x34:
		{
			//==============================================================================================
			//production DWDM
			/*if(lenght==0x31)
			{
					//return Coeff_out_1310_4 + 1.5;
					return Coeff_1310[7] + 1.5;
			}
			if(lenght==0x32)
			{
					//return Coeff_out_1550_4 + 1.2;
					return Coeff_1550[7] + 1.2;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[7] + 2.2;
			}*/
			
			//==============================================================================================
			//opm LR production
			/*if(lenght==0x31)
			{
					return Coeff_1310[7] + 1.8;
			}
			if(lenght==0x32)
			{
					return Coeff_1550[7] + 1.7;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[7] + 1.7;//2;
			}*/
			
			//==============================================================================================
			//opm is our
			/*
			if(lenght==0x31)
			{
				if(gain == 0x35)
						return Coeff_1310[7] - 13.0;//7.5;
				else
						return Coeff_1310[7] - 3.3;//7.5;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[7] - 15.0;//7.5;
				else
					return Coeff_1550[7] - 5.3;//7.5;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[7] - 2.4;//2;
			}
			*/
			//=============================================
			//=================================================
			//New VOP-OPM silver
			if(lenght==0x31)
			{
				if(gain == 0x35)
					return Coeff_1310[7] - 9.5;
				if(gain == 0x34)
					return Coeff_1310[7] - 1.7;
				else
					return Coeff_1310[7] + 1.1;
			}
			if(lenght==0x32)
			{
				if(gain == 0x35)
					return Coeff_1550[7] - 8.3;
				if(gain == 0x34)
					return Coeff_1550[7] - 1.0;
				else
					return Coeff_1550[7] + 2.0;
			}
			if(lenght==0x33)
			{
					return Coeff_1310[7] + 1.5;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------
uint8_t getCoefficientPGA(uint8_t gainPGA) //получение коэффициента усиления программируемого усилителя
{
//----------------------------------------------------------------------------------------------------
	uint8_t data;
	switch(gainPGA)
	{
		case 0x01:
		{
			data=1;
			break;
		}
		case 0x02:
		{
			data=2;
			break;
		}
		default:break;
	}
	return data;
}

