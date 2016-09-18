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
#include "DW1000.h"

#define Push1		PIO_PA11
#define Push2		PIO_PA12
#define JoySW		PIO_PA13
#define SW4Down		PIO_PA14
#define SW4Up		PIO_PA15
#define SW4Left		PIO_PA16
#define SW4Right	PIO_PA17
#define SW4Push		PIO_PA18
#define SW5Down		PIO_PA19
#define SW5Up		PIO_PA20
#define SW5Left		PIO_PA22
#define SW5Right	PIO_PA23
#define SW5Push		PIO_PA24



//define task functions
void Task1 (void*);
void ButtonTask (void*);

//semaphores
SemaphoreHandle_t PIOAsem = NULL;


uint32_t ButtonStatus;
int LEDtg;
int holdFrame;





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
	
	
	
	
	for(;;){
		
		if(LEDtg)
		
		
		if (tg){
			
			pio_set(LED1);
			tg = !tg;
			//sendDebugString("On\n");
		}
		else {
			
			pio_clear(LED1);
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
	
	int SW4Uptg = 0;
	int SW4Downtg = 0;
	int SW4Lefttg = 0;
	int SW4Righttg = 0;
	int SW5Uptg = 0;
	int SW5Downtg = 0;
	int SW5Lefttg = 0;
	int SW5Righttg = 0;
	
	
	Byte sendArr[18];
	float moveTurn = 0;
	float movDir = 0;
	int cycle = 60;
	int max_i = 0;
	Byte walkEN = 0;
	
	PIOAsem = xSemaphoreCreateBinary();
	
	//shit for stuff
	char buffloat[40];
	char buffcast[40];
	float testfloat;
	
	
	for(;;){
		
		if( PIOAsem !=NULL){
			
			if( xSemaphoreTake(PIOAsem,0xFFFF) == pdTRUE){
				
				switch(ButtonStatus){
					
					case(Push1) :
					sendDebugString("Push Switch 1\n");
					
					
					
					
					
					DW1000_toggleGPIO_MODE();
					DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_LEDC_OFFSET, 0x000F01FF, PMSC_LEDC_LEN);
					/*
					testfloat = 5.5;
					char *c = (char *) &testfloat;
					sprintf(buffloat,"float: %x\n",testfloat);
					
					sendDebugString(buffloat);
					sprintf(buffcast,"cast: %x\n",c);
					sendDebugString(buffcast);
					sprintf(buffcast,"cast2: %x\n",*c);
					sendDebugString(buffcast);
					*/
					
					
					
					if(tg2){
						pio_set(LED2);
						tg2 = !tg2;
					}
					else {
						pio_clear(LED2);
						tg2 = !tg2;
					}
					break;
					
					case(Push2) :
					sendDebugString("Push Switch 2\n");
					
					char buf[40];
					

					delay_us(1);
					sprintf(buf,"TestDevID: 0x%x\n",DW1000_readDeviceIdentifier());
					sendDebugString(buf);
					sendDebugString("\n");
					sprintf(buf,"SysStatus: 0x%x\n", DW1000_readSystemStatus());
					sendDebugString(buf);
					sendDebugString("\n");
					DW1000_writeReg(PANADR_ID,DW1000_NO_SUB,DW1000_NO_OFFSET,0xDECA2230,PANADR_LEN);
					sprintf(buf,"ID WRITTEN\nREAD BACK: 0x%x\n",DW1000_readReg(PANADR_ID,DW1000_NO_SUB,DW1000_NO_OFFSET,PANADR_LEN));
					sendDebugString(buf);
					
					if(tg1){
						pio_set(LED1);
						tg1 = !tg1;
					}
					else {
						pio_clear(LED1);
						tg1 = !tg1;
					}
					break;
					
					case(SW4Left) :
					sendDebugString("NAV4 Left\n");
					
					SW4Lefttg = !SW4Lefttg;
					
					if (SW4Lefttg){
						moveTurn = 0;
						movDir = 4.71;
						cycle = 60;
						max_i = 30;
						walkEN = 1;
						holdFrame = 1;
						
						pio_set(LED1);
						LEDtg = 1;
						
					}
					else{
						
						walkEN = 0;
						holdFrame = 0;
						pio_clear(LED1);
						LEDtg = 0;
					}
					break;
					
					case(SW4Right) :
					sendDebugString("NAV4 Right\n");
					
					SW4Righttg = !SW4Righttg;
					
					if (SW4Righttg){
						moveTurn = 0;
						movDir = 1.57;
						cycle = 60;
						max_i = 30;
						walkEN = 1;
						holdFrame = 1;
						
						pio_set(LED1);
						LEDtg = 1;
						
					}
					else{
						
						walkEN = 0;
						holdFrame = 0;
						pio_clear(LED1);
						LEDtg = 0;
					}
					break;
					
					
					
					case(SW4Up) :
					sendDebugString("NAV4 Up\n");
					
					
					SW4Uptg = !SW4Uptg;
					
					if (SW4Uptg){
						moveTurn = 0;
						movDir = 0;
						cycle = 60;
						max_i = 30;
						walkEN = 1;
						holdFrame = 1;
						
						pio_set(LED1);
						LEDtg = 1;
						
					}
					else{
						
						walkEN = 0;
						holdFrame = 0;
						pio_clear(LED1);
						LEDtg = 0;
					}
					
					break;
					
					
					
					case(SW4Down) :
					sendDebugString("NAV4 Down\n");
					
					
					SW4Downtg = !SW4Downtg;
					
					
					if (SW4Downtg){
						moveTurn = 0;
						movDir = 3.14;
						cycle = 60;
						max_i = 30;
						walkEN = 1;
						holdFrame = 1;
						
						pio_set(LED1);
						LEDtg = 1;
						
					}
					else{
						
						walkEN = 0;
						holdFrame = 0;
						pio_clear(LED1);
						LEDtg = 0;
					}
					
					
					break;
					
					case(SW4Push) :
					sendDebugString("NAV4 Push\n");
					if(tg1){
						pio_set(LED1);
						tg1 = !tg1;
					}
					else {
						pio_clear(LED1);
						tg1 = !tg1;
					}
					break;
					
					case(SW5Left) :
					sendDebugString("NAV5 Left\n");
					
					SW5Lefttg = !SW5Lefttg;
					
					if (SW5Lefttg){
						moveTurn = -1;
						movDir = 0;
						cycle = 60;
						max_i = 30;
						walkEN = 1;
						holdFrame = 1;
						
						pio_set(LED1);
						LEDtg = 1;
						
					}
					else{
						
						walkEN = 0;
						holdFrame = 0;
						pio_clear(LED1);
						LEDtg = 0;
					}
					break;
					
					case(SW5Right) :
					sendDebugString("NAV5 Right\n");
					
					SW5Righttg = !SW5Righttg;
					
					if (SW4Righttg){
						moveTurn = 1;
						movDir = 0;
						cycle = 60;
						max_i = 30;
						walkEN = 1;
						holdFrame = 1;
						
						pio_set(LED1);
						LEDtg = 1;
						
					}
					else{
						
						walkEN = 0;
						holdFrame = 0;
						pio_clear(LED1);
						LEDtg = 0;
					}
					break;
					
					case(SW5Up) :
					sendDebugString("NAV5 Up\n");
					
					SW5Uptg = !SW5Uptg;
					
					if(tg2){
						pio_set(LED2);
						tg2 = !tg2;
					}
					else {
						pio_clear(LED2);
						tg2 = !tg2;
					}
					break;
					
					case(SW5Down) :
					sendDebugString("NAV5 Down\n");
					
					SW5Downtg = !SW5Downtg;
					
					if(tg2){
						pio_set(LED2);
						tg2 = !tg2;
					}
					else {
						pio_clear(LED2);
						tg2 = !tg2;
					}
					break;
					
					case(SW5Push) :
					sendDebugString("NAV5 Push\n");
					if(tg2){
						pio_set(LED2);
						tg2 = !tg2;
					}
					else {
						pio_clear(LED2);
						tg2 = !tg2;
					}
					break;
					
					default :
					sendDebugString("ANALOGUE BITCH!\n");
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
