/// @file   fdc.h
/// @brief  Definitions about the floppy.
/// @author shainer <shainer@debianclan.org>
/// @date   Mar 31 2007
/// @copyright
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
/// This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software Foundation,
/// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#pragma once

/// @brief Floppy Disk Controller (FDC) registers.
typedef enum fdc_registers_t
{
    STATUS_REGISTER_A = 0x3F0,
    ///< @brief This register is read-only and monitors the state of the
    ///< interrupt pin and several disk interface pins.
        STATUS_REGISTER_B = 0x3F1,
    ///< @brief This register is read-only and monitors the state of several
    ///< disk interface pins.
        DOR = 0x3F2,
    ///< @brief The Digital Output Register contains the drive select and
    ///< motor enable bits, a reset bit and a DMA GATE bit.
        TAPE_DRIVE_REGISTER = 0x3F3,
    ///< @brief This register allows the user to assign tape support to a
    ///< particular drive during initialization.
        MAIN_STATUS_REGISTER = 0x3F4,
    ///< @brief The Main Status Register is a read-only register and is used
    ///< for controlling command input and result output for all commands.
        DATARATE_SELECT_REGISTER = 0x3F4,
    ///< @brief This register is included for compatibility with the 82072
    ///< floppy controller and is write-only.
        DATA_FIFO = 0x3F5,
    ///< @brief All command parameter information and disk data transfers go
    ///< through the FIFO.
        DIGITAL_INPUT_REGISTER = 0x3F7,
    ///< @brief This register is read only in all modes.
        CONFIGURATION_CONTROL_REGISTER = 0x3F7
    ///< @brief This register sets the datarate and is write only.
} fdc_registers_t;

/// @brief Allows to enable the motor.
/// @details The motor enable pins are directly controlled via the DOR and
/// are a function of the mapping based on BOOTSEL bits in the TDR.
void fdc_enable_motor(void);

/// @brief Allows to disable the motor.
void fdc_disable_motor(void);
