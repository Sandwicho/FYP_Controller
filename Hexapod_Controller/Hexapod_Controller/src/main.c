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
void SendFrameTask (void*);
//function prototypes, move them to a .h one day, one day
void buildFrame(float Turn,float Dir,int cycle,int max_i,Byte walkEN);
uint32_t getAnalog(int channel);

//semaphores
SemaphoreHandle_t PIOAsem = NULL;
SemaphoreHandle_t FRAMEsem = NULL;

uint32_t ButtonStatus;
int LEDtg = 0;
int holdFrame = 0;
int sendFrame = 0;
int button = 0;
int sendlength = 0;
int cycle = 0;

Byte sendArr[18];

/*
struct spi_device2{
	uint32_t id;
	}spidevice1;
	*/






int main (void){
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	//DW1000_toggleGPIO_MODE();
	xTaskCreate(Task1,"TASK1",600,NULL,2,NULL);
	xTaskCreate(ButtonTask,"BUTTONTASK",600,NULL,1,NULL);
	xTaskCreate(SendFrameTask,"SENDFRAMETASK",600,NULL,3,NULL);

	pio_clear(LED1);
	pio_clear(LED1);
	sendDebugString("Lights on\n Hi Shovel Lord\n");
	vTaskStartScheduler();
	
	
	return 0;
	/* Insert application code here, after the board has been initialized. */
}



void Task1 (void* pvParameters) {
	int tg = 1;
	uint32_t readAnalog = 0;
	char buf[20];
	pio_clear(LED1);
	pio_clear(LED2);
	
	
	
	
	for(;;){
		
		
		readAnalog = getAnalog(0);
		sprintf(buf,"y: %f\n",(float)readAnalog);
		sendDebugString(buf);
		
		readAnalog = getAnalog(1);
		sprintf(buf,"x: %f\n",(float)readAnalog);
		sendDebugString(buf);
		
		if(LEDtg){
		
		
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
		
		}
		vTaskDelay(200);
	

}
}


void SendFrameTask (void* pvParameters){
	//int sendlength = 18;
	int status = 0;
	char buf[20];
	char rxbuf[10];
	for (;;){
	 // kill this
		if (sendFrame){
			if (FRAMEsem !=NULL){
				if(xSemaphoreTake(FRAMEsem,0xFFFF) == pdTRUE){
					//DW1000_clearSystemStatus(0xFFFFFFFF);
				
					
					cmdDWMsend(sendArr,sendlength+2);
					//sendDebugString("HEIL HITLER!!\n");
					status = DW1000_readSystemStatus();
					sprintf(buf,"%x\n",status);
					sendDebugString(buf);
					 //kill this
					if(holdFrame){
						
					
					
					}
					else{
						sendFrame = 0;
						
					}
				
					xSemaphoreGive(FRAMEsem);
				}
			}
		}
	
	
	vTaskDelay(500);
	}
	
	
	
}

void ButtonTask(void* pvParameters){
	
	int tg1 = 1;
	int tg2 = 1;
	int tgd = 1;
	int tgstand = 0;
	
	int SW4Uptg = 0;
	int SW4Downtg = 0;
	int SW4Lefttg = 0;
	int SW4Righttg = 0;
	int SW5Uptg = 0;
	int SW5Downtg = 0;
	int SW5Lefttg = 0;
	int SW5Righttg = 0;
	int floatchange = 0;
	
	
	
	float moveTurn = 0;
	float movDir = 0;
	int cycle = 60;
	int max_i = 0;
	Byte walkEN = 0;
	
	PIOAsem = xSemaphoreCreateBinary();
	FRAMEsem = xSemaphoreCreateMutex();
	
	//shit for stuff
	char buf[40];
	
	float testfloat;
	int i,led = 0;
	
	//spidevice1.id = 0;
	
	for(;;){
		
		if( PIOAsem !=NULL){
			
			if( xSemaphoreTake(PIOAsem,0xFFFF) == pdTRUE){
				
				sendFrame = 1;
				
				switch(ButtonStatus){
					
					case(Push1) :
					sendDebugString("Push Switch 1\n");
					
					button = 1;
					sendlength = 2;
					if (!tgstand){
						sendArr[0] = 4;
						sendArr[1] = 1;
						tgstand = !tgstand;
					} 
					else{
						sendArr[0] = 4;
						sendArr[1] = 0;
						tgstand = !tgstand;
					}
					
					//DW1000_toggleGPIO_MODE();
					
					
					
					/*
					testfloat = 5.5;
					char *c = (char *) &testfloat;
					sprintf(buffloat,"float: %x\n",testfloat);
					try this nigger *((uint32_t*)&variable)
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
					
					//spi_select_device(SPI0,&spidevice1);

					delay_us(1);
					sprintf(buf,"TestDevID: 0x%x\n",DW1000_readDeviceIdentifier());
					sendDebugString(buf);
					sendDebugString("\n");
					
					
					//spi_deselect_device(SPI0,&spidevice1);
					
					sprintf(buf,"SysStatus: 0x%x\n", DW1000_readSystemStatus());
					sendDebugString(buf);
					sendDebugString("\n");
					DW1000_writeReg(PANADR_ID,DW1000_NO_SUB,DW1000_NO_OFFSET,0xDECA2230,PANADR_LEN);
					sprintf(buf,"ID WRITTEN\nREAD BACK: 0x%x\n",DW1000_readReg(PANADR_ID,DW1000_NO_SUB,DW1000_NO_OFFSET,PANADR_LEN));
					sendDebugString(buf);
					
					//pio_set(PIOB_DWM_RESET);
					//pio_clear(PIOB_DWM_RESET);
					
					
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
						
						floatchange = *((uint32_t*)&movDir);
						
						sprintf(buf,"floatchange hex: %x\n",floatchange);
						sendDebugString(buf);
						sprintf(buf,"floatchange float: %f\n",floatchange);
						sendDebugString(buf);
						sprintf(buf,"float orig: %x\n",movDir);
						sendDebugString(buf);
						
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
					
					
					
					SW4Uptg = !SW4Uptg;
					
					if (SW4Uptg){
						sendDebugString("NAV4 Up On\n");
						moveTurn = 0.2;
						movDir = 0;
						cycle = 60;
						max_i = 30;
						walkEN = 1;
						holdFrame = 1;
						
						pio_set(LED1);
						LEDtg = 1;
						
					}
					else{
						sendDebugString("NAV4 Up Off\n");
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
						moveTurn = 0.2;
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
						
						pio_set(LED2);
						LEDtg = 1;
						
					}
					else{
						
						walkEN = 0;
						holdFrame = 0;
						pio_clear(LED2);
						LEDtg = 0;
					}
					break;
					
					case(SW5Right) :
					sendDebugString("NAV5 Right\n");
					
					SW5Righttg = !SW5Righttg;
					
					if (SW5Righttg){
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
				if (!button){
				if (FRAMEsem !=NULL){
					if(xSemaphoreTake(FRAMEsem,0xFFFF) == pdTRUE){
						
						buildFrame(moveTurn,movDir,cycle,max_i,walkEN);
				
				
						xSemaphoreGive(FRAMEsem);
				}
			}
				}
				button = 0;
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

void buildFrame(float Turn,float Dir,int cycle,int max_i,Byte walkEN) {
	int floatchangeTurn = 0;
	int floatchangeDir = 0;
	
	//************************
	//kill this later yo
	char buf[40];
	//************************
	sendlength = 18;
	sendArr[0] = 3;
	
	floatchangeTurn = *((uint32_t*)&Turn);
	floatchangeDir = *((uint32_t*)&Dir);
	
	sendArr[1] = (Byte)(floatchangeTurn);
	sendArr[2] = (Byte)(floatchangeTurn >> 8);
	sendArr[3] = (Byte)(floatchangeTurn >> 16);
	sendArr[4] = (Byte)(floatchangeTurn >> 24);
	
	sendArr[5] = (Byte)(floatchangeDir);
	sendArr[6] = (Byte)(floatchangeDir >> 8);
	sendArr[7] = (Byte)(floatchangeDir >> 16);
	sendArr[8] = (Byte)(floatchangeDir >> 24);
	
	sendArr[9] = (Byte)cycle;
	sendArr[10] = (Byte)(cycle >> 8);
	sendArr[11] = (Byte)(cycle >> 16);
	sendArr[12] = (Byte)(cycle >> 24);
	
	sendArr[13] = (Byte)max_i;
	sendArr[14] = (Byte)(max_i >> 8);
	sendArr[15] = (Byte)(max_i >> 16);
	sendArr[16] = (Byte)(max_i >> 24);
	
	sendArr[17] = walkEN;
	
	
	//printy printy in my winky
	for (int i = 0;i<18;i++){
		sprintf(buf,"byte %d: %x\n",i,sendArr[i]);
		sendDebugString(buf);
		
		
	}
}
	
	uint32_t getAnalog(int channel) {
	uint32_t result;
	
	if (channel == 0){
	afec_channel_enable(AFEC0, AFEC_CHANNEL_0);
	afec_start_software_conversion(AFEC0);
	
	while (!(afec_get_interrupt_status(AFEC0) & (1 << AFEC_CHANNEL_0)));
	//delay_ms(10);
	result = afec_channel_get_value(AFEC0, AFEC_CHANNEL_0);
	//afec_channel_disable(AFEC0, AFEC_CHANNEL_0);
	}
	else{
	afec_channel_enable(AFEC0, AFEC_CHANNEL_1);
	afec_start_software_conversion(AFEC0);
	
	while (!(afec_get_interrupt_status(AFEC0) & (1 << AFEC_CHANNEL_1)));
	//delay_ms(10);
	result = afec_channel_get_value(AFEC0, AFEC_CHANNEL_1);
	//afec_channel_disable(AFEC0, AFEC_CHANNEL_1);
		
	}
	return result;
}
	

	

