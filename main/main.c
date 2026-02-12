#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "servo.h"
#include "driver/gpio.h"


#define SERVO_GPIO GPIO_NUM_21




void app_main(void)
{
    ESP_LOGI(TAG, "APP START");

    servo_t servo;
    
    servo_init(&servo, SERVO_GPIO);

    ESP_LOGI(TAG, "STARTING MAIN LOOP");
    //MAIN LOOP
    //Moves the Servo from 0 - 180 Degrees
    for(int angle = SERVO_MIN_DEGREES; angle <= SERVO_MAX_DEGREES; angle += 10){ 
        set_servo_angle(&servo, angle);
        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MS));       //Delays to allow servo to move
    }
    //Moves Servo back from 180 - 0 Degrees 
    for(int angle = SERVO_MAX_DEGREES; angle >= SERVO_MIN_DEGREES; angle -= 10){    
        set_servo_angle(&servo, angle);
        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MS));       //Delays to allow servo to move
    }

}
