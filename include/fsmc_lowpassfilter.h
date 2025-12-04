#ifndef __FSMC_LOWPASSFILTER_H__
#define __FSMC_LOWPASSFILTER_H__
/*
FSMC Low Pass Filter
Based on the SimpleFOC low pass filter by the SimpleFOC Arduino team
	https://github.com/simplefoc/Arduino-FOC

MIT License
*/

#include <cstdint>

namespace FSMC3
{
	class LowPassFilter
	{
	private:
		double *diffTime;
		double *wOutput;
		bool isEnabled;
		double lastOutput;
		double alpha;
		double lpfOutput;
		double lpfTimeConstant;
		int16_t lpfCutoffFreq;

	public:
		LowPassFilter(unsigned long *diffTime, double *wOutput);
		void doFilter();
		void setEnable(bool lpfEnabled_in);
		/// @brief Sets cutoff frequency in Hz (default 100Hz)
		void setCutoffFreq(int16_t lpfCutoffFreq_in);
		void lpfCalcTimeConstant();
		bool getEnabled();
	};
}
#endif