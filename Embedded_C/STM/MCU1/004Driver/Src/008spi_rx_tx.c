/*
 * 008spi_rx_tx.c
 *
 *  Created on: 19-Dec-2022
 *      Author: SOUMYA
 */

#include "F446RE.h"
#include "spi_driver.h"
#include "gpio_driver.h"

// Command Codes
#define COMMAND_LED_CTRL      0x50
#define COMMAND_SENSOR_READ   0x51
#define COMMAND_LED_READ      0x52
#define COMMAND_PRINT         0x53
#define COMMAND_ID_READ       0x54

#define LED_ON                1
#define LED_OFF               0

#define ANALOG_PIN0           0
#define ANALOG_PIN1           1
#define ANALOG_PIN2           2
#define ANALOG_PIN3           3
#define ANALOG_PIN4           4

#define LED_PIN               9

void SPI2_GPIOInits(){
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    //MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits(void){
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;

	SPI_Init(&SPI2handle);
}

void GPIO_ButtonInit(void){
	GPIO_Handle_t GPIOBtn;

	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);
}

uint8_t SPI_VerifyResponse(uint8_t ackbyte){
	if(ackbyte == 0xF5){
		// ACK
		return 1;
	}else{
		// NACK
		return 0;
	}
}

void delay(void){
	for(uint32_t i = 0; i<500000; i++);
}

int main(void){
	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;

	GPIO_ButtonInit();

	// This function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();
	// This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	/*
	 *  Making SSOE 1 does NSS output enable.
	 *  The NSS pin is automatically manages by the hardware
	 *  i.e when SPE=1, NSS will be pulled to low
	 *  and NSS pin will be high when SPE=0
	 */
//	SPI_SSOEConfig(SPI2, ENABLE);

	while(1){
		while(! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

		delay();

		// Enable the SPI2 peripherals
		SPI_PeripheralControl(SPI2, ENABLE);

		// 1. CMD_LED_CTRL <pin no> <value>
		uint8_t commandcode = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t args[2];
		SPI_SendData(SPI2, &(commandcode), 1);
		// Do Dummy read here
		SPI_RecieveData(SPI2, &dummy_read, 1);
		// Send some dummy bits to fetch the response from the slave
		SPI_SendData(SPI2, &(dummy_write), 1);
		SPI_RecieveData(SPI2, &ackbyte, 1);
		if(SPI_VerifyResponse(ackbyte)){
			// Send Arguments
			args[0] = LED_PIN;
			args[1] = LED_ON;
			SPI_SendData(SPI2, args, 2);
		}

		// 2. CMD_SENSOR_READ <analog pin number>
		while(! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
        delay();
        commandcode = COMMAND_SENSOR_READ;
        SPI_SendData(SPI2, &(commandcode), 1);
		// Do Dummy read here
		SPI_RecieveData(SPI2, &dummy_read, 1);
		// Send some dummy bits to fetch the response from the slave
		SPI_SendData(SPI2, &(dummy_write), 1);
		SPI_RecieveData(SPI2, &ackbyte, 1);
		if(SPI_VerifyResponse(ackbyte)){
			// Send Arguments
			args[0] = ANALOG_PIN0;
			SPI_SendData(SPI2, args, 1);

			// Do Dummy read here
			SPI_RecieveData(SPI2, &dummy_read, 1);
			// Insert some delay so that slave can receive some data
			delay();
			SPI_SendData(SPI2, &(dummy_write), 1);
			uint8_t analog_read;
			SPI_RecieveData(SPI2, &analog_read, 1);
		}






		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

		// Disable the SPI2 peripherals
		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;
}
