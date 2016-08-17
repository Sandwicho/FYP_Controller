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

//define task functions
void Task1 (void*);

//semaphores



int main (void){
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	xTaskCreate(Task1,"TASK1",200,NULL,1,NULL);
	pio_set(LED0);
	pio_set(LED1);

	vTaskStartScheduler();
	
	
	return 0;
	/* Insert application code here, after the board has been initialized. */
}


void Task1 (void* pvParameters) {
	int tg = 1;
	
	for( ;; ){
	pio_clear(LED0);
	pio_clear(LED1);
	}
	
	/*for(;;){
		if (tg){
			pio_set(LED0);
			pio_set(LED1);
			tg = !tg;
		}
		else {
			pio_clear(LED0);
			pio_clear(LED1);
			tg = !tg;
		}
		
	}*/

}
	
	
	
	
