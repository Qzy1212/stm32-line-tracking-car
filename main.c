#include "main.h"
#include "sensor.h"
#include "pid.h"
#include "motor.h"

Sensor_Data_t    sensor_data;
PID_Controller_t pid;

void SystemClock_Config(void) { /* 由 STM32CubeMX 生成 */ }
void MX_GPIO_Init(void)       { /* 由 STM32CubeMX 生成 */ }

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    Sensor_Init();
    PID_Init(&pid, 25.0f, 0.0f, 12.0f);
    Motor_Init();
    Motor_PWM_Init(1000);

    while (1)
    {
        Sensor_Read(&sensor_data);
        PID_Calculate(&pid, (float)sensor_data.error);
        Motor_SetSpeed(
            (int16_t)(MOTOR_BASE_SPEED - pid.output),
            (int16_t)(MOTOR_BASE_SPEED + pid.output)
        );
    }
}
