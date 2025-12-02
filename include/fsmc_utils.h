#ifndef __FSMC_HELPERS_H__
#define __FSMC_HELPERS_H__

#include <Arduino.h>

namespace FSMC3
{
	class Utils
	{
	public:
		static double mapInt16ToDouble(int16_t target, int16_t fromLow, int16_t fromHigh, double toLow, double toHigh);
		static int16_t mapDoubleToInt16(double target, double fromLow, double fromHigh, int16_t toLow, int16_t toHigh);
	};
}
#endif // __FSMC_HELPERS_H__