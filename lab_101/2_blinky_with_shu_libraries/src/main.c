/*
 * main.c
 *
 * this is the main blinky application (this code is dependent on the
 * extra shu libraries such as the pinmappings list and the clock configuration
 * library)
 *
 * author:    Dr. Alex Shenfield
 * date:      01/09/2017
 * purpose:   55-604481 embedded computer networks : lab 101
 */

// include the hal drivers
#include "stm32f7xx_hal.h"

// include the shu bsp libraries for the stm32f7 discovery board
#include "pinmappings.h"
#include "clock.h"
#include "gpio.h"
#include "random_numbers.h"

// map the led to GPIO PI1 (again, this is the inbuilt led)
gpio_pin_t one = {PB_4, GPIOB, GPIO_PIN_4};
gpio_pin_t two = {PC_7, GPIOC, GPIO_PIN_7};
gpio_pin_t three = {PG_7, GPIOG, GPIO_PIN_7};
gpio_pin_t four = {PH_6, GPIOH, GPIO_PIN_6};
gpio_pin_t five = {PC_6, GPIOC, GPIO_PIN_6};
gpio_pin_t six = {PI_0, GPIOI, GPIO_PIN_0};
gpio_pin_t seven = {PG_6, GPIOG, GPIO_PIN_6};
gpio_pin_t s = {PI_1, GPIOI, GPIO_PIN_1};
// this is the main method
int main()
{
  // we need to initialise the hal library and set up the SystemCoreClock 
  // properly
  HAL_Init();
  init_sysclk_216MHz();
  
  // initialise the gpio pins
  init_gpio(one, OUTPUT);
  init_gpio(two, OUTPUT);
	init_gpio(three, OUTPUT);
	init_gpio(four, OUTPUT);
	init_gpio(five, OUTPUT);
	init_gpio(six, OUTPUT);
	init_gpio(seven, OUTPUT);
	init_gpio(s, INPUT);
  
	init_random();
	
  // loop forever ...
  while(1)
  {
		// clear all leds
		write_gpio(one, LOW);
		write_gpio(two, LOW);
		write_gpio(three, LOW);
		write_gpio(four, LOW);
		write_gpio(five, LOW);
		write_gpio(six, LOW);
		write_gpio(seven, LOW);
		
		// read button
		if(read_gpio(s))
		{
			// throw random number and modulo by 6
			uint32_t rnd = (get_random_int() %6 ) +1;
			
			// turn on appropriate leds
			if(rnd==1)
			{
				write_gpio(four, HIGH);			
			}
			else if (rnd==2)
			{
				write_gpio(two, HIGH);
				write_gpio(six, HIGH);
			}	
			else if (rnd==3)
			{
				write_gpio(two, HIGH);
				write_gpio(six, HIGH);
				write_gpio(four, HIGH);
			}
			else if (rnd==4)
			{
				write_gpio(one, HIGH);
				write_gpio(two, HIGH);
				write_gpio(six, HIGH);
				write_gpio(seven, HIGH);
			}
			else if (rnd==5)
			{
				write_gpio(one, HIGH);
				write_gpio(two, HIGH);
				write_gpio(four, HIGH);
				write_gpio(six, HIGH);
				write_gpio(seven, HIGH);
			}
			else
			{
				write_gpio(one, HIGH);
				write_gpio(two, HIGH);
				write_gpio(three, HIGH);
				write_gpio(five, HIGH);
				write_gpio(six, HIGH);
				write_gpio(seven, HIGH);
			}	
			//time delay before resetting
			HAL_Delay(2000);
		}
	}
}
