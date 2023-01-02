/*
 * RTCDriver.h
 *
 *  Created on: Nov 1, 2022
 *      Author: RaphaelACH
 */

#ifndef RTCDRIVER_H_
#define RTCDRIVER_H_

#include "stm32f4xx.h"


#define MONDAY		0b001
#define	TUESDAY 	0b010
#define	WEDNESDAY	0b011
#define	THURSDAY	0b100
#define	FRIDAY		0b101
#define SATURDAY	0b110
#define	SUNDAY		0b111

#define RTC_AMPM	1
#define RTC_24H		0

#define DISABLE_RTC 1
#define ENABLE_RTC	0

/* Estructura que contiene la configuración mínima necesaria para el manejo del RTC.*/
typedef struct
{
	uint8_t		RTC_Hours; 		// Hora
	uint8_t		RTC_Minutes;	// Minutos
	uint8_t		RTC_Seconds;	// Segundos
	uint8_t		RTC_Days;		// Dias
	uint8_t		RTC_Months;		// Meses
	uint16_t	RTC_Years;		//Años
	uint8_t		RTC_Wdu;
	uint8_t		RTC_AmPm;
}RTC_Handler_t;


void RTC_Config(RTC_Handler_t *ptrRtcHandler);
uint8_t* read_time(void);
uint8_t* read_date(void);
void DisableRTC(uint8_t disable);
void EnableRTC(uint8_t enable);



#endif /* RTCDRIVER_H_ */
