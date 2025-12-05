/*
FSMC PID Library
Based on Arduino PID_v1 by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
	https://github.com/br3ttb/Arduino-PID-Library/
MIT License
*/

#include "Arduino.h"
#include "fsmc_pid.h"
#include "fsmc_utils.h"

void FSMC3::PID::init()
{
	outIntegral = *myOutput;
	lastInput = *myInput;
	if (outIntegral > outMax)
		outIntegral = outMax;
	else if (outIntegral < outMin)
		outIntegral = outMin;
	filter.lpfCalcTimeConstant();
}

bool FSMC3::PID::checkTimer()
{
	nowTime = micros();
	diffTime = (nowTime - lastTime);
	if (diffTime >= intervalTime)
		return true;
	else
		return false;
}

FSMC3::PID::PID(double *input, double *output, double *setpoint,
				double kp_in, double ki_in, double kd_in)
	: filter{
		  &diffTime,
		  &wOutput}
{
	myOutput = output;
	myInput = input;
	mySetpoint = setpoint;

	rawKp = kp_in;
	rawKi = ki_in;
	rawKd = kd_in;

	// Default limits
	PID::setOutputLimits(
		PIDConst::PID_OUTPUT_MIN,
		PIDConst::PID_OUTPUT_MAX);
	// Default interval (microseconds) 100 = 10KHz
	intervalTime = 100;
	// set incoming tunings
	PID::setTunings();
	// timestamp to start
	lastTime = micros() - intervalTime;
}

bool FSMC3::PID::compute()
{
	if (checkTimer())
	{
		// Compute all the working error variables
		wInput = *myInput;
		wError = *mySetpoint - wInput;
		wInputDerivative = (wInput - lastInput);
		outIntegral += (ki * wError);

		if (outIntegral > outMax)
			outIntegral = outMax;
		else if (outIntegral < outMin)
			outIntegral = outMin;

		// Start with proportional on error
		wOutput = kp * wError;

		// Add integral
		wOutput += outIntegral;

		// Subtract derivative
		wOutput -= (kd * wInputDerivative);

		// Clamping outputs
		if (wOutput > outMax)
			wOutput = outMax;
		else if (wOutput < outMin)
			wOutput = outMin;

		// Run low-pass filter if enabled
		if (filter.getEnabled())
			filter.doFilter();

		*myOutput = wOutput;

		// Tracking variables
		lastInput = wInput;
		lastTime = nowTime;
		return true;
	}
	else
	{
		return false;
	}
}

void FSMC3::PID::setOutputLimits(double lowerBound_in, double upperBound_in)
{
	if (lowerBound_in >= upperBound_in)
		return;
	outMin = lowerBound_in;
	outMax = upperBound_in;

	if (*myOutput > outMax)
		*myOutput = outMax;
	else if (*myOutput < outMin)
		*myOutput = outMin;

	if (outIntegral > outMax)
		outIntegral = outMax;
	else if (outIntegral < outMin)
		outIntegral = outMin;
}

void FSMC3::PID::setTunings()
{
	if (rawKp < 0 || rawKi < 0 || rawKd < 0)
		return;

	double intervalTimeInSec = (static_cast<double>(intervalTime)) / 1000000;
	kp = rawKp;
	ki = rawKi * intervalTimeInSec;
	kd = rawKd / intervalTimeInSec;
}

void FSMC3::PID::setIntervalTime(int intervalMicros_in)
{
	if (intervalMicros_in > 0)
	{
		intervalTime = static_cast<unsigned long>(intervalMicros_in);
		setTunings();
	}
}

void FSMC3::PID::setFilterCutoffFreq(int16_t lpfCutoffFreq_in)
{
	filter.setCutoffFreq(lpfCutoffFreq_in);
}

void FSMC3::PID::setKp(int16_t kp_in)
{
	rawKp = FSMC3::Utils::mapInt16ToDouble(
		kp_in,
		FSMC3::Utils::INT16_LO,
		FSMC3::Utils::INT16_HI,
		PIDConst::KPID_LO,
		PIDConst::KP_HI);
	setTunings();
}

void FSMC3::PID::setKi(int16_t ki_in)
{
	rawKi = FSMC3::Utils::mapInt16ToDouble(
		ki_in, FSMC3::Utils::INT16_LO,
		FSMC3::Utils::INT16_HI,
		PIDConst::KPID_LO,
		PIDConst::KI_HI);
	setTunings();
}

void FSMC3::PID::setKd(int16_t kd_in)
{
	rawKd = FSMC3::Utils::mapInt16ToDouble(
		kd_in,
		FSMC3::Utils::INT16_LO,
		FSMC3::Utils::INT16_HI,
		PIDConst::KPID_LO,
		PIDConst::KD_HI);
	setTunings();
}

// Status Funcions
double FSMC3::PID::getKp() { return rawKp; }
double FSMC3::PID::getKi() { return rawKi; }
double FSMC3::PID::getKd() { return rawKd; }
