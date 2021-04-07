/**
 * -------------------------------------------------------------------------
 * @file rv8523.c
 * Access routines for MicroCrystal realtime clock RV-8523-C3
 * for datasheet see 
 *  https://www.microcrystal.com/en/products/real-time-clock-rtc-modules/rv-8523-c3/
 *
 * Version 0.1
 *
 * Copyright 2021 - Michael Staudenmaier
 * -------------------------------------------------------------------------
 */

#include <stdbool.h>

#include "i2cmaster.h"
#include "rv8523_regs.h"
#include "rv8523.h"

/*
 * local functions
 */


static uint8_t bcd_to_dec(uint8_t x)
{
    return x - 6 * (x>>4);
}

static uint8_t dec_to_bcd(uint8_t x)
{
    return x + 6 * (x/10);
}


static uint8_t read_reg(uint8_t regno){
    uint8_t ret;
    i2c_start_wait(DEV_RV8523+I2C_WRITE);
    i2c_write(regno);
    i2c_rep_start(DEV_RV8523+I2C_READ);
    ret = i2c_readNak();
    i2c_stop();
    return ret;
}

static void read_nregs(uint8_t start_regno, uint8_t nregs, uint8_t *pvalues){
    i2c_start_wait(DEV_RV8523+I2C_WRITE);
    i2c_write(start_regno);
    i2c_rep_start(DEV_RV8523+I2C_READ);
    for(; nregs>1; --nregs, pvalues++)
        *pvalues = i2c_readAck();
    *pvalues = i2c_readNak();
    i2c_stop();
}


static void write_reg(uint8_t regno, uint8_t value){
    i2c_start_wait(DEV_RV8523+I2C_WRITE);
    i2c_write(regno);
    i2c_write(value);
    i2c_stop();
}

static void write_nregs(uint8_t start_regno, uint8_t nregs, const uint8_t *pvalues)
{
    i2c_start_wait(DEV_RV8523+I2C_WRITE);
    i2c_write(start_regno);
    for(; nregs>0; --nregs, ++pvalues)
        i2c_write(*pvalues);
    i2c_stop();    
}

/*
 * global functions
 */

void rv8523_init(void)
{
    write_reg(RV8523_TIMER_CLOCKOUT, 0x38); /* switch off CLKOUT on INT1 */

    /* \fixme the line below only works with battery connected! */
    /* write_reg(RV8523_CONTROL3, 0x00); */  /* battery switchover in standard mode */

    /* \todo rtc correction mechanism */
}


void rv8523_coldInit(void)
{
    write_reg(RV8523_CONTROL1, 0x58);  /* we first reset the RTC */

    write_reg(RV8523_TIMER_CLOCKOUT, 0x38); /* switch off CLKOUT on INT1 */
    /* \fixme the line below only works with battery connected! */
    /* write_reg(RV8523_CONTROL3, 0x00); */  /* battery switchover in standard mode */

    /* \todo rtc correction mechanism */
}


bool rv8523_batteryEmtpy(void)
{
    return (read_reg(RV8523_CONTROL3) & 0x4) != 0;
}


bool rv8523_getDateTime24(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *weekday,
                          uint8_t *hour, uint8_t *min, uint8_t *sec, bool bcd_mode)
{
    uint8_t vals[7];
    read_nregs(RV8523_SECONDS, 7, vals);
    if(bcd_mode){
        *sec = vals[0];
        *min = vals[1] & 0x7f;
        *hour = vals[2] & 0x3f;
        *day = vals[3] & 0x3f;
        *month = vals[5] & 0x1f;
        *year = vals[6];
    }else{
        *sec     = bcd_to_dec(vals[0]);
        *min     = bcd_to_dec(vals[1] & 0x7f);
        *hour    = bcd_to_dec(vals[2] & 0x3f);
        *day     = bcd_to_dec(vals[3] & 0x3f);
        *month   = bcd_to_dec(vals[5] & 0x1f);
        *year    = bcd_to_dec(vals[6]);
    }
    *weekday = vals[4] & 0x07;
    return (vals[0] & 0x80)==0;
}


void rv8523_setDateTime24(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday,
                          uint8_t hour, uint8_t min, uint8_t sec, bool bcd_mode)
{
    uint8_t vals[7];
    if(bcd_mode){
        vals[0] = sec;
        vals[1] = min & 0x7f;
        vals[2] = hour & 0x3f;
        vals[3] = day & 0x3f;
        vals[5] = month & 0x1f;
        vals[6] = year;
    }else{
        vals[0] = dec_to_bcd(sec);
        vals[1] = dec_to_bcd(min & 0x7f);
        vals[2] = dec_to_bcd(hour & 0x3f);
        vals[3] = dec_to_bcd(day & 0x3f);
        vals[5] = dec_to_bcd(month & 0x1f);
        vals[6] = dec_to_bcd(year);
    }
    vals[4] = weekday & 0x07;
    write_nregs(RV8523_SECONDS, 7, vals);
}


void rv8523_setAlarmMinute(bool enable, uint8_t min, bool bcd_mode)
{
    uint8_t val;

    if(bcd_mode){
        val = min & 0x7f;
    }else{
        val = dec_to_bcd(min) & 0x7f;
    }
    if(!enable)
        val |= 0x80;
    write_reg(RV8523_MINUTE_ALARM, val);
}


void rv8523_setAlarmHour(bool enable, uint8_t hour, bool bcd_mode)
{
    uint8_t val;

    if(bcd_mode){
        val = hour & 0x3f;
    }else{
        val = dec_to_bcd(hour) & 0x3f;
    }
    if(!enable)
        val |= 0x80;
    write_reg(RV8523_HOUR_ALARM, val);
}

void rv8523_setAlarmDay(bool enable, uint8_t day, bool bcd_mode)
{
    uint8_t val;

    if(bcd_mode){
        val = day & 0x7f;
    }else{
        val = dec_to_bcd(day) & 0x7f;
    }
    if(!enable)
        val |= 0x80;
    write_reg(RV8523_DAY_ALARM, val);
}


void rv8523_setAlarmWeekday(bool enable, uint8_t wday)
{
    uint8_t val;

    val = dec_to_bcd(wday) & 0x07;
    if(!enable)
        val |= 0x80;
    write_reg(RV8523_WEEKDAY_ALARM, val);
}


void rv8523_setAlarmIrq(bool enable)
{
    uint8_t val = read_reg(RV8523_CONTROL1) & ~(1<<6); /* clear N flag */
    if(enable)
        val |= 0x02;
    else
        val &= ~0x02;
            
    write_reg(RV8523_CONTROL1, val);
}

void rv8523_clearAlarmFlag(void)
{
    /* \fixme we destroy the watchdog timer settings here! */
    write_reg(RV8523_CONTROL2, 0x70); /* clear AF only */    
}


void rv8523_getAllRegs(uint8_t *ptr){
    read_nregs(RV8523_CONTROL1, 0x0e, ptr);        
}
