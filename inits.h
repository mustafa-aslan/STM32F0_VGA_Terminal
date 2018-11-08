#ifndef __INITS_H
#define __INITS_H


#include "stm32f0xx.h"

// LED fonksiyonlari
#define LED_On()     GPIOA->BSRR = GPIO_BSRR_BS_10
#define LED_Off()    GPIOA->BSRR = GPIO_BSRR_BR_10
#define LED_Toggle() GPIOA->ODR ^= GPIO_ODR_10

static void RCC_Initialize(void);
static void GPIO_Initialize(void);
static void TIM_Initialize(void);
static void NVIC_Initialize(void);
static void DMA_Initialize(void);
static void SPI_Initialize(void);
static void USART_Initialize(void);
void STM32_Initialize(void);




#endif /* __INITS_H */
