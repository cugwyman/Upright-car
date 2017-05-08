#ifndef __UARTS_H
#define __UARTS_H

#define UARTS

#include "uart.h"
#include "Root.h"

void UartInit(void);
void vcan_sendimg(uint8_t *imgaddr, uint32_t imgsize);
void uart_putbuff (uint8_t *buff, uint32_t len);
void ImgTrans(void);
#include "TrackIdentify.h"

#endif
