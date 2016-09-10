/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../Debug.h"

#define Push1 PIO_PA11
#define Push2 PIO_PA12


//define task functions
void Task1 (void*);
void ButtonTask (void*);

//semaphores
SemaphoreHandle_t PIOAsem = NULL;

uint32_t ButtonStatus;



int main (void){
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	xTaskCreate(Task1,"TASK1",200,NULL,2,NULL);
	xTaskCreate(ButtonTask,"BUTTONTASK",200,NULL,1,NULL);
	pio_set(LED1);
	pio_set(LED2);
	
	sendDebugString("Lights on\n Hi Shovel Lord\n");
	vTaskStartScheduler();
	
	
	return 0;
	/* Insert application code here, after the board has been initialized. */
}


void Task1 (void* pvParameters) {
	int tg = 1;
	
	pio_clear(LED1);
	pio_clear(LED2);
	
	for(;;){
		if (tg){
			//pio_set(LED0);
			//pio_set(LED1);
			tg = !tg;
			//sendDebugString("On\n");
		}
		else {
			//pio_clear(LED0);
			//pio_clear(LED1);
			tg = !tg;
			//sendDebugString("Fresh\n");
		}
		
		
		vTaskDelay(1000);
	}

}

void ButtonTask(void* pvParameters){
	
	int tg1 = 1;
	int tg2 = 1;
	int tgd = 1;
	
	PIOAsem = xSemaphoreCreateBinary();
	
	for(;;){
		
		if( PIOAsem !=NULL){
			
			if( xSemaphoreTake(PIOAsem,0xFFFF) == pdTRUE){
				
				switch(ButtonStatus){
					
					case(Push1) :
						if(tg1){
						pio_set(LED2);
						tg1 = !tg1;
						}
						else {
							pio_clear(LED2);
							tg1 = !tg1;
						}
						break;
				
					case(Push2) :
						if(tg2){
						pio_set(LED1);
						tg2 = !tg2;
						}
						else {
							pio_clear(LED1);
							tg2 = !tg2;
						}
						break;
					
					default :
						if(tgd){
							pio_set(LED1);
							pio_set(LED2);
							tgd = !tgd;
						}
						else {
							pio_clear(LED1);
							pio_clear(LED2);
							tgd = !tgd;
						}
						break;
				
				
				}	
			}
		}
	}
}




	/*
	void UART4_Handler(void) {
		uint32_t imr = ISI->ISI_IMR;
		char temp;
		uart_read(UART4,&temp);
		CLIbuf[CLIbufIndex] = temp;
		CLIbufIndex++;
		if(temp = "\n") xSemaphoreGiveFromISR(UARTsem,NULL);
	}*/
	
void PIOA_Handler (void) {
	
	ButtonStatus = pio_get_interrupt_status(PIOA);
	ButtonStatus &= pio_get_interrupt_mask(PIOA);
	xSemaphoreGiveFromISR(PIOAsem,NULL);

}
