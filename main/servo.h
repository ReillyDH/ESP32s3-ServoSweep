#include "esp_err.h"
#include "driver/mcpwm_prelude.h"


#define TAG "Servo"
#define TASK_DELAY_MS 500   

#define SERVO_MIN_PULSEWIDTH 500        //0.5 ms
#define SERVO_MAX_PULSEWIDTH 2500       //2.5 ms
#define SERVO_MIN_DEGREES    0
#define SERVO_MAX_DEGREES    180


#define SERVO_TIMER_RESOLUTION_HZ   1000000          //1MHz
#define SREVO_TIMER_PERIOD          20000            //20000 ticks, 20ms





typedef struct{
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t operator;
    mcpwm_cmpr_handle_t comparator;
    mcpwm_gen_handle_t generator;
    uint32_t current_angle;
} servo_t;





/**
 * @brief Sets the Servo to the angle specified
 * @param angle Angle the Servo is to move to
 * @param handle PWM Comparator Handle
 * 
 */
esp_err_t set_servo_angle(servo_t *servo, uint8_t angle);

/**
 * @brief Initialises the PWM Comparator
 * @param 
 */
esp_err_t servo_init(servo_t *servo, uint8_t gpio_pin);

/**
 * @brief Get last commanded angle
 *
 * @param servo Pointer to servo object
 * @return uint32_t
 */
uint32_t servo_get_angle(servo_t *servo);

