/**
  ******************************************************************************
  * File Name          : main.hpp
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//----------------------------------------------------------------------------------------------------
#include "stm32f7xx_hal.h"
//----------------------------------------------------------------------------------------------------
#include "stdint.h"
//----------------------------------------------------------------------------------------------------
#include <math.h>
//----------------------------------------------------------------------------------------------------
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
void table_and_0_point(uint8_t lenght, uint8_t number_channel);

void Init_Analog(void);

void enablyChannel(uint8_t number_channel);

uint16_t adc_read (void);

void read_dBm(uint8_t number_channel, uint8_t lenght, float dBm);

double getCoefficientR(uint8_t gain);

double getCoefficientLenght(uint8_t lenght);

float getCoefficientChannelIn(uint8_t number_channel ,uint8_t lenght);

float getCoefficientChannelOut(uint8_t number_channel ,uint8_t lenght);

uint8_t getCoefficientPGA(uint8_t gainPGA);

void gain_1(void);
void gain_2(void);
void gain_3(void);
void gain_4(void);
void gain_5(void);

void PGA112 (char value);

void channel_1(void);
void channel_2(void);
void channel_3(void);
void channel_4(void);

void dac_write(float dBm, uint8_t lenght);

#define DA1_CLRn_Pin GPIO_PIN_3
#define DA1_CLRn_GPIO_Port GPIOE
#define DAC_CS_Pin GPIO_PIN_4
#define DAC_CS_GPIO_Port GPIOE
#define TFT_CS_Pin GPIO_PIN_6
#define TFT_CS_GPIO_Port GPIOF
#define PGA_CS_Pin GPIO_PIN_4
#define PGA_CS_GPIO_Port GPIOA
#define LED_HL2_Pin GPIO_PIN_8
#define LED_HL2_GPIO_Port GPIOA
#define LED_HL3_Pin GPIO_PIN_10
#define LED_HL3_GPIO_Port GPIOA
#define M1_ENN_Pin GPIO_PIN_13
#define M1_ENN_GPIO_Port GPIOH
#define M1_A0_Pin GPIO_PIN_14
#define M1_A0_GPIO_Port GPIOH
#define M1_A1_Pin GPIO_PIN_15
#define M1_A1_GPIO_Port GPIOH
#define ADC_CS_Pin GPIO_PIN_0
#define ADC_CS_GPIO_Port GPIOI

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
