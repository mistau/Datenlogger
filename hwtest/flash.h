/**
 * -------------------------------------------------------------------------
 * @file flash.h
 * Access routines for Adesto flash
 * currently supporting AT45DB321E
 *
 * Version 0.1
 *
 * Copyright 2021 - Michael Staudenmaier
 * -------------------------------------------------------------------------
 */

#ifndef _FLASH_H_
#define _FLASH_H_


/**
 * @brief flash_init
 *
 * @desc setup the flash device
 *
 */
void flash_init(void);

/**
 * @brief flash_wait_ready
 *
 * @desc query status of the flash device and wait until it gets ready
 *
 */
void flash_wait_ready(void);

/**
 * @brief flash_get_status
 *
 * @desc return the status of the flash device
 *
 * @param buf  array holding the 2 status bytes
 * @return status byte 1 (holding the busy flag). This is redundant to buf[0]!
 */
uint8_t flash_get_status(uint8_t *buf);

/**
 * @brief flash_get_id
 *
 * @desc return the id of the flash device
 *
 * @param buf  array of 5 bytes filled with the id signature
 */
void flash_get_id(uint8_t *buf);

/**
 * @brief flash_conf_power2_size
 *
 * @desc configure the flash with a pagesize of 512 bytes
 */
void flash_conf_power2_size(void);


/**
 * @brief flash_conf_standard_size
 *
 * @desc configure the flash with a pagesize of 528 bytes
 */
void flash_conf_standard_size(void);


/**
 * @brief flash_read_page
 *
 * @desc read one page (=512/528 bytes) from the flash
 * 
 * @param *buf   buffer filled with the data from flash
 * @param pageno specifies the page number, valid values from 0-8191
 */
void flash_read_page(uint8_t *buf, uint16_t pageno);

/**
 * @brief flash_write_page
 *
 * @desc write one page (=512/528 bytes) to the flash
 * 
 * @param pageno specifies the page number, valid values from 0-8191
 * @param *buf   buffer containing the data
 */
void flash_write_page(uint16_t pageno, const uint8_t *buf);


/**
 * @brief flash_erase_chip
 *
 * @desc erases the entire flash content
 * @note this requires 45-80sec!
 * 
 */
void flash_erase_chip(void);


/**
 * @brief flash_enter_deep_powerdown
 *
 * @desc enter the power down mode
 * @note device only reacts to @flash_resume_powerdown
 * @note if buffer content is not required use @flash_enter_ultradeep_powerdown
 */
void flash_enter_deep_powerdown(void);


/**
 * @brief flash_resume_deep_powerdown
 *
 * @desc leave the power down mode
 */
void flash_resume_deep_powerdown(void);


/**
 * @brief flash_enter_ultradeep_powerdown
 *
 * @desc enter the ultradeep power down mode
 * @note device only reacts to @flash_resume_ultradeep_powerdown
 */
void flash_enter_ultradeep_powerdown(void);


/**
 * @brief flash_resume_ultradeep_powerdown
 *
 * @desc leave the ultradeep power down mode
 */
void flash_resume_ultradeep_powerdown(void);

#endif
