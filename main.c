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
    Motor_Init();
    Motor_PWM_Init(1000);

    while (1)
    {
        Sensor_Read(&sensor_data);
        /* TODO: PID 计算与电机控制 */
    }
}
