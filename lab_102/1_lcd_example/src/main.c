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

gpio_pin_t pot = {PF_6, GPIOF, GPIO_PIN_6};
gpio_pin_t temp = {PF_7, GPIOF, GPIO_PIN_7};

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
	init_adc(temp);
	
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
  BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"Calibrating ...");
	
	int sensor_max = 0;
	int sensor_min = 100000;
	
	while (HAL_GetTick() < 10000)
	{
		int sensor_val = read_adc(pot);
		
		if (sensor_val > sensor_max)
		{
			sensor_max = sensor_val;
		}
		if (sensor_val < sensor_min)
		{
			sensor_min = sensor_val;
		}		
	}
	
	float sensor_range = (float)(sensor_max-sensor_min);
	char st4[20];
	sprintf(st4, "Range = %4d - %4d", sensor_min, sensor_max);
	BSP_LCD_DisplayStringAtLine(4, (uint8_t *)st4);
		
  // display an "uptime" counter
  while(1)
  {
		// make adc values for temp 
		uint16_t adc_val_tmp = read_adc(temp);
		float temp = ((((adc_val_tmp / 4095.0) * 3.3) - 0.5) * 1000.0) / 10.0;
		
		uint16_t adc_val_lht = read_adc(pot);
		float LightPercentage = (((adc_val_lht - sensor_min) / (sensor_range))*100.0);
		
		//format a string based around the adc value and print to lcd
		char st1[20];
		sprintf(st1, "Temp = %2.2f", temp);
		
		char st3[20];
		sprintf(st3, "Light \% = %3.2f", LightPercentage);
				
		BSP_LCD_DisplayStringAtLine(6, (uint8_t *)st1);
		BSP_LCD_DisplayStringAtLine(7, (uint8_t *)st3);
		
		// clear bar charts
		BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
		BSP_LCD_FillRect( 0, 200, 450, 12 );
		BSP_LCD_FillRect( 0, 220, 450, 12 );
		
		// draw bar charts
		BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
		BSP_LCD_FillRect( 0, 200, adc_val_tmp/5, 12 );
		BSP_LCD_FillRect( 0, 220, (int)((LightPercentage / 100.0) * 420), 12 );
		
    
    HAL_Delay(1000);
  }
}
