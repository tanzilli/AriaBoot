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
 * ----------------------------------------------------------------------------
 * File Name           : at91sam9263ek.h
 * Object              :
 * Creation            : NLe Aug 8th 2006
 *-----------------------------------------------------------------------------
 */
#ifndef _AT91SAM9263EK_H
#define _AT91SAM9263EK_H

/* ******************************************************************* */
/* PMC Settings                                                        */
/*                                                                     */
/* The main oscillator is enabled as soon as possible in the c_startup */
/* and MCK is switched on the main oscillator.                         */
/* PLL initialization is done later in the hw_init() function          */
/* ******************************************************************* */
#if	defined(CONFIG_CPU_CLK_200MHZ)

#if	defined(CONFIG_CRYSTAL_16_36766MHZ)
#define MASTER_CLOCK		(199919000/2)
#define PLL_LOCK_TIMEOUT	1000000
#define PLLA_SETTINGS		0x20AABF0E
#define PLLB_SETTINGS		0x10483F0E
#endif

#if	defined(CONFIG_CRYSTAL_18_432MHZ)
#define MASTER_CLOCK		(198656000/2)
#define PLL_LOCK_TIMEOUT	1000000
#define PLLA_SETTINGS		0x2060BF09
#define PLLB_SETTINGS		0x10483F0E
#endif

#endif

#if	defined(CONFIG_CPU_CLK_240MHZ)

#if	defined(CONFIG_CRYSTAL_16_36766MHZ)
#error	"240 MHz not supported for a 16.36766 MHz crystal (Only 200 MHz)"
#endif

#if	defined(CONFIG_CRYSTAL_18_432MHZ)
#define MASTER_CLOCK		(240000000/2)
#define PLL_LOCK_TIMEOUT	1000000
#define PLLA_SETTINGS		0x2271BF30      // 00100 01001110001 10 111111 00110000b
#define PLLB_SETTINGS		0x10483F0E
#endif

#endif

#define	TOP_OF_MEMORY		0x314000

/* Switch MCK on PLLA output PCK = PLLA = 2 * MCK */
#define MCKR_SETTINGS		(AT91C_PMC_PRES_CLK | AT91C_PMC_MDIV_2)
#define MCKR_CSS_SETTINGS	(AT91C_PMC_CSS_PLLA_CLK | MCKR_SETTINGS)

/* ******************************************************************* */
/* DataFlash Settings                                                  */
/*                                                                     */
/* ******************************************************************* */
#define AT91C_BASE_SPI	AT91C_BASE_SPI0
#define AT91C_ID_SPI	AT91C_ID_SPI0

/* AC characteristics */
/* DLYBS = tCSS= 250ns min and DLYBCT = tCSH = 250ns */
#define DATAFLASH_TCSS		(0x1a << 16)    /* 250ns min (tCSS) <=> 12/48000000 = 250ns */
#define DATAFLASH_TCHS		(0x1 << 24)     /* 250ns min (tCSH) <=> (64*1+SCBR)/(2*48000000) */

#define DF_CS_SETTINGS 		(AT91C_SPI_NCPHA | (AT91C_SPI_DLYBS & DATAFLASH_TCSS) | (AT91C_SPI_DLYBCT & DATAFLASH_TCHS) | ((MASTER_CLOCK / AT91C_SPI_CLK) << 8))

/* ******************************************************************* */
/* SDRAMC Settings                                                     */
/*                                                                     */
/* ******************************************************************* */
#define AT91C_BASE_SDRAMC 	AT91C_BASE_SDRAMC0
#define AT91C_EBI_SDRAM		AT91C_EBI0_SDRAM

/* ******************************************************************* */
/* NandFlash Settings                                                  */
/*                                                                     */
/* ******************************************************************* */
#define AT91C_SMARTMEDIA_BASE	0x40000000

#define AT91_SMART_MEDIA_ALE    (1 << 21)       /* our ALE is AD21 */
#define AT91_SMART_MEDIA_CLE    (1 << 22)       /* our CLE is AD22 */

#define NAND_DISABLE_CE() do { *(volatile unsigned int *)AT91C_PIOD_SODR = AT91C_PIO_PD15;} while(0)
#define NAND_ENABLE_CE()  do { *(volatile unsigned int *)AT91C_PIOD_CODR = AT91C_PIO_PD15;} while(0)
#define NAND_WAIT_READY() while (!(*(volatile unsigned int *)AT91C_PIOA_PDSR & AT91C_PIO_PA22))

/* ******************************************************************* */
/* SDRAMC Settings                                                     */
/*                                                                     */
/* ******************************************************************* */
#define AT91C_BASE_SDRAMC 	AT91C_BASE_SDRAMC0
#define AT91C_EBI_SDRAM		AT91C_EBI0_SDRAM

/* ******************************************************************** */
/* SMC Chip Select 3 Timings for NandFlash for MASTER_CLOCK = 100000000.*/
/* Please refer to SMC section in AT91SAM9x datasheet to learn how 	*/
/* to generate these values. 						*/
/* ******************************************************************** */
#define AT91C_SM_NWE_SETUP	(1 << 0)
#define AT91C_SM_NCS_WR_SETUP	(1 << 8)
#define AT91C_SM_NRD_SETUP	(1 << 16)
#define AT91C_SM_NCS_RD_SETUP	(1 << 24)

#define AT91C_SM_NWE_PULSE 	(3 << 0)
#define AT91C_SM_NCS_WR_PULSE	(3 << 8)
#define AT91C_SM_NRD_PULSE	(3 << 16)
#define AT91C_SM_NCS_RD_PULSE	(3 << 24)

#define AT91C_SM_NWE_CYCLE 	(5 << 0)
#define AT91C_SM_NRD_CYCLE	(5 << 16)

#define AT91C_SM_TDF	        (2 << 16)

#define OP_BOOTSTRAP_MCI_on
#define BOARD_SD_PIN_CD \
    {1 << 16, AT91C_BASE_PIOE, AT91C_ID_PIOCDE, PIO_INPUT, PIO_PULLUP}

#define BOARD_SD_PINS \
    {0x0000003B, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}, \
    {1 << 12, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

#define BOARD_CLK_SETTING \
	{1 << 20, AT91C_BASE_PIOE, AT91C_ID_PIOCDE, PIO_OUTPUT_0, PIO_PULLUP}

#define BOARD_SD_MCI1_PIN_CD \
    {1 << 18, AT91C_BASE_PIOE, AT91C_ID_PIOCDE, PIO_INPUT, PIO_PULLUP}

#define BOARD_SD_MCI1_PINS  \
    {0xf80, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}, \
    {1 << 6, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

#define at91sam9263
#endif                          /* _AT91SAM9263EK_H */
