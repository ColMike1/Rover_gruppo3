/**
 * @file comm_uart.h
 * @brief Driver UART con buffer circolare per comunicazione inter-board.
 */

#ifndef INC_COMM_COMM_UART_H_
#define INC_COMM_COMM_UART_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Inizializzazione anticipata del driver UART.
 */
void CommUart_EarlyInit(void);

/**
 * @brief Inizializza il modulo UART nel contesto del task chiamante.
 */
void CommUart_Init(void);

/**
 * @brief Invia un buffer su UART.
 * @param buf Buffer dati.
 * @param len Lunghezza in byte.
 */
void CommUart_Send(const uint8_t *buf, uint16_t len);

/**
 * @brief Estrae un byte dal buffer RX.
 * @param b Puntatore al byte di output.
 * @return 'true' se un byte e' disponibile, 'false' altrimenti.
 */
bool CommUart_GetByte(uint8_t *b);

#endif /* INC_COMM_COMM_UART_H_ */
