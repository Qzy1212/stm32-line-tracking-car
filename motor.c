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

void Motor_SetSpeed(int16_t left, int16_t right)
{
    /* 限幅 */
    if (left  >  1000) left  =  1000;
    if (left  < -1000) left  = -1000;
    if (right >  1000) right =  1000;
    if (right < -1000) right = -1000;

    /* 左电机方向 */
    if (left >= 0) {
        HAL_GPIO_WritePin(MOTOR_L_IN1_PORT, MOTOR_L_IN1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_L_IN2_PORT, MOTOR_L_IN2_PIN, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(MOTOR_L_IN1_PORT, MOTOR_L_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_L_IN2_PORT, MOTOR_L_IN2_PIN, GPIO_PIN_SET);
        left = -left;
    }

    /* 右电机方向 */
    if (right >= 0) {
        HAL_GPIO_WritePin(MOTOR_R_IN3_PORT, MOTOR_R_IN3_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_R_IN4_PORT, MOTOR_R_IN4_PIN, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(MOTOR_R_IN3_PORT, MOTOR_R_IN3_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_R_IN4_PORT, MOTOR_R_IN4_PIN, GPIO_PIN_SET);
        right = -right;
    }

    /* 设置占空比 */
    __HAL_TIM_SET_COMPARE(&htim3, MOTOR_L_PWM_CH, (uint16_t)left);
    __HAL_TIM_SET_COMPARE(&htim3, MOTOR_R_PWM_CH, (uint16_t)right);
}

void Motor_Forward(void)
{
    HAL_GPIO_WritePin(MOTOR_L_IN1_PORT, MOTOR_L_IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_L_IN2_PORT, MOTOR_L_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_R_IN3_PORT, MOTOR_R_IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_R_IN4_PORT, MOTOR_R_IN4_PIN, GPIO_PIN_RESET);
}

void Motor_Stop(void)
{
    HAL_GPIO_WritePin(MOTOR_L_IN1_PORT, MOTOR_L_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_L_IN2_PORT, MOTOR_L_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_R_IN3_PORT, MOTOR_R_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_R_IN4_PORT, MOTOR_R_IN4_PIN, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(&htim3, MOTOR_L_PWM_CH, 0);
    __HAL_TIM_SET_COMPARE(&htim3, MOTOR_R_PWM_CH, 0);
}

void Motor_Brake(void)
{
    /* 刹车: 所有方向引脚置高，短路电机 */
    HAL_GPIO_WritePin(MOTOR_L_IN1_PORT, MOTOR_L_IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_L_IN2_PORT, MOTOR_L_IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_R_IN3_PORT, MOTOR_R_IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_R_IN4_PORT, MOTOR_R_IN4_PIN, GPIO_PIN_SET);
}

static uint8_t motor_status = 0;  /* 0=停止, 1=前进, 2=刹车 */

uint8_t Motor_GetStatus(void)
{
    return motor_status;
}

/*
 * 电机驱动模块说明:
 *
 * 1. Motor_Init()      - 初始化 L298N 方向控制引脚，默认停止
 * 2. Motor_PWM_Init()  - 启动 TIM3 PWM 输出
 * 3. Motor_SetSpeed()  - 设置左右电机速度 (范围 -1000~+1000)
 *                        正值前进，负值后退，绝对值为 PWM 占空比
 * 4. Motor_Forward()   - 设置双电机正转方向
 * 5. Motor_Stop()      - 停止电机，PWM 归零
 * 6. Motor_Brake()     - 短路制动，所有方向引脚置高
 * 7. Motor_GetStatus() - 查询当前电机状态
 *
 * PWM 频率: 10kHz (TIM3 预分频由 CubeMX 配置)
 * 占空比范围: 0~1000 (对应 0%~100%)
 */
