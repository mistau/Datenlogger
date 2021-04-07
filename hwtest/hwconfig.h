/*
 * -------------------------------------------------------------------------
 * Controller fuer den Tauben Datenlogger
 * Version 0.1 fuer HW Rev 1.0
 * hardware definitionen
 * -------------------------------------------------------------------------
 *  M.Staudenmaier
 */

#ifndef _HWCONFIG_H_
#define _HWCONFIG_H_

/* the LEDs connections */
#define LED_STATE        PD7       /**< on-pcb status LED */

/* Input Keys */
#define BUTTON           PD6

/* Interrupt Inputs */
#define RTCINT1          PD4
#define RTCINT2          PD5

/* outputs */
#define PSWITCH0         PD2
#define PSWITCH1         PD3

/* connections to flash device */
#define FLASH_DDR    DDRB
#define FLASH_NRESET PB0
#define FLASH_NWP    PB1
#define FLASH_NCS    PB2

#endif
