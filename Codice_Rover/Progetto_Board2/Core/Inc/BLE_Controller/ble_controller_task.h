/**
 * @file ble_controller_task.h
 * @brief Task di acquisizione e normalizzazione dati del controller BLE.
 */

#ifndef INC_BLE_CONTROLLER_BLE_CONTROLLER_TASK_H_
#define INC_BLE_CONTROLLER_BLE_CONTROLLER_TASK_H_

/**
 * @brief Inizializza il task BLE.
 */
void BleController_TaskInit(void);

/**
 * @brief Esegue uno step del task BLE.
 */
void BleController_TaskStep(void);

#endif /* INC_BLE_CONTROLLER_BLE_CONTROLLER_TASK_H_ */
