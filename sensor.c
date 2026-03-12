#include "sensor.h"

static GPIO_TypeDef * const SENSOR_PORT[SENSOR_COUNT] = {
    GPIOA, GPIOA, GPIOA, GPIOA, GPIOA
};
static const uint16_t SENSOR_PIN[SENSOR_COUNT] = {
    GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4
};

void Sensor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    for (int i = 0; i < SENSOR_COUNT; i++) {
        GPIO_InitStruct.Pin = SENSOR_PIN[i];
        HAL_GPIO_Init(SENSOR_PORT[i], &GPIO_InitStruct);
    }
}

void Sensor_Read(Sensor_Data_t *data)
{
    data->value = 0;
    for (int i = 0; i < SENSOR_COUNT; i++) {
        data->raw[i] = (HAL_GPIO_ReadPin(SENSOR_PORT[i], SENSOR_PIN[i]) == GPIO_PIN_RESET) ? 1 : 0;
        data->value |= (data->raw[i] << i);
    }
    data->error = Sensor_GetError(data->value);
}

int8_t Sensor_GetError(uint8_t value)
{
    switch (value) {
        case 0b00100: return  0;
        case 0b01100: return -1;
        case 0b00010: return  1;
        case 0b11000: return -2;
        case 0b00011: return  2;
        case 0b01000: return -1;
        case 0b00001: return  2;
        case 0b11100: return -2;
        case 0b00111: return  2;
        default:      return  0;
    }
}

uint8_t Sensor_Filter(Sensor_Data_t *data, uint8_t times)
{
    uint8_t count[SENSOR_COUNT] = {0};
    Sensor_Data_t tmp;
    for (uint8_t t = 0; t < times; t++) {
        Sensor_Read(&tmp);
        for (int i = 0; i < SENSOR_COUNT; i++) {
            if (tmp.raw[i]) count[i]++;
        }
        HAL_Delay(1);
    }
    data->value = 0;
    uint8_t threshold = times / 2;
    for (int i = 0; i < SENSOR_COUNT; i++) {
        data->raw[i] = (count[i] > threshold) ? 1 : 0;
        data->value |= (data->raw[i] << i);
    }
    data->error = Sensor_GetError(data->value);
    return data->value;
}
