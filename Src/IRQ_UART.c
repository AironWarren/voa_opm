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
#define rx_buffer_size 500
//----------------------------------------------------------------------------------------------------
extern uint8_t firstByteWait;
extern uint16_t rx_buffer_len;
extern char str_rx[rx_buffer_size];
extern UART_HandleTypeDef huart2;
HAL_UART_StateTypeDef status;

void UART_IDLE_Callback(UART_HandleTypeDef *huart);
//----------------------------------------------------------------------------------------------------
/*void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart) 
{
	if(huart == &huart2) 
	{ //проверка, из нужного ли uart пришли данные
		//__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);
		//rx_buffer_len = rx_buffer_size - huart->RxXferCount;
		
		firstByteWait=2;
		
		//HAL_UART_AbortReceive_IT(&huart2);
		//__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		//__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
		HAL_UART_Receive_IT(&huart2, str_rx, rx_buffer_size);
	}
}*/
//----------------------------------------------------------------------------------------------------
void UART_IDLE_Callback(UART_HandleTypeDef *huart)
{
	uint32_t tmp1, tmp2;
	
	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE);
	
	if((tmp1 != RESET) && (tmp2 != RESET))
  {
		
    __HAL_UART_CLEAR_IDLEFLAG(huart);
		
		huart->gState = HAL_UART_STATE_READY;	
		
		__HAL_UNLOCK(huart->hdmarx);
    if(huart->Instance == USART2)
    {
      /* get rx data len */
			
			__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);
			//rx_buffer_len = rx_buffer_size - huart->RxXferCount;
			
			firstByteWait=2;
			
			//HAL_UART_AbortReceive_IT(&huart2);
			__HAL_UART_CLEAR_IDLEFLAG(&huart2);
			__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
      status = HAL_UART_Receive_IT(huart,str_rx,rx_buffer_size);		
		}
	}
}