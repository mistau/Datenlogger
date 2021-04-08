/*
 * -------------------------------------------------------------------------
 * HW Test Routine
 * Datensammler
 * Version 0.1 fuer HW Rev 1.0
 *
 * Copyright 2021 - Michael Staudenmaier
 * -------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>

#include "i2cmaster.h"
#include "spi_master.h"

#include "flash.h"
#include "rv8523.h"
#include "rv8523_regs.h"

#include "hwconfig.h"


/* =================================================================
   settings
   ================================================================= */

#define UART_OUTPUT(x)   x    // define this to x to get output, otherwise to nothing

#define TEST_LED                   1
#define TEST_UART                  1
#define TEST_RTC                   1
#define TEST_RTC_IRQ               1
#define TEST_FLASH                 1
#define TEST_PUSHBUTTON_IRQ        1

/* =================================================================
   Deklarationen
   ================================================================= */

static int uart_putchar(char c, FILE *stream);
static int uart_getchar(FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

static volatile uint8_t rtcint_counter = 0;
static volatile uint8_t buttonint_counter = 0;


/* =================================================================
   ISR functions
   ================================================================= */

// ISR for INT1 from RTC -> AtMega328P PD4 = PCINT20
// ISR für Pushbutton -> ATMEGA328P PD6 = PCINT22
ISR(PCINT2_vect)
{

    /* handler for RTC wakeup interrupt */
    if(!(PIND & _BV(RTCINT1))){ // triggered by the RTC
        PORTD |= _BV(LED_STATE);
        rv8523_clearAlarmFlag();
        rv8523_setAlarmMinute(false, 0, true);
        ++rtcint_counter;
        // wait until IRQ line is back up!
        while(!(PIND & _BV(RTCINT1)))
            ;
        PORTD &= ~_BV(LED_STATE);
    }

    /* handler for button pressed interrupt */
    if(!(PIND & _BV(BUTTON))){ // triggered by the button
        do{
            _delay_ms(10);
        }while(!(PIND & _BV(BUTTON)));
        _delay_ms(10);
        ++buttonint_counter;
    }
}

// ISR for wakeup-key
#if(0)
ISR(INT0_vect)
{
    LED_PORT = _BV(LED_STATUS);
    _delay_ms(200);
    LED_PORT &= ~_BV(LED_STATUS);
}
#endif

/* =================================================================
   Initialisierung des Chips
   
   In: nix
   Out: nix
   ================================================================= */
static void ioinit(void)
{
    // set baud rate - 1156200
    const unsigned int ubrr = 0xb;
    UBRR0H = (unsigned char) (ubrr>>8);
    UBRR0L = (unsigned char) ubrr;

    UCSR0A |= (1<<U2X0);

    /* set frame format 8N1 */
    UCSR0C = (0<<USBS0) | (3<<UCSZ00);

    /* enable receiver and transmitter */
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
        
    stdin = stdout = &mystdout; //Required for printf init
}


static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n') uart_putchar('\r', stream);
    
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

static int uart_getchar(FILE *stream)
{
    uint8_t data;
    loop_until_bit_is_set(UCSR0A, RXC0);
    data = UDR0;

    // echo output
    uart_putchar(data, stream);

    return data;
}


/**
 * Initialize processor
 * Set status
 * Endless loop putting the processor in sleep mode
 */
int main (void)
{
    int i;
    uint8_t val;
    uint8_t errors = 0;

    uint8_t year = 0x21;
    uint8_t month= 0x02;
    uint8_t weekday = 0x05;
    uint8_t day = 0x19;
    uint8_t hour = 0x23;
    uint8_t min = 0x50;
    uint8_t sec = 0x00;
    uint8_t buffer[512];

    ioinit();
    i2c_init();
    spi_masterInit();
    
    // set time once
#if(1)
    rv8523_coldInit();
    rv8523_setDateTime24(year, month, day, weekday, hour, min, sec, true);
#else
    rv8523_init();
#endif
    
    // configure GPIO
    DDRD = _BV(LED_STATE);             // output: LED, - input: INT1, Pushbutton
    PORTD = _BV(BUTTON) | _BV(RTCINT1); // pullup for RTC:INT1 and Pushbutton

    //DDRB &= ~0x07;    // all control signals to input (i.e. pull-up/pull-down drives them inactive!)
    //PORTB |= 0x02;    // nReset=0 nWP=1 nCS=0

    PORTB |= 0x05;    // nReset=1 nWP=0 nCS=1
    DDRB |= 0x07;    // all control signals to output
    
    // configure interrupt settings
    cli();
    PCMSK2 = _BV(PCINT20) | _BV(PCINT22);
    PCICR = _BV(PCIE2);

    printf_P(PSTR("\n\n*Datenlogger Rev 1.0 Board HW test\n"));

    while(1){

#if(TEST_LED)
        printf_P(PSTR("Testcase 1: observe LED test pattern on board!\n"));
        for(i=0; i<3; ++i){
            PORTD |= _BV(LED_STATE);
            _delay_ms(500);
            PORTD &= ~_BV(LED_STATE);
            _delay_ms(500);
        }
#endif
       
#if(TEST_UART)    
        printf_P(PSTR("Testcase 2: press 'a' in the terminal. "));
        scanf("%c", &val);
        if(val=='a')
            printf_P(PSTR("pass\n"));
        else{
            printf_P(PSTR("FAIL\n"));
            ++errors;
        } 
#endif
        
#if(TEST_RTC)
        printf_P(PSTR("Testcase 3: check RTC via I2C."));
        /* let's read the current time */
        uint8_t year, month, day, weekday, hour, min, sec;
        rv8523_getDateTime24(&year, &month, &day, &weekday,
                             &hour, &min, &sec, false);
        bool bat = false;
        printf_P(PSTR("  %02d:%02d:%02d %02d.%02d.20%02d (BattLow=%s)\n"),
                 hour, min, sec, day, month, year, bat? "true":"false");
#endif

#if(TEST_RTC_IRQ)
        printf_P(PSTR("Testcase 4: check interrupts triggered by RTC (alarm min=%02d) "), (min+1)%60);
        while(!(PIND & _BV(RTCINT1))){
            printf_P(PSTR("Failed\n*** Warning: RTCIN0 active - trying to de-assert IRQ\n"));
            rv8523_clearAlarmFlag();
            PCIFR |= _BV(PCIF2); // clear IRQ request as well
            ++errors;
        }

        if(PIND & _BV(RTCINT1)){  // IRQ line is high
            uint8_t vals[0x14];
            // PCIFR |= _BV(PCIF2); // clear IRQ request as well
            rv8523_setAlarmMinute(true, min+1, false);
            rv8523_setAlarmIrq(true);
            if(!(PIND & _BV(RTCINT1))){
                printf("aarrgg - low again!\n");
            }

            rtcint_counter = 0;
            sei(); // is that required?
            while(rtcint_counter==0)
                ;
            cli();
            printf_P(PSTR("Passed\n"));
        }
#endif

#if(TEST_FLASH)
        printf_P(PSTR("Testcase 5: read ID of Flash device. "));
        uint8_t buf[5];
        uint8_t buf2[2];
            
        //flash_init();  // trigger a reset to start with
        // spi_masterInit();
        flash_get_id(buf);
        flash_get_status(buf2);
        printf_P(PSTR("ID(%02x %02x %02x %02x %02x) STATUS(%02x %02x). "), buf[0], buf[1], buf[2], buf[3], buf[4], buf2[0], buf2[1]);
        if((buf[0]==0x1f)
           && (buf[1]==0x27)
           && (buf[2]==0x01))
            printf_P(PSTR("ok\n"));
        else{
            printf_P(PSTR("FAIL\n"));
            ++errors;
        }
        if(buf2[0] & 0x1){
            printf_P(PSTR("            flash configured with 512bytes/sector\n"));
        }else{
            printf_P(PSTR("            flash configured with 528bytes/sector\n"));
            printf_P(PSTR("            ->we'll fix that for you. "));
            flash_conf_power2_size();
            flash_get_status(buf2);
            if(buf2[0]&0x01){
                printf_P(PSTR("ok\n"));
            }else{
                printf_P(PSTR("FAIL\n"));
                ++errors;
            }     
        }

        printf_P(PSTR("Testcase 6: write page to flash.\n"));
        int i=8000;
        buffer[0] = (i >>8) & 0xff;
        buffer[1] = i & 0xff;
        flash_write_page(i, buffer);
        flash_enter_ultradeep_powerdown();
        _delay_ms(1000);
        flash_resume_ultradeep_powerdown();        

        printf_P(PSTR("Testcase 7: read page from flash. "));
        flash_read_page(buffer, 8000);
        if((buffer[0] == ((i >>8) & 0xff)) &&
           (buffer[1] == (i & 0xff))){
            printf_P(PSTR("ok\n"));            
        }else{
            printf_P(PSTR("FAIL\n"));
            ++errors;
        }
#endif

#if(TEST_PUSHBUTTON_IRQ)        
        printf_P(PSTR("Testcase 8: check interrupts triggered by pushbutton "));
        if(!(PIND & _BV(BUTTON))){
            printf_P(PSTR("Failed\n*** Error: pin not pulled high!\n"));
            ++errors;
        }else{
            buttonint_counter = 0;
            sei();
            while(!buttonint_counter)
                ;
            cli();
            printf_P(PSTR("Passed\n"));        
        }
#endif

        UART_OUTPUT(printf_P(PSTR("\n*****************Summary %d errors ******************\n\n"), errors));
    }

    return (0);
}
