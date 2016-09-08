/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "../Debug.h"

#define UART_SERIAL_BAUDRATE		9600
#define UART_SERIAL_CHAR_LENGTH		US_MR_CHRL_8_BIT
#define UART_SERIAL_PARITY			US_MR_PAR_NO
#define	UART_SERIAL_STOP_BIT		US_MR_NBSTOP_1_BIT
#define PIOA_BUTTONS				PIO_PA11|PIO_PA12|PIO_PA13|PIO_PA14|PIO_PA15|PIO_PA16|PIO_PA17|PIO_PA18|PIO_PA19|PIO_PA20|PIO_PA22|PIO_PA23|PIO_PA24

void board_init(void)
{
	
	/* ######################################
	   ######################################
					Setup Clock
	   ######################################
	   ###################################### */
	CONFIG_SYSCLK_SOURCE;
	supc_switch_sclk_to_32kxtal(SUPC,0);
	sysclk_init();
	delay_init(150000000);

	/* ######################################
	   ######################################
					Enable UART
	   ######################################
	   ###################################### */
	const sam_uart_opt_t usart_console_settings = {
		UART_SERIAL_BAUDRATE,
		UART_SERIAL_CHAR_LENGTH,
		UART_SERIAL_PARITY,
		UART_SERIAL_STOP_BIT,
		UART_MR_CHMODE_NORMAL
	};
	sysclk_enable_peripheral_clock(ID_UART1);
	pmc_enable_periph_clk(ID_UART1);
	uart_init(UART1, &usart_console_settings);
	uart_enable(UART1);
	uart_enable_tx(UART1);
	uart_enable_rx(UART1);
	uart_set_clock_divisor(UART1,(83/1)); //Pippin had GLOBAL_SLOWDOWN IT WAS VALUED 1
	pmc_enable_periph_clk(ID_PIOA);
	pio_set_peripheral(PIOA,PIO_TYPE_PIO_PERIPH_C,1<<4 | 1<<5);
	//Test UART
	sendDebugString("MELLATRON9000 BOOT SEQUENCE\nUART CONSOLE STARTED ON UART4\nBaudRate: 115200\nBits: 8\nNo stop bits \n");
	
	
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	/* ######################################
	   ######################################
					Enable LEDS
	   ######################################
	   ###################################### */
	//sendDebugString("LED INITIALIZATION - STARTED\n");
		//pio_set_peripheral(PIOB,PIO_TYPE_PIO_OUTPUT_1,1<<12);
		//pmc_enable_periph_clk(ID_PIOA);
		pmc_enable_periph_clk(ID_PIOD);
		pio_set_output(LED1,LOW,DISABLE,DISABLE);
		pio_set_output(LED2,LOW,DISABLE,DISABLE);
		//sendDebugString("LED INITIALIZATION - FINISHED\n");
		
		
		/* ######################################
	   ######################################
					Disable Watchdog
	   ######################################
	   ###################################### */
		sendDebugString("WATCHDOG INITIALIZATION - STARTED\n");
		wdt_disable(WDT);
		sendDebugString("WATCHDOG INITIALIZATION - FINISHED\n");
		
		
		/* ######################################
	   ######################################
					Enable IRQ
	   ######################################
	   ###################################### */
		sendDebugString("GLOBAL IRQ INITIALIZATION - STARTED\n");
		cpu_irq_enable();
		sendDebugString("GLOBAL IRQ INITIALIZATION - FINISHED\n");
		
		
	/* ######################################
	   ######################################
                  Setup Switches
	   ######################################
	   ###################################### */
		pmc_enable_periph_clk(ID_PIOA);
		pio_set_input(PIOA,PIOA_BUTTONS,PIO_PULLUP|PIO_DEBOUNCE);
	
	
	/* ######################################
	   ######################################
                  Setup Interrupts
	   ######################################
	   ###################################### */
	
		sendDebugString("PERIPHERAL IRQ INITIALIZATION - STARTED\n");
		//ISI
		//isi_enable_interrupt(ISI,1<<16|1<<17);
		//NVIC_ClearPendingIRQ(ISI_IRQn);
		//NVIC_SetPriority(ISI_IRQn,7);
		//NVIC_EnableIRQ(ISI_IRQn);
		
		//UART4
		uart_enable_interrupt(UART1,UART_IER_RXRDY);
		NVIC_ClearPendingIRQ(UART1_IRQn);
		NVIC_SetPriority(UART1_IRQn,6);
		NVIC_EnableIRQ(UART1_IRQn);
		sendDebugString("PERIPHERAL IRQ INITIALIZATION - FINISHED\n");

		NVIC_DisableIRQ(PIOA_IRQn);
		NVIC_ClearPendingIRQ(PIOA_IRQn);
		NVIC_SetPriority(PIOA_IRQn, 6);
		pio_enable_interrupt(PIOA,PIOA_BUTTONS);
		pio_configure_interrupt(PIOA,PIOA_BUTTONS, PIO_IT_FALL_EDGE);
		NVIC_EnableIRQ(PIOA_IRQn);
		
		 
}
