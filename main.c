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


/*------------------------------------------------------------------------------*/
/* Function Name       : main							*/
/* Object              : Main function						*/
/* Input Parameters    : none							*/
/* Output Parameters   : True							*/
/*------------------------------------------------------------------------------*/
int main(void)
{
	// Board hardware initialization
    hw_init();


	// Disable TX & RX and more
	//AT91C_BASE_EMACB->EMAC_NCR = 0;

	// disable 
	//AT91C_BASE_EMACB->EMAC_IDR = ~0;

	writel(0,AT91C_EMACB0_NCR);
	writel(~0,AT91C_EMACB0_IDR);
	

	// Set the MAC address
	writel(0x25111111, AT91C_EMACB0_SA1L);
	writel(0x00000004, AT91C_EMACB0_SA1H);
	writel(0x25222222, AT91C_EMACB0_SA2L);
	writel(0x00000004, AT91C_EMACB0_SA2H);
	writel(0x25333333, AT91C_EMACB0_SA3L);
	writel(0x00000004, AT91C_EMACB0_SA3H);
	writel(0x25444444, AT91C_EMACB0_SA4L);
	writel(0x00000004, AT91C_EMACB0_SA4H);

    dbg_log(3,"AT91C_EMACB0_SA1L = %x \n\r",readl(AT91C_EMACB0_SA1L));
    dbg_log(3,"AT91C_EMACB0_SA1H = %x \n\r",readl(AT91C_EMACB0_SA1H));
	

	// Turn-on the green led on Aria G25 board
	pio_set_gpio_output(AT91C_PIN_PB(8),1);

    dbgu_print("Reading Linux image from microSD card ...\n\r");
    LoadLinux();

	while(1);
	return 0;
}
