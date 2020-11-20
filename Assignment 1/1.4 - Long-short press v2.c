#include "stm32g431xx.h"

void dummy_delay (uint32_t duration);


int main(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

	GPIOC->MODER &= ~(GPIO_MODER_MODE14_Msk);
	GPIOC->MODER |= 0 << GPIO_MODER_MODE14_Pos;

	GPIOB->MODER &= ~(GPIO_MODER_MODE6_Msk | GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE4_Msk | GPIO_MODER_MODE3_Msk);
	GPIOB->MODER |= 1 << GPIO_MODER_MODE6_Pos | 1 << GPIO_MODER_MODE5_Pos | 1 << GPIO_MODER_MODE4_Pos| 1 << GPIO_MODER_MODE3_Pos;

	uint32_t x = 0;
	int32_t button_is_pressed = 0;
	int32_t long_press_detected = 0;

	while(1)
	{
		if ((GPIOC->IDR & GPIO_IDR_ID14) == 0 && button_is_pressed == 0)
		{
			button_is_pressed = 1;
			dummy_delay(300000);
			if ((GPIOC->IDR & GPIO_IDR_ID14))
			{
				x += 1;
				button_is_pressed = 0;
			}
			else
			{
				x = 0;
				long_press_detected = 1;
			}
		}

		// Отображение счетчика на светодиодах
		GPIOB->ODR &= 0xFF87;	// Очистить уже имеющиеся значения
		GPIOB->ODR |= (x & 0x0F) << 3; //Отобразить новые значения

		if((GPIOC->IDR & GPIO_IDR_ID14) && long_press_detected == 1)
		{
			long_press_detected = 0;
			button_is_pressed = 0;
		}
	}
}

void dummy_delay  (uint32_t duration)
{
	for (uint32_t i = 0; i < duration; ++i);
}
