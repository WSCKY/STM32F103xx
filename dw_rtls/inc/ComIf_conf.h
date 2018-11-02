#ifndef __COMIF_CONF_H
#define __COMIF_CONF_H

#include "stm32f10x.h"

#include "usbd_usr.h"
#include "usbd_cdc_vcp.h"

#define COM_IF_TX_BYTES(p, l)          USB_CDC_SendBufferFast(p, l)
#define COM_IF_TX_CHECK()              USBD_isEnabled()

#define COM_USER_TYPE                { \
									                   }

#define COM_USER_TYPE_DATA           { \
                                     }

#endif /* __COMIF_CONF_H */
