#include "servo.h"
#include "esp_log.h"






//-------------------Private Function------------------

/**
 * @brief Re-scales a number form one range to another
 * @return Re-scaled number
 */
static uint32_t map(uint8_t angle)
{
    return (((angle - SERVO_MIN_DEGREES) * (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH)) / (SERVO_MAX_DEGREES - SERVO_MIN_DEGREES) + SERVO_MIN_PULSEWIDTH);
}


/**
 * @brief Initialises the PWM Timer
 * @return Timer Handle
 */
static mcpwm_timer_handle_t PWM_timer_init(void)
{
    // PWM TIMER SETUP
    mcpwm_timer_handle_t timer_handle = NULL;
    mcpwm_timer_config_t timer_conf = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMER_RESOLUTION_HZ,
        .period_ticks = SREVO_TIMER_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP
    };
    ESP_LOGI(TAG, "Creating Timer");
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_conf, &timer_handle));
    return timer_handle;
}


/**
 * @brief Initialises the PWM Operator
 * @return Operator Handle
 */
static mcpwm_oper_handle_t PWM_operator_init(void)
{
    //PWM OPERATOR SETUP
    mcpwm_oper_handle_t operator_handle = NULL;
    mcpwm_operator_config_t operator_conf = {
        .group_id = 0   //MUST BE SAME GROUP AS TIMER
    };

    ESP_LOGI(TAG, "Creating Operator");
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_conf, &operator_handle));

    return operator_handle;
}


/**
 * @brief Initialises the PWM Comparator
 * @param operator_handle PWM Operator Handle
 * @return Comparator Handle
 */
static mcpwm_cmpr_handle_t PWM_comparator_init(mcpwm_oper_handle_t operator_handle)
{
    //PWM COMPARATOR SETUP
    mcpwm_cmpr_handle_t comparator_handle = NULL;
    mcpwm_comparator_config_t comparator_conf = {
        .flags.update_cmp_on_tez = true
    };
    
    ESP_LOGI(TAG, "Creating Comparator");
    ESP_ERROR_CHECK(mcpwm_new_comparator(operator_handle, &comparator_conf, &comparator_handle));
    return comparator_handle;
}


/**
 * @brief Initialises the PWM Generator
 * @param operator_handle PWM Operator Handle
 * @return Generator Handle
 */
mcpwm_gen_handle_t PWM_generator_init(mcpwm_oper_handle_t operator_handle, uint8_t gpio_pin)
{
    //PWM GENERATOR SETUP
    mcpwm_gen_handle_t generator_handle = NULL;
    mcpwm_generator_config_t generator_conf = {
        .gen_gpio_num = gpio_pin
    };

    ESP_LOGI(TAG, "Creating Generator");
    ESP_ERROR_CHECK(mcpwm_new_generator(operator_handle, &generator_conf, &generator_handle));
    return generator_handle;
}



//-------------------Public Function------------------

esp_err_t set_servo_angle(servo_t *servo, uint8_t angle)
{
    if(angle > SERVO_MAX_DEGREES){
        angle = SERVO_MAX_DEGREES;
    }

    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(servo->comparator, map(angle)));    //move to angle specified
    return ESP_OK;
}


esp_err_t servo_init(servo_t *servo, uint8_t gpio_pin)
{
    //PWM Timer Setup
    servo->timer = PWM_timer_init();
    servo->operator = PWM_operator_init();

    ESP_LOGI(TAG, "Linking Operator and Timer");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(servo->operator, servo->timer));
    
    servo->comparator = PWM_comparator_init(servo->operator);
    servo->generator = PWM_generator_init(servo->operator, gpio_pin);

    // Set initial compare value
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(servo->comparator, map(0)));

    // Set high when timer is empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(servo->generator, MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));

    // Set low on comparator threshold 
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(servo->generator, MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, servo->comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "PWM Initialisation Complete");
    ESP_LOGI(TAG, "Enabling Timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(servo->timer));  //Enables PWM TIMER

    ESP_LOGI(TAG, "Starting Timer");
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(servo->timer, MCPWM_TIMER_START_NO_STOP));   //Starts PWM TIMER

    return ESP_OK;
}

uint32_t servo_get_angle(servo_t *servo){
    return servo->current_angle;
}