
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

struct _WheelRoll {
	void (*Forward)(const struct _Wheel *);
	void (*Backward)(const struct _Wheel *);
	void (*Stop)(const struct _Wheel *);
};

const struct _WheelRoll *GetWheelRoller(void);

#endif
