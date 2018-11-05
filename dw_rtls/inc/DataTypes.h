#ifndef __DATATYPES_H
#define __DATATYPES_H

#include "SysConfig.h"

#include "dw1000_instance.h"

__packed typedef struct {
  INST_MODE InstType;
  uint8_t InstId;
  uint8_t ListSize;
  uint32_t Distance[MAX_ANCHOR_LIST_SIZE];
} DistDataRespDef;

#endif /* __DATATYPES_H */
