/*
 * RTCDriver.c
 *
 *  Created on: Nov 1, 2022
 *      Author: RaphaelACH
 */

#include "RTCDriver.h"

uint8_t tiempo[6]= {0};
uint8_t fecha[7] = {0};

void RTC_Config(RTC_Handler_t *ptrRtcHandler){

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_DBP;

	RCC->BDCR |= RCC_BDCR_RTCEN;
	RCC->BDCR |= RCC_BDCR_LSEON;
	RCC->BDCR |= RCC_BDCR_RTCSEL_0;

	while(!(RCC->BDCR & RCC_BDCR_LSERDY));


	RTC->WPR |= 0xCA;
	RTC->WPR |= 0x53;

	RTC->ISR |= RTC_ISR_INIT;
	while(!(RTC->ISR & RTC_ISR_INITF));

	RTC->PRER |= 128 << RTC_PRER_PREDIV_A_Pos;
	RTC->PRER |= 255 << RTC_PRER_PREDIV_S_Pos;

	RTC->CR |= RTC_CR_BYPSHAD;

	RTC->TR = 0;
	RTC->DR = 0;

	RTC->CR |= RTC_CR_FMT;
	RTC->TR |= ptrRtcHandler->RTC_Hours/10 << RTC_TR_HT_Pos;
	RTC->TR |= ptrRtcHandler->RTC_Hours%10 << RTC_TR_HU_Pos;
	RTC->TR |= ptrRtcHandler->RTC_Minutes/10 << RTC_TR_MNT_Pos;
	RTC->TR |= ptrRtcHandler->RTC_Minutes%10 << RTC_TR_MNU_Pos;
	RTC->TR |= ptrRtcHandler->RTC_Seconds/10  << RTC_TR_ST_Pos;
	RTC->TR |= ptrRtcHandler->RTC_Seconds%10 << RTC_TR_SU_Pos;
	RTC->TR |= ptrRtcHandler->RTC_AmPm << RTC_TR_PM_Pos;

	RTC->DR |= ptrRtcHandler->RTC_Days/10 << RTC_DR_DT_Pos;
	RTC->DR |= ptrRtcHandler->RTC_Days%10 << RTC_DR_DU_Pos;
	RTC->DR |= ptrRtcHandler->RTC_Months%10 << RTC_DR_MU_Pos;
	RTC->DR |= ptrRtcHandler->RTC_Months/10 << RTC_DR_MT_Pos;
	RTC->DR |= ptrRtcHandler->RTC_Wdu<< RTC_DR_WDU_Pos;
	RTC->DR |= ((ptrRtcHandler->RTC_Years -2000)%10) << RTC_DR_YU_Pos;
	RTC->DR |= (ptrRtcHandler->RTC_Years - 2000)/10 << RTC_DR_YT_Pos;

	RCC->BDCR |= RCC_BDCR_RTCEN;
	RTC->ISR &= ~RTC_ISR_INIT;
	PWR->CR &= ~ PWR_CR_DBP;

	RTC->WPR = (0xFF);

}


uint8_t* read_time(void){

	uint8_t RTC_HoursUnits	 = 0;
	uint8_t RTC_HoursTens	 = 0;
	uint8_t RTC_MinutesUnits = 0;
	uint8_t RTC_MinutesTens = 0;
	uint8_t RTC_SecondsUnits = 0;
	uint8_t RTC_SecondsTens = 0;
	uint8_t RTC_AmPm = 0;


	RTC_HoursUnits = ((RTC->TR & RTC_TR_HU_Msk)>>RTC_TR_HU_Pos);
	RTC_HoursTens = ((RTC->TR & RTC_TR_HT_Msk)>> RTC_TR_HT_Pos);
	RTC_MinutesUnits = ((RTC->TR & RTC_TR_MNU_Msk)>>RTC_TR_MNU_Pos);
	RTC_MinutesTens = ((RTC->TR & RTC_TR_MNT_Msk)>>RTC_TR_MNT_Pos);
	RTC_SecondsUnits = ((RTC->TR & RTC_TR_SU_Msk)>>RTC_TR_SU_Pos);
	RTC_SecondsTens = ((RTC->TR & RTC_TR_ST_Msk)>>RTC_TR_ST_Pos);
	RTC_AmPm = ((RTC->TR & RTC_TR_PM_Msk)>>RTC_TR_PM_Pos);

	tiempo[0] = RTC_SecondsUnits;
	tiempo[1] = RTC_SecondsTens;
	tiempo[2] = RTC_MinutesUnits;
	tiempo[3] = RTC_MinutesTens;
	tiempo[4] = RTC_HoursUnits;
	tiempo[5] = RTC_HoursTens;
	tiempo[6] = RTC_AmPm;

	return tiempo;
}
uint8_t* read_date(void){

	uint8_t RTC_dayUnits = 0;
	uint8_t RTC_dayTens	 = 0;
	uint8_t RTC_month = 0;
	uint8_t RTC_yearUnits = 0;
	uint8_t RTC_yearTens = 0;
	uint8_t RTC_wdu = 0;

	RTC_dayTens=((RTC->DR & RTC_DR_DT_Msk)>> RTC_DR_DT_Pos);
	RTC_dayUnits=((RTC->DR & RTC_DR_DU_Msk)>>RTC_DR_DU_Pos);
	RTC_yearUnits=((RTC->DR & RTC_DR_YU_Msk)>>RTC_DR_YU_Pos);
	RTC_yearTens = ((RTC->DR & RTC_DR_YT_Msk)>>RTC_DR_YT_Pos);
	RTC_month = (((RTC->DR & RTC_DR_MT_Msk)>>RTC_DR_MT_Pos)*10) + (((RTC->DR & RTC_DR_MU_Msk)>>RTC_DR_MU_Pos)%10);
	RTC_wdu = (RTC->DR & RTC_DR_WDU_Msk)>>RTC_DR_WDU_Pos;

	fecha[0] = RTC_dayUnits;
	fecha[1] = RTC_dayTens;
	fecha[2] = RTC_month;
	fecha[3] = RTC_yearUnits;
	fecha[4] = RTC_yearTens;
	fecha[5] = RTC_wdu;

	return fecha;
}

void DisableRTC(uint8_t disable){
	if(disable == DISABLE_RTC){
//		RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		PWR->CR |= PWR_CR_DBP;

		RCC->BDCR &= ~RCC_BDCR_RTCEN;

		PWR->CR &= ~ PWR_CR_DBP;
	}
}

void EnableRTC(uint8_t enable){
	if(enable == ENABLE_RTC){
		PWR->CR |= PWR_CR_DBP;

		RCC->BDCR |= RCC_BDCR_RTCEN;

		PWR->CR &= ~ PWR_CR_DBP;
	}
}
