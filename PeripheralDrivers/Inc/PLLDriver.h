/*
 * PLLDriver.h
 *
 *  Created on: Nov 14, 2022
 *      Author: RaphaelACH
 */

#include <stdio.h>

#ifndef PLLDRIVER_H_
#define PLLDRIVER_H_

#define VOLTAGE_64MHZ	0	// Frecuencia menor o igual a 64MHz
#define VOLTAGE_84MHZ	1	// Frecuencia menor o igual a 84MHz
#define VOLTAGE_100MHZ	2	// Frecuencia menor o igual a 100MHz

#define APB1_PRESCALER_2	0		// 50MHz
#define APB1_PRESCALER_4	1		// 25MHz
#define APB1_PRESCALER_8	2		// 12.5MHz
#define APB1_PRESCALER_16	3		// 6.25MHz

#define APB2_PRESCALER_0	0		// 100MHz
#define APB2_PRESCALER_2	1		// 50MHz
#define APB2_PRESCALER_4	2		// 25MHz
#define APB2_PRESCALER_8	3		// 12.5MHz
#define APB2_PRESCALER_16	4		// 6.25MHz


typedef struct
{
	uint8_t PLL_voltage;
	uint8_t	APB1_prescaler;
	uint8_t	APB2_prescaler;
}PLL_Config_t;

typedef struct
{
	RCC_TypeDef		*ptrPLL;
	PLL_Config_t	PLL_Config;
}PLL_Handler_t;


// Definicion de cabeceras para funciones del PLL
void PLL_Config(PLL_Handler_t *ptrPLLHandler);

#endif /* PLLDRIVER_H_ */
