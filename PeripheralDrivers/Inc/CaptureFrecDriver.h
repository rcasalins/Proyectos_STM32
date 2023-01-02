/*
 * CaptureFrecDriver.h
 *
 *  Created on: 19/11/2022
 *      Author: Rafael Casalins
 */

#ifndef CAPTUREFRECDRIVER_H_
#define CAPTUREFRECDRIVER_H_

#include "stm32f4xx.h"

#define CAPTURE_CHANNEL_1	0
#define CAPTURE_CHANNEL_2	1
#define CAPTURE_CHANNEL_3	2
#define CAPTURE_CHANNEL_4	3

#define CAPTURE_RISING_EDGE		0
#define CAPTURE_FALLING_EDGE	1

#define CAPTURE_PRESCALER_1_1	0
#define CAPTURE_PRESCALER_2_1	1
#define CAPTURE_PRESCALER_4_1	2
#define CAPTURE_PRESCALER_8_1	3

#define CAPTURE_TIMER_SPEED_1us		16
#define CAPTURE_TIMER_SPEED_10us	160
#define CAPTURE_TIMER_SPEED_100us	1600
#define CAPTURE_TIMER_SPEED_1ms		16000

/*
 * Estructura para configurar los valores de captura
 */
typedef struct
{
	uint8_t		channel;			// Canal de captura relacionado con el timer
	uint8_t		edgeSignal;			// Selección del tipo de flanco que se desea capturar
	uint32_t	timerSpeed;			// velocidad de incremento del timer
	uint32_t	prescalerCapture;	// Numero de periodos leidos
}Capture_Config_t;

/*
 * Estructura para el handler de la captura
 */
typedef struct
{
	TIM_TypeDef			*ptrTIMx;		// Timer al que esta asociado la captura
	Capture_Config_t	config;		// Configuracion inicial de la captura
}Capture_Handler_t;

/*
 * Cabeceras de las funciones
 */
void capture_Config(Capture_Handler_t *ptrCaptureHandler);
uint64_t getPeriodFrec(Capture_Handler_t *ptrCaptureHandler);
uint64_t getDutty(Capture_Handler_t *ptrCaptureHandler);
uint64_t getDutty_2(Capture_Handler_t *ptrCaptureHandler);
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
void stopCapture(Capture_Handler_t *ptrCaptureHandler);
void startCapture(Capture_Handler_t *ptrCaptureHandler);


#endif /* CAPTUREFRECDRIVER_H_ */
