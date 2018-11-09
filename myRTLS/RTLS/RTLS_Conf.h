#ifndef __RTLS_CONF_H
#define __RTLS_CONF_H

#include "SysConfig.h"

#define ANC_ADDR_DEFAULT               (0x0)

#define IS_ANC_ADDR(x)                 (!((x) & 0x80))

#define TAG_ADDR_DEFAULT               (0x80)

#define IS_TAG_ADDR(x)                 ((x) & 0x80)

/*Should be accurately calculated during calibration*/
#define TX_ANT_DLY 16456
#define RX_ANT_DLY 16456

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
 * 1 uus = 512 / 499.2us and 1us = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME 65536

/* Speed of light in air, in metres per second. */
#define SPEED_OF_LIGHT 299702547

#define INSTANCE_MODE_TAG              (0)

#endif /* __RTLS_CONF_H */
