#include "main.h"
//----------------------------------------------------------------------------------------------------
extern uint16_t adc_value;
extern float mV;
uint8_t gainPGA = 1;
uint8_t gain = 0x31;
extern float dBm_in;
extern float dBm_out;
float mV_test;
uint8_t Calib_dBm_in = 0;
uint8_t Calib_dBm_out = 0;
uint8_t dBm_in_old = 0;
uint8_t dBm_out_old = 0;
//----------------------------------------------------------------------------------------------------
	void read_dBm(uint8_t number_channel, uint8_t lenght, float dBm)	
	{
		//=======================================ADC READ=========================================	
		
		adc_value = adc_read();	//получение значения с ADC
		
		mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000)); //мощность на выходе 
		
		/*if(gain == 0x35 && mV < 100)
		{
			gain_1();
			//HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}*/
		
		
		if(gain == 0x31 && mV < 100)
		{
			gain_2();
			HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}
		if(gain == 0x32 && mV < 100)
		{
			gain_3();
			HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}
		if(gain == 0x33 && mV < 100)
		{
			gain_4();
			HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}
		if(gain == 0x34 && mV < 100)
		{
			PGA112(49); //максимальный коэффициент усиления программируемого усилителя
			HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
			gain=0x36;
		}
		
		
		/*if(gain == 0x34 && mV < 100)
		{
			gain_5();
			//HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}*/
		/*if(gain == 0x35 && mV > 2000)
		{
			gain_4();
			//HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}*/
		if(gain == 0x36 && mV > 2200)
		{
			PGA112(17);
			HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
			gain=0x34;
		}
		if(gain == 0x34 && mV > 2200)
		{
			gain_3();
			HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}
		if(gain == 0x33 && mV > 2200)
		{
			gain_2();
			HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}
		if(gain == 0x32 && mV > 2200)
		{
			gain_1();
			HAL_Delay(500);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}
		
		
		
		/*if(gainPGA == 1 && gain == 0x31 && mV < 100) //если условие выполнилось, то пересчитываем усиленине и считываем выходную мощность 
		{ 
			PGA112(17);//*2
			gainPGA = 2;
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}	
		if(gainPGA == 2 && gain == 0x31 && mV < 100)
		{
			PGA112(1);//*1
			gainPGA = 1;
			gain_2();
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}
		if(gainPGA == 1 && gain == 0x32 && mV < 100)
		{
			PGA112(17);//*2
			gainPGA = 2;
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}
		if(gainPGA == 2 && gain == 0x32 && mV < 100)
		{
			PGA112(1);//*2
			gainPGA = 1;
			gain_3();
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}
		
		if(gainPGA == 1 && gain == 0x33 && mV < 100)
		{
			PGA112(17);//*2
			gainPGA = 2;
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}	
		if(gainPGA == 2 && gain == 0x33 && mV < 100)
		{
			PGA112(1);//*2
			gainPGA = 1;
			gain_4();
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}	
		if(gainPGA == 1 && gain == 0x34 && mV < 100)
		{
			PGA112(17);//*2
			gainPGA = 2;
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}	
		if(gainPGA == 2 && gain == 0x34 && mV < 100)
		{
			PGA112(1);//*2
			gainPGA = 1;
			gain_5();
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}	
		if(gain == 0x35 && mV < 3.5) //первый усилительный каскад не работает
		{
			PGA112(113); //максимальный коэффициент усиления программируемого усилителя
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
			gain=0x36;
		}		
		if(gain == 0x36 && mV > 2000)
		{
			PGA112(1);
			gainPGA = 1;
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
			gain=0x35;
		}				
		if(gainPGA == 1 && gain == 0x35 && mV > 2000)
		{
			PGA112(17);//*2
			gainPGA = 2;
			gain_4();
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}
		if(gainPGA == 2 && gain == 0x34 && mV > 2000)
		{
			PGA112(1);
			gainPGA = 1;
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}		
		if(gainPGA == 1 && gain == 0x34 && mV > 2000)
		{
			PGA112(17);//*2
			gainPGA = 2;
			gain_3();
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}		
		if(gainPGA == 2 && gain == 0x33 && mV > 2000)
		{
			PGA112(1);
			gainPGA = 1;
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}	
		if(gainPGA == 1 && gain == 0x33 && mV > 2000)
		{
			PGA112(17);//*2
			gainPGA = 2;
			gain_2();
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}	
		if(gainPGA == 2 && gain == 0x32 && mV > 2000)
		{
			PGA112(1);
			gainPGA = 1;
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}			
		if(gainPGA == 1 && gain == 0x32 && mV > 2000)
		{
			PGA112(17);//*2
			gainPGA = 2;
			gain_1();
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}	
		if(gainPGA == 2 && gain == 0x31 && mV > 2000)
		{
			PGA112(1);
			gainPGA = 1;
			HAL_Delay(5);
			adc_value = adc_read();	
			mV = (float)((int32_t)(((adc_value * 3.3)/65535)*1000));
		}*/
		
		if(gain == 0x36)
		{
			dBm_in = 10*(log10(adc_value*3.3*1000/(128*getCoefficientR(0x35)*getCoefficientLenght(lenght)*65535))) + getCoefficientChannelIn(number_channel,lenght) + dBm;
			dBm_out = 10*(log10(adc_value*3.3*1000/(128*getCoefficientR(0x35)*getCoefficientLenght(lenght)*65535))) - getCoefficientChannelOut(number_channel,lenght);
		}
		else
		{			
			dBm_in = 10*(log10(adc_value*3.3*1000/(getCoefficientPGA(gainPGA)*getCoefficientR(gain)*getCoefficientLenght(lenght)*65535))) + getCoefficientChannelIn(number_channel,lenght) + dBm;
			dBm_out = 10*(log10(adc_value*3.3*1000/(getCoefficientPGA(gainPGA)*getCoefficientR(gain)*getCoefficientLenght(lenght)*65535))) - getCoefficientChannelOut(number_channel,lenght);	
			mV_test = getCoefficientR(gain);
		}

		if(mV <= 30 && gain == 0x34)
		{
			dBm_in = -60;
			dBm_out = -60;
		}
	}
