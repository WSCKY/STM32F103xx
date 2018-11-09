#ifndef __FRAMESTRUCT_H
#define __FRAMESTRUCT_H

#include "PlatformConfig.h"

#define FRAME_MAX_LENGTH               (40)

#define DEFAULT_STX                    (0x5A)

typedef enum {
  poll_msg = 0x1,
  resp_msg = 0x2,
  final_msg = 0x3
} FrameTypeDef;

typedef struct {
  uint8_t TagAddr;
} PollMsgDataDef;

typedef struct {
  uint8_t TagAddr;
  uint8_t AncAddr;
  FloatUnion Distance;
} RespMsgDataDef;

typedef struct {
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
