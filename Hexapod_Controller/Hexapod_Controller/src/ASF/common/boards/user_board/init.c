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

	
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
}
