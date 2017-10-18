/*
 * main.c
 *
 * this is the main lcd application
 *
 * author:    Dr. Alex Shenfield
 * date:      04/09/2017
 * purpose:   55-604481 embedded computer networks : lab 102
 */

// include the basic headers and hal drivers
#include "stm32f7xx_hal.h"

// include the shu bsp libraries for the stm32f7 discovery board
#include "pinmappings.h"
#include "clock.h"
#include "stm32746g_discovery_lcd.h"
#include "adc.h"
#include "gpio.h"

// include the serial configuration files
#include "serial.h"

// include the stdio library
#include <stdio.h>

// map the led to GPIO PA8 and the potentiometer to PA0
gpio_pin_t led = {PI_1, GPIOI, GPIO_PIN_1};
gpio_pin_t pot = {PA_0, GPIOA, GPIO_PIN_0};

// LCD DEFINES

// define a message boarder (note the lcd is 28 characters wide using Font24)
#define BOARDER     "****************************"

// specify a welcome message
const char * welcome_message[2] = 
{
  "*     Hello LCD World!     *",
  "*      Welcome to SHU      *"
};

// CODE

// this is the main method
int main()
{
  // we need to initialise the hal library and set up the SystemCoreClock 
  // properly
  HAL_Init();
  init_sysclk_216MHz();
  
  // initialise the lcd
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, SDRAM_DEVICE_ADDR);
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
	
	 // initialise the uart, adc and gpio pins
  init_adc(pot);
  init_gpio(led, OUTPUT);
  

  // set the background colour to blue and clear the lcd
  BSP_LCD_SetBackColor(LCD_COLOR_CYAN);
  BSP_LCD_Clear(LCD_COLOR_CYAN);
  
  // set the font to use
  BSP_LCD_SetFont(&Font24); 
  
  // print the welcome message ...
  BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
  BSP_LCD_DisplayStringAtLine(0, (uint8_t *)BOARDER);
  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)welcome_message[0]);
  BSP_LCD_DisplayStringAtLine(2, (uint8_t *)welcome_message[1]);
  BSP_LCD_DisplayStringAtLine(3, (uint8_t *)BOARDER); 
    
  // delay a little ...
  HAL_Delay(5000);
  
  // display an "uptime" counter
  while(1)
  {
		uint16_t adc_val = read_adc(pot);
		
		//format a string based around the adc value and print to lcd
		char st[12];
		sprintf(st, "ADC = %4d", adc_val);
		
		BSP_LCD_DisplayStringAtLine(6, (uint8_t *)st);
		BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
		BSP_LCD_FillRect( 0, 200, 450, 12 );
		BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
		BSP_LCD_FillRect( 0, 200, adc_val/11, 12 );
		
    
    HAL_Delay(1000);
  }
}
