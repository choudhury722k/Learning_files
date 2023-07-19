/*
 * main.c
 *
 *  Created on: 19-Dec-2022
 *      Author: SOUMYA
 */


#include "F446RE.h"

int main(void){
	return 0;
}

void EXTI0_IRQHandler(void){
	// handle the interrupt
	GPIO_IRQHandling(0);
}
