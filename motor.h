#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

/* L298N 方向控制引脚 */
#define MOTOR_L_IN1_PIN   GPIO_PIN_5
#define MOTOR_L_IN1_PORT  GPIOA
#define MOTOR_L_IN2_PIN   GPIO_PIN_6
#define MOTOR_L_IN2_PORT  GPIOA
#define MOTOR_R_IN3_PIN   GPIO_PIN_7
#define MOTOR_R_IN3_PORT  GPIOA
#define MOTOR_R_IN4_PIN   GPIO_PIN_B0
#define MOTOR_R_IN4_PORT  GPIOB

/* PWM 定时器 */
#define MOTOR_PWM_TIM     TIM3
#define MOTOR_L_PWM_CH    TIM_CHANNEL_1
#define MOTOR_R_PWM_CH    TIM_CHANNEL_2

void Motor_Init(void);
void Motor_PWM_Init(uint16_t freq);
void Motor_SetSpeed(int16_t left, int16_t right);
void Motor_Forward(void);
void Motor_Stop(void);
void Motor_Brake(void);
uint8_t Motor_GetStatus(void);

#endif /* __MOTOR_H */

/* 速度参数宏 */
#define MOTOR_SPEED_MAX   1000
#define MOTOR_SPEED_MIN   0
#define MOTOR_BASE_SPEED  600
#define MOTOR_TURN_BIAS   400
