#include "AncMain.h"

#if !(INSTANCE_MODE_TAG)
/* anchor id */
static uint8_t AncId = 0 & 0x7F;
/* alloc static memory to store the RX frame data. */
static FrameDataUnion _frameRX = {0};
/* alloc static memory where stored TX frame data. */
static FrameDataUnion _frameTX = {0};
/* Frame sequence number, incremented after each transmission. */
static uint8_t frame_seq_nb = 0;
/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32_t status_reg = 0;
/* distance report to tag */
static float _report_dist = 0;

/* Timestamps of frames transmission/reception.
 * As they are 40-bit wide, we need to define a 64-bit int type to handle them. */
static uint64_t poll_rx_ts;
static uint64_t resp_tx_ts;
static uint64_t final_rx_ts;

/* Declaration of static functions. */
static uint64 get_tx_timestamp_u64(void);
static uint64 get_rx_timestamp_u64(void);

static void rtls_init(void)
{
  _frameTX.Frame.stx = DEFAULT_STX;
  _frameTX.Frame.fType = resp_msg;
  _frameTX.Frame.SepNbr = frame_seq_nb;
}

static void anc_rtls_run(void)
{
  /* Clear reception timeout to start next ranging process. */
  dwt_setrxtimeout(0);

  /* Activate reception immediately. */
  dwt_rxenable(DWT_START_RX_IMMEDIATE);

  /* Poll for reception of a frame or error/timeout. See NOTE 8 below. */
  while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
  { };

  if (status_reg & SYS_STATUS_RXFCG) {
    uint32 frame_len;
    /* Clear good RX frame event in the DW1000 status register. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);

    /* A frame has been received, read it into the local buffer. */
    frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
    if(frame_len <= FRAME_MAX_LENGTH) {
      dwt_readrxdata(_frameRX.uData, frame_len, 0);
    }
    /* Check that the frame is a poll sent by Tag. */
    if(_frameRX.Frame.stx == DEFAULT_STX && _frameRX.Frame.fType == poll_msg) {
      uint32 resp_tx_time;
      /* Retrieve poll reception timestamp. */
      poll_rx_ts = get_rx_timestamp_u64();

      /* Set send time for response. See NOTE 9 below. */
      resp_tx_time = (poll_rx_ts + (POLL_RX_TO_RESP_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
      dwt_setdelayedtrxtime(resp_tx_time);

      /* Set expected delay and timeout for final message reception. See NOTE 4 and 5 below. */
      dwt_setrxaftertxdelay(RESP_TX_TO_FINAL_RX_DLY_UUS);
      dwt_setrxtimeout(FINAL_RX_TIMEOUT_UUS);
      
      /* Write and send the response message. See NOTE 10 below.*/
      _frameTX.Frame.SepNbr = frame_seq_nb;
      _frameTX.Frame.fType = resp_msg;
      _frameTX.Frame.Msg.RespMsg.DstAddr = _frameRX.Frame.Msg.PollMsg.SrcAddr;
      _frameTX.Frame.Msg.RespMsg.SrcAddr = AncId;
      _frameTX.Frame.Msg.RespMsg.Distance.fData = _report_dist;
      dwt_writetxdata(RESP_MSG_LENGTH, _frameTX.uData, 0); /* Zero offset in TX buffer. */
      dwt_writetxfctrl(RESP_MSG_LENGTH, 0, 1); /* Zero offset in TX buffer, ranging. */
      if(dwt_starttx(DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED) != DWT_SUCCESS) {
        return;
      }
      
      /* Poll for reception of expected "final" frame or error/timeout. See NOTE 8 below. */
      while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
      { };
      
      /* Increment frame sequence number after transmission of the response message (modulo 256). */
      frame_seq_nb++;

      if (status_reg & SYS_STATUS_RXFCG) {
        /* Clear good RX frame event and TX frame sent in the DW1000 status register. */
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);

        /* A frame has been received, read it into the local buffer. */
        frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
        if(frame_len <= FRAME_MAX_LENGTH) {
          dwt_readrxdata(_frameRX.uData, frame_len, 0);
        }
        /* Check that the frame is a final message sent by Tag. */
        if(_frameRX.Frame.stx == DEFAULT_STX && _frameRX.Frame.fType == final_msg) {
          double Ra, Rb, Da, Db;

          /* Retrieve response transmission and final reception timestamps. */
          resp_tx_ts = get_tx_timestamp_u64();
          final_rx_ts = get_rx_timestamp_u64();
          
          Ra = (double)_frameRX.Frame.Msg.FinalMsg.tRspRX2PolTX;
          Rb = (double)((uint32_t)final_rx_ts - (uint32_t)resp_tx_ts);
          Da = (double)_frameRX.Frame.Msg.FinalMsg.tFinTX2RspRX;
          Db = (double)((uint32_t)resp_tx_ts - (uint32_t)poll_rx_ts);
          _report_dist = (int64_t)((Ra * Rb - Da * Db) / (Ra + Rb + Da + Db)) * DWT_TIME_UNITS * SPEED_OF_LIGHT;
        }
      } else {
        /* Clear RX error/timeout events in the DW1000 status register. */
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

        /* Reset RX to properly reinitialise LDE operation. */
        dwt_rxreset();
      }
    }
  } else {
    /* Clear RX error/timeout events in the DW1000 status register. */
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

    /* Reset RX to properly reinitialise LDE operation. */
    dwt_rxreset();
  }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn get_tx_timestamp_u64()
 *
 * @brief Get the TX time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
static uint64 get_tx_timestamp_u64(void)
{
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readtxtimestamp(ts_tab);
    for (i = 4; i >= 0; i--)
    {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn get_rx_timestamp_u64()
 *
 * @brief Get the RX time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
static uint64 get_rx_timestamp_u64(void)
{
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readrxtimestamp(ts_tab);
    for (i = 4; i >= 0; i--)
    {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/**
  * @brief task entry function.
  * @param[in] pvParameter   Pointer that will be used as the parameter for the task.
  */
void anc_rtls_task_function(void * pvParameter)
{
  UNUSED_PARAMETER(pvParameter);
  dwt_setleds(DWT_LEDS_ENABLE);
  rtls_init();

//  dwt_setrxtimeout(0);

  /* Loop forever initiating ranging exchanges. */
  while (true)
  {
    anc_rtls_run();
    /* Delay a task for a given number of ticks */
//    vTaskDelay(50);
    /* Tasks must be implemented to never return... */
  }
}

/*****************************************************************************************************************************************************
 * NOTES:
 *
 * 1. The sum of the values is the TX to RX antenna delay, experimentally determined by a calibration process. Here we use a hard coded typical value
 *    but, in a real application, each device should have its own antenna delay properly calibrated to get the best possible precision when performing
 *    range measurements.
 * 2. The messages here are similar to those used in the DecaRanging ARM application (shipped with EVK1000 kit). They comply with the IEEE
 *    802.15.4 standard MAC data frame encoding and they are following the ISO/IEC:24730-62:2013 standard. The messages used are:
 *     - a poll message sent by the initiator to trigger the ranging exchange.
 *     - a response message sent by the responder allowing the initiator to go on with the process
 *     - a final message sent by the initiator to complete the exchange and provide all information needed by the responder to compute the
 *       time-of-flight (distance) estimate.
 *    The first 10 bytes of those frame are common and are composed of the following fields:
 *     - byte 0/1: frame control (0x8841 to indicate a data frame using 16-bit addressing).
 *     - byte 2: sequence number, incremented for each new frame.
 *     - byte 3/4: PAN ID (0xDECA).
 *     - byte 5/6: destination address, see NOTE 3 below.
 *     - byte 7/8: source address, see NOTE 3 below.
 *     - byte 9: function code (specific values to indicate which message it is in the ranging process).
 *    The remaining bytes are specific to each message as follows:
 *    Poll message:
 *     - no more data
 *    Response message:
 *     - byte 10: activity code (0x02 to tell the initiator to go on with the ranging exchange).
 *     - byte 11/12: activity parameter, not used for activity code 0x02.
 *    Final message:
 *     - byte 10 -> 13: poll message transmission timestamp.
 *     - byte 14 -> 17: response message reception timestamp.
 *     - byte 18 -> 21: final message transmission timestamp.
 *    All messages end with a 2-byte checksum automatically set by DW1000.
 * 3. Source and destination addresses are hard coded constants in this example to keep it simple but for a real product every device should have a
 *    unique ID. Here, 16-bit addressing is used to keep the messages as short as possible but, in an actual application, this should be done only
 *    after an exchange of specific messages used to define those short addresses for each device participating to the ranging exchange.
 * 4. Delays between frames have been chosen here to ensure proper synchronisation of transmission and reception of the frames between the initiator
 *    and the responder and to ensure a correct accuracy of the computed distance. The user is referred to DecaRanging ARM Source Code Guide for more
 *    details about the timings involved in the ranging process.
 * 5. This timeout is for complete reception of a frame, i.e. timeout duration must take into account the length of the expected frame. Here the value
 *    is arbitrary but chosen large enough to make sure that there is enough time to receive the complete final frame sent by the responder at the
 *    110k data rate used (around 3.5 ms).
 * 6. The preamble timeout allows the receiver to stop listening in situations where preamble is not starting (which might be because the responder is
 *    out of range or did not receive the message to respond to). This saves the power waste of listening for a message that is not coming. We
 *    recommend a minimum preamble timeout of 5 PACs for short range applications and a larger value (e.g. in the range of 50% to 80% of the preamble
 *    length) for more challenging longer range, NLOS or noisy environments.
 * 7. In a real application, for optimum performance within regulatory limits, it may be necessary to set TX pulse bandwidth and TX power, (using
 *    the dwt_configuretxrf API call) to per device calibrated values saved in the target system or the DW1000 OTP memory.
 * 8. We use polled mode of operation here to keep the example as simple as possible but all status events can be used to generate interrupts. Please
 *    refer to DW1000 User Manual for more details on "interrupts". It is also to be noted that STATUS register is 5 bytes long but, as the event we
 *    use are all in the first bytes of the register, we can use the simple dwt_read32bitreg() API call to access it instead of reading the whole 5
 *    bytes.
 * 9. Timestamps and delayed transmission time are both expressed in device time units so we just have to add the desired response delay to poll RX
 *    timestamp to get response transmission time. The delayed transmission time resolution is 512 device time units which means that the lower 9 bits
 *    of the obtained value must be zeroed. This also allows to encode the 40-bit value in a 32-bit words by shifting the all-zero lower 8 bits.
 * 10. dwt_writetxdata() takes the full size of the message as a parameter but only copies (size - 2) bytes as the check-sum at the end of the frame is
 *     automatically appended by the DW1000. This means that our variable could be two bytes shorter without losing any data (but the sizeof would not
 *     work anymore then as we would still have to indicate the full length of the frame to dwt_writetxdata()).
 * 11. When running this example on the EVB1000 platform with the POLL_RX_TO_RESP_TX_DLY response delay provided, the dwt_starttx() is always
 *     successful. However, in cases where the delay is too short (or something else interrupts the code flow), then the dwt_starttx() might be issued
 *     too late for the configured start time. The code below provides an example of how to handle this condition: In this case it abandons the
 *     ranging exchange and simply goes back to awaiting another poll message. If this error handling code was not here, a late dwt_starttx() would
 *     result in the code flow getting stuck waiting subsequent RX event that will will never come. The companion "initiator" example (ex_05a) should
 *     timeout from awaiting the "response" and proceed to send another poll in due course to initiate another ranging exchange.
 * 12. The high order byte of each 40-bit time-stamps is discarded here. This is acceptable as, on each device, those time-stamps are not separated by
 *     more than 2**32 device time units (which is around 67 ms) which means that the calculation of the round-trip delays can be handled by a 32-bit
 *     subtraction.
 * 13. The user is referred to DecaRanging ARM application (distributed with EVK1000 product) for additional practical example of usage, and to the
 *     DW1000 API Guide for more details on the DW1000 driver functions.
 ****************************************************************************************************************************************************/

#endif /* !(INSTANCE_MODE_TAG) */
