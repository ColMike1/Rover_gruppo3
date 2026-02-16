/**
 * @file comm_uart.h
 * @brief Driver di basso livello per l'interfaccia UART di comunicazione.
 */

#ifndef INC_COMM_COMM_UART_H_
#define INC_COMM_COMM_UART_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Inizializzazione precoce della periferica (es. configurazione GPIO e clock).
 */
void CommUart_EarlyInit(void);

/**
 * @brief Inizializzazione completa della periferica UART e degli interrupt/DMA.
 */
void CommUart_Init(void);

/**
 * @brief Invia un buffer di dati grezzi sulla UART.
 * @param buf Puntatore al buffer di dati.
 * @param len Lunghezza dei dati in byte.
 */
void CommUart_Send(const uint8_t *buf, uint16_t len);

/**
 * @brief Tenta di prelevare un singolo byte dal buffer di ricezione.
 * @param[out] b Puntatore dove memorizzare il byte ricevuto.
 * @return true se un byte è stato prelevato correttamente, false se il buffer è vuoto.
 */
bool CommUart_GetByte(uint8_t *b);

#endif /* INC_COMM_COMM_UART_H_ */
