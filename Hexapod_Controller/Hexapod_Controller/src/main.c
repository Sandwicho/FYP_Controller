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

#define ALL_DIG_STICKS (PIO_PA14|PIO_PA15|PIO_PA16|PIO_PA17|PIO_PA18|PIO_PA19|PIO_PA20|PIO_PA20|PIO_PA22|PIO_PA23|PIO_PA24)

#define ANACTRL 1
#define ANA_THRESH 250

#define EX_HGT_MAX 100
#define EX_HGT_MIN 0


//define task functions
void Task1 (void*);
void BuildFrameTask (void*);
void SendFrameTask (void*);
void AnalogTask (void*);
void UpdateTask (void*);
void SendSpecTask (void*);
//function prototypes, move them to a .h one day, one day
void buildFrame(float Turn,float Dir,int cycle,int max_i,Byte walkEN);
void buildFrameExtd(float Turn,float Dir,int cycle,int max_i,Byte walkEN,int stance, int height, int pUp, int stride);
uint32_t getAnalog(int channel);

//semaphores
SemaphoreHandle_t CMDFRAMEsem = NULL;
SemaphoreHandle_t PIOAsem = NULL;
SemaphoreHandle_t FRAMEsem = NULL;

uint32_t ButtonStatus;
int LEDtg = 0;
int holdFrame = 0;
int sendFrame = 0;
int button = 0;

int cycle = 0;


int analogFlag = 0;

uint32_t buttonState = 0;
float anaMag = 0;
float anaAng = 0;

Byte sendArr[40];
int sendlength = 0;

int wasWalking = 0;
//adjustable paramiters
int extraHgt = 0;
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
	xTaskCreate(AnalogTask,"SENDFRAMETASK",600,NULL,4,NULL);
	xTaskCreate(BuildFrameTask,"BUILDFRAMETASK",600,NULL,5,NULL);
	xTaskCreate(SendFrameTask,"SENDFRAMETASK",600,NULL,3,NULL);
	xTaskCreate(UpdateTask,"UPDATETASK",600,NULL,6,NULL);
	xTaskCreate(SendSpecTask,"SENDSPECTASK",600,NULL,7,NULL);
	
	pio_clear(LED1);
	pio_clear(LED1);
	sendDebugString("Lights on\n Hi Shovel Lord\n");
	vTaskStartScheduler();
	
	
	return 0;
	/* Insert application code here, after the board has been initialized. */
}


//flashy task
void Task1 (void* pvParameters) {
	int tg = 1;
	char buf[20];
	pio_clear(LED1);
	pio_clear(LED2);
	
	
	
	
	for(;;){
		//sprintf(buf,"anaMag:%f, anaAngle:%f\n",sqrt(AnaX*AnaX + AnaY*AnaY));
		//sendDebugString(buf);

		
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
		vTaskDelay(500);
		

	}
}
//done
void SendFrameTask (void* pvParameters){
	
	for (;;){
		if (FRAMEsem !=NULL){
			if(xSemaphoreTake(FRAMEsem,0xFFFF) == pdTRUE){
				cmdDWMsend(sendArr,sendlength+2);
			}
		}
	}
}

void BuildFrameTask(void* pvParameters){
	FRAMEsem = xSemaphoreCreateBinary();
	float moveTurn = 0.2;
	float movDir = 0;
	int cycle = 60;
	int max_i = 45;
	Byte walkEN = 0;
	int stance = 165;
	int height = 20;
	int pUp = 85;
	int stride = 65;
	
	char buf[20];
	
	for(;;){
		
		//sprintf(buf,"buttonState: %8x : %8x\n",buttonState&SW5Left,buttonState&SW5Right);
		//sendDebugString(buf);
		if(anaMag > ANA_THRESH || !(buttonState&SW5Left) || !(buttonState&SW5Right)) {
			if(!(buttonState&SW5Left)) moveTurn = -1;
			else if(!(buttonState&SW5Right)) moveTurn = 1;
			else moveTurn = 0.2;
			
			if(!(anaMag > ANA_THRESH)) anaAng = 0;
			
			wasWalking = 5;
			max_i = 45;
			stance =  165;
			//default is 20; scale to 100 at sideways
			height = (20+extraHgt) + (80-extraHgt)*(fabsf(sin(anaAng)));
			//default is 85; scale to 40 at sideways
			pUp    = 85 - 45*(fabsf(sin(anaAng)));
			//default is 65; reduce to 40 at sideways
			stride = 65 - 35*(fabsf(sin(anaAng))) - 0.2*extraHgt;
			//start at 100 for 250; move to 30 at 2100
			cycle = 100 - (anaMag-250.00)*0.025;
			//walkEN
			walkEN = 1;
			buildFrameExtd(moveTurn,anaAng,cycle,max_i,walkEN,stance,height,pUp,stride);
			sendlength = 35;
			xSemaphoreGive(FRAMEsem);
		}
		
		else {
			if(wasWalking != 0) {
				wasWalking--;
				max_i = 0;
				walkEN = 0;
				height = (20+extraHgt);
				buildFrameExtd(moveTurn,anaAng,cycle,max_i,walkEN,stance,height,pUp,stride);
				sendlength = 35;
				xSemaphoreGive(FRAMEsem);
			}
		}
		vTaskDelay(200);
		

	}
}
//done
void AnalogTask (void* pvParameters) {
	int AnaX = 0;
	int AnaY = 0;
	
	for(;;) {
		AnaY = 2048-getAnalog(0);
		AnaX = getAnalog(1)-2048;
		anaAng = atan2f(AnaY,AnaX) - (M_PI/2.00);
		anaMag = sqrt(AnaX*AnaX + AnaY*AnaY);
		vTaskDelay(20);
	}
}
//done
void UpdateTask (void* pvParameters) {
	PIOAsem = xSemaphoreCreateBinary();
	buttonState = PIOA->PIO_PDSR;
	for(;;) {
		if( PIOAsem !=NULL){
			if( xSemaphoreTake(PIOAsem,0xFFFF) == pdTRUE){				
				buttonState = PIOA->PIO_PDSR;
				if(ButtonStatus&SW4Up && !(buttonState&SW4Up)) {
					extraHgt+=5;
					wasWalking = 1;
				}
				if(ButtonStatus&SW4Down && !(buttonState&SW4Down)) {
					extraHgt-=5;
					wasWalking = 1;
				}
				if(extraHgt > EX_HGT_MAX) extraHgt = EX_HGT_MAX;
				if(extraHgt < EX_HGT_MIN) extraHgt = EX_HGT_MIN;
			}
		}
	}
}
//done
void SendSpecTask (void* pvParameters) {
	CMDFRAMEsem = xSemaphoreCreateBinary();
	int standing = 0;
	int surp = 0;
	char sendbuf[10];
	for(;;) {
		if( CMDFRAMEsem !=NULL){
			if( xSemaphoreTake(CMDFRAMEsem,0xFFFF) == pdTRUE){
				sendDebugString("sendSpecialFrame\n");
				if(ButtonStatus&Push1) {
					sendbuf[0] = 4;
					if(standing) {
						standing = 0;
						sendbuf[1] = 1;
						sendDebugString("sitDown\n");
					}
					else {
						standing = 1;
						sendDebugString("standUp\n");
						sendbuf[1] = 0;
					}
				}
				
				else if(ButtonStatus&Push2) {
					sendbuf[0] = 5;
					if(surp) {
						surp = 0;
						sendbuf[1] = 1;
					}
					else {
						surp = 1;
						sendbuf[1] = 0;
					}
				}
				cmdDWMsend(sendbuf,4);
			}
		}
	}
}
//done
void PIOA_Handler (void) {
	ButtonStatus = pio_get_interrupt_status(PIOA);
	ButtonStatus &= pio_get_interrupt_mask(PIOA);
	if(ButtonStatus&Push1 || ButtonStatus&Push2) xSemaphoreGiveFromISR(CMDFRAMEsem,NULL);
	else {
		sendDebugString("UpdateSemCalled\n");
		 xSemaphoreGiveFromISR(PIOAsem,NULL);
	}

}



//                       **done functions**

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
void buildFrameExtd(float Turn,float Dir,int cycle,int max_i,Byte walkEN,int stance, int height, int pUp, int stride) {
	int floatchangeTurn = 0;
	int floatchangeDir = 0;
	
	//************************
	//kill this later yo
	char buf[40];
	//************************
	sendlength = 35;
	sendArr[0] = 2;
	
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
	
	//stance
	sendArr[18] = (Byte)stance;
	sendArr[19] = (Byte)(stance >> 8);
	sendArr[20] = (Byte)(stance >> 16);
	sendArr[21] = (Byte)(stance >> 24);
	
	//height
	sendArr[22] = (Byte)height;
	sendArr[23] = (Byte)(height >> 8);
	sendArr[24] = (Byte)(height >> 16);
	sendArr[25] = (Byte)(height >> 24);
	
	//pull up
	sendArr[26] = (Byte)pUp;
	sendArr[27] = (Byte)(pUp >> 8);
	sendArr[28] = (Byte)(pUp >> 16);
	sendArr[29] = (Byte)(pUp >> 24);
	
	//stride
	sendArr[30] = (Byte)stride;
	sendArr[31] = (Byte)(stride >> 8);
	sendArr[32] = (Byte)(stride >> 16);
	sendArr[33] = (Byte)(stride >> 24);
	
	//retval
	sendArr[34] = 1;
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




