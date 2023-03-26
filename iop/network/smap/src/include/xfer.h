
#ifndef XFER_H
#define XFER_H

#include "main.h"

/**
 * Send data over the network
 * @param buf data buffer to be sent
 * @param size Size of the data in bytes
 * @return 0 on success, -1 on failure
 */
int smap_transmit(const void **datas, const u16 *data_sizes, int data_count, int blocking);

void xfer_init(void);
int HandleRxIntr(struct SmapDriverData *SmapDrivPrivData);
int HandleTxReqs(struct SmapDriverData *SmapDrivPrivData);

#endif
