#include "encoder_snapshot.h"
#include "supervisor_snapshot.h"
#include "rx_snapshot.h"
#include "board_health_snapshot.h"
#include "gpio.h"
#include <string.h>

EncoderSnapshot_t mock_enc_data;
SupervisorSnapshot_t mock_sup_data;
RxSnapshot_t         mock_rx;
BoardHealthSnapshot_t bh_mock;

void EncoderSnapshot_Read(EncoderSnapshot_t *enc) {
    if (enc){
    	memcpy(enc, &mock_enc_data, sizeof(EncoderSnapshot_t));
    }
}

void SupervisorSnapshot_Read(SupervisorSnapshot_t *sup) {
    if (sup){
    	memcpy(sup, &mock_sup_data, sizeof(SupervisorSnapshot_t));
    }
}

void RxSnapshot_Read(RxSnapshot_t *r) {
	memcpy(r, &mock_rx, sizeof(mock_rx));
}

void BoardHealthSnapshot_Read(BoardHealthSnapshot_t* s) {
    if (s) memcpy(s, &bh_mock, sizeof(BoardHealthSnapshot_t));
}

void EncoderSnapshot_Write(EncoderSnapshot_t *snap)
{
	memcpy(&mock_enc_data, snap, sizeof(EncoderSnapshot_t));
}

void SupervisorSnapshot_Write(SupervisorSnapshot_t* s) {
    if (s) memcpy(&mock_sup_data, s, sizeof(SupervisorSnapshot_t));
}
