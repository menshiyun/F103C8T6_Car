
#include "stm32f1xx_hal.h"
#include "CarWheel.h"

static const struct _Wheel Wheels[4] = {
	{{GPIOA, GPIO_PIN_1}, {GPIOA, GPIO_PIN_0}},
	{{GPIOA, GPIO_PIN_2}, {GPIOA, GPIO_PIN_3}},
	{{GPIOA, GPIO_PIN_5}, {GPIOA, GPIO_PIN_4}},
	{{GPIOA, GPIO_PIN_7}, {GPIOA, GPIO_PIN_6}},
};

const struct _Wheel *GetWheel(uint8_t i)
{
	return &Wheels[i];
}

static void WheelForward(const struct _Wheel *pWheel)
{
	HAL_GPIO_WritePin(pWheel->PinA.GPIO, pWheel->PinA.Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(pWheel->PinB.GPIO, pWheel->PinB.Pin, GPIO_PIN_RESET);
}

static void WheelBackward(const struct _Wheel *pWheel)
{
	HAL_GPIO_WritePin(pWheel->PinA.GPIO, pWheel->PinA.Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(pWheel->PinB.GPIO, pWheel->PinB.Pin, GPIO_PIN_SET);
}

static void WheelStop(const struct _Wheel *pWheel)
{
	HAL_GPIO_WritePin(pWheel->PinA.GPIO, pWheel->PinA.Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(pWheel->PinB.GPIO, pWheel->PinB.Pin, GPIO_PIN_RESET);
}

static const struct _WheelRoll WheelRoller = {
	.Forward  = WheelForward,
	.Backward = WheelBackward,
	.Stop     = WheelStop,
};

const struct _WheelRoll *GetWheelRoller(void)
{
	return &WheelRoller;
}
