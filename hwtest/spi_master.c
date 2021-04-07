/**
 * -------------------------------------------------------------------------
 * @file spi_master.c
 * Access routines SPI in master mode
 * tested with Atmega 328P
 *
 * Version 0.1
 *
 * Copyright 2021 - Michael Staudenmaier
 * -------------------------------------------------------------------------
 */

#include <avr/io.h>

#include "spi_master.h"

/* this is for AtMega328P */
#define DDR_SPI  DDRB
#define DD_MOSI  PB3
#define DD_MISO  PB4
#define DD_SCK   PB5


void spi_masterInit(void)
{
    /* Set MOSI and SCK output */
    DDR_SPI = _BV(DD_MOSI) | _BV(DD_SCK);
    /* enable SPI, master, set clock rate fck/16 */
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
}

uint8_t spi_masterTransmit(uint8_t data)
{
    /* start transmission */
    SPDR = data;

    /* wait for transmission completed */
    loop_until_bit_is_set(SPSR, SPIF);
    
    return SPDR;
}
