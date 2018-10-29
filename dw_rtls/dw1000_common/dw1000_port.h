/**
 * Copyright 2017 Decawave Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*! ----------------------------------------------------------------------------
 * @file    dw1000_port.h
 * @brief   HW specific definitions and functions for portability
 *
 */

#ifndef __DW1000_PORT_H_
#define __DW1000_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "DW1000.h"
#include "dw1000_device_api.h"
#include "TimerCounter.h"

#define writetospi                     DW1000_SPI_Write
#define readfromspi                    DW1000_SPI_Read

void SPI_ChangeRate(uint16_t scalingfactor);

#define portGetTickCnt()               _Get_Millis()

void setup_DW1000RSTnIRQ(int enable);

/**
 * @brief This function clears chip select line
 */
#define port_SPIx_clear_chip_select    _DW_SPI_CS_ENABLE

/**
 * @brief This function set chip select line
 */
#define port_SPIx_set_chip_select      _DW_SPI_CS_DISABLE

/**
 * @brief This function gets IRQ status
 */
#define port_GetEXT_IRQStatus          _DW_GET_EXT_IRQ_STATUS

/**
 * @brief This function enables external IRQ
 */
#define port_EnableEXT_IRQ             _DW_ENABLE_EXT_IRQ

/**
 * @brief This function disables external IRQ
 */
#define port_DisableEXT_IRQ            _DW_DISABLE_EXT_IRQ

/**
 * @brief This function checks external IRQ status
 */
#define port_CheckEXT_IRQ              _DW_IRQn_GET_LEVEL

/**
 * @brief This function provides a time delay in milliseconds
 */
#define port_Delay_MS                  _delay_ms

#ifdef __cplusplus
}
#endif

#endif /* __DW1000_PORT_H_ */
