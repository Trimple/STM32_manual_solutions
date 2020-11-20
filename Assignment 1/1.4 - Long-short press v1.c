#include "stm32g431xx.h"

void dummy_delay(uint32_t duration);

int main(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

	GPIOC->MODER &= ~(GPIO_MODER_MODE13_Msk); // Настройка на цифровой вход

	GPIOB->MODER &= ~(GPIO_MODER_MODE3_Msk | GPIO_MODER_MODE4_Msk | GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE6_Msk);
	GPIOB->MODER |= 1 << GPIO_MODER_MODE3_Pos | 1 << GPIO_MODER_MODE4_Pos | 1 << GPIO_MODER_MODE5_Pos | 1 << GPIO_MODER_MODE6_Pos;

	uint32_t button_is_pressed = 0;

	uint32_t counter_is_started = 0;
	uint32_t counter_value = 0;
	uint32_t counter_value_new = 0;


	while(1)
	{
		dummy_delay(1000);

		// Обработка кнопки №1
		if((GPIOC->IDR & GPIO_IDR_ID13) == 0 && button_is_pressed == 0)
		{
			button_is_pressed = 1;
			counter_is_started = 1;
			++counter_value;
		}
		else if ((GPIOC->IDR & GPIO_IDR_ID13)) // Если кнопка отпущена
		{
			if (button_is_pressed == 1)
			{
				counter_is_started = 0;

				if ( counter_value < 200)
				{
					counter_value_new++;
				}
				else
				{
					counter_value_new = 0;
				}

				counter_value = 0;
			}
			button_is_pressed = 0;
		}

		if(counter_is_started == 1)
		{
			++counter_value;
		}

		GPIOB->ODR &= ~(GPIO_ODR_OD3 | GPIO_ODR_OD4 | GPIO_ODR_OD5 | GPIO_ODR_OD6);
		GPIOB->ODR |= (counter_value_new & 0x0F) << 3;
	}
}

void dummy_delay(uint32_t duration)
{
		for(uint32_t i = 0; i < duration; ++i);
}
