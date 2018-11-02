#ifndef __DATATYPES_H
#define __DATATYPES_H

#include "SysConfig.h"

#include "dw1000_instance.h"

__packed typedef struct {
  INST_MODE InstType;
  uint8_t InstId;
  uint32_t Distance;
} DistDataRespDef;

#endif /* __DATATYPES_H */
