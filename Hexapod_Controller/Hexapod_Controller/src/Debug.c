/*
 * Debug.c
 *
 * Created: 9/05/2016 6:08:38 PM
 *  Author: to300
 */ 
#include "Debug.h"

void sendDebugString(const char* in) {
	for(int i=0;i<strlen(in);i++){
		while(!uart_is_tx_ready(UART1));
		uart_write(UART1,in[i]);
	}
}
