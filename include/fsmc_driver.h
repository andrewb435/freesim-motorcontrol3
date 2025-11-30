#ifndef __FSMC_DRIVER_H__
#define __FSMC_DRIVER_H__

#include <Arduino.h>
#include "hw_blackpill.h"

namespace FSMC3
{
	class PWMDriver
	{
	private:
		bool isEnabled;
		uint16_t pinEnable;
		uint16_t pinPWMA;
		uint16_t pinPWMB;
		uint8_t channelA;
		uint8_t channelB;
		uint16_t pwmFrequency;
		uint16_t pwmResolution;
		HardwareTimer* hwtimer;
		int16_t mapFloatToTick(double target, double fromLow, double fromHigh, int16_t toLow, int16_t toHigh);

	public:
		/// @brief Constructor for PWMDriver compatible with a IBT-2 or other 1en+2pwm DC H-bridge driver
		/// @param hwtimer_in Reference to the hardware timer in use by this driver for PWM output
		/// @param channelA_in Channel number for A (e.g. TIM2 CH3 = 3)
		/// @param channelB_in Channel number for B (e.g. TIM2 CH4 = 4)
		/// @param enablePin_in Pin for driver enable (e.g. PB13)
		PWMDriver(FSMC3Config::Driver *driver_in, FSMC3Config::SystemHW *system_in);
		/// @brief Drive the h-bridge at the target value
		/// @param target -1.0 to 1.0
		void init();
		void drive(float target);
		void setEnable(bool enable_in);
		uint32_t getTarget();
	};
}
#endif // __FSMC_DRIVER_H__