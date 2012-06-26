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

// The Magic number can be used to find and to change on-the-fly the board 
// MAC address 

static struct {
	unsigned int magic_number;
	unsigned char mac_address[6];
} magic_and_mac = {
	0x5C5C5C5C,
	{0x00, 0x04, 0x25, 0x12, 0x34, 0x56}
};



void user_hw_init(void);
extern void Jump(unsigned int addr);
extern unsigned int load_SDCard(void *dst);
void LoadLinux();

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

int main(void)
{
	unsigned int SA1L_value;
	unsigned int SA1H_value;

	// Board hardware initialization
    hw_init();
    dbgu_print("\n\r");
    dbgu_print("AriaBoot Version 0.08\n\r");

    /*
     * Se the Ethernet MAC address
     */

	dbg_log(3,"Ethernet MAC address: %02X%02X%02X%02X%02X%02X\n\r",
		magic_and_mac.mac_address[0],  magic_and_mac.mac_address[1],  magic_and_mac.mac_address[2],
		magic_and_mac.mac_address[3],  magic_and_mac.mac_address[4],  magic_and_mac.mac_address[5]);

	// Enable the MAC0 interface clock	
	writel(1<<AT91C_ID_EMAC0, AT91C_PMC_PCER);

	SA1L_value=	(unsigned int)magic_and_mac.mac_address[3] << 24 | 
				(unsigned int)magic_and_mac.mac_address[2] << 16 | 
				(unsigned int)magic_and_mac.mac_address[1] << 8  | 
				(unsigned int)magic_and_mac.mac_address[0];

	SA1H_value=	(unsigned int)magic_and_mac.mac_address[5] << 8  | 
				(unsigned int)magic_and_mac.mac_address[4];

	// Save the MAC address
	writel(SA1L_value,AT91C_EMACB0_SA1L);
	writel(SA1H_value,AT91C_EMACB0_SA1H);

    //dbg_log(3,"AT91C_EMACB0_SA1L = %x \n\r",readl(AT91C_EMACB0_SA1L));
    //dbg_log(3,"AT91C_EMACB0_SA1H = %x \n\r",readl(AT91C_EMACB0_SA1H));

	// Turn-on the green led on Aria G25 board
	pio_set_gpio_output(AT91C_PIN_PB(8),1);

	pio_set_gpio_output(AT91C_PIN_PA(7),0);
	pio_set_gpio_output(AT91C_PIN_PA(11),1);
	pio_set_gpio_output(AT91C_PIN_PA(12),0);
	pio_set_gpio_output(AT91C_PIN_PA(13),1);
	pio_set_gpio_output(AT91C_PIN_PA(14),0);

    dbgu_print("Read and run the Linux (uImage) from microSD card ...\n\r");
    LoadLinux("uImage");
    dbgu_print("Linux not found !\n\r\n\r");
    dbgu_print("Read and run an user application (userapp.bin) from microSD card ...\n\r");
    LoadApp("userapp.bin");
    dbgu_print("User application not found !\n\r\n\r");
    dbgu_print("Loop forever...\n\r\n\r");
	while(1);
	return 0;
}
