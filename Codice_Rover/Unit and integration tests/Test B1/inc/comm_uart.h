/*
 * comm_uart.h
 *
 *  Created on: Jan 11, 2026
 *      Author: Sterm
 */

#ifndef INC_COMM_COMM_UART_H_
#define INC_COMM_COMM_UART_H_


#include <stdint.h>
#include <stdbool.h>

void CommUart_EarlyInit(void);
void CommUart_Init(void);
/**
 * @brief Send raw buffer over UART
 * @param buf Data buffer
 * @param len Length in bytes
 */
void CommUart_Send(const uint8_t *buf, uint16_t len);


bool CommUart_GetByte(uint8_t *b);

#endif /* INC_COMM_COMM_UART_H_ */
