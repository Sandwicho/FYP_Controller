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
//#include "spi_master.h"
#include "../Debug.h"
#include "DW1000.h"

#define UART_SERIAL_BAUDRATE		9600
#define UART_SERIAL_CHAR_LENGTH		US_MR_CHRL_8_BIT
#define UART_SERIAL_PARITY			US_MR_PAR_NO
#define	UART_SERIAL_STOP_BIT		US_MR_NBSTOP_1_BIT
#define PIOA_BUTTSANDDIR			PIO_PA11|PIO_PA12|PIO_PA13|PIO_PA18|PIO_PA24|PIO_PA14|PIO_PA15|PIO_PA16|PIO_PA17|PIO_PA19|PIO_PA20|PIO_PA22|PIO_PA23
#define PIOA_BUTTONS				PIO_PA11|PIO_PA12|PIO_PA13|PIO_PA18|PIO_PA24
#define PIOA_DIRECTIONS				PIO_PA14|PIO_PA15|PIO_PA16|PIO_PA17|PIO_PA19|PIO_PA20|PIO_PA22|PIO_PA23
#define PIOD_SPI					PIO_PD20|PIO_PD21|PIO_PD22
#define PIOB_SPI					PIO_PB2


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
                  ADC
	   ######################################
	   ###################################### */
		uint32_t reg = 0;
		
		struct afec_config {
			uint32_t afec_clock;
			bool anach;
			uint8_t ibctl;
			uint32_t mck;
			enum afec_resolution resolution;
			enum afec_startup_time startup_time;
			bool stm;
			bool tag;
			uint8_t tracktim;
			uint8_t transfer;
			bool useq;
			}afec_cfg;
			
		struct afec_ch_config {
			bool diff;
			enum afec_gainvalue gain;
			
			}afec_ch_cfg;
		
		afec_ch_cfg.diff = false;
		afec_ch_cfg.gain = AFEC_GAINVALUE_0;
		
		pmc_enable_periph_clk(AFEC0);
		
		//afec_disable(AFEC0);
		afec_get_config_defaults(&afec_cfg);
		afec_init(AFEC0, &afec_cfg);
		
		afec_channel_set_analog_offset(AFEC0,AFEC_CHANNEL_0,1023>>1);
		afec_channel_set_analog_offset(AFEC0,AFEC_CHANNEL_1,1023>>1);
		//afec_enable_interrupt(AFEC0,AFEC_INTERRUPT_ALL);
		
		
		afec_channel_enable(AFEC0, AFEC_CHANNEL_0);
		afec_channel_enable(AFEC0, AFEC_CHANNEL_1);
		
		//afec_ch_set_config(AFEC0,AFEC_CHANNEL_0,&afec_ch_cfg);
		afec_enable(AFEC0);
		
		//pio_pull_up(PIOD,1<<30,0);
		//pio_pull_down(PIOD,1<<30,0);
		//reg = AFEC0->AFEC_COCR;
		//reg |= 0x400;
		//AFEC0->AFEC_CGR = reg;
		//afec_ch_get_config_defaults(&afec_ch_cfg);
		
		
		//afec_set_trigger(AFEC0, AFEC_TRIG_SW);
		//afec_channel_enable(AFEC0, AFEC_CHANNEL_0);
		
		
	/* ######################################
	   ######################################
					Enable SPI
	   ######################################
	   ###################################### */
		sendDebugString("SPI INITIALIZATION - STARTED\n");
		sysclk_enable_peripheral_clock(ID_SPI0);
		pmc_enable_periph_clk(ID_PIOD);
		pmc_enable_periph_clk(ID_PIOB);
		
		struct spi_device spidevice;
		spidevice.id = 0;
		
		
		//Set up SPI
		spi_master_init(SPI0);
		spi_master_setup_device(SPI0,&spidevice,SPI_MODE_0,16000000,0); //was 16000000
		spi_enable(SPI0);
		spi_select_device(SPI0,&spidevice);
		/*spi_enable_clock(SPI0);
		spi_reset(SPI0);
		spi_set_master_mode(SPI0);
		spi_disable_mode_fault_detect(SPI0);
		spi_disable_loopback(SPI0);
		spi_set_peripheral_chip_select_value(SPI0,spi_get_pcs(DEFAULT_CHIP_ID))*/
		
		
		
		pio_set_peripheral(PIOD,PIO_TYPE_PIO_PERIPH_B,PIOD_SPI);
		pio_set_peripheral(PIOB,PIO_TYPE_PIO_PERIPH_D,PIOB_SPI);
		sendDebugString("SPI INITIALIZATION - FINISHED\n");
		sendDebugString("DWM1000 INITIALIZATION - STARTED\n");
		pio_set_output(PIOB_DWM_RESET,LOW,DISABLE,DISABLE);
		pio_set(PIOB_DWM_RESET);
		pio_clear(PIOB_DWM_RESET);
		
		delay_ms(10);
		//DW1000_initialise();
		DW1000_initialise();
		delay_ms(10);
		DW1000_toggleGPIO_MODE();
		
		
		
		sendDebugString("DWM1000 INITIALIZATION - FINISHED\n");
		
	
	
		
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
		pio_set_input(PIOA,PIOA_BUTTSANDDIR,PIO_PULLUP|PIO_DEBOUNCE);
		pio_set_debounce_filter(PIOA,PIOA_BUTTSANDDIR,100);
		

	
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
		pio_enable_interrupt(PIOA,PIOA_BUTTSANDDIR);
		pio_configure_interrupt(PIOA,PIOA_BUTTONS, PIO_IT_FALL_EDGE);
		pio_configure_interrupt(PIOA,PIOA_DIRECTIONS, PIO_IT_EDGE);
		NVIC_EnableIRQ(PIOA_IRQn);
		
		 
}
