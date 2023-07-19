/*
 * spi_driver.c
 *
 *  Created on: 19-Dec-2022
 *      Author: SOUMYA
 */

#include "F446RE.h"
#include "spi_driver.h"

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_interrupt_handle(SPI_Handle_t *pSPIHandle);

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pSPIx == SPI1){
			SPI1_PCLK_EN();
		}else if(pSPIx == SPI2){
			SPI2_PCLK_EN();
		}else if(pSPIx == SPI3){
			SPI3_PCLK_EN();
		}
	}
	else{
		if(pSPIx == SPI1){
			SPI1_PCLK_DI();
		}else if(pSPIx == SPI2){
			SPI2_PCLK_DI();
		}else if(pSPIx == SPI3){
			SPI3_PCLK_DI();
		}
	}
}

void SPI_Init(SPI_Handle_t *pSPIHandle){
	uint32_t tempreg = 0;

	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	// 1. Configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

	// 2. Configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD){
		// BIDI mode should be cleared
		tempreg &= ~(1 << 15);
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD){
		// BIDI mode should be set
		tempreg |= (1 << 15);
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY){
		// BIDI mode should be cleared
		tempreg &= ~(1 << 15);
		// BIDI mode should be set
		tempreg |= (1 << 10);
	}

	// 3. Configure the SPI serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	// 4. Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << 11;

	// 5. Configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << 1;

	// 6. Configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	pSPIHandle->pSPIx->CR1 = tempreg;

}
void SPI_DeInit(SPI_RegDef_t *pSPIx);

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName){
	if(pSPIx->SR & FlagName){
		return FLAG_SET;
	}
	return FLAG_RESET;
}
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len){
	while(Len > 0){
		// 1. wait until TXE is set
		// while(!(pSPIx->SR & (1 << 1)));
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		// 2. check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
			// 16bit
			// Load the data into data register
			pSPIx->DR = *((uint16_t *)pTxBuffer);
			Len--;
			Len--; // because we have sent 2 bits of data
			(uint16_t *)pTxBuffer++;
		}else{
			// 8bit
			pSPIx->DR = *(pTxBuffer);
			Len--; // because we have sent 1 bits of data
			pTxBuffer++;
		}
	}
}
void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
	while(Len > 0){
		// 1. wait until TXE is set
		// while(!(pSPIx->SR & (1 << 1)));
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		// 2. check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
			// 16bit
			// Load the data into data register to Rx buffer
			*((uint16_t *)pRxBuffer) = pSPIx->DR;
			Len--;
			Len--; // because we have sent 2 bits of data
			(uint16_t *)pRxBuffer++;
		}else{
			// 8bit
			*pRxBuffer = pSPIx->DR;
			Len--; // because we have sent 1 bits of data
			pRxBuffer++;
		}
	}
}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}else{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}else{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}else{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len){
	uint8_t state = pSPIHandle->TxState;
	if(state != SPI_BUSY_IN_TX){
		// 1. Save the Tx buffer address and Len info in some global variables
		pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen;
		// 2. Mark the SPI state as busy in transmission so that
		// no other code can take over same SPI peripheral until transmission is over
		pSPIHandle->TxState - SPI_BUSY_IN_TX;
		// 3. Enable the TXEIE control bit to get interrupt whenevr TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
        // 4. Data Transmission will be handled by ISR code
	}
	return state;
}
uint8_t SPI_RecieveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len){
	uint8_t state = pSPIHandle->RxState;
	if(state != SPI_BUSY_IN_RX){
		// 1. Save the Tx buffer address and Len info in some global variables
		pSPIHandle->pRxBuffer;
		pSPIHandle->RxLen;
		// 2. Mark the SPI state as busy in transmission so that
		// no other code can take over same SPI peripheral until transmission is over
		pSPIHandle->RxState - SPI_BUSY_IN_RX;
		// 3. Enable the TXEIE control bit to get interrupt whenevr TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
		// 4. Data Transmission will be handled by ISR code
	}
	return state;
}

void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(IRQNumber <= 31)	{
			//program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER3 |= ( 1 << (IRQNumber % 64) );
		}
	}else{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 6 && IRQNumber < 96 )
		{
			//program ICER2 register
			*NVIC_ICER3 |= ( 1 << (IRQNumber % 64) );
		}
	}
}
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;

	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}

void SPI_IRQHandlingConfig(SPI_Handle_t *pHandle){
	uint8_t temp1, temp2;
	// First lets check for TXE
	temp1 = pHandle->pSPIx->SR &(1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 &(1 << SPI_CR2_TXEIE);
	if(temp1 && temp2){
		// Handle TXE
		spi_txe_interrupt_handle(pHandle);
	}

	// Second lets check for RXNE
	temp1 = pHandle->pSPIx->SR &(1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 &(1 << SPI_CR2_RXNEIE);
	if(temp1 && temp2){
		// Handle RXNE
		spi_rxne_interrupt_handle(pHandle);
	}

	// Third lets check for RXNE
	temp1 = pHandle->pSPIx->SR &(1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 &(1 << SPI_CR2_ERRIE);
	if(temp1 && temp2){
		// Handle OVR ERROR
		spi_ovr_interrupt_handle(pHandle);
	}
}

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle){
	// Check the DFF bit in CR1
	if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){
		// 16bit
		// Load the data into data register
		pSPIHandle->pSPIx->DR = *((uint16_t *)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--; // because we have sent 2 bits of data
		(uint16_t *)pSPIHandle->pTxBuffer++;
	}else{
		// 8bit DFF
		pSPIHandle->pSPIx->DR = *(pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--; // because we have sent 1 bits of data
		pSPIHandle->pTxBuffer++;
	}
	if(! pSPIHandle->TxLen){
		// TxLen is zero, close the SPI communication and inform the application that
		// Tx is over
		// This prevents interrupts from setting up of TXE flags
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}
}
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle){
	// Check the DFF bit in CR1
	if(pSPIHandle->pSPIx->CR1 & (1 << 11)){
		// 16bit
		// Load the data into data register
		*((uint16_t *)pSPIHandle->pRxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->RxLen--; // because we have sent 2 bits of data
		pSPIHandle->pRxBuffer--;
		pSPIHandle->pRxBuffer--;
	}else{
		// 8bit DFF
		*(pSPIHandle->pRxBuffer) = (uint8_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--; // because we have sent 1 bits of data
		pSPIHandle->pRxBuffer--;
	}
	if(! pSPIHandle->RxLen){
		// Reception is complete
		// Lets turn off the rxneie interrupt
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}
}
static void spi_ovr_interrupt_handle(SPI_Handle_t *pSPIHandle){
	// 1. Clear the OVR Flag
	uint8_t temp;
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX){
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;
	// 2. Inform the application
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

void SPI_CloseTransmission(SPI_Handle_t *pHandle){
	pHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pHandle->pTxBuffer = NULL;
	pHandle->TxLen = 0;
	pHandle->TxState = SPI_READY;
}
void SPI_CloseReception(SPI_Handle_t *pHandle){
	pHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pHandle->pRxBuffer = NULL;
	pHandle->RxLen = 0;
	pHandle->RxState = SPI_READY;
}
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx){
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pHandle, uint8_t AppEv){

}

