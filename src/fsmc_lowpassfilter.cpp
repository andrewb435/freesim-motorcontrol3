/*
FSMC Low Pass Filter
Based on the SimpleFOC low pass filter by the SimpleFOC Arduino team
	https://github.com/simplefoc/Arduino-FOC

MIT License
*/

#include "fsmc_lowpassfilter.h"

FSMC3::LowPassFilter::LowPassFilter(unsigned long *diffTime, double *wOutput)
{
	lpfCutoffFreq = 100;
	isEnabled = true;
}

void FSMC3::LowPassFilter::doFilter()
{
	alpha = lpfTimeConstant / (lpfTimeConstant + *diffTime);
	lpfOutput = alpha * lastOutput + (1.0f - alpha) * *wOutput;
	lastOutput = lpfOutput;
	wOutput = &lpfOutput;
}

void FSMC3::LowPassFilter::setEnable(bool lpfEnabled_in)
{
	isEnabled = lpfEnabled_in;
}

void FSMC3::LowPassFilter::setCutoffFreq(int16_t lpfCutoffFreq_in)
{
	lpfCutoffFreq = lpfCutoffFreq_in;
	lpfCalcTimeConstant();
}

void FSMC3::LowPassFilter::lpfCalcTimeConstant()
{
	lpfTimeConstant = 1.0 / lpfCutoffFreq;
}

bool FSMC3::LowPassFilter::getEnabled()
{
	return isEnabled;
}
