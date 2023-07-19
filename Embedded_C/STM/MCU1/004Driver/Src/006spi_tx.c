/*
 * main.c
 *
 *  Created on: 19-Dec-2022
 *      Author: SOUMYA
 */

/*
 *  PB14 --> SPI2_MISO
 *  PB15 --> SPI2_MOSI
 *  PB13 --> SPI2_SCLK
 *  PB12 --> SPI2_NSS
 *  ALT Function mode : 5
 */

#include "F446RE.h"
#include "spi_driver.h"
#include "gpio_driver.h"

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
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);

	//NSS
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	//GPIO_Init(&SPIPins);
}

void SPI2_Inits(void){
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2handle);
}

int main(void){
	char user_data[] = "Hello World";

	// This function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();
	// This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	// This makes NSS signal internally high and avoids MODF error
	SPI_SSIConfig(SPI2, ENABLE);

	// Enable the SPI2 peripherals
	SPI_PeripheralControl(SPI2, ENABLE);

	SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

	// Disable the SPI2 peripherals
	SPI_PeripheralControl(SPI2, DISABLE);

	while(1);

	return 0;
}
