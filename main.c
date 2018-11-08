/** 
	* @Header: MAIN
  * @FileName: vga.h
  * @Author: Mustafa Aslan
	* @Date:
  */
	
#include "main.h"

uint16_t i,t,a;
uint8_t frame_buffer[120][20];
void TIM1_CC_IRQHandler(void)
{
	if(TIM1->SR & TIM_SR_CC1IF)
	{
		// DMA disable
		DMA1_Channel3->CCR &= ~DMA_CCR_EN;
		// 320 / 8 = 40
		DMA1_Channel3->CNDTR = (uint16_t)20;

		// Memory address 
		DMA1_Channel3->CMAR = (uint32_t)&frame_buffer[i][0];
		
		// DMA enable
		DMA1_Channel3->CCR |= DMA_CCR_EN;		
		
		if(++t % 4 == 3)
		{	
			++i;
		}
		else
		{
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		}
		
		// Son satira ulastiginda
		if(i == 119)
		{
			i = 0;
			// TIM3 CC3 interrupt enable
			TIM3->DIER |= TIM_DIER_CC3IE;
			// TIM1 CC1 interrupt disable
			TIM1->DIER &= ~TIM_DIER_CC1IE;
		}
		// TIM1 CC1 interrupt clear
		TIM1->SR &= ~TIM_SR_CC1IF;
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM3->SR & TIM_SR_CC3IF)
	{
		// TIM1 CC1 interrupt enable
		TIM1->DIER |= TIM_DIER_CC1IE;
		// TIM3 CC3 interrupt disable
		TIM3->DIER &= ~TIM_DIER_CC3IE;	
		// TIM3 CC3 interrupt clear
		TIM3->SR &= ~TIM_SR_CC3IF;
	}
}

int main(void)
{
	frame_buffer[5][5] = 64 + 4;
	frame_buffer[6][5] = 64 + 32 + 8 + 4;
	frame_buffer[7][5] = 64 + 16 +4;
	frame_buffer[8][5] = 64 + 4;
	frame_buffer[9][5] = 64 + 4;
	frame_buffer[10][5] = 64 + 4;
	
	frame_buffer[5][6] = 32;
	frame_buffer[6][6] = 64 + 16;
	frame_buffer[7][6] = 128 + 8;
	frame_buffer[8][6] = 128 + 64 + 32 + 16 + 8;
	frame_buffer[9][6] = 128 + 8;
	frame_buffer[10][6] = 128 + 8;

	// STM32 baslangic konfigurasyonu
	STM32_Initialize();
	

	while(1)
	{

	}


}
