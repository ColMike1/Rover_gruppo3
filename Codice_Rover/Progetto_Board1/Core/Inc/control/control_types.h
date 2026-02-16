/**
 * @file control_types.h
 * @brief Definizioni delle strutture dati per l'input e l'output del controllo.
 */

#ifndef INC_CONTROL_CONTROL_TYPES_H_
#define INC_CONTROL_CONTROL_TYPES_H_

#include <stdint.h>

/**
 * @struct ControlInput_t
 * @brief Struttura contenente i riferimenti e i feedback per la legge di controllo.
 */
typedef struct
{
    float speed_ref_rpm;   /**< Velocità longitudinale di riferimento [RPM] */
    float steering_cmd;    /**< Comando di sterzata / raggio di curvatura */

    float rpm_sx_p;        /**< Feedback velocità ruota posteriore sinistra [RPM] */
    float rpm_dx_p;        /**< Feedback velocità ruota posteriore destra [RPM] */
    float rpm_sx_a;        /**< Feedback velocità ruota anteriore sinistra [RPM] */
    float rpm_dx_a;        /**< Feedback velocità ruota anteriore destra [RPM] */

} ControlInput_t;

/**
 * @struct ControlOutput_t
 * @brief Struttura contenente i comandi calcolati per ogni singolo motore.
 */
typedef struct
{
    float u_sx_a;          /**< Comando motore anteriore sinistro [RPM] */
    float u_dx_a;          /**< Comando motore anteriore destro [RPM] */
    float u_sx_p;          /**< Comando motore posteriore sinistro [RPM] */
    float u_dx_p;          /**< Comando motore posteriore destro [RPM] */

} ControlOutput_t;

#endif /* INC_CONTROL_CONTROL_TYPES_H_ */
