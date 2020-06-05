/*
* c_serial_avr_328.cpp
*
* Created: 2/27/2019 10:45:57 AM
* Author: jeff_d
*/


#include "../../../../talos_hardware_def.h"
#include "c_serial_stm32h7.h"
#include <stm32h7xx_hal.h>
#include <stm32_hal_legacy.h>

#define HOST_USART USART1

namespace Hardware_Abstraction_Layer
{
	c_ring_buffer <char>* Serial::host_ring_buffer;

	static UART_HandleTypeDef s_HostUARTHandle = UART_HandleTypeDef();

	static bool host_com_init = false;

	void Serial::initialize(uint8_t Port, c_ring_buffer<char>* buffer)
	{
		switch (Port)
		{
		case HOST_CPU_ID:
			if (!host_com_init) __init_host_comm(buffer);
			break;
		case CORD_CPU_ID:
			//if (!host_com_init) __init_host_comm();
			break;
		case MACH_CPU_ID:
			//if (!host_com_init) __init_host_comm();
			break;
		case SPDL_CPU_ID:
			//if (!host_com_init) __init_host_comm();
			break;
		case PRPH_CPU_ID:
			//if (!host_com_init) __init_host_comm();
			break;
		default:
			break;
		}
	}

	void Serial::__init_host_gpio()
	{
		__GPIOA_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.Pin = GPIO_PIN_9;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Alternate = GPIO_AF7_USART3;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.Pin = GPIO_PIN_10;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	static uint8_t byte = 0;
	void Serial::__init_host_comm(c_ring_buffer<char>* buffer)
	{
		//setup the input buffer
		Serial::host_ring_buffer = buffer;

		__init_host_gpio();

		__USART1_CLK_ENABLE();
		s_HostUARTHandle.Instance = HOST_USART;
		s_HostUARTHandle.Init.BaudRate = 115200;
		s_HostUARTHandle.Init.WordLength = UART_WORDLENGTH_8B;
		s_HostUARTHandle.Init.StopBits = UART_STOPBITS_1;
		s_HostUARTHandle.Init.Parity = UART_PARITY_NONE;
		s_HostUARTHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		s_HostUARTHandle.Init.Mode = UART_MODE_TX_RX;
		s_HostUARTHandle.Init.OverSampling = UART_OVERSAMPLING_16;

		if (HAL_UART_Init(&s_HostUARTHandle) != HAL_OK)
			asm("bkpt 255");

		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);

		host_com_init = true;

		//HAL_UART_Receive_IT(&s_UARTHandle, &byte, 1);

		SET_BIT(HOST_USART->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE_RXFNEIE);
	}


	/////* This callback is called by the HAL_UART_IRQHandler when the given number of bytes are received */
	//void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
	//{
	//	if (huart->Instance == USART3)
	//	{
	//		/* Transmit one byte with 100 ms timeout */
	//		//HAL_UART_Transmit(&s_UARTHandle, &byte, 1, 100);
	//
	//		/* Receive one byte in interrupt mode */
	//		HAL_UART_Receive_IT(&s_HostUARTHandle, &byte, 1);
	//	}
	//}

	uint8_t Serial::send(uint8_t Port, uint8_t* byte)
	{

		switch (Port)
		{
		case HOST_CPU_ID:
			if (host_com_init)
			{
				//Wait until we are not transmitting
				while (!(s_HostUARTHandle.Instance->ISR & UART_FLAG_TC));
				s_HostUARTHandle.Instance->TDR = (uint8_t)(*byte);
			}
			break;
		case CORD_CPU_ID:
			//if (!host_com_init) __init_host_comm();
			break;
		case MACH_CPU_ID:
			//if (!host_com_init) __init_host_comm();
			break;
		case SPDL_CPU_ID:
			//if (!host_com_init) __init_host_comm();
			break;
		case PRPH_CPU_ID:
			//if (!host_com_init) __init_host_comm();
			break;
		default:
			break;
		}
	}
};

/* UART3 Interrupt Service Routine */
#ifdef __cplusplus
extern "C"
#endif
void USART3_IRQHandler()
{
	//HAL_UART_IRQHandler(&s_UARTHandle);
	uint32_t IIR = HOST_USART->ISR;   //<--flag clears when we read the ISR
	if (IIR & USART_FLAG_RXNE) //<-- check to see if this is an RXNE (read data register not empty
	{
		// read interrupt
		char byte = HOST_USART->RDR;
		Hardware_Abstraction_Layer::Serial::host_ring_buffer->put(byte);
		////USART3->ISR &= ~USART_FLAG_RXNE;	          // clear interrupt
		//uint8_t buffer[] = "XX";
		//HAL_UART_Transmit(&s_HostUARTHandle, buffer, sizeof(buffer), HAL_MAX_DELAY);
	}

}