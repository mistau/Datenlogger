/**
 * -------------------------------------------------------------------------
 * @file rv8523.h
 * Access routines for MicroCrystal realtime clock RV-8523-C3
 * for datasheet see 
 *  https://www.microcrystal.com/en/products/real-time-clock-rtc-modules/rv-8523-c3/
 *
 * Version 0.1
 *
 * Copyright 2021 - Michael Staudenmaier
 * -------------------------------------------------------------------------
 */
#ifndef _RV8523_H_
#define _RV8523_H_

#include <stdbool.h>  /* constant source of problems due to bool */


/**
 * @brief rv8523_init
 *
 * @desc initialized the RV8523 basic settings.
 *       This does not affect the time/date setting.
 */
void rv8523_init(void);


/**
 * @brief rv8523_coldInit
 *
 * @desc initialized the RV8523 from scratch to a defined stats.
 *       The RTS is reset first, afterwards the standard switchover to battery is selected.
 * @warning after this the time is gone and has to be set again!
 */
void rv8523_coldInit(void);


/**
 * @brief rv8523_batteryEmpty
 * 
 * @desc checks if the battery is empty and should be replaced.
 *
 * @return true in case the battery is empty, false otherwise
 */
bool rv8523_batteryEmtpy(void);


/**
 * @brief rv8523_getDateTime24
 *
 * @desc Returns the time and data from the RTC in 24h mode.
 * 
 * @param *year    returns the year (2 digits)
 * @param *month   returns the month
 * @param *day     returns the day
 * @param *weekday returns the day of the week, 0=sunday, 6=saturday
 * @param *hour    returns the hours
 * @param *minutes returns the minutes 
 * @param *seconds returns the second
 * @param bcd_mode selects if all values are provided in bcd (true) or as decimal(false)
 *
 * @return clock integrity guaranteed (true) or not guaranteed (false) which means the oscilator stopped!
 *
 * @note as the RTC uses bcd values directly this is the faster mode saving conversions
 */
bool rv8523_getDateTime24(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *weekday,
                          uint8_t *hour, uint8_t *min, uint8_t *sec, bool bcd_mode);


/**
 * @brief rv8523_setDateTime24
 *
 * @desc Set the time and date of the RTC in 24h mode.
 * 
 * @param year     year to set (2 digits)
 * @param month    month
 * @param day      day
 * @param weekday  day of the week, 0=sunday, 6=saturday
 * @param hour     hours
 * @param minutes  minutes 
 * @param seconds  seconds
 * @param bcd_mode selects if all values are provided in bcd (true) or as decimal(false)
 *
 * @note as the RTC uses bcd values directly this is the faster mode saving conversions
 */
void rv8523_setDateTime24(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday,
                          uint8_t hour, uint8_t min, uint8_t sec, bool bcd_mode);



/**
 * @brief set minutes of alarm clock
 * 
 * @desc set the minute value for the alarm clock
 *
 * @param enable   makes this setting active(true) or inactive(false)
 * @param min      minute value when to trigger the alarm
 * @param bcd_mode selects if min is provided in bcd (true) or as decimal(false)
 */
void rv8523_setAlarmMinute(bool enable, uint8_t min, bool bcd_mode);


/**
 * @brief set hours of alarm clock
 * 
 * @desc set the hours value for the alarm clock
 *
 * @param enable   makes this setting active(true) or inactive(false)
 * @param hour     hour value when to trigger the alarm
 * @param bcd_mode selects if hour is provided in bcd (true) or as decimal(false)
 */
void rv8523_setAlarmHour(bool enable, uint8_t hour, bool bcd_mode);


/**
 * @brief set day of alarm clock
 * 
 * @desc set the day value for the alarm clock
 *
 * @param enable   makes this setting active(true) or inactive(false)
 * @param day      day value when to trigger the alarm
 * @param bcd_mode selects if day is provided in bcd (true) or as decimal(false)
 */
void rv8523_setAlarmDay(bool enable, uint8_t day, bool bcd_mode);


/**
 * @brief set weekday of alarm clock
 * 
 * @desc set the weekday value for the alarm clock
 *
 * @param enable   makes this setting active(true) or inactive(false)
 * @param weekday  weekday value when to trigger the alarm
 *
 * @note as weekday is 0..7 only bcd and dec representation are the same here.
 */
void rv8523_setAlarmWeekday(bool enable, uint8_t wday);


/**
 * @brief set alarm interrupt
 *
 * @desc allows to enable or disable the alarm interrupt to the processor
 *
 * @param enable  enables the interrupt if true, disables otherwise
 */
void rv8523_setAlarmIrq(bool enable);


/**
 * @brief clear alarm interrupt status
 *
 * @desc clears the alarm interrupt flag of the RTC
 *
 */
void rv8523_clearAlarmFlag(void);


void rv8523_getAllRegs(uint8_t *ptr);

#endif
