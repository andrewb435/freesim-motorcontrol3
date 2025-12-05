#ifndef __FSMC_PID_H__
#define __FSMC_PID_H__
/*
FSMC PID Library
Based on Arduino PID_v1 by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
	https://github.com/br3ttb/Arduino-PID-Library/
MIT License
*/

#include "fsmc_lowpassfilter.h"

namespace FSMC3
{
	class PID
	{
	private:
		// Raw tuning values
		double rawKp, rawKi, rawKd;

		// Internal tuning values (scaled to intervalTime)
		double kp; // (P)roportional Tuning Parameter
		double ki; // (I)ntegral Tuning Parameter
		double kd; // (D)erivative Tuning Parameter

		// Output limiter
		double outMin, outMax;

		// Pointers to the user input/output/setpoint parameters provided during construction
		// Intended to reduce the calls into the PID controller for simple access

		double *myInput;
		double *myOutput;
		double *mySetpoint;

		// timer
		unsigned long lastTime, nowTime, diffTime, intervalTime;
		double outIntegral, lastInput;

		// working var
		double wInput, wError, wInputDerivative, wOutput;

		// Filter

		FSMC3::LowPassFilter filter;

		// Methods

		void init();
		bool checkTimer();
		void setTunings();

	public:
		/// @brief Constructs a PID controller running at a default 10kHz with a default low pass filter @ 100Hz
		/// @param input Pointer to the input value
		/// @param output Pointer to the output value
		/// @param setpoint Pointer to the setpoint value
		/// @param kp_in Proportional control value
		/// @param ki_in Integral control value
		/// @param kd_in Derivative control value
		PID(double *input, double *output, double *setpoint,
			double kp_in, double ki_in, double kd_in);

		/// @brief Checks against the internal time interval and calculates the PID output
		/// value. Should be run as often as possible in loop.
		/// @return true if there is a new value; false if the time interval has not passed and no new value was calculated
		bool compute();

		void setOutputLimits(double lowerBound_in, double upperBound_in);
		void setIntervalTime(int intervalMicros_in);
		void setFilterCutoffFreq(int16_t lpfCutoffFreq_in);

		// Tuning functions

		void setKp(int16_t kp_in);
		void setKi(int16_t ki_in);
		void setKd(int16_t kd_in);

		// Status functions

		double getKp();
		double getKi();
		double getKd();
	};
	namespace PIDConst
	{
		static const double KP_HI = 10.0f;
		static const double KPID_LO = 0.0f;
		static const double KI_HI = 1.0f;
		static const double KD_HI = 1.0f;
		static const double PID_OUTPUT_MIN = -1.0f;
		static const double PID_OUTPUT_MAX = 1.0f;
	};
	namespace PIDTuneDefault
	{
		static const double DEFAULT_P = 1.0f;
		static const double DEFAULT_I = 0.01f;
		static const double DEFAULT_D = 0.01f;
	};
}
#endif // __FSMC_PID_H__