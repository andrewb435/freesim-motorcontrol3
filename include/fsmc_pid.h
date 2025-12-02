#ifndef __FSMC_PID_H__
#define __FSMC_PID_H__
/*
FSMC PID Library

Based on Arduino PID_v1 by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
	https://github.com/br3ttb/Arduino-PID-Library/
as well as the SimpleFOC low pass filter by the SimpleFOC Arduino team
	https://github.com/simplefoc/Arduino-FOC

MIT License
*/
namespace FSMC3
{
	class PID
	{
	private:
		// Hold raw tuning values for re-calculations and getters
		double rawKp;
		double rawKi;
		double rawKd;
		// Internal tuning values (scaled to intervalTime)
		double kp; // * (P)roportional Tuning Parameter
		double ki; // * (I)ntegral Tuning Parameter
		double kd; // * (D)erivative Tuning Parameter
		// Output limiters
		double outMin, outMax;
		// Pointers to the user input/output/setpoint parameters provided during construction
		// Intended to reduce the calls into the PID controller for simple access
		double *myInput;
		double *myOutput;
		double *mySetpoint;

		// Various timers
		unsigned long lastTime, nowTime, diffTime, intervalTime;

		double outputSum, lastInput;

		// Working variables
		double wInput, wError, wInputDerivative, wOutput;

		// Low Pass Filter variables
		// Follows PID calculation frequency
		bool lpfEnabled;
		double lastOutput;
		double lpfAlpha;
		double lpfOutput;
		double lpfTimeConstant;
		/// @brief Cutoff frequency in Hz
		int16_t lpfCutoffFreq;

		// Methods
		void init();
		bool checkTimer();
		void doLPF();
		void lpfCalcTimeConstant();

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
		void setTunings(double kp_in, double ki_in, double kd_in);
		void setIntervalTime(int intervalMicros_in);
		void setLPF(bool lpfEnabled_in);
		void setLPFCutoffFreq(int16_t lpfCutoffFreq_in);

		// Status functions
		double getKp(); // These functions query the pid for interal values.
		double getKi(); //  they were created mainly for the pid front-end,
		double getKd(); // where it's important to know what is actually
	};
}
#endif // __FSMC_PID_H__