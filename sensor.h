#ifndef __SENSOR_H
#define __SENSOR_H

#include "main.h"

#define SENSOR_COUNT 5

/* TCRT5000 引脚: PA0 ~ PA4 */
typedef struct {
    uint8_t value;      /* 5位二进制, bit=1 表示检测到黑线 */
    int8_t  error;      /* 偏差: -2(最左) ~ +2(最右) */
    uint8_t raw[SENSOR_COUNT];
} Sensor_Data_t;

void Sensor_Init(void);
void Sensor_Read(Sensor_Data_t *data);
int8_t Sensor_GetError(uint8_t value);
uint8_t Sensor_Filter(Sensor_Data_t *data, uint8_t times);

#endif /* __SENSOR_H */
