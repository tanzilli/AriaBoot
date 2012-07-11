/* ----------------------------------------------------------------------------
 * AriaBoot - Bootloader utility for Aria G25 
 *
 * AriaBoot is just a customized version on Atmel AT91Bootstrap for
 * Aria G25 CPU module. 
 *
 * http://www.acmesystems.it/aria
 * Sergio Tanzilli - tanzilli@acmesystems.it
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ACME SYSTEMS "AS IS" IN THE SAME 
 * TERMS OF THE ORIGINAL ATMEL DISCLAIMER LISTED BELOW.
 * ----------------------------------------------------------------------------
 */

/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *-----------------------------------------------------------------------------
 */

#include "part.h"
#include "main.h"
#include "dbgu.h"
#include "debug.h"
#include "linux_tags.h"
#include "gpio.h"
#include "include/AT91SAM9GX5_inc.h"

void LoadLinux();
void LoadApp();
extern unsigned int load_SDCard(unsigned char *,char *);

/*------------------------------------------------------------------------------*/
/* Function Name       : Wait							*/
/* Object              : software loop waiting function				*/
/*------------------------------------------------------------------------------*/
void Wait(unsigned int count)
{
    unsigned int i;

    for (i = 0; i < count; i++)
        asm volatile ("    nop");
}

// Convert a hex char in a binary value
// ex: '0' ret 0x30
// ex: 'A' ret 0x41
unsigned char c2b(unsigned char value) {
	if (value>='0' && value<='9') return value-0x30; 
	if (value>='A' && value<='F') return value-0x41+0x0A; 
	return 0;
}

int main(void)
{
	// Dummy buffer to load the MAC address from file
	unsigned char mac_buffer[100];
	unsigned char mac_address[6];

	// Register used by the CPU to save the MAC address
	unsigned int SA1L_value;
	unsigned int SA1H_value;

	// Board hardware initialization
    hw_init();

	// Turn-on the green led on Aria G25 board
	pio_set_gpio_output(AT91C_PIN_PB(8),1);

	// Set here the default GPIO config you need at reset
	pio_set_gpio_input(AT91C_PIN_PA(7),1);
	pio_set_gpio_input(AT91C_PIN_PA(11),1);
	pio_set_gpio_input(AT91C_PIN_PA(12),1);
	pio_set_gpio_input(AT91C_PIN_PA(13),1);
	pio_set_gpio_input(AT91C_PIN_PA(14),1);

	// First messages on the debug port
    dbgu_print("\n\r");
    dbgu_print("AriaBoot Version 0.24\n\r");
    dbgu_print("Acme Systems srl (c) 2012 - http://www.acmesystems.it\n\r");

	// Load the ethernet MAC address from macaddress.txt
    if (load_SDCard(mac_buffer,"macaddr.txt")==1) {
		mac_buffer[17]=0;
		dbg_log(0,"Load MAC address from macaddr.txt\n\r");

		mac_address[0]=(c2b(mac_buffer[0])<<4)+c2b(mac_buffer[1]);
		mac_address[1]=(c2b(mac_buffer[3])<<4)+c2b(mac_buffer[4]);
		mac_address[2]=(c2b(mac_buffer[6])<<4)+c2b(mac_buffer[7]);
		mac_address[3]=(c2b(mac_buffer[9])<<4)+c2b(mac_buffer[10]);
		mac_address[4]=(c2b(mac_buffer[12])<<4)+c2b(mac_buffer[13]);
		mac_address[5]=(c2b(mac_buffer[15])<<4)+c2b(mac_buffer[16]);
	} else {
	    dbg_log(0,"Set a dummy MAC address\n\r");
		mac_address[0]=0x00;
		mac_address[1]=0x04;
		mac_address[2]=0x25;
		mac_address[3]=0x12;
		mac_address[4]=0x34;
		mac_address[5]=0x56;
	}
	
    // Set the Ethernet MAC address
	dbg_log(0,"Ethernet MAC address will be: %x:%x:%x:%x:%x:%x\n\r",
		mac_address[0],  mac_address[1],  mac_address[2],
		mac_address[3],  mac_address[4],  mac_address[5]);

	// Enable the MAC0 interface clock	
	writel(1<<AT91C_ID_EMAC0, AT91C_PMC_PCER);

	SA1L_value=	(unsigned int)mac_address[3] << 24 | 
				(unsigned int)mac_address[2] << 16 | 
				(unsigned int)mac_address[1] << 8  | 
				(unsigned int)mac_address[0];

	SA1H_value=	(unsigned int)mac_address[5] << 8  | 
				(unsigned int)mac_address[4];

	// Save the MAC address on the Atmel registers
	writel(SA1L_value,AT91C_EMACB0_SA1L);
	writel(SA1H_value,AT91C_EMACB0_SA1H);

	// Search for Linux uImage...
    dbgu_print("Search for the Linux image (uImage) on SD card ...\n\r");
    LoadLinux("uImage");

	// ..or for an user application
    dbgu_print("Search for an user application (userapp.bin) on SD card ...\n\r");
    LoadApp("userapp.bin");

    dbgu_print("Nothing to do. Forever loop...\n\r");
	while(1);
	return 0;
}
