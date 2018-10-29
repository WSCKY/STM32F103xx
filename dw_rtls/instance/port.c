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
 * @file    port.c
 * @brief   HW specific definitions and functions for portability
 *
 */

#include "dw1000_port.h"

/* RTT 32 bit variable defined by the platform */
extern __IO unsigned long time32_incr;

unsigned long portGetTickCnt(void)
{
    return time32_incr;
}

//void deca_reset(void *arg)
//{
//	_DW_CLEAR_PENDING_EXT_IRQ();
//}
