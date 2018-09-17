
#ifndef _CarControl_H_
#define _CarControl_H_

typedef void (*ControlFunc)(void);

struct _CarControl {
	ControlFunc Forward;
	ControlFunc Backward;
	ControlFunc Left;
	ControlFunc Right;
	ControlFunc Stop;
	ControlFunc ForLeft;
	ControlFunc BackLeft;
	ControlFunc ForRight;
	ControlFunc BackRight;
};

const struct _CarControl *GetCarController(void);

#endif
