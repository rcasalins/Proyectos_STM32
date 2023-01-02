/*
 * HC_SR04_Driver.c
 *
 *  Created on: Nov 23, 2022
 *      Author: rafaelach
 */

#include "HC_SR04_Driver.h"

uint32_t timestampHCSR04 	= 		0;
uint32_t timestampHCSR041 = 		0;
uint32_t timestampHCSR042 = 		0;
uint32_t deltatimestampHCSR04 = 	0;
//uint8_t flagFreq = 			0;
/*
 * Funcion para configurar la captura
 */

void HCSR04_Config(HC_SR04_Handler_t *ptrHCSR04Handler){

	/* 0. Desactivamos las interrupciones globales mientras configuramos el sistema.*/
	__disable_irq();

	/*
	 * 1. Activar la señal de reloj del periferico requerido
	 */
	if(ptrHCSR04Handler->ptrTIMx == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if(ptrHCSR04Handler->ptrTIMx == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if(ptrHCSR04Handler->ptrTIMx == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	else if(ptrHCSR04Handler->ptrTIMx == TIM5){
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	}
	else{
		__NOP();
	}


	/*
	 * 2a. Estamos en UP mode, el limite se carga en el ARR y se comienza en 0
	 */
	ptrHCSR04Handler->ptrTIMx->CR1 &= ~TIM_CR1_DIR;

	/*
	 * 3.Configuramos los bits CCxS del registro TIMy_CCMR1, de forma que sea modo salida
	 * (para cada canal hay un conjunto CCxS)
	 *
	 * 4. Además en el mismo "case" el modo del PWM, su polaridad.
	 *
	 * 5. Y además activamos el preload bit, para que cada vez que exista un update-event
	 * el valor cargado en el CCRx será recargado en el registro "shadow" del PWM
	 */
	switch(ptrHCSR04Handler->config.channel){
	case HCSR04_CHANNEL_1:{
		// Borramos el posible valor cargado
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(TIM_CCMR1_CC1S);

		// Configuramos como entrada el mismo canal
		ptrHCSR04Handler->ptrTIMx->CCMR1 = (1 << TIM_CCMR1_CC1S_Pos);

		// Configuramos el filtro (sin filtro)
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(0xF << TIM_CCMR1_IC1F_Pos);
		ptrHCSR04Handler->ptrTIMx->CCMR1 |= (0x3 << TIM_CCMR1_IC1F_Pos);

		// Configuramos el prescaler
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(0x3 << TIM_CCMR1_IC1PSC_Pos);
		ptrHCSR04Handler->ptrTIMx->CCMR1 |= (HCSR04_PRESCALER_1_1 << TIM_CCMR1_IC1PSC_Pos);

		// Configuramos el flanco para que primero detecte rising edge
		// Configuramos 00, para rising edge
		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1P);
		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);

//		// COnfiguramos 01, para falling edge
//		ptrHCSR04Handler->ptrTIMx->CCER |= (TIM_CCER_CC1P);
//		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);

		// Activamos el modulo de captura
//		ptrHCSR04Handler->ptrTIMx->CCER |= TIM_CCER_CC1E;

		// Se activa la interrupcion para el modo captura del timer en el canal correspondiente
		ptrHCSR04Handler->ptrTIMx->DIER |= TIM_DIER_CC1IE;

		break;
	}

	case HCSR04_CHANNEL_2:{
		// Borramos el posible valor cargado
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(TIM_CCMR1_CC1S);

		// Configuramos como entrada el mismo canal
		ptrHCSR04Handler->ptrTIMx->CCMR1 = (1 << TIM_CCMR1_CC1S_Pos);

		// Configuramos el filtro (sin filtro)
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(0xF << TIM_CCMR1_IC1F_Pos);
		ptrHCSR04Handler->ptrTIMx->CCMR1 |= (0x3 << TIM_CCMR1_IC1F_Pos);

		// Configuramos el prescaler
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(0x3 << TIM_CCMR1_IC1PSC_Pos);
		ptrHCSR04Handler->ptrTIMx->CCMR1 |= (HCSR04_PRESCALER_1_1 << TIM_CCMR1_IC1PSC_Pos);

		// Configuramos el flanco para que primero detecte rising edge
		// Configuramos 00, para rising edge
		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1P);
		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);

//		// COnfiguramos 01, para falling edge
//		ptrHCSR04Handler->ptrTIMx->CCER |= (TIM_CCER_CC1P);
//		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);

		// Activamos el modulo de captura
//		ptrHCSR04Handler->ptrTIMx->CCER |= TIM_CCER_CC1E;

		// Se activa la interrupcion para el modo captura del timer en el canal correspondiente
		ptrHCSR04Handler->ptrTIMx->DIER |= TIM_DIER_CC1IE;

		break;
	}

	case HCSR04_CHANNEL_3:{
		// Borramos el posible valor cargado
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(TIM_CCMR1_CC1S);

		// Configuramos como entrada el mismo canal
		ptrHCSR04Handler->ptrTIMx->CCMR1 = (1 << TIM_CCMR1_CC1S_Pos);

		// Configuramos el filtro (sin filtro)
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(0xF << TIM_CCMR1_IC1F_Pos);
		ptrHCSR04Handler->ptrTIMx->CCMR1 |= (0x3 << TIM_CCMR1_IC1F_Pos);

		// Configuramos el prescaler
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(0x3 << TIM_CCMR1_IC1PSC_Pos);
		ptrHCSR04Handler->ptrTIMx->CCMR1 |= (HCSR04_PRESCALER_1_1 << TIM_CCMR1_IC1PSC_Pos);

		// Configuramos el flanco para que primero detecte rising edge
		// Configuramos 00, para rising edge
		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1P);
		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);

//		// COnfiguramos 01, para falling edge
//		ptrHCSR04Handler->ptrTIMx->CCER |= (TIM_CCER_CC1P);
//		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);

		// Activamos el modulo de captura
//		ptrHCSR04Handler->ptrTIMx->CCER |= TIM_CCER_CC1E;

		// Se activa la interrupcion para el modo captura del timer en el canal correspondiente
		ptrHCSR04Handler->ptrTIMx->DIER |= TIM_DIER_CC1IE;

		break;
	}

	case HCSR04_CHANNEL_4:{
		// Borramos el posible valor cargado
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(TIM_CCMR1_CC1S);

		// Configuramos como entrada el mismo canal
		ptrHCSR04Handler->ptrTIMx->CCMR1 = (1 << TIM_CCMR1_CC1S_Pos);

		// Configuramos el filtro (sin filtro)
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(0xF << TIM_CCMR1_IC1F_Pos);
		ptrHCSR04Handler->ptrTIMx->CCMR1 |= (0x3 << TIM_CCMR1_IC1F_Pos);

		// Configuramos el prescaler
		ptrHCSR04Handler->ptrTIMx->CCMR1 &= ~(0x3 << TIM_CCMR1_IC1PSC_Pos);
		ptrHCSR04Handler->ptrTIMx->CCMR1 |= (HCSR04_PRESCALER_1_1 << TIM_CCMR1_IC1PSC_Pos);

		// Configuramos el flanco para que primero detecte rising edge
		// Configuramos 00, para rising edge
		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1P);
		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);

//		// COnfiguramos 01, para falling edge
//		ptrHCSR04Handler->ptrTIMx->CCER |= (TIM_CCER_CC1P);
//		ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);

		// Activamos el modulo de captura
//		ptrHCSR04Handler->ptrTIMx->CCER |= TIM_CCER_CC1E;

		// Se activa la interrupcion para el modo captura del timer en el canal correspondiente
		ptrHCSR04Handler->ptrTIMx->DIER |= TIM_DIER_CC1IE;

		break;
	}

	default:{
		__NOP();
		break;
	}

	} // FIN del switch-case

	/*
	 * Configuramos el prescaler del timer, el cual define a qué velocidad se incrmenta nuestro timer
	 */
	ptrHCSR04Handler->ptrTIMx->PSC = ptrHCSR04Handler->config.timerSpeed;

	/* 6. Activamos el canal del sistema NVIC para que lea la interrupción*/
	if(ptrHCSR04Handler->ptrTIMx == TIM2){
		// Activando en NVIC para la interrupción del TIM2
		NVIC_EnableIRQ(TIM2_IRQn);
	}
	else if(ptrHCSR04Handler->ptrTIMx == TIM3){
		// Activando en NVIC para la interrupción del TIM3
		/* Escriba codigo aca */
		NVIC_EnableIRQ(TIM3_IRQn);
	}
	else if(ptrHCSR04Handler->ptrTIMx == TIM4){
		// Activando en NVIC para la interrupción del TIM4
		/* Escriba codigo aca */
		NVIC_EnableIRQ(TIM4_IRQn);
	}
	else if(ptrHCSR04Handler->ptrTIMx == TIM5){
		// Activando en NVIC para la interrupción del TIM5
		/* Escriba codigo aca */
		NVIC_EnableIRQ(TIM5_IRQn);
	}
	else{
		__NOP();
	}

	/* 7. Volvemos a activar las interrupciones del sistema */
	__enable_irq();

	// Enciendo el timer para que empice a contar
	ptrHCSR04Handler->ptrTIMx->CR1 |= TIM_CR1_CEN;


}


/*
 * Función encargada de lanzar la captura de frecuencia, para esto, se utiliza una bandera (timestampHCSR04)
 * que nos indica cuando ha ocurrido una sola interrupción o dos. Al pasar por una interrupción captura
 * el valor almacenado en el CCRx y lo almacena en "timestampHCSR041", al pasar por la segunda interrupción
 * captura nuevamente el valor almacenado en el CCRx y resetea la bandera "timestampHCSR04". Finalmente se
 * restan los valores de timestampHCSR042 y timestampHCSR041 para obtener el periodo.
 */

//uint32_t getDutty(HC_SR04_Handler_t *ptrHCSR04Handler){
//
//	/* Todo lo que sigue de aca en adelante depende del canal que se ha seleccionado,
//	 * por lo cual se escribe cada código dentro del "case" específico.
//	 * Antes de lanzar la captura (encender el Timer para que cuente),
//	 * debemos borrar el valor de los registros CCP, de forma que arranquen limpios
//	 */
//	switch(ptrHCSR04Handler->config.channel){
//		case HCSR04_CHANNEL_1:{
//
//			if(timestampHCSR04 == 0){
//
//				// Bandera que indica una interrupcion
//				timestampHCSR04 = 1;
//
//				// Borramos el valor inicial del CCP
//				ptrHCSR04Handler->ptrTIMx->CCR1 = 0;
//
//				// Capturamos el valor del tiempo almacenado en el CCRx
//				timestampHCSR041 = ptrHCSR04Handler->ptrTIMx->CCR1;
//
//				// COnfiguramos 01, para falling edge
//				ptrHCSR04Handler->ptrTIMx->CCER |= (TIM_CCER_CC1P);
//				ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);
//
//			}
//			else if(timestampHCSR04 == 1){
//				// Capturamos el valor del tiempo almacenado en el CCRx
//				timestampHCSR042 = ptrHCSR04Handler->ptrTIMx->CCR1;
//
//				// Se resetea la bandera
//				timestampHCSR04 = 0;
//
//				// Se obtiene el periodo con el deltatimestampHCSR04
//				deltatimestampHCSR04 = (timestampHCSR042 - timestampHCSR041);
//			}
//
//
//			break;
//		}
//		case HCSR04_CHANNEL_2:{
//
//			if(timestampHCSR04 == 0){
//				timestampHCSR04 = 1;
//
//				// Borramos el valor inicial del CCP
//				ptrHCSR04Handler->ptrTIMx->CCR2 = 0;
//
//				// Capturamos el valor del tiempo almacenado en el CCRx
//				timestampHCSR041 = ptrHCSR04Handler->ptrTIMx->CCR2;
//
//				// COnfiguramos 01, para falling edge
//				ptrHCSR04Handler->ptrTIMx->CCER |= (TIM_CCER_CC1P);
//				ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);
//
//			}
//			else if(timestampHCSR04 == 1){
//				// Capturamos el valor del tiempo almacenado en el CCRx
//				timestampHCSR042 = ptrHCSR04Handler->ptrTIMx->CCR2;
//
//				timestampHCSR04 = 0;
//
//				deltatimestampHCSR04 = (timestampHCSR042 - timestampHCSR041);
//			}
//
//			break;
//		}
//		case HCSR04_CHANNEL_3:{
//
//			if(timestampHCSR04 == 0){
//				timestampHCSR04 = 1;
//
//				// Borramos el valor inicial del CCP
//				ptrHCSR04Handler->ptrTIMx->CCR3 = 0;
//
//				// Capturamos el valor del tiempo almacenado en el CCRx
//				timestampHCSR041 = ptrHCSR04Handler->ptrTIMx->CCR3;
//
//				// COnfiguramos 01, para falling edge
////				ptrHCSR04Handler->ptrTIMx->CCER |= (TIM_CCER_CC1P);
////				ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);
//
//			}
//			else if(timestampHCSR04 == 1){
//				// Capturamos el valor del tiempo almacenado en el CCRx
//				timestampHCSR042 = ptrHCSR04Handler->ptrTIMx->CCR3;
//
//				timestampHCSR04 = 0;
//
//				deltatimestampHCSR04 = (timestampHCSR042 - timestampHCSR041);
//			}
//
//
//			break;
//
//		}
//		case HCSR04_CHANNEL_4:{
//
//			if(timestampHCSR04 == 0){
//				timestampHCSR04 = 1;
//
//				// Borramos el valor inicial del CCP
//				ptrHCSR04Handler->ptrTIMx->CCR4 = 0;
//
//				// Capturamos el valor del tiempo almacenado en el CCRx
//				timestampHCSR041 = ptrHCSR04Handler->ptrTIMx->CCR4;
//
//				// COnfiguramos 01, para falling edge
//				ptrHCSR04Handler->ptrTIMx->CCER |= (TIM_CCER_CC1P);
//				ptrHCSR04Handler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);
//
//			}
//			else if(timestampHCSR04 == 1){
//				// Capturamos el valor del tiempo almacenado en el CCRx
//				timestampHCSR042 = ptrHCSR04Handler->ptrTIMx->CCR4;
//
//				timestampHCSR04 = 0;
//
//				deltatimestampHCSR04 = (timestampHCSR042 - timestampHCSR041);
//			}
//
//
//			break;
//		}
//		default: {
//			break;
//		}
//	}
//
//	return deltatimestampHCSR04;
//
//}



//// Callbacks para captura de frecuencia
//__attribute__((weak)) void CaptureTimer2_Channel1_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be	 implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer2_Channel2_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be	 implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer2_Channel3_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be	 implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer2_Channel4_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be	 implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer3_Channel1_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer3_Channel2_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer3_Channel3_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer3_Channel4_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer4_Channel1_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer4_Channel2_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer4_Channel3_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer4_Channel4_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer5_Channel1_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer5_Channel2_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer5_Channel3_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}
//
//__attribute__((weak)) void CaptureTimer5_Channel4_Callback(void){
//	  /* NOTE : This function should not be modified, when the callback is needed,
//	            the BasicTimerX_Callback could be implemented in the main file
//	   */
//	__NOP();
//}


void stopHCSR04(HC_SR04_Handler_t *ptrHCSR04Handler){

	switch(ptrHCSR04Handler->config.channel){
	case HCSR04_CHANNEL_1:{

		// desactivamos el modulo de captura
		ptrHCSR04Handler->ptrTIMx->CCER &= ~TIM_CCER_CC1E;

	}

	case HCSR04_CHANNEL_2:{

		// desactivamos el modulo de captura
		ptrHCSR04Handler->ptrTIMx->CCER &= ~TIM_CCER_CC2E;

	}

	case HCSR04_CHANNEL_3:{

		// desactivamos el modulo de captura
		ptrHCSR04Handler->ptrTIMx->CCER &= ~TIM_CCER_CC3E;

	}

	case HCSR04_CHANNEL_4:{

		// desactivamos el modulo de captura
		ptrHCSR04Handler->ptrTIMx->CCER &= ~TIM_CCER_CC4E;

	}

	}

}

void startHCSR04(HC_SR04_Handler_t *ptrHCSR04Handler){

	switch(ptrHCSR04Handler->config.channel){
	case HCSR04_CHANNEL_1:{

		// Activamos el modulo de captura
		ptrHCSR04Handler->ptrTIMx->CCER |= TIM_CCER_CC1E;

	}

	case HCSR04_CHANNEL_2:{

		// Activamos el modulo de captura
		ptrHCSR04Handler->ptrTIMx->CCER |= TIM_CCER_CC2E;

	}

	case HCSR04_CHANNEL_3:{

		// Activamos el modulo de captura
		ptrHCSR04Handler->ptrTIMx->CCER |= TIM_CCER_CC3E;

	}

	case HCSR04_CHANNEL_4:{

		// Activamos el modulo de captura
		ptrHCSR04Handler->ptrTIMx->CCER |= TIM_CCER_CC4E;

	}

	}

}
