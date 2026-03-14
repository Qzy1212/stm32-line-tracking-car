# STM32 红外循迹小车

基于 STM32F103C8T6 的五路红外循迹小车项目。

## 项目成员
- 墙（230100101）：传感器读取与 PID 控制
- 王（230100102）：电机驱动与 PWM 调速

## 硬件平台
- 主控：STM32F103C8T6
- 传感器：5 x TCRT5000 红外对管 (PA0~PA4)
- 驱动：L298N 双 H 桥 (PA5~PA7, PB0)
- 电机：2 x TT 直流减速电机
- PWM：TIM3 CH1/CH2, 10kHz

## 软件架构
- sensor.c/h - 五路红外传感器读取与偏差计算
- pid.c/h    - 位置式 PID 控制算法 (Kp=25, Ki=0, Kd=12)
- motor.c/h  - L298N 电机驱动与 PWM 调速
- main.c     - 主程序入口

## 引脚分配
| 模块   | 引脚       | 功能           |
|--------|------------|----------------|
| S1~S5  | PA0~PA4    | 红外传感器输入 |
| IN1    | PA5        | 左电机方向 A   |
| IN2    | PA6        | 左电机方向 B   |
| IN3    | PA7        | 右电机方向 A   |
| IN4    | PB0        | 右电机方向 B   |
| PWM-L  | TIM3_CH1   | 左电机 PWM     |
| PWM-R  | TIM3_CH2   | 右电机 PWM     |

## 开发环境
- Keil MDK 5.36 + STM32CubeMX 6.8.0
- HAL 库 1.8.4
- Git + GitHub Desktop

## 编译与烧录
1. STM32CubeMX 配置引脚并生成工程
2. 将 sensor.c / pid.c / motor.c 加入工程
3. Keil 编译生成 .hex
4. ST-Link 烧录
