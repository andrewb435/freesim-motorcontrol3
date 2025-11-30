#include "fsmc_driver.h"

int16_t FSMC3::PWMDriver::mapFloatToTick(double target, double fromLow, double fromHigh, int16_t toLow, int16_t toHigh)
{
	if (target < fromLow) {
		return toLow;
	}
	if (target > fromHigh) {
		return toHigh;
	}
	return static_cast<int>((target - fromLow) * (static_cast<double>(toHigh) - static_cast<double>(toLow)) / (fromHigh - fromLow) + static_cast<double>(toLow));
}

FSMC3::PWMDriver::PWMDriver(FSMC3Config::Driver *driver_in, FSMC3Config::SystemHW *system_in)
{
	hwtimer = driver_in->hwtimer;
	pinEnable = driver_in->enablePin;
	pinPWMA = driver_in->pwmChAPin;
	pinPWMB = driver_in->pwmChBPin;
	channelA = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pinPWMA), PinMap_PWM));
	channelB = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pinPWMB), PinMap_PWM));
	pwmFrequency = system_in->pwmFrequency;
	pwmResolution = system_in->pwmResolution;
	pinMode(pinEnable, OUTPUT);
	setEnable(false);
}

void FSMC3::PWMDriver::init()
{
	hwtimer->setPWM(channelA, pinPWMA, pwmFrequency, 0);
	hwtimer->setPWM(channelB, pinPWMB, pwmFrequency, 0);
	hwtimer->resume();
	setEnable(isEnabled);
}

void FSMC3::PWMDriver::drive(float target)
{
	if (isEnabled) {
		int16_t pwm_target = mapFloatToTick(target, -1.0, 1.0, pwmResolution * -1 , pwmResolution);
		if (pwm_target < 0) {
			hwtimer->setCaptureCompare(channelA, 0);
			hwtimer->setCaptureCompare(channelB, abs(pwm_target));
		} else if (pwm_target > 0) {
			hwtimer->setCaptureCompare(channelB, 0);
			hwtimer->setCaptureCompare(channelA, abs(pwm_target));
		} else {
			hwtimer->setCaptureCompare(channelA, 0);
			hwtimer->setCaptureCompare(channelB, 0);
		}
	}
}

void FSMC3::PWMDriver::setEnable(bool enable_in)
{
	isEnabled = enable_in;
	if (!enable_in) {
		hwtimer->setCaptureCompare(channelA, 0);
		hwtimer->setCaptureCompare(channelB, 0);
		digitalWrite(pinEnable, LOW);
	} else {
		digitalWrite(pinEnable, HIGH);
	}
}

uint32_t FSMC3::PWMDriver::getTarget()
{
	if (hwtimer->getCaptureCompare(channelA) > 0){
		return hwtimer->getCaptureCompare(channelA);
	} else if (hwtimer->getCaptureCompare(channelB)) {
		return hwtimer->getCaptureCompare(channelB);
	} else {
		return 0;
	}
}
