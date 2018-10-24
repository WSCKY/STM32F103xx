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
 *  @file   dw1000_types.h
 *  @brief  DecaWave general type definitions
 *
 */

#ifndef _DW1000_TYPES_H_
#define _DW1000_TYPES_H_

#include "SysConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

//#include "dw1000_compiler.h"

//#ifndef uint8
//#ifndef _DECA_UINT8_
//#define _DECA_UINT8_
//typedef unsigned char uint8;
//#endif
//#endif

//#ifndef uint16
//#ifndef _DECA_UINT16_
//#define _DECA_UINT16_
//typedef unsigned short uint16;
//#endif
//#endif

//#ifndef uint32
//#ifndef _DECA_UINT32_
//#define _DECA_UINT32_
//typedef unsigned long uint32;
//#endif
//#endif

//#ifndef int8
//#ifndef _DECA_INT8_
//#define _DECA_INT8_
//typedef signed char int8;
//#endif
//#endif

//#ifndef int16
//#ifndef _DECA_INT16_
//#define _DECA_INT16_
//typedef signed short int16;
//#endif
//#endif

//#ifndef int32
//#ifndef _DECA_INT32_
//#define _DECA_INT32_
//typedef signed long int32;
//#endif
//#endif

#define int8                 int8_t
#define uint8                uint8_t
#define int16                int16_t
#define uint16               uint16_t
#define int32                int32_t
#define uint32               uint32_t
#define int64                int64_t
#define uint64               uint64_t

#ifndef NULL
#define NULL                0
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DW1000_TYPES_H_ */

