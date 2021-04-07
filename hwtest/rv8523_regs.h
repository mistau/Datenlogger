/*
 * -------------------------------------------------------------------------
 * HW register description for 
 * MicroCrystal realtime clock RV-8523-C3
 * for datasheet see 
 *  https://www.microcrystal.com/en/products/real-time-clock-rtc-modules/rv-8523-c3/
 *
 * Version 0.1
 *
 * Copyright 2021 - Michael Staudenmaier
 * -------------------------------------------------------------------------
 */
#ifndef _RV8523_REGS_H_
#define _RV8523_REGS_H_

#define DEV_RV8523  0xd0  /**< I2C slave address */

/* accessible register */
#define RV8523_CONTROL1         0x00
#define RV8523_CONTROL2         0x01
#define RV8523_CONTROL3         0x02
#define RV8523_SECONDS          0x03
#define RV8523_MINUTES          0x04
#define RV8523_HOURS            0x05
#define RV8523_DAYS             0x06
#define RV8523_WEEKDAYS         0x07
#define RV8523_MONTHS           0x08
#define RV8523_YEARS            0x09
#define RV8523_MINUTE_ALARM     0x0a
#define RV8523_HOUR_ALARM       0x0b
#define RV8523_DAY_ALARM        0x0c
#define RV8523_WEEKDAY_ALARM    0x0d
#define RV8523_FREQUENCY_OFFSET 0x0e
#define RV8523_TIMER_CLOCKOUT   0x0f
#define RV8523_TIMER_A_CLOCK    0x10
#define RV8523_TIMER_A          0x11
#define RV8523_TIMER_B_CLOCK    0x12
#define RV8523_TIMER_B          0x13


#endif
