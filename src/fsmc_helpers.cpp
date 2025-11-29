#include "fsmc_helpers.h"

double FSMC3::Helpers::mapInt16ToDouble(int16_t target, int16_t fromLow, int16_t fromHigh, double toLow, double toHigh)
{
	if (target < fromLow) {
		return toLow;
	}
	if (target > fromHigh) {
		return toHigh;
	}
	return static_cast<double>((target - static_cast<double>(fromLow)) * (toHigh - toLow) / (static_cast<double>(fromHigh) - static_cast<double>(fromLow)) + toLow);;
}

int16_t FSMC3::Helpers::mapDoubleToInt16(double target, double fromLow, double fromHigh, int16_t toLow, int16_t toHigh)
{
	if (target < fromLow) {
		return toLow;
	}
	if (target > fromHigh) {
		return toHigh;
	}
	return static_cast<int16_t>((target - static_cast<int16_t>(fromLow)) * (toHigh - toLow) / (static_cast<int16_t>(fromHigh) - static_cast<int16_t>(fromLow)) + toLow);;
}
