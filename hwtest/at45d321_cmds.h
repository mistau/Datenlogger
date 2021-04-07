/**
 * -------------------------------------------------------------------------
 * @file at45d321_cmds.h
 * commands for Adesto flash AT45DB321E
 * 
 * @note based on adesto datasheet
 * https://www.mouser.de/datasheet/2/590/doc8784-1385823.pdf
 *
 * Version 0.1
 *
 * Copyright 2021 - Michael Staudenmaier
 * -------------------------------------------------------------------------
 */

#ifndef _AT45D321_CMDS_H_
#define _AT45D321_CMDS_H_

/* read commands */
#define FLASHCMD_MAIN_MEM_PAGE_READ                           0xd2
#define FLASHCMD_CONTINUOUS_ARRAY_READ_LOW_POWER_MODE         0x01
#define FLASHCMD_CONTINUOUS_ARRAY_READ_LOW_FREQUENCY          0x03
#define FLASHCMD_CONTINUOUS_ARRAY_READ_HIGH_FREQUENCY         0x0b
#define FLASHCMD_BUF1_READ_LOW_FREQUENCY                      0xd1
#define FLASHCMD_BUF2_READ_LOW_FREQUENCY                      0xd3
#define FLASHCMD_BUF1_READ_HIGH_FREQUENCY                     0xd4
#define FLASHCMD_BUF2_READ_HIGH_FREQUENCY                     0xd6
                                                                 
/* program and erase commands */                                 
#define FLASHCMD_BUF1_WRITE                                   0x84
#define FLASHCMD_BUF2_WRITE                                   0x87
#define FLASHCMD_BUF1_TO_MAIN_MEM_PAGE_WITH_ERASE             0x83
#define FLASHCMD_BUF2_TO_MAIN_MEM_PAGE_WITH_ERASE             0x86
#define FLASHCMD_BUF1_TO_MAIN_MEM_PAGE_WITHOUT_ERASE          0x88
#define FLASHCMD_BUF2_TO_MAIN_MEM_PAGE_WITHOUT_ERASE          0x89
#define FLASHCMD_MAIN_MEM_PAGE_PROGRAM_THROUGH_BUF1_ERASE     0x82
#define FLASHCMD_MAIN_MEM_PAGE_PROGRAM_THROUGH_BUF2_ERASE     0x85
#define FLASHCMD_MAIN_MEM_BYTE_PAGE_PROGRAM_THOUGH_BUF1_ERASE 0x02
#define FLASHCMD_PAGE_ERASE                                   0x81
#define FLASHCMD_BLOCK_ERASE                                  0x50
#define FLASHCMD_SECTOR_ERASE                                 0x7c
#define FLASHCMD_CHIP_ERASE0                                  0xc7
#define FLASHCMD_CHIP_ERASE1                                  0x94
#define FLASHCMD_CHIP_ERASE2                                  0x80
#define FLASHCMD_CHIP_ERASE3                                  0x9a
#define FLASHCMD_PROGRAM_ERASE_SUSPEND                        0xb0
#define FLASHCMD_PROGRAM_ERASE_RESUME                         0xd0
#define FLASHCMD_READ_MODIFY_WRITE_THROUGH_BUF1               0x58
#define FLASHCMD_READ_MODIFY_WRITE_THROUGH_BUF2               0x59

/* protection and security commands */
#define FLASHCMD_ENABLE_SECTOR_PROTECTION0                    0x3d
#define FLASHCMD_ENABLE_SECTOR_PROTECTION1                    0x2a
#define FLASHCMD_ENABLE_SECTOR_PROTECTION2                    0x7f
#define FLASHCMD_ENABLE_SECTOR_PROTECTION3                    0xa9
#define FLASHCMD_DISABLE_SECTOR_PROTECTION0                   0x3d
#define FLASHCMD_DISABLE_SECTOR_PROTECTION1                   0x2a
#define FLASHCMD_DISABLE_SECTOR_PROTECTION2                   0x7f
#define FLASHCMD_DISABLE_SECTOR_PROTECTION3                   0x9a
#define FLASHCMD_ERASE_SECTOR_PROTECTION_REGISTER0            0x3d
#define FLASHCMD_ERASE_SECTOR_PROTECTION_REGISTER1            0x2a
#define FLASHCMD_ERASE_SECTOR_PROTECTION_REGISTER2            0x7f
#define FLASHCMD_ERASE_SECTOR_PROTECTION_REGISTER3            0xcf
#define FLASHCMD_PROGRAM_SECTOR_PROTECTION_REGISTER0          0x3d
#define FLASHCMD_PROGRAM_SECTOR_PROTECTION_REGISTER1          0x2a
#define FLASHCMD_PROGRAM_SECTOR_PROTECTION_REGISTER2          0x7f
#define FLASHCMD_PROGRAM_SECTOR_PROTECTION_REGISTER3          0xfc
#define FLASHCMD_READ_SECTOR_PROTECTION_REGISTER              0x32
#define FLASHCMD_SECTOR_LOCKDOWN0                             0x3d
#define FLASHCMD_SECTOR_LOCKDOWN1                             0x2a
#define FLASHCMD_SECTOR_LOCKDOWN2                             0x7f
#define FLASHCMD_SECTOR_LOCKDOWN3                             0x30
#define FLASHCMD_READ_SECTOR_LOCKDOWN_REGISTER                0x35
#define FLASHCMD_FREEZE_SECTOR_LOCKDOWN_REGISTER0             0x34
#define FLASHCMD_FREEZE_SECTOR_LOCKDOWN_REGISTER1             0x55
#define FLASHCMD_FREEZE_SECTOR_LOCKDOWN_REGISTER2             0xaa
#define FLASHCMD_FREEZE_SECTOR_LOCKDOWN_REGISTER3             0x40
#define FLASHCMD_PROGRAM_SECURITY_REGISTER0                   0x9b
#define FLASHCMD_PROGRAM_SECURITY_REGISTER1                   0x00
#define FLASHCMD_PROGRAM_SECURITY_REGISTER2                   0x00
#define FLASHCMD_PROGRAM_SECURITY_REGISTER3                   0x00
#define FLASHCMD_READ_SECURITY_REGISTER                       0x77

/* addition commands */
#define FLASHCMD_MAIN_MEM_PAGE_TO_BUFFER1_TRANSFER            0x53
#define FLASHCMD_MAIN_MEM_PAGE_TO_BUFFER2_TRANSFER            0x55
#define FLASHCMD_MAIN_MEM_PAGE_TO_BUFFER1_COMPARE             0x60
#define FLASHCMD_MAIN_MEM_PAGE_TO_BUFFER2_COMPARE             0x61
#define FLASHCMD_AUTO_PAGE_REWRITE_THROUGH_BUFFER1            0x58
#define FLASHCMD_AUTO_PAGE_REWRITE_THROUGH_BUFFER2            0x59
#define FLASHCMD_DEEP_POWER_DOWN                              0xb9
#define FLASHCMD_RESUME_FROM_DEEP_POWER_DOWN                  0xab
#define FLASHCMD_ULTRA_DEEP_POWER_DOWN                        0x79
#define FLASHCMD_STATUS_REGISTER_READ                         0xd7
#define FLASHCMD_MANUFACTURER_AND_DEVICE_ID_READ              0x9f
#define FLASHCMD_CONFIGURE_POWER_OF_TWO_PAGE_SIZE0            0x3d
#define FLASHCMD_CONFIGURE_POWER_OF_TWO_PAGE_SIZE1            0x2a
#define FLASHCMD_CONFIGURE_POWER_OF_TWO_PAGE_SIZE2            0x80
#define FLASHCMD_CONFIGURE_POWER_OF_TWO_PAGE_SIZE3            0xa6
#define FLASHCMD_CONFIGURE_STANDARD_DATAFLASH_PAGE_SIZE0      0x3d
#define FLASHCMD_CONFIGURE_STANDARD_DATAFLASH_PAGE_SIZE1      0x2a
#define FLASHCMD_CONFIGURE_STANDARD_DATAFLASH_PAGE_SIZE2      0x80
#define FLASHCMD_CONFIGURE_STANDARD_DATAFLASH_PAGE_SIZE3      0xa7
#define FLASHCMD_SOFTWARE_RESET0                              0xf0
#define FLASHCMD_SOFTWARE_RESET1                              0x00
#define FLASHCMD_SOFTWARE_RESET2                              0x00
#define FLASHCMD_SOFTWARE_RESET3                              0x00

#endif
