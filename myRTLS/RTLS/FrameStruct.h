#ifndef __FRAMESTRUCT_H
#define __FRAMESTRUCT_H

#include "PlatformConfig.h"

#ifndef __packed
  /* __packed keyword used to decrease the data type alignment to 1-byte */
  #if defined (__CC_ARM)         /* ARM Compiler */
    #define __packed    __packed
  #elif defined (__ICCARM__)     /* IAR Compiler */
    #define __packed    __packed
  #elif defined   ( __GNUC__ )   /* GNU Compiler */                        
    #define __packed    __attribute__ ((__packed__))
  #elif defined   (__TASKING__)  /* TASKING Compiler */
    #define __packed    __unaligned
  #endif /* __CC_ARM */
#endif

#define FRAME_MAX_LENGTH               (40)

#define DEFAULT_STX                    (0x5A)

typedef enum {
  poll_msg = 0x1,
  resp_msg = 0x2,
  final_msg = 0x3
} FrameTypeDef;

__packed typedef struct {
  uint8_t TagAddr;
} PollMsgDataDef;

__packed typedef struct {
  uint8_t TagAddr;
  uint8_t AncAddr;
  FloatUnion Distance;
} RespMsgDataDef;

__packed typedef struct {
  uint8_t TagAddr;
  uint64_t x;
} FinalMsgDataDef;

__packed typedef struct {
  uint8_t stx;
  uint8_t SepNbr;
  FrameTypeDef fType;
  uint16_t CRC16;
} FrameDataDef;

typedef union {
  FrameDataDef Frame;
  uint8_t uData[FRAME_MAX_LENGTH];
} FrameDataUnion;

#endif /* __FRAMESTRUCT_H */
