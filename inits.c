#include "inits.h"

/*
 VGA Signal 640x480 @60Hz Industry Standart Timing	
---------------------------------------------------
	 Screen refresh rate 60       Hz
	 Vertical refresh    31.46875 kHz
	 Pixel frequency     25.175   MHz(Bu uygulamada 25Mhz olarak kullanildi.)
	 
	Horizontal Sync
----------------------------------------------------
	 Visible area 640 cycles
	 Front porch  16  cycles 0.64us
	 Sync pulse   96  cycles 3.84us
	 Back porch   48  cycles 1.92us
	 Whole line   800 cycles 3.2us
	
	Vertical Sync
----------------------------------------------------
	 Visible area 480 Horizontal cycles
	 Front porch  10  Horizontal cycles
	 Sync pulse   2   Horizontal cycles
	 Back porch   33  Horizontal cycles
	 Whole frame  525 Horizontal cycles

 STM32F030F4P6
----------------------------------------------------
	1  BOOT0
	2  PF0 - 25_MHz_OSC_IN
	3	 PF1 - 25_MHz_OSC_OUT
	4  NRST
	5  VDDA
	6  PA0 - X
	7  PA1 - X
	8  PA2 - USART1_TX
	9  PA3 - USART1_RX
	10 PA4 - X
	11 PA5 - SPI1_SCK
	12 PA6 - X
	13 PA7 - SPI1_MOSI
	14 PB1 - TIM3_CH4(v)
	15 VSS 
	16 VDD
	17 PA9  - TIM1_CH2(h)
	18 PA10 - LED
	19 PA13 - SYS_SWCLK
	20 PA14 - SYS_SWDIO	
*/

//

static void RCC_Initialize(void)
{
	//---------------------------------------
	// AHB
	//---------------------------------------
	// GPIOA
	// GPIOB
	// DMA1
	//---------------------------------------
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_DMA1EN;
	
	//---------------------------------------
	// APB2
	//---------------------------------------
	// TIM1
	// SPI1
	// USART1
	//---------------------------------------
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_SPI1EN | RCC_APB2ENR_USART1EN;
	
	//---------------------------------------
	// APB1
	//---------------------------------------
	// TIM3
	//---------------------------------------
	RCC->APB1ENR |=	RCC_APB1ENR_TIM3EN;
}

static void GPIO_Initialize(void)
{
	//---------------------------------------
	// USART1
	//---------------------------------------
	// PA2/TX
	// PA3/RX
	//---------------------------------------
	// Alternate function
	GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;
	// High speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3;
	// AF1
	GPIOA->AFR[0] |= (1 << 8) | (1 << 12);
	
	//---------------------------------------
	// LED(PA10)
	//---------------------------------------
	// Output mode
	GPIOA->MODER |= GPIO_MODER_MODER10_0;
	// High speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;
	
	//---------------------------------------
	// SPI1
	//---------------------------------------
	// PA5/CLK
	// PA7/MOSI
	//---------------------------------------
	// Alternate function
	GPIOA->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER7_1;
	// High speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR7;
	
	//---------------------------------------
	// TIM1_CH2(PA9)
	//---------------------------------------
	// Alternate funtion
	GPIOA->MODER |= GPIO_MODER_MODER9_1;
	// High speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
	// AF2
	GPIOA->AFR[1] |= (1 << 5);
	
	//---------------------------------------
	// TIM3_CH4(PB1)
	//---------------------------------------
	/* PB1: Push-pull, No-pull */
	// Alternate function
	GPIOB->MODER |= GPIO_MODER_MODER1_1;
	// High speed
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1;
	// AF1
	GPIOB->AFR[0] |= (1 << 4);
}

static void TIM_Initialize(void)
{

	//---------------------------------------
	// TIM1(25MHz)
	//---------------------------------------
	// 800 cycle = HSync cycle
	TIM1->ARR = (uint16_t)800;
	
	//---------------------------------------
	// TIM1_CH2(HSync)
	//---------------------------------------
	// PWM mode 1
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
	// Preload register set
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;
	// Channel 2 out enable
	TIM1->CCER |= TIM_CCER_CC2E;
	//
	TIM1->CCER |= TIM_CCER_CC2P;	
	// Main output enable
	TIM1->BDTR |= TIM_BDTR_MOE;
	// 705 cycle
	TIM1->CCR2 = (uint16_t)704;
	
	//---------------------------------------
	// TIM1_CH1(No_Output)
	//---------------------------------------
	// 800 - 48
	TIM1->CCR1 = (uint16_t)48;
	
	//---------------------------------------
	// TIM3(25Mhz)
	//---------------------------------------
	// 1 HSync prescaler
	TIM3->PSC = (uint16_t)800;
	// 525 Hsync cycle 
	TIM3->ARR = (uint16_t)525;
	
	//---------------------------------------
	// TIM3_CH4
	//---------------------------------------
	// PWM mode 1
	TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
	// Preload register set
	TIM3->CCMR2 |= TIM_CCMR2_OC4PE;
	// Channel 4 out enable
	TIM3->CCER |= TIM_CCER_CC4E;
	//
	TIM3->CCER |= TIM_CCER_CC4P;
	// Main output enable
	TIM3->BDTR |= TIM_BDTR_MOE;
	// 523 cycle
	TIM3->CCR4 = (uint16_t)523;
	
	//---------------------------------------
	// TIM3_CH3(No_Output)
	//---------------------------------------
	// TIM3 CC3 interrupt enable
	TIM3->DIER |= TIM_DIER_CC3IE;
	// 525 - 33
	TIM3->CCR3 = (uint16_t)33;
	
	
	//---------------------------------------
	// TIM1 ve TIM3 ayni anda baslamasi icin
	//---------------------------------------
	// TIM1 Master(Enable mode)
	TIM1->CR2 |= TIM_CR2_MMS_0;
	// TIM3 Slave(Trigger mode)
	TIM3->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1;	
	// Master/Salve mode set
	TIM3->SMCR |= TIM_SMCR_MSM;
	// TIM3 update generation
	TIM1->EGR |= TIM_EGR_UG;
	// TIM1 update generation
	TIM3->EGR |= TIM_EGR_UG;	
	
	//---------------------------------------
	// TIM1 counter enable
	//---------------------------------------
	TIM1->CR1 |= TIM_CR1_CEN;
}

static void SPI_Initialize(void)
{
	//---------------------------------------
	// SPI1
	//---------------------------------------
	// MSB fisrt
	// Baudrate: fPCLK/2 = 12.5MHz
	// CPOL = 0
	// CPHA first edge
	//---------------------------------------
	// 1-line data mode, Ouytput enable
	SPI1->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;
	// Software slave, Internal slave select
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
	//
	SPI1->CR1 |= SPI_CR1_BR_0;
	// Master selection
	SPI1->CR1 |= SPI_CR1_MSTR;
	// Data size: 8bit
	SPI1->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;
	// Tx buffer DMA enable
	SPI1->CR2 |= SPI_CR2_TXDMAEN;
	// SPI enable
	SPI1->CR1 |= SPI_CR1_SPE;
}

static void USART_Initialize(void)
{
	//---------------------------------------
	// USART1
	//---------------------------------------
	// LSB first
	// 1 stop bit
	// Parity disable
	//---------------------------------------
	// 921.6 Kbaud
	USART1->BRR = (uint16_t)(480000/9216);
	// Transmitter, Reciever enable
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
	// USART enable
	USART1->CR1 |= USART_CR1_UE;
}

static void NVIC_Initialize(void)
{
	NVIC_EnableIRQ(TIM1_CC_IRQn);
	NVIC_SetPriority(TIM1_CC_IRQn, 0x00);
	
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn, 0x00);
}

static void DMA_Initialize(void)
{
	// Channel priority: High speed
	DMA1_Channel3->CCR |= DMA_CCR_PL;
	// Memory increment
	DMA1_Channel3->CCR |= DMA_CCR_MINC;
	// Read from memory
	DMA1_Channel3->CCR |= DMA_CCR_DIR;
	// Peripheral adrress
	DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR;
}

void STM32_Initialize(void)
{
	RCC_Initialize();
	GPIO_Initialize();
	TIM_Initialize();
	USART_Initialize();
	NVIC_Initialize();	
	SPI_Initialize();
	DMA_Initialize();	
	
	// Initialize tamamlandi
	LED_On();
}
