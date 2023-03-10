/*
 * PLLDriver.c
 *
 *  Created on: Nov 14, 2022
 *      Author: RaphaelACH
 */

#include <stm32f4xx.h>
#include "PLLDriver.h"

void PLL_Config(PLL_Handler_t *ptrPLLHandler){
//
//	// Se habilita el HSI
	ptrPLLHandler->ptrPLL->CR |= RCC_CR_HSION;

//	// Esperamos que el HSE esta activando verificando el valor del HSE RDY
	while( !(RCC->CR & RCC_CR_HSIRDY) );

	// Se activa el Power Enable Clock desde el APB1
	RCC->APB1ENR |= RCC_APB1RSTR_PWRRST;


	/*
	 * Intermedio
	 * Data cache enable
	 * Instruction cache enable
	 * Prefetch enable
	 */
	FLASH->ACR |= 1<<8;
	FLASH->ACR |= 1<<9;
	FLASH->ACR |= 1<<10;


	/*
	 * Power Selection y Wait State (WS) configuration
	 * Se le asigna el bit de potencia dependiendo de la frecuencia maxima qu se use en el micro
	 * Para nuestro caso que se esta usando 100MZ el usuario deberá seleccionar VOLTAGE_100MHZ
	 * Además, se asigna el numero de Wait States dependiendo de la frecuencia maxima que este usando
	 * el usuario, dejando un rango de tiempo un poco el necesario, se asignaron 1 wait state para
	 * menos de 64MHz, 3 para menos 84MHz y 5 para 100Mhz
	 */

	// Le doy al micro el voltaje necesario dependiendo de la freceuncia con la que trabaje
	if(ptrPLLHandler->PLL_Config.PLL_voltage == VOLTAGE_64MHZ){
		// Se escribe 01 en los bit 14 y 15 del PWR_CR para obtener la potencia necesaria con
		// menos de 64MHz
		PWR->CR |= 01<<14;

		// Wait State configuration
		FLASH->ACR |= 1<<0;
	}
	else if(ptrPLLHandler->PLL_Config.PLL_voltage == VOLTAGE_84MHZ){
		// Se escribe 10 en los bit 14 y 15 del PWR_CR para obtener la potencia necesaria con
		// menos de 84MHz
		PWR->CR |= 10<<14;

		// Wait State configuration
		FLASH->ACR |= 3<<0;
	}
	else if(ptrPLLHandler->PLL_Config.PLL_voltage == VOLTAGE_100MHZ){
		// Se escribe 11 en los bit 14 y 15 del PWR_CR para obtener la potencia necesaria con
		// menos de 100MHz
		PWR->CR |= 11<<14;

		// Wait State configuration
		FLASH->ACR |= 5<<0;
	}
	else{
		// Se usa toda potencia por defecto
		PWR->CR |= 11<<14;

		// Wait State configuration
		FLASH->ACR |= 5<<0;
	}

	/*
	 * HSE Prescaler null, para 100 MHz
	 * El prescaler para el HSE fue de 1, para así obtener 100MHz en la salida
	 */
	RCC->CFGR |= 0001<<4;

	/*
	 * APB1 frecuencia
	 * Se dispone para el usuario 4 prescalers distintos, teniendo en cuenta que el APB1 puede tener
	 * como maximo 50MHz, entonces el prescaler minimo es de 2.
	 */
	if(ptrPLLHandler->PLL_Config.APB1_prescaler == APB1_PRESCALER_2){
		// 50MHz
		RCC->CFGR |= 100<<10;
	}
	else if(ptrPLLHandler->PLL_Config.APB1_prescaler == APB1_PRESCALER_4){
		// 25MHz
		RCC->CFGR |= 101<<10;
	}
	else if(ptrPLLHandler->PLL_Config.APB1_prescaler == APB1_PRESCALER_8){
		// 12.5MHz
		RCC->CFGR |= 110<<10;
	}
	else if(ptrPLLHandler->PLL_Config.APB1_prescaler == APB1_PRESCALER_16){
		// 6.25MHz
		RCC->CFGR |= 111<<10;
	}
	else{
		// defaulf 50MHz
		RCC->CFGR |= 100<<10;
	}

	/*
	 * APB2 frecuencia
	 * Se dispone para el usuario 5 prescalers, donde el prescaler minimo es nulo, es decir,
	 * no realiza nada sobre la frecuencia original
	 */
	if(ptrPLLHandler->PLL_Config.APB2_prescaler == APB2_PRESCALER_0){
		// 100MHz
		RCC->CFGR |= 001<<13;
	}
	if(ptrPLLHandler->PLL_Config.APB2_prescaler == APB2_PRESCALER_2){
		// 50MHz
		RCC->CFGR |= 100<<13;
	}
	else if(ptrPLLHandler->PLL_Config.APB2_prescaler == APB2_PRESCALER_4){
		// 25MHz
		RCC->CFGR |= 101<<13;
	}
	else if(ptrPLLHandler->PLL_Config.APB2_prescaler == APB2_PRESCALER_8){
		// 12.5MHz
		RCC->CFGR |= 110<<13;
	}
	else if(ptrPLLHandler->PLL_Config.APB2_prescaler == APB2_PRESCALER_16){
		// 6.25MHz
		RCC->CFGR |= 111<<13;
	}
	else{
		// defaulf 50MHz
		RCC->CFGR |= 100<<10;
	}


	/*
	 * Se configura el PLL para que al tener como input el HSI se obtengan 100MHz
	 */

	// Se configura el PLL para que su fuente sea el HSI
	RCC->PLLCFGR |= (0 << 22);

	// Se divide la frecuencia por 8, obteniendo 2MHz (PLLM)
	RCC->PLLCFGR |= (001000 << 0);

	// Se multiplica la frecuencia por 100, obteniendo 200MHz (PLLN)
	RCC->PLLCFGR |= (001100100 << 6);

	// Factor de division para reloj principal (PLLP), se divide entre 2, obteniendo 100MHz
	RCC->PLLCFGR |= (00 << 16);



	/*
	 * Se activa el PLL para que se cargue la configuración anterior
	 */

	// Se enciende el PLL desde su registro
	RCC->CR |= (1<<24);

	// Se espera a a que el PLL este activando, revisando el registro que lo indica
	while (!(RCC->CR & (1<<25)));

	// Seleccionamos el clock source como PLL que corresponde a poner un 10 (un 2) en los primeros
	// bits del registro CFGR
	RCC->CFGR |= (2<<0);

	// Esperamos a que el PLL este configurado como source esperando que el bit que lo indica este listo
	while (!(RCC->CFGR & (2<<2)));

}

