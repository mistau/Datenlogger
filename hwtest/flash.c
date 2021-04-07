/**
 * -------------------------------------------------------------------------
 * @file flash.c
 * Access routines for Adesto flash
 * currently supporting AT45DB321E
 *
 * Version 0.1
 *
 * Copyright 2021 - Michael Staudenmaier
 * -------------------------------------------------------------------------
 */

#include <avr/io.h>
#include <util/delay.h>

#include "hwconfig.h"
#include "spi_master.h"
#include "at45d321_cmds.h"

#include "flash.h"

//#define FLASH_CS_ACTIVE    (FLASH_DDR |= _BV(FLASH_NCS))
//#define FLASH_CS_INACTIVE  (FLASH_DDR &= ~_BV(FLASH_NCS))

#define FLASH_CS_ACTIVE    (PORTB &= ~_BV(FLASH_NCS))
#define FLASH_CS_INACTIVE  (PORTB |= _BV(FLASH_NCS))
#define FLASH_WP_INACTIVE  (PORTB |= _BV(FLASH_NWP))
#define FLASH_WP_ACTIVE    (PORTB &= ~_BV(FLASH_NWP))


#define FLASH_PAGE_SIZE 512 /**< value can be either 512 or 528 */


void flash_init(void)
{
    /* pull reset line */
    FLASH_DDR |= _BV(FLASH_NRESET);
    _delay_us(10);                  /* 10us delay required */
    FLASH_DDR &= ~_BV(FLASH_NRESET);
    _delay_us(35);                  /* wait for 35us */
}


void flash_wait_ready(void)
{
    uint8_t val0, val1;
    FLASH_CS_ACTIVE;
    spi_masterTransmit(FLASHCMD_STATUS_REGISTER_READ);
    do{
        val0 = spi_masterTransmit(0xff);
        val1 = spi_masterTransmit(0xff);
    }while(!(val0 & 0x80));
    FLASH_CS_INACTIVE;
}

uint8_t flash_get_status(uint8_t *buf)
{
    FLASH_CS_ACTIVE;
    spi_masterTransmit(FLASHCMD_STATUS_REGISTER_READ);
    *buf = spi_masterTransmit(0xff);
    *(buf+1) = spi_masterTransmit(0xff);
    FLASH_CS_INACTIVE;
    return *buf;
}


void flash_get_id(uint8_t *buf)
{
    int i;
    FLASH_CS_ACTIVE;
    spi_masterTransmit(FLASHCMD_MANUFACTURER_AND_DEVICE_ID_READ);
    for(i=0; i<5; ++i, ++buf)
        *buf = spi_masterTransmit(0xff);
    FLASH_CS_INACTIVE;
}

void flash_conf_power2_size(void)
{
    FLASH_CS_ACTIVE;
    spi_masterTransmit(FLASHCMD_CONFIGURE_POWER_OF_TWO_PAGE_SIZE0);
    spi_masterTransmit(FLASHCMD_CONFIGURE_POWER_OF_TWO_PAGE_SIZE1);
    spi_masterTransmit(FLASHCMD_CONFIGURE_POWER_OF_TWO_PAGE_SIZE2);
    spi_masterTransmit(FLASHCMD_CONFIGURE_POWER_OF_TWO_PAGE_SIZE3);
    FLASH_CS_INACTIVE;
    flash_wait_ready();
}

void flash_conf_standard_size(void)
{
    FLASH_CS_ACTIVE;
    spi_masterTransmit(FLASHCMD_CONFIGURE_STANDARD_DATAFLASH_PAGE_SIZE0);
    spi_masterTransmit(FLASHCMD_CONFIGURE_STANDARD_DATAFLASH_PAGE_SIZE1);
    spi_masterTransmit(FLASHCMD_CONFIGURE_STANDARD_DATAFLASH_PAGE_SIZE2);
    spi_masterTransmit(FLASHCMD_CONFIGURE_STANDARD_DATAFLASH_PAGE_SIZE3);
    FLASH_CS_INACTIVE;
    flash_wait_ready();
}


void flash_read_page(uint8_t *buf, uint16_t pageno)
{
    FLASH_CS_ACTIVE;
    spi_masterTransmit(FLASHCMD_CONTINUOUS_ARRAY_READ_LOW_FREQUENCY);
    spi_masterTransmit((pageno>>7) & 0x3f);   // 6bit of pageno
    spi_masterTransmit((pageno<<1) & 0xfe);   // 7bit of pageno
    spi_masterTransmit(pageno & 0xff);        // dummy address
    for(int i=0; i<FLASH_PAGE_SIZE; ++i)
        *(buf++) = spi_masterTransmit(0xff);
    FLASH_CS_INACTIVE;
}

void flash_write_page(uint16_t pageno, const uint8_t *buf)
{
    FLASH_WP_INACTIVE;
    FLASH_CS_ACTIVE;
    spi_masterTransmit(FLASHCMD_MAIN_MEM_PAGE_PROGRAM_THROUGH_BUF1_ERASE);
    spi_masterTransmit((pageno>>7) & 0x3f);   // 6bit of pageno
    spi_masterTransmit((pageno<<1) & 0xfe);   // 7bit of pageno
    spi_masterTransmit(pageno & 0xff);        // dummy address
    for(int i=0; i<FLASH_PAGE_SIZE; ++i)
        spi_masterTransmit(*(buf++));
    FLASH_CS_INACTIVE;
    flash_wait_ready(); // wait until erase+write completed
    FLASH_WP_ACTIVE;
}

void flash_erase_chip(void)
{
    FLASH_WP_INACTIVE;
    FLASH_CS_ACTIVE;    
    spi_masterTransmit(FLASHCMD_CHIP_ERASE0);
    spi_masterTransmit(FLASHCMD_CHIP_ERASE1);
    spi_masterTransmit(FLASHCMD_CHIP_ERASE2);
    spi_masterTransmit(FLASHCMD_CHIP_ERASE3);    
    FLASH_CS_INACTIVE;
    flash_wait_ready(); // wait until erase+write completed
    FLASH_WP_ACTIVE;    
}


void flash_enter_deep_powerdown(void)
{
    FLASH_CS_ACTIVE;    
    spi_masterTransmit(FLASHCMD_DEEP_POWER_DOWN);
    FLASH_CS_INACTIVE;
}

void flash_resume_deep_powerdown(void)
{
    FLASH_CS_ACTIVE;    
    spi_masterTransmit(FLASHCMD_RESUME_FROM_DEEP_POWER_DOWN);
    FLASH_CS_INACTIVE;
    _delay_us(35);    // wait for tRDPD=35us
}


void flash_enter_ultradeep_powerdown(void)
{
    FLASH_CS_ACTIVE;    
    spi_masterTransmit(FLASHCMD_ULTRA_DEEP_POWER_DOWN);
    FLASH_CS_INACTIVE;
}

void flash_resume_ultradeep_powerdown(void)
{
    FLASH_CS_ACTIVE;    
    asm("NOP");         // wait for tCSLU = 20ns
    FLASH_CS_INACTIVE;
    _delay_us(180);      // wait for tXUDPD = 180us    
}
