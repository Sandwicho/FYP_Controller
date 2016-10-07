/* DW1000 library */
#include "DW1000.h"

/****************************************************************************/
/* CONSTRUCTOR */

DW1000(int ss, int rst) {
	
	//Initalisation code that came with the arduino Lib, not required for our system
	//DW1000__ss = ss;	// global _ss slave select pin
	//DW1000__rst = rst;	// global _rst reset pin
	
//
	//pinMode(DW1000__ss, OUTPUT);
	//digitalWrite(DW1000__ss, HIGH);
	//pinMode(DW1000__rst, OUTPUT);
	//digitalWrite(DW1000__rst, HIGH);
//
	//SPI.begin();
	//SPI.setBitOrder(MSBFIRST);
	//SPI.setDataMode(SPI_MODE0);
	//SPI.setClockDivider(SPI_CLOCK_DIV8);
}

/****************************************************************************/
/* DW1000 API */

void DW1000_resetChip() {
	//digitalWrite(DW1000__rst, LOW);
	//delay(5);
	//digitalWrite(DW1000__rst, HIGH);
	//delay(5);
}

void DW1000_softReset() {
	long buffer = DW1000_readReg(PMSC_ID, DW1000_SUB, PMSC_CTRL0_OFFSET, PMSC_CTRL0_LEN);
	 DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL0_OFFSET, (buffer | PMSC_CTRL0_SYSCLKS_19M), PMSC_CTRL0_LEN);
	 DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL0_OFFSET, (buffer & 0xEFFFFFFF), PMSC_CTRL0_LEN);
	 DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL0_OFFSET, (buffer | 0xF0000000), PMSC_CTRL0_LEN);
	 DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL0_OFFSET, (buffer | PMSC_CTRL0_SYSCLKS_AUTO), PMSC_CTRL0_LEN);
}

void DW1000_printAnything(uint8_t cmd, int subindex, uint16_t offset, int n) {
	long result = DW1000_readReg(cmd, subindex, offset, n);
	//Serial.print(result, HEX);
}

void DW1000_printAnythingln(uint8_t cmd, int subindex, uint16_t offset, int n) {
	long result = DW1000_readReg(cmd, subindex, offset, n);
	//Serial.println(result, HEX);
}

void DW1000_writePANAddress() {
	 DW1000_writeReg(PANADR_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, 0x12345678, PANADR_LEN);
}

void DW1000_initialise2() {
	

}
/* My settings (recommended values from DWM1000 User Manual)
 * Channel:				3
 * PRF:					16 MHz
 * Data rate:			110 kbps (changed from 6.8 Mbps)
 * Preamble code:		5
 * Preamble length:		2048 (changed from 128)
 * PAC size:			64 (changed from 8)
 * SFD length:			64 (changed from 8 (standard))
 */
void DW1000_initialise() {
	// Reset chip
	//digitalWrite(DW1000__rst, LOW);
	//delay(5);
	//digitalWrite(DW1000__rst, HIGH);
	//delay(5);

	// Channel, preamble, bitrate selection
	 DW1000_writeReg(CHAN_CTRL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, 0x10840011, CHAN_CTRL_LEN);
	 //DW1000_writeReg(TX_FCTRL_ID, NO_SUB, NO_OFFSET, 0x0015400C, TX_FCTRL_LEN);
	 DW1000_writeReg(ACK_RESP_T_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, 0x00000000, ACK_RESP_T_LEN); // changed
	 DW1000_writeReg(SYS_CFG_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, 0x20441200, SYS_CFG_LEN); // changed
	 DW1000_writeReg(TX_POWER_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, 0x1F1F1F1F, TX_POWER_LEN);

	// Default values that should be modified
	 DW1000_writeReg(AGC_CTRL_ID, DW1000_SUB, AGC_TUNE1_OFFSET, (AGC_TUNE1_16M & AGC_TUNE1_MASK), AGC_TUNE1_LEN);
	 DW1000_writeReg(AGC_CTRL_ID, DW1000_SUB, AGC_TUNE2_OFFSET, (AGC_TUNE2_VAL & AGC_TUNE2_MASK), AGC_TUNE2_LEN);
	 DW1000_writeReg(AGC_CTRL_ID, DW1000_SUB, AGC_TUNE3_OFFSET, (AGC_TUNE3_VAL & AGC_TUNE3_MASK), AGC_TUNE3_LEN);

	 DW1000_writeReg(DRX_CONF_ID, DW1000_SUB, DRX_TUNE0b_OFFSET, 0x000A, DRX_TUNE0b_LEN); // changed
	 DW1000_writeReg(DRX_CONF_ID, DW1000_SUB, DRX_TUNE1a_OFFSET, 0x0087, DRX_TUNE1a_LEN);
	 DW1000_writeReg(DRX_CONF_ID, DW1000_SUB, DRX_TUNE1b_OFFSET, 0x0064, DRX_TUNE1b_LEN); // changed
	 DW1000_writeReg(DRX_CONF_ID, DW1000_SUB, DRX_TUNE2_OFFSET, 0x371A011D, DRX_TUNE2_LEN); // changed````
	 DW1000_writeReg(DRX_CONF_ID, DW1000_SUB, DRX_TUNE4H_OFFSET, 0x0028, DRX_TUNE4H_LEN); // changed
	 
	// DW1000_writeReg(DRX_CONF_ID, DW1000_SUB, DRX_SFDTOC_OFFSET, 0xFFFF, DRX_SFDTOC_LEN); // changed

	 DW1000_writeReg(RF_CONF_ID, DW1000_SUB, RF_RXCTRLH_OFFSET, 0xD8, 1);
	 DW1000_writeReg(RF_CONF_ID, DW1000_SUB, RF_TXCTRL_OFFSET, RF_TXCTRL_CH1, RF_TXCTRL_LEN);

	 DW1000_writeReg(TX_CAL_ID, DW1000_SUB, TC_PGDELAY_OFFSET, TC_PGDELAY_CH1, TC_PGDELAY_LEN);

	 DW1000_writeReg(FS_CTRL_ID, DW1000_SUB, FS_PLLCFG_OFFSET, FS_PLLCFG_CH1, FS_PLLCFG_LEN);
	 DW1000_writeReg(FS_CTRL_ID, DW1000_SUB, FS_PLLTUNE_OFFSET, FS_PLLTUNE_CH1, FS_PLLTUNE_LEN);

	 DW1000_writeReg(LDE_IF_ID, DW1000_SUB, LDE_CFG1_OFFSET, 0x6D, LDE_CFG1_LEN);
	 DW1000_writeReg(LDE_IF_ID, DW1000_SUB, LDE_CFG2_OFFSET, 0x1607, LDE_CFG2_LEN);
	 DW1000_writeReg(LDE_IF_ID, DW1000_SUB, LDE_REPC_OFFSET, (0x451E/8), LDE_REPC_LEN); // changed

	// Ensure CPLOCK and CPLL_LL flags are working correctly
	 DW1000_writeReg(EXT_SYNC_ID, DW1000_SUB, EC_CTRL_OFFSET, 0x4, EC_CTRL_LEN);
	 DW1000_writeReg(SYS_MASK_ID, DW1000_NO_SUB,DW1000_NO_OFFSET,1<<13,SYS_MASK_LEN);
	 	DW1000_writeReg(OTP_IF_ID, DW1000_SUB, OTP_CTRL, 0x03,1);
	// DW1000_writeReg(AON_ID, SUB, AON_WCFG_OFFSET, 0, AON_WCFG_LEN);
	 delay_ms(100);
	
	// Load LDE microcode from ROM to RAM - NAH FUCK THAT
	 //DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL0_OFFSET, (0x0301),  2);
	 //DW1000_writeReg(OTP_IF_ID, DW1000_SUB, OTP_CTRL, 0x8000,			2);
	 //delay_ms(2);
	 //DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL0_OFFSET, (0x0200),  2);

	 
	 DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL1_OFFSET, (0x10000738),  4);
	 
	 

	//long temp = DW1000_readReg(PMSC_ID, DW1000_SUB, PMSC_CTRL1_OFFSET, PMSC_CTRL1_LEN);
	//DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL1_OFFSET, (temp & 0xFFFDFFFF), PMSC_CTRL1_LEN);
	//DW1000_writeReg(OTP_IF_ID, DW1000_SUB, OTP_CTRL, 0x8000, OTP_IF_LEN);
	//delay_us(150);
	//DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL1_OFFSET, (temp | PMSC_CTRL1_LDERUNE), PMSC_CTRL1_LEN);
}

void DW1000_loadLDE() {
	long temp = DW1000_readReg(PMSC_ID, DW1000_SUB, PMSC_CTRL1_OFFSET, PMSC_CTRL1_LEN);
	 DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL1_OFFSET, (temp & 0xFFFDFFFF), PMSC_CTRL1_LEN);
	 DW1000_writeReg(OTP_IF_ID, DW1000_SUB, OTP_CTRL, 0x8000, OTP_IF_LEN);
	 delay_ms(150);
	 DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL1_OFFSET, (temp | PMSC_CTRL1_LDERUNE), PMSC_CTRL1_LEN);
}

uint64_t DW1000_readDeviceIdentifier() {
	uint64_t result = DW1000_readReg(DEV_ID_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, DEV_ID_LEN);
	return result;
}

uint64_t DW1000_readSystemConfiguration() {
	DW1000__syscfg = DW1000_readReg(SYS_CFG_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, SYS_CFG_LEN);
	return DW1000__syscfg;
}

uint64_t DW1000_readSystemStatus() {
	DW1000__sysstat = DW1000_readReg(SYS_STATUS_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, SYS_STATUS_LEN);
	return DW1000__sysstat;
}

// uint64_t temp = DW1000_readReg(RX_TIME_ID, SUB, RX_TIME_RX_STAMP_OFFSET, RX_TIME_RX_STAMP_LEN);
double DW1000_readRxTimestamp() {
	uint64_t low = DW1000_readReg(RX_TIME_ID, DW1000_SUB, RX_TIME_RX_STAMP_OFFSET, 4);
	uint64_t high = DW1000_readReg(RX_TIME_ID, DW1000_SUB, RX_TIME_RX_STAMP_OFFSET+4, 1);

	uint64_t rxtime = low | (high << 32);
	double result = rxtime * (1/(128*499.2e6));

	return result;
}

int DW1000_fpsp_f1() {
	return DW1000_readReg(RX_TIME_ID, DW1000_SUB, RX_TIME_FP_AMPL1_OFFSET, 2);
}

int DW1000_fpsp_f2() {
	return ((DW1000_readReg(RX_FQUAL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, 4) & RX_EQUAL_FP_AMPL2_MASK) >> RX_EQUAL_FP_AMPL2_SHIFT);
}

int DW1000_fpsp_f3() {
	return ((DW1000_readReg(RX_FQUAL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, RX_FQUAL_LEN) & RX_EQUAL_PP_AMPL3_MASK) >> RX_EQUAL_PP_AMPL3_SHIFT);
}

int DW1000_fpsp_n() {
	return ((DW1000_readReg(RX_FINFO_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, RX_FINFO_LEN) & RX_FINFO_RXPACC_MASK) >> RX_FINFO_RXPACC_SHIFT);
}

int DW1000_rx_c() {
	return ((DW1000_readReg(RX_FQUAL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, RX_FQUAL_LEN) & RX_EQUAL_CIR_MXG_MASK) >> RX_EQUAL_CIR_MXG_SHIFT);
}

double DW1000_readFirstPathSignalPower(int PRF) {
	uint16_t f1 = DW1000_readReg(RX_TIME_ID, DW1000_SUB, RX_TIME_FP_AMPL1_OFFSET, 2);
	uint16_t f2 = (DW1000_readReg(RX_FQUAL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, 4) & RX_EQUAL_FP_AMPL2_MASK) >> RX_EQUAL_FP_AMPL2_SHIFT;
	uint16_t f3 = (DW1000_readReg(RX_FQUAL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, RX_FQUAL_LEN) & RX_EQUAL_PP_AMPL3_MASK) >> RX_EQUAL_PP_AMPL3_SHIFT;
	uint32_t n = (DW1000_readReg(RX_FINFO_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, RX_FINFO_LEN) & RX_FINFO_RXPACC_MASK) >> RX_FINFO_RXPACC_SHIFT;
	double A = 115.72;

	double result = (10*log10(((f1^2)+(f2^2)+(f3^2))/(n^2))) - 115.72;
	return result;
}

double DW1000_readRxLevel(int PRF) {
	uint16_t c = (DW1000_readReg(RX_FQUAL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, RX_FQUAL_LEN) & RX_EQUAL_CIR_MXG_MASK) >> RX_EQUAL_CIR_MXG_SHIFT;
	uint32_t n = (DW1000_readReg(RX_FINFO_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, RX_FINFO_LEN) & RX_FINFO_RXPACC_MASK) >> RX_FINFO_RXPACC_SHIFT;
	double A = 115.72;

	double result = (10*log10((c*(2^17))/(n^2))) - 115.72;
	return result;
}

uint64_t DW1000_readICTempAndVoltage() {
	 DW1000_writeReg(RF_CONF_ID, DW1000_SUB, 0x11, 0x80, 1);
	 DW1000_writeReg(RF_CONF_ID, DW1000_SUB, 0x12, 0x0A, 1);
	 DW1000_writeReg(RF_CONF_ID, DW1000_SUB, 0x12, 0x0F, 1);
	 DW1000_writeReg(TX_CAL_ID, DW1000_SUB, TC_SARL_SAR_C, 0x00, 1);
	 DW1000_writeReg(TX_CAL_ID, DW1000_SUB, TC_SARL_SAR_C, 0x01, 1);	// Set SAR enable
	
	DW1000__ICtemp = DW1000_readReg(TX_CAL_ID, DW1000_SUB, TC_SARL_SAR_LVBAT_OFFSET, 1);
	DW1000__ICvoltage = DW1000_readReg(TX_CAL_ID, DW1000_SUB, TC_SARL_SAR_LTEMP_OFFSET, 1);
	 DW1000_writeReg(TX_CAL_ID, DW1000_SUB, TC_SARL_SAR_C, 0x00, 1);	// Clears SAR enable

	return ((DW1000__ICtemp << 8) | DW1000__ICvoltage);
}

void DW1000_readOTP(int addr) {
	 DW1000_writeReg(OTP_IF_ID, DW1000_SUB, OTP_ADDR, addr, OTP_ADDR_LEN);
	 DW1000_writeReg(OTP_IF_ID, DW1000_SUB, OTP_CTRL, 0x03, OTP_CTRL_LEN);
	 DW1000_writeReg(OTP_IF_ID, DW1000_SUB, OTP_CTRL, 0x00, OTP_CTRL_LEN);

	long result = DW1000_readReg(OTP_IF_ID, DW1000_SUB, OTP_RDAT, OTP_RDAT_LEN);
	//Serial.println(result, HEX);
}

void DW1000_clearSystemStatus(long bit) {
	 DW1000_writeReg(SYS_STATUS_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, (0xFFFFFFFF & bit), SYS_STATUS_LEN);
}

void DW1000_setSystemConfig(uint64_t buffer) {
	 DW1000_writeReg(SYS_CFG_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, buffer, SYS_CFG_LEN);
}


//if running this right after doing a reset of the DW1000 put in a dealy
void DW1000_toggleGPIO_MODE() {
	uint32_t led = 0;
	//read the gio_mode register so we collect any of the reserved bits, not necessary for this one its all 0's  
	led = DW1000_readReg(GPIO_CTRL_ID, DW1000_SUB, GPIO_MODE_OFFSET, GPIO_MODE_LEN);
	//write to set up all the gpios as leds plus an extra 4 in the first 5, all the cool kids are doing it
	DW1000_writeReg(GPIO_CTRL_ID, DW1000_SUB, GPIO_MODE_OFFSET,0x1540, GPIO_MODE_LEN);
	
	//read the ctrl0 register to get all those reserved bits
	led = DW1000_readReg(PMSC_ID, DW1000_SUB, PMSC_CTRL0_OFFSET, PMSC_CTRL0_LEN);
	led |= (1<<18) | (1<<23); //activate those 2 weird clocks bro
	//and slam them in there
	DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_CTRL0_OFFSET, led, PMSC_CTRL0_LEN);
	
	//apparently reading this is garbage and just gives whats in ctrl0 the first time so fuck that noise
	//led = DW1000_readReg(PMSC_ID, DW1000_SUB, PMSC_LEDC_OFFSET, PMSC_LEDC_LEN);
	
	//enable blinking and set the default blink time
	led = PMSC_LEDC_BLNKEN | (1<<5);
	
	//this makes all the lights blink now
	led|= 0xf0000;
	//slam it in
	DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_LEDC_OFFSET, led, PMSC_LEDC_LEN);
	
	led &= ~0xf0000; //write the same thing without the blink now
	//and it should be ready to do stuff normally
	DW1000_writeReg(PMSC_ID, DW1000_SUB, PMSC_LEDC_OFFSET, led, PMSC_LEDC_LEN);
	
	
}
 
void DW1000_setTxFrameControl(long buffer) {
	 DW1000_writeReg(TX_FCTRL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, buffer, TX_FCTRL_LEN);
}

void DW1000_writeTxBuffer(uint16_t offset, uint64_t buffer, int n) {
	 DW1000_writeReg(TX_BUFFER_ID, DW1000_SUB, offset, buffer, n);
}

uint64_t DW1000_readRxBuffer(uint16_t offset, int n) {
	uint64_t rxBuffer = DW1000_readReg(RX_BUFFER_ID, DW1000_NO_SUB, offset, n);
	return rxBuffer;
}

void DW1000_startTx() {
	 DW1000_writeReg(SYS_CTRL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, SYS_CTRL_TXSTRT, SYS_CTRL_LEN);
}

void DW1000_startRx() {
	 DW1000_writeReg(SYS_CTRL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, SYS_CTRL_RXENAB, SYS_CTRL_LEN);
}

void DW1000_startTxRx() {
	 DW1000_writeReg(SYS_CTRL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, (SYS_CTRL_TXSTRT | SYS_CTRL_RXENAB), SYS_CTRL_LEN);
}

void DW1000_stopTxRx() {
	 DW1000_writeReg(SYS_CTRL_ID, DW1000_NO_SUB, DW1000_NO_OFFSET, 0x40, SYS_CTRL_LEN);
}

/****************************************************************************/
/* HELPER FUNCTIONS */

uint64_t DW1000_readReg(uint8_t cmd, int subindex, uint16_t offset, int n) {
	int header [3];
	int headerLen = 1;
	uint8_t data[n];
	uint64_t result = 0;
	int i;


	struct spi_device2{
		uint32_t id;
	}spidevice1;

	spidevice1.id = 0;


	/* Filter results more than 4 octets */
	if (n > 8) {
		return 0;									// TODO: return error
	}

	/* Generate header */
    if (!subindex) {
    	header[0] = DW1000_READ | cmd;						// 0x80 OR with command if no sub-index
    }
    else {
    	header[0] = DW1000_READ_SUB | cmd;					// 0x40 OR with command if sub-index is present
    	if (offset < 128) {							// Check if extended sub-index is needed
    		header[1] = offset;						
    		headerLen = 2;
    	}
    	else {
    		int sub1 = 0x7F & offset;				// 0x7F OR with offset if extended sub-index is used 
    		int sub2 = (0x7F80 & offset) >> 7;		// Remaining offset shifted right 7 bits
    		header[1] = 0x80 | sub1;
    		header[2] = sub2;
    		headerLen = 3;
    	}
    }
	
	spi_select_device(SPI0,&spidevice1);
	spi_write_packet(SPI0,header,headerLen);


	spi_read_packet(SPI0,data,n);
	spi_deselect_device(SPI0,&spidevice1);
	
	
	
	///* SPI transaction */
	//digitalWrite(_ss, LOW);
	//for (i = 0; i < headerLen; i++) {
		//SPI.transfer(header[i]);
	//}
	//for (i = 0; i < n; i++) {
		//data[i] = SPI.transfer(0x00);
	//}
	//digitalWrite(_ss, HIGH);
//
	///* Invert uint8_ts because SPI transactions reads LSB first, data[] is arranged from MSB to LSB */
	
					//Qbuf[0] = 0x26 | 1<<7 | 1<<6;
				//Qbuf[1] = 0x08;
				//Qbuf[2] = 0xF0;
				//Qbuf[3] = 0x00;
				//Qbuf[4] = 0x00;
				//Qbuf[5] = 0x00;
				//qspi_write(QSPI,Qbuf,1);
				//qspi_write(QSPI,Qbuf+1,1);
				//qspi_write(QSPI,Qbuf+2,1);
				//qspi_write(QSPI,Qbuf+3,1);
				//qspi_write(QSPI,Qbuf+4,1);
				//qspi_write(QSPI,Qbuf+5,1);
				//delay_ms(100);
			//
				//
				//
				//memset(Qbuf,0,20);
				//Qbuf[0] = 0x26 | 1<<6;
				//Qbuf[1] = 0x08;
				//qspi_write(QSPI,Qbuf,1);
				//qspi_write(QSPI,Qbuf+1,1);
				//qspi_read(QSPI,Qbuf,4);
				//sprintf(buf,"recived Test: 0x%02x%02x%02x%02x\n",Qbuf[3],Qbuf[2],Qbuf[1],Qbuf[0]);
				//sendDebugString(buf);
	
	for (i = n-1; i >= 0; i--) {
		result = (result << 8) | data[i];
	}

	return result;
}

void DW1000_writeReg(uint8_t cmd, int subindex, uint16_t offset, uint64_t buffer, int n) {
	uint8_t header[3];									// SPI transaction header
	int headerLen = 1;								// SPI transaction header length
	int i;											// Counter

	struct spi_device2{
		uint32_t id;
	}spidevice1;
	
	spidevice1.id = 0;

	/* Split data buffer */
	uint8_t data[n];									// Array
    uint64_t mask = 0xFF;							// Mask for bitwise operation (eg: first iter.- 0000 0000 0000 00FF)
    for (i = 0; i < n; i++) {
        data[i] = (buffer & mask) >> (i * 8);
        mask = mask << 8;
    }

    /* Generate header */
    if (!subindex) {
    	header[0] = DW1000_WRITE | cmd;					// 0x80 OR with command if no sub-index
    }
    else {
    	header[0] = DW1000_WRITE_SUB | cmd;				// 0xC0 OR with command if sub-index is present
    	if (offset < 128) {							// Check if extended sub-index is needed
    		header[1] = offset;						
    		headerLen = 2;
    	}
    	else {
    		int sub1 = 0x7F & offset;				// 0x7F OR with offset if extended sub-index is used 
    		int sub2 = (0x7F80 & offset) >> 7;		// Remaining offset shifted right 7 bits
    		header[1] = 0x80 | sub1;
    		header[2] = sub2;
    		headerLen = 3;
    	}
    }
	//while(!(SPI0->SPI_SR & SPI_SR_TXEMPTY));
	spi_select_device(SPI0,&spidevice1);
	
	spi_write_packet(SPI0,header,headerLen);
	
	//for(int i = 0;i<headerLen;i++){
	//	 spi_write(SPI0,header+i,NULL,NULL);
	//}
	
	spi_write_packet(SPI0,data,n);
	
	spi_deselect_device(SPI0,&spidevice1);
	/*
	for(int i = 0;i<n;i++)
	{
		 spi_write_packet(SPI0,data+i,n);
	}*/
					//T++;
				//memset(Qbuf,0,20);
				
				//delay_ms(100);
				//
				//
				//memset(Qbuf,0,20);
				//Qbuf[0] = 0x00;
				//delay_ms(25);
				//qspi_write(QSPI,Qbuf,1);
				//qspi_read(QSPI,Qbuf,4);
				//sprintf(buf,"recived Base: 0x%02x%02x%02x%02x\n",Qbuf[3],Qbuf[2],Qbuf[1],Qbuf[0]);
				//sendDebugString(buf);
				//sendDebugString("\n");

    ///* SPI transaction */
    //digitalWrite(_ss, LOW);
    //for (i = 0; i < headerLen; i++) {
    	//SPI.transfer(header[i]);
    //}
    //for (i = 0; i < n; i++) {
    	//SPI.transfer(data[i]);
    //}
    //digitalWrite(_ss, HIGH);
}

void cmdDWMsend(char* tosend, int charlen) {
	for(int i = 0; i< charlen;i++){
	DW1000_writeTxBuffer(i,tosend[i],1);
	}
	DW1000_setTxFrameControl( 0x00090000 | 0x7F&charlen  );
	DW1000_startTx();
}
