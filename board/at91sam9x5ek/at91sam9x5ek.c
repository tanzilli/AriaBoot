/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation

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
 * File Name           : at91sam9x5ek.c
 * Object              :
 * Creation            : HXu Aug. 2010
 *-----------------------------------------------------------------------------
 * Version for AriaG25. Sergio Tanzilli
 */
#if defined(WINCE) && !defined(CONFIG_AT91SAM9X5EK)

#else

#include "part.h"
#include "gpio.h"
#include "pmc.h"
#include "rstc.h"
#include "dbgu.h"
#include "debug.h"
#include "main.h"
#include "ddramc.h"
#include "onewire_info.h"

int get_cp15(void);

void set_cp15(unsigned int value);

int get_cpsr(void);

void set_cpsr(unsigned int value);

#ifdef CONFIG_SCLK
void sclk_enable(void)
{
    writel(readl(AT91C_SYS_SLCKSEL) | AT91C_SLCKSEL_OSC32EN, AT91C_SYS_SLCKSEL);
    /* must wait for slow clock startup time ~ 1000ms
     * (~6 core cycles per iteration, core is at 400MHz: 66666000 min loops) */
    Wait(66700000);

    writel(readl(AT91C_SYS_SLCKSEL) | AT91C_SLCKSEL_OSCSEL, AT91C_SYS_SLCKSEL);
    /* must wait 5 slow clock cycles = ~153 us
     * (~6 core cycles per iteration, core is at 400MHz: min 10200 loops) */
    Wait(10200);

    /* now disable the internal RC oscillator */
    writel(readl(AT91C_SYS_SLCKSEL) & ~AT91C_SLCKSEL_RCEN, AT91C_SYS_SLCKSEL);
}
#endif                          /* CONFIG_SCLK */

#ifdef CONFIG_HW_INIT
/*----------------------------------------------------------------------------*/
/* \fn    hw_init							      */
/* \brief This function performs very low level HW initialization	      */
/* This function is invoked as soon as possible during the c_startup	      */
/* The bss segment must be initialized					      */
/*----------------------------------------------------------------------------*/
void hw_init(void)
{
    unsigned int cp15;

    /*
     * Configure PIOs 
     */
    const struct pio_desc hw_pio[] = {
#ifdef CONFIG_DEBUG
        {"RXD", AT91C_PIN_PA(9), 0, PIO_DEFAULT, PIO_PERIPH_A},
        {"TXD", AT91C_PIN_PA(10), 0, PIO_DEFAULT, PIO_PERIPH_A},
#endif
        {(char *)0, 0, 0, PIO_DEFAULT, PIO_PERIPH_A},
    };

    /*
     * Disable watchdog 
     */
    writel(AT91C_WDTC_WDDIS, AT91C_BASE_WDTC + WDTC_WDMR);

    /*
     * At this stage the main oscillator is supposed to be enabled
     * * PCK = MCK = MOSC 
     */
    writel(0x00, AT91C_BASE_PMC + PMC_PLLICPR);

    /*
     * Configure PLLA = MOSC * (PLL_MULA + 1) / PLL_DIVA 
     */
    pmc_cfg_plla(PLLA_SETTINGS, PLL_LOCK_TIMEOUT);

    /*
     * PCK = PLLA/2 = 3 * MCK 
     */
    pmc_cfg_mck(BOARD_PRESCALER_MAIN_CLOCK, PLL_LOCK_TIMEOUT);

    /*
     * Switch MCK on PLLA output 
     */
    pmc_cfg_mck(BOARD_PRESCALER_PLLA, PLL_LOCK_TIMEOUT);

    /*
     * Enable External Reset 
     */
    writel(AT91C_RSTC_KEY_UNLOCK
           || AT91C_RSTC_URSTEN, AT91C_BASE_RSTC + RSTC_RMR);

    /*
     * Configure CP15 
     */
    cp15 = get_cp15();
    cp15 |= I_CACHE;
    set_cp15(cp15);

#ifdef CONFIG_SCLK
    sclk_enable();
#endif
    /*
     * Configure the PIO controller 
     */
    writel((1 << AT91C_ID_PIOA_B), (PMC_PCER + AT91C_BASE_PMC));
    pio_setup(hw_pio);

    /*
     * Enable Debug messages on the DBGU 
     */
#ifdef CONFIG_DEBUG
    dbgu_init(BAUDRATE(MASTER_CLOCK, 115200));
    dbgu_print("Start AriaBoot...\n\r");
#endif

#ifdef CONFIG_DDR2
    /*
     * Configure DDRAM Controller 
     */
    dbg_log(1, "Init DDR... ");
    ddramc_hw_init();
    dbg_log(1, "Done!\n\r");
#endif                          /* CONFIG_DDR2 */
}
#endif                          /* CONFIG_HW_INIT */

#ifdef CONFIG_DDR2
static SDdramConfig ddram_config;

/*------------------------------------------------------------------------------*/
/* \fn    ddramc_hw_init							*/
/* \brief This function performs DDRAMC HW initialization			*/
/*										*/
/* Using the Micron MT47H64M16HR-3										*/
/*------------------------------------------------------------------------------*/
void ddramc_hw_init(void)
{
    ddram_config.ddramc_mdr =
        (AT91C_DDRC2_DBW_16_BITS | AT91C_DDRC2_MD_DDR2_SDRAM);

    ddram_config.ddramc_cr = (AT91C_DDRC2_NC_DDR10_SDR9 |      // 10 column bits (1K)
                              AT91C_DDRC2_NR_13 |              // 13 row bits    (8K)
                              AT91C_DDRC2_CAS_3 |              // CAS Latency 3
                              AT91C_DDRC2_NB_BANKS_8 |         // 8 banks
                              AT91C_DDRC2_DLL_RESET_DISABLED | // DLL not reset
                              AT91C_DDRC2_DECOD_INTERLEAVED);  // Interleaved decoding

    /* The DDR2-SDRAM device requires a refresh every 15.625 us or 7.81 us.
     * With a 133 MHz frequency, the refresh timer count register must to be
     * set with (15.625 x 133 MHz) ~ 2084 i.e. 0x824
     * or (7.81 x 133 MHz) ~ 1040 i.e. 0x410.
     */
    ddram_config.ddramc_rtr = 0x411;    /* Refresh timer: 7.8125us */

    /* One clock cycle @ 133 MHz = 7.5 ns */
    ddram_config.ddramc_t0pr = (AT91C_DDRC2_TRAS_6 |    //  6 * 7.5 = 45   ns
                                AT91C_DDRC2_TRCD_2 |    //  2 * 7.5 = 15   ns
                                AT91C_DDRC2_TWR_2 |     //  2 * 7.5 = 15   ns
                                AT91C_DDRC2_TRC_8 |     //  8 * 7.5 = 60   ns
                                AT91C_DDRC2_TRP_2 |     //  2 * 7.5 = 15   ns
                                AT91C_DDRC2_TRRD_2 |    //  2 * 7.5 = 15   ns (x16 memory)
                                AT91C_DDRC2_TWTR_2 |    //  2 clock cycles min
                                AT91C_DDRC2_TMRD_2);    //  2 clock cycles

    ddram_config.ddramc_t1pr = (AT91C_DDRC2_TXP_2 |     //  2 clock cycles
                                200 << 16 |             //  200 clock cycles
                                19 << 8 |               //  19 * 7.5 = 142.5 ns ( > 128 + 10 ns)
                                AT91C_DDRC2_TRFC_18);   //  18 * 7.5 = 135   ns (must be 128 ns for 1Gb DDR)

    ddram_config.ddramc_t2pr = (AT91C_DDRC2_TFAW_7 |	//  7 * 7.5 = 52.5 ns
    				AT91C_DDRC2_TRTP_2 |    //  2 clock cycles min
                                AT91C_DDRC2_TRPA_3 |    //  3 * 7.5 = 22.5 ns
                                AT91C_DDRC2_TXARDS_7 |  //  7 clock cycles
                                AT91C_DDRC2_TXARD_2);   //  2 clock cycles

    // ENABLE DDR2 clock 
    writel(AT91C_PMC_DDR, AT91C_BASE_PMC + PMC_SCER);

    /*
     * Chip select 1 is for DDR2/SDRAM
     */
    writel((readl(AT91C_BASE_CCFG + CCFG_EBICSA) | AT91C_EBI_CS1A_SDRAMC),
            AT91C_BASE_CCFG + CCFG_EBICSA);


    /*
     * DDRAM2 Controller
     */
    ddram_init(AT91C_BASE_DDR2C, AT91C_EBI_CS1, &ddram_config);
}
#endif                          /* CONFIG_DDR2 */

void one_wire_hw_init(void)
{
	const struct pio_desc wire_pio[] = {
		{"1-Wire", AT91C_PIN_PB(18), 0, PIO_DEFAULT, PIO_OUTPUT},
		{(char *)0, 0, 0, PIO_DEFAULT, PIO_PERIPH_A},
	};

	writel((1 << AT91C_ID_PIOA_B), (PMC_PCER + AT91C_BASE_PMC));
	pio_setup(wire_pio);
}

#endif                          /* CONFIG_AT91SAM9X5EK */
