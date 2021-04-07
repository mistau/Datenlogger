/**
 * -------------------------------------------------------------------------
 * @file spi_master.h
 * Access routines SPI in master mode
 * tested with Atmega 328P
 *
 * Version 0.1
 *
 * Copyright 2021 - Michael Staudenmaier
 * -------------------------------------------------------------------------
 */

#ifndef _SPI_MASTER_H_
#define _SPI_MASTER_H_

void spi_masterInit(void);

uint8_t spi_masterTransmit(uint8_t data);

#endif
