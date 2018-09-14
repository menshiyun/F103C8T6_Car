
#include "stm32f1xx_hal.h"
#include "CarControl.h"
#include "CarWheel.h"

static const struct _WheelRoll *WheelRoller = NULL;
static const struct _Wheel     *LaWheel     = NULL;
static const struct _Wheel     *LbWheel     = NULL;
static const struct _Wheel     *RaWheel     = NULL;
static const struct _Wheel     *RbWheel     = NULL;

static void CarForward(void)
{
	WheelRoller->Forward(LaWheel);
	WheelRoller->Forward(LbWheel);
	WheelRoller->Forward(RaWheel);
	WheelRoller->Forward(RbWheel);
}

static void CarBackward(void)
{
	WheelRoller->Backward(LaWheel);
	WheelRoller->Backward(LbWheel);
	WheelRoller->Backward(RaWheel);
	WheelRoller->Backward(RbWheel);
}

static void CarLeft(void)
{
	WheelRoller->Backward(LaWheel);
	WheelRoller->Backward(LbWheel);
	WheelRoller->Forward(RaWheel);
	WheelRoller->Forward(RbWheel);
}

static void CarRight(void)
{
	WheelRoller->Forward(LaWheel);
	WheelRoller->Forward(LbWheel);
	WheelRoller->Backward(RaWheel);
	WheelRoller->Backward(RbWheel);
}

static void CarStop(void)
{
	WheelRoller->Stop(LaWheel);
	WheelRoller->Stop(LbWheel);
	WheelRoller->Stop(RaWheel);
	WheelRoller->Stop(RbWheel);
}

static void CarForLeft(void)
{
	WheelRoller->Stop(LaWheel);
	WheelRoller->Stop(LbWheel);
	WheelRoller->Forward(RaWheel);
	WheelRoller->Forward(RbWheel);
}

static void CarBackLeft(void)
{
	WheelRoller->Stop(LaWheel);
	WheelRoller->Stop(LbWheel);
	WheelRoller->Backward(RaWheel);
	WheelRoller->Backward(RbWheel);
}

static void CarForRight(void)
{
	WheelRoller->Forward(LaWheel);
	WheelRoller->Forward(LbWheel);
	WheelRoller->Stop(RaWheel);
	WheelRoller->Stop(RbWheel);
}

static void CarBackRight(void)
{
	WheelRoller->Backward(LaWheel);
	WheelRoller->Backward(LbWheel);
	WheelRoller->Stop(RaWheel);
	WheelRoller->Stop(RbWheel);
}

static const struct _CarControl CarController = {
	.Forward   = CarForward,
	.Backward  = CarBackward,
	.Left      = CarLeft,
	.Right     = CarRight,
	.Stop      = CarStop,
	.ForLeft   = CarForLeft,
	.BackLeft  = CarBackLeft,
	.ForRight  = CarForRight,
	.BackRight = CarBackRight,
};

const struct _CarControl *GetCarController(void)
{
	WheelRoller = GetWheelRoller();
	LaWheel     = GetWheel(0);
	LbWheel     = GetWheel(1);
	RaWheel     = GetWheel(2);
	RbWheel     = GetWheel(3);
	return &CarController;
}
