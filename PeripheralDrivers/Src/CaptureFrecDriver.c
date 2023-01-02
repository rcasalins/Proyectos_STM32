/*
 * CaptureFrecDriver.c
 *
 *  Created on: 19/11/2022
 *      Author: Rafael Casalins
 */

#include "CaptureFrecDriver.h"
#include <math.h>

// #include "BasicTimer.h"
uint32_t timestamp 	= 		0;
uint32_t timestamp1 = 		0;
uint32_t timestamp2 = 		0;
uint32_t deltaTimestamp = 	0;

uint8_t flagFreq = 			0;

uint32_t timestamp_c2 	= 		0;
uint32_t timestamp1_c2 = 		0;
uint32_t timestamp2_c2 = 		0;
uint32_t deltaTimestamp_c2 = 	0;

/*
 * Funcion para configurar la captura
 */

void capture_Config(Capture_Handler_t *ptrCaptureHandler){

	/* 0. Desactivamos las interrupciones globales mientras configuramos el sistema.*/
	__disable_irq();

	/*
	 * 1. Activar la señal de reloj del periferico requerido
	 */
	if(ptrCaptureHandler->ptrTIMx == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if(ptrCaptureHandler->ptrTIMx == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if(ptrCaptureHandler->ptrTIMx == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	else if(ptrCaptureHandler->ptrTIMx == TIM5){
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	}
	else{
		__NOP();
	}


	/*
	 * 2a. Estamos en UP mode, el limite se carga en el ARR y se comienza en 0
	 */
	ptrCaptureHandler->ptrTIMx->CR1 &= ~TIM_CR1_DIR;

	/*
	 * 3.Configuramos los bits CCxS del registro TIMy_CCMR1, de forma que sea modo salida
	 * (para cada canal hay un conjunto CCxS)
	 *
	 * 4. Además en el mismo "case" el modo del PWM, su polaridad.
	 *
	 * 5. Y además activamos el preload bit, para que cada vez que exista un update-event
	 * el valor cargado en el CCRx será recargado en el registro "shadow" del PWM
	 */
	switch(ptrCaptureHandler->config.channel){
	case CAPTURE_CHANNEL_1:{
		// Borramos el posible valor cargado
		ptrCaptureHandler->ptrTIMx->CCMR1 &= ~(TIM_CCMR1_CC1S);

		// Configuramos como entrada el mismo canal
		ptrCaptureHandler->ptrTIMx->CCMR1 = (1 << TIM_CCMR1_CC1S_Pos);

		// Configuramos el filtro (sin filtro)
		ptrCaptureHandler->ptrTIMx->CCMR1 &= ~(0xF << TIM_CCMR1_IC1F_Pos);
		ptrCaptureHandler->ptrTIMx->CCMR1 |= (0x3 << TIM_CCMR1_IC1F_Pos);

		// Configuramos el prescaler
		ptrCaptureHandler->ptrTIMx->CCMR1 &= ~(0x3 << TIM_CCMR1_IC1PSC_Pos);
		ptrCaptureHandler->ptrTIMx->CCMR1 |= (ptrCaptureHandler->config.prescalerCapture << TIM_CCMR1_IC1PSC_Pos);

		// Configuramos el flanco que deseamos capturar
		if(ptrCaptureHandler->config.edgeSignal == CAPTURE_RISING_EDGE){
			// Configuramos 00, para rising edge
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC1P);
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);
		}
		else{
			// COnfiguramos 01, para falling edge
			ptrCaptureHandler->ptrTIMx->CCER |= (TIM_CCER_CC1P);
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);
		}

		// Activamos el modulo de captura
//		ptrCaptureHandler->ptrTIMx->CCER |= TIM_CCER_CC1E;

		// Se activa la interrupcion para el modo captura del timer en el canal correspondiente
		ptrCaptureHandler->ptrTIMx->DIER |= TIM_DIER_CC1IE;

		break;
	}

	case CAPTURE_CHANNEL_2:{
		// Borramos el posible valor cargado
		ptrCaptureHandler->ptrTIMx->CCMR1 &= ~(TIM_CCMR1_CC2S);

		// Configuramos como entrada el mismo canal
		ptrCaptureHandler->ptrTIMx->CCMR1 = (1 << TIM_CCMR1_CC2S_Pos);

		// Configuramos el filtro (sin filtro)
		ptrCaptureHandler->ptrTIMx->CCMR1 &= ~(0xF << TIM_CCMR1_IC2F_Pos);
		ptrCaptureHandler->ptrTIMx->CCMR1 |= (0x3 << TIM_CCMR1_IC2F_Pos);

		// Configuramos el prescaler
		ptrCaptureHandler->ptrTIMx->CCMR1 &= ~(0x3 << TIM_CCMR1_IC2PSC_Pos);
		ptrCaptureHandler->ptrTIMx->CCMR1 |= (ptrCaptureHandler->config.prescalerCapture << TIM_CCMR1_IC2PSC_Pos);

		// Configuramos el flanco que deseamos capturar
		if(ptrCaptureHandler->config.edgeSignal == CAPTURE_RISING_EDGE){
			// Configuramos 00, para rising edge
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC2P);
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC2NP);
		}
		else{
			// COnfiguramos 01, para falling edge
			ptrCaptureHandler->ptrTIMx->CCER |= (TIM_CCER_CC2P);
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC2NP);
		}

		// Activamos el modulo de captura
//		ptrCaptureHandler->ptrTIMx->CCER |= TIM_CCER_CC2E;

		// Se activa la interrupcion para el modo captura del timer en el canal correspondiente
		ptrCaptureHandler->ptrTIMx->DIER |= TIM_DIER_CC2IE;

		break;
	}

	case CAPTURE_CHANNEL_3:{
		// Borramos el posible valor cargado
		ptrCaptureHandler->ptrTIMx->CCMR2 &= ~(TIM_CCMR2_CC3S);

		// Configuramos como entrada el mismo canal
		ptrCaptureHandler->ptrTIMx->CCMR2 = (1 << TIM_CCMR2_CC3S_Pos);

		// Configuramos el filtro (sin filtro)
		ptrCaptureHandler->ptrTIMx->CCMR2 &= ~(0xF << TIM_CCMR2_IC3F_Pos);
		ptrCaptureHandler->ptrTIMx->CCMR2 |= (0x3 << TIM_CCMR2_IC3F_Pos);

		// Configuramos el prescaler
		ptrCaptureHandler->ptrTIMx->CCMR2 &= ~(0x3 << TIM_CCMR2_IC3PSC_Pos);
		ptrCaptureHandler->ptrTIMx->CCMR2 |= (ptrCaptureHandler->config.prescalerCapture << TIM_CCMR2_IC3PSC_Pos);

		// Configuramos el flanco que deseamos capturar
		if(ptrCaptureHandler->config.edgeSignal == CAPTURE_RISING_EDGE){
			// Configuramos 00, para rising edge
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC3P);
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC3NP);
		}
		else{
			// COnfiguramos 01, para falling edge
			ptrCaptureHandler->ptrTIMx->CCER |= (TIM_CCER_CC3P);
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC3NP);
		}

		// Activamos el modulo de captura
//		ptrCaptureHandler->ptrTIMx->CCER |= TIM_CCER_CC3E;

		// Se activa la interrupcion para el modo captura del timer en el canal correspondiente
		ptrCaptureHandler->ptrTIMx->DIER |= TIM_DIER_CC3IE;

		break;
	}

	case CAPTURE_CHANNEL_4:{
		// Borramos el posible valor cargado
		ptrCaptureHandler->ptrTIMx->CCMR2 &= ~(TIM_CCMR2_CC4S);

		// Configuramos como entrada el mismo canal
		ptrCaptureHandler->ptrTIMx->CCMR2 = (1 << TIM_CCMR2_CC4S_Pos);

		// Configuramos el filtro (sin filtro)
		ptrCaptureHandler->ptrTIMx->CCMR2 &= ~(0xF << TIM_CCMR2_IC4F_Pos);
		ptrCaptureHandler->ptrTIMx->CCMR2 |= (0x3 << TIM_CCMR2_IC4F_Pos);

		// Configuramos el prescaler
		ptrCaptureHandler->ptrTIMx->CCMR2 &= ~(0x3 << TIM_CCMR2_IC4PSC_Pos);
		ptrCaptureHandler->ptrTIMx->CCMR2 |= (ptrCaptureHandler->config.prescalerCapture << TIM_CCMR2_IC4PSC_Pos);

		// Configuramos el flanco que deseamos capturar
		if(ptrCaptureHandler->config.edgeSignal == CAPTURE_RISING_EDGE){
			// Configuramos 00, para rising edge
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC4P);
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC4NP);
		}
		else{
			// COnfiguramos 01, para falling edge
			ptrCaptureHandler->ptrTIMx->CCER |= (TIM_CCER_CC3P);
			ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC4NP);
		}

		// Activamos el modulo de captura
//		ptrCaptureHandler->ptrTIMx->CCER |= TIM_CCER_CC4E;

		// Se activa la interrupcion para el modo captura del timer en el canal correspondiente
		ptrCaptureHandler->ptrTIMx->DIER |= TIM_DIER_CC4IE;

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
	ptrCaptureHandler->ptrTIMx->PSC = ptrCaptureHandler->config.timerSpeed;

	/* 6. Activamos el canal del sistema NVIC para que lea la interrupción*/
	if(ptrCaptureHandler->ptrTIMx == TIM2){
		// Activando en NVIC para la interrupción del TIM2
		NVIC_EnableIRQ(TIM2_IRQn);
	}
	else if(ptrCaptureHandler->ptrTIMx == TIM3){
		// Activando en NVIC para la interrupción del TIM3
		/* Escriba codigo aca */
		NVIC_EnableIRQ(TIM3_IRQn);
	}
	else if(ptrCaptureHandler->ptrTIMx == TIM4){
		// Activando en NVIC para la interrupción del TIM4
		/* Escriba codigo aca */
		NVIC_EnableIRQ(TIM4_IRQn);
	}
	else if(ptrCaptureHandler->ptrTIMx == TIM5){
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
	ptrCaptureHandler->ptrTIMx->CR1 |= TIM_CR1_CEN;


}


/*
 * Función encargada de lanzar la captura de frecuencia, para esto, se utiliza una bandera (timestamp)
 * que nos indica cuando ha ocurrido una sola interrupción o dos. Al pasar por una interrupción captura
 * el valor almacenado en el CCRx y lo almacena en "timestamp1", al pasar por la segunda interrupción
 * captura nuevamente el valor almacenado en el CCRx y resetea la bandera "timestamp". Finalmente se
 * restan los valores de timestamp2 y timestamp1 para obtener el periodo.
 */

uint64_t getPeriodFrec(Capture_Handler_t *ptrCaptureHandler){

	/* Todo lo que sigue de aca en adelante depende del canal que se ha seleccionado,
	 * por lo cual se escribe cada código dentro del "case" específico.
	 * Antes de lanzar la captura (encender el Timer para que cuente),
	 * debemos borrar el valor de los registros CCP, de forma que arranquen limpios
	 */
	switch(ptrCaptureHandler->config.channel){
		case CAPTURE_CHANNEL_1:{

			if(timestamp == 0){

				// Bandera que indica una interrupcion
				timestamp = 1;

				// Borramos el valor inicial del CCP
				ptrCaptureHandler->ptrTIMx->CCR1 = 0;

				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp1 = ptrCaptureHandler->ptrTIMx->CCR1;

			}
			else if(timestamp == 1){
				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp2 = ptrCaptureHandler->ptrTIMx->CCR1;

				// Se resetea la bandera
				timestamp = 0;

				// Se obtiene el periodo con el deltaTimestamp
				deltaTimestamp = (timestamp2 - timestamp1);
			}


			break;
		}
		case CAPTURE_CHANNEL_2:{

			if(timestamp == 0){
				timestamp = 1;

				// Borramos el valor inicial del CCP
				ptrCaptureHandler->ptrTIMx->CCR2 = 0;

				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp1 = ptrCaptureHandler->ptrTIMx->CCR2;

			}
			else if(timestamp == 1){
				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp2 = ptrCaptureHandler->ptrTIMx->CCR2;

				timestamp = 0;

				deltaTimestamp = (timestamp2 - timestamp1);
			}

			break;
		}
		case CAPTURE_CHANNEL_3:{

			if(timestamp == 0){
				timestamp = 1;

				// Borramos el valor inicial del CCP
				ptrCaptureHandler->ptrTIMx->CCR3 = 0;

				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp1 = ptrCaptureHandler->ptrTIMx->CCR3;

			}
			else if(timestamp == 1){
				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp2 = ptrCaptureHandler->ptrTIMx->CCR3;

				timestamp = 0;

				deltaTimestamp = (timestamp2 - timestamp1);
			}


			break;

		}
		case CAPTURE_CHANNEL_4:{

			if(timestamp == 0){
				timestamp = 1;

				// Borramos el valor inicial del CCP
				ptrCaptureHandler->ptrTIMx->CCR4 = 0;

				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp1 = ptrCaptureHandler->ptrTIMx->CCR4;

			}
			else if(timestamp == 1){
				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp2 = ptrCaptureHandler->ptrTIMx->CCR4;

				timestamp = 0;

				deltaTimestamp = (timestamp2 - timestamp1);
			}


			break;
		}
		default: {
			break;
		}
	}

	return deltaTimestamp;

}

uint64_t getDutty(Capture_Handler_t *ptrCaptureHandler){
	/* Todo lo que sigue de aca en adelante depende del canal que se ha seleccionado,
	 * por lo cual se escribe cada código dentro del "case" específico.
	 * Antes de lanzar la captura (encender el Timer para que cuente),
	 * debemos borrar el valor de los registros CCP, de forma que arranquen limpios
	 */

	switch(ptrCaptureHandler->config.channel){
		case CAPTURE_CHANNEL_2:{

			if(timestamp == 0){
				timestamp = 1;

				// Borramos el valor inicial del CCP
				ptrCaptureHandler->ptrTIMx->CCR2 = 0;

				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp1 = ptrCaptureHandler->ptrTIMx->CCR2;

				// COnfiguramos 01, para falling edge
				ptrCaptureHandler->ptrTIMx->CCER |= (TIM_CCER_CC2P);
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC2NP);

			}
			else if(timestamp == 1){
				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp2 = ptrCaptureHandler->ptrTIMx->CCR2;

				timestamp = 0;

				deltaTimestamp = (timestamp2 - timestamp1);

				// Configuramos 00, para rising edge
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC2P);
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC2NP);
			}

			break;
		}
		case CAPTURE_CHANNEL_3:{

			if(timestamp == 0){
				timestamp = 1;

				// Borramos el valor inicial del CCP
				ptrCaptureHandler->ptrTIMx->CCR3 = 0;

				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp1 = ptrCaptureHandler->ptrTIMx->CCR3;

				// COnfiguramos 01, para falling edge
				ptrCaptureHandler->ptrTIMx->CCER |= (TIM_CCER_CC3P);
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC3NP);

			}
			else if(timestamp == 1){
				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp2 = ptrCaptureHandler->ptrTIMx->CCR3;

				timestamp = 0;

				deltaTimestamp = (timestamp2 - timestamp1);

				// Configuramos 00, para rising edge
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC3P);
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC3NP);
			}


			break;

		}
		case CAPTURE_CHANNEL_4:{

			if(timestamp == 0){
				timestamp = 1;

				// Borramos el valor inicial del CCP
				ptrCaptureHandler->ptrTIMx->CCR4 = 0;

				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp1 = ptrCaptureHandler->ptrTIMx->CCR4;

				// COnfiguramos 01, para falling edge
				ptrCaptureHandler->ptrTIMx->CCER |= (TIM_CCER_CC4P);
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC4NP);

			}
			else if(timestamp == 1){
				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp2 = ptrCaptureHandler->ptrTIMx->CCR4;

				timestamp = 0;

				deltaTimestamp = (timestamp2 - timestamp1);

				// Configuramos 00, para rising edge
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC4P);
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC4NP);
			}


			break;
		}
		default: {
			break;
		}
	}

	return deltaTimestamp;
}


uint64_t getDutty_2(Capture_Handler_t *ptrCaptureHandler){
	/* Todo lo que sigue de aca en adelante depende del canal que se ha seleccionado,
	 * por lo cual se escribe cada código dentro del "case" específico.
	 * Antes de lanzar la captura (encender el Timer para que cuente),
	 * debemos borrar el valor de los registros CCP, de forma que arranquen limpios
	 */

	switch(ptrCaptureHandler->config.channel){
		case CAPTURE_CHANNEL_1:{

			if(timestamp_c2 == 0){

				// Bandera que indica una interrupcion
				timestamp_c2 = 1;

				// Borramos el valor inicial del CCP
				ptrCaptureHandler->ptrTIMx->CCR1 = 0;

				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp1_c2 = ptrCaptureHandler->ptrTIMx->CCR1;

				// COnfiguramos 01, para falling edge
				ptrCaptureHandler->ptrTIMx->CCER |= (TIM_CCER_CC1P);
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);

			}
			else if(timestamp_c2 == 1){
				// Capturamos el valor del tiempo almacenado en el CCRx
				timestamp2_c2 = ptrCaptureHandler->ptrTIMx->CCR1;

				// Se resetea la bandera
				timestamp_c2 = 0;

				// Se obtiene el periodo con el deltaTimestamp
				deltaTimestamp_c2 = (timestamp2_c2 - timestamp1_c2);

				// Configuramos 00, para rising edge
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC1P);
				ptrCaptureHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC1NP);
			}


			break;
		}
		default: {
			break;
		}
	}

	return deltaTimestamp_c2;
}




// Callbacks para captura de frecuencia
__attribute__((weak)) void CaptureTimer2_Channel1_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be	 implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer2_Channel2_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be	 implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer2_Channel3_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be	 implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer2_Channel4_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be	 implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer3_Channel1_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer3_Channel2_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer3_Channel3_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer3_Channel4_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer4_Channel1_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer4_Channel2_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer4_Channel3_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer4_Channel4_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer5_Channel1_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer5_Channel2_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer5_Channel3_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void CaptureTimer5_Channel4_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}


void stopCapture(Capture_Handler_t *ptrCaptureHandler){

	switch(ptrCaptureHandler->config.channel){
	case CAPTURE_CHANNEL_1:{

		// desactivamos el modulo de captura
		ptrCaptureHandler->ptrTIMx->CCER &= ~TIM_CCER_CC1E;

	}

	case CAPTURE_CHANNEL_2:{

		// desactivamos el modulo de captura
		ptrCaptureHandler->ptrTIMx->CCER &= ~TIM_CCER_CC2E;

	}

	case CAPTURE_CHANNEL_3:{

		// desactivamos el modulo de captura
		ptrCaptureHandler->ptrTIMx->CCER &= ~TIM_CCER_CC3E;

	}

	case CAPTURE_CHANNEL_4:{

		// desactivamos el modulo de captura
		ptrCaptureHandler->ptrTIMx->CCER &= ~TIM_CCER_CC4E;

	}

	}

}

void startCapture(Capture_Handler_t *ptrCaptureHandler){

	switch(ptrCaptureHandler->config.channel){
	case CAPTURE_CHANNEL_1:{

		// Activamos el modulo de captura
		ptrCaptureHandler->ptrTIMx->CCER |= TIM_CCER_CC1E;

	}

	case CAPTURE_CHANNEL_2:{

		// Activamos el modulo de captura
		ptrCaptureHandler->ptrTIMx->CCER |= TIM_CCER_CC2E;

	}

	case CAPTURE_CHANNEL_3:{

		// Activamos el modulo de captura
		ptrCaptureHandler->ptrTIMx->CCER |= TIM_CCER_CC3E;

	}

	case CAPTURE_CHANNEL_4:{

		// Activamos el modulo de captura
		ptrCaptureHandler->ptrTIMx->CCER |= TIM_CCER_CC4E;

	}

	}

}









