
#ifndef _CarWheel_H_
#define _CarWheel_H_

struct _Pin {
	GPIO_TypeDef *GPIO;
	uint16_t      Pin;
};

struct _Wheel {
	struct _Pin PinA;
	struct _Pin PinB;
};

const struct _Wheel *GetWheel(uint8_t);

typedef void (*WheelFunc)(const struct _Wheel *);

struct _WheelRoll {
	WheelFunc Forward;
	WheelFunc Backward;
	WheelFunc Stop;
};

const struct _WheelRoll *GetWheelRoller(void);

#endif
