#include "motor.h"

extern TIM_HandleTypeDef htim3;

void Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 左电机方向引脚: PA5(IN1), PA6(IN2) */
    GPIO_InitStruct.Pin   = MOTOR_L_IN1_PIN | MOTOR_L_IN2_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(MOTOR_L_IN1_PORT, &GPIO_InitStruct);

    /* 右电机方向引脚: PA7(IN3), PB0(IN4) */
    GPIO_InitStruct.Pin = MOTOR_R_IN3_PIN;
    HAL_GPIO_Init(MOTOR_R_IN3_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MOTOR_R_IN4_PIN;
    HAL_GPIO_Init(MOTOR_R_IN4_PORT, &GPIO_InitStruct);

    Motor_Stop();
}

void Motor_PWM_Init(uint16_t freq)
{
    /* TIM3 已由 CubeMX 配置，此处仅启动 PWM 输出 */
    HAL_TIM_PWM_Start(&htim3, MOTOR_L_PWM_CH);
    HAL_TIM_PWM_Start(&htim3, MOTOR_R_PWM_CH);

    /* 设置 PWM 频率: freq 参数用于记录，实际频率由 CubeMX 预分频决定 */
    (void)freq;
}
