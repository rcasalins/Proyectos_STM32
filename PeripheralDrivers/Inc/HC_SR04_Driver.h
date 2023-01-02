/*
 * HC_SR04_Driver.h
 *
 *  Created on: Nov 23, 2022
 *      Author: rafaelach
 */

#ifndef HC_SR04_DRIVER_H_
#define HC_SR04_DRIVER_H_

#include "stm32f4xx.h"

#define HCSR04_CHANNEL_1	0
#define HCSR04_CHANNEL_2	1
#define HCSR04_CHANNEL_3	2
#define HCSR04_CHANNEL_4	3

#define HCSR04_RISING_EDGE		0
#define HCSR04_FALLING_EDGE	1

#define HCSR04_PRESCALER_1_1	0

#define HCSR04_TIMER_SPEED_1us			16
#define HCSR04_TIMER_SPEED_10us			160
#define HCSR04_TIMER_SPEED_100us		1600
#define HCSR04_TIMER_SPEED_1ms			16000

/*
 * Estructura para configurar los valores de captura
 */
typedef struct
{
	uint8_t		channel;			// Canal de captura relacionado con el timer
	uint32_t	timerSpeed;			// velocidad de incremento del timer
//	uint32_t	prescalerCapture;	// Numero de periodos leidos
}HC_SR04_Config_t;

/*
 * Estructura para el handler de la captura
 */
typedef struct
{
	TIM_TypeDef			*ptrTIMx;		// Timer al que esta asociado la captura
	HC_SR04_Config_t	config;		// Configuracion inicial de la captura
}HC_SR04_Handler_t;

/*
 * Cabeceras de las funciones
 */
void HCSR04_Config(HC_SR04_Handler_t *ptrHCSR04Handler);
//uint32_t getDutty(HC_SR04_Handler_t *ptrHCSR04Handler);
void CaptureTimer2_Channel1_Callback(void);
void CaptureTimer2_Channel2_Callback(void);
void CaptureTimer2_Channel3_Callback(void);
void CaptureTimer2_Channel4_Callback(void);
void CaptureTimer3_Channel1_Callback(void);
void CaptureTimer3_Channel2_Callback(void);
void CaptureTimer3_Channel3_Callback(void);
void CaptureTimer3_Channel4_Callback(void);
void CaptureTimer4_Channel1_Callback(void);
void CaptureTimer4_Channel2_Callback(void);
void CaptureTimer4_Channel3_Callback(void);
void CaptureTimer4_Channel4_Callback(void);
void CaptureTimer5_Channel1_Callback(void);
void CaptureTimer5_Channel2_Callback(void);
void CaptureTimer5_Channel3_Callback(void);
void CaptureTimer5_Channel4_Callback(void);
void stopHCSR04(HC_SR04_Handler_t *ptrHCSR04Handler);
void startHCSR04(HC_SR04_Handler_t *ptrHCSR04Handler);

#endif /* HC_SR04_DRIVER_H_ */
