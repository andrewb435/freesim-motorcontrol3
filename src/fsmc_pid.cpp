/*
FSMC PID Library

Based on Arduino PID_v1 by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
	https://github.com/br3ttb/Arduino-PID-Library/
as well as the SimpleFOC low pass filter by the SimpleFOC Arduino team
	https://github.com/simplefoc/Arduino-FOC

MIT License
*/
#include "Arduino.h"
#include "fsmc_pid.h"

void FSMC3::PID::init()
{
	outputSum = *myOutput;
	lastInput = *myInput;
	if (outputSum > outMax)
		outputSum = outMax;
	else if (outputSum < outMin)
		outputSum = outMin;
	lpfCalcTimeConstant();
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

void FSMC3::PID::doLPF()
{
	lpfAlpha = lpfTimeConstant / (lpfTimeConstant + diffTime);
	lpfOutput = lpfAlpha * lastOutput + (1.0f - lpfAlpha) * wOutput;
	lastOutput = lpfOutput;
	wOutput = lpfOutput;
}

void FSMC3::PID::lpfCalcTimeConstant()
{
	lpfTimeConstant = 1.0 / lpfCutoffFreq;
}

FSMC3::PID::PID(double *input, double *output, double *setpoint,
				double kp_in, double ki_in, double kd_in)
{
	myOutput = output;
	myInput = input;
	mySetpoint = setpoint;

	// Default limits
	PID::setOutputLimits(-1.0f, 1.0f);
	// Default interval (microseconds) 100 = 10KHz
	intervalTime = 100;
	// set incoming tunings
	PID::setTunings(kp_in, ki_in, kd_in);
	// timestamp to start
	lastTime = micros() - intervalTime;

	// Low pass filter
	lpfCutoffFreq = 100;
	lpfEnabled = true;
}

bool FSMC3::PID::compute()
{
	if (checkTimer())
	{
		// Compute all the working error variables
		wInput = *myInput;
		wError = *mySetpoint - wInput;
		wInputDerivative = (wInput - lastInput);
		outputSum += (ki * wError);

		if (outputSum > outMax)
			outputSum = outMax;
		else if (outputSum < outMin)
			outputSum = outMin;

		// Start with proportional on error
		wOutput = kp * wError;

		// Add integral
		wOutput += outputSum;

		// Subtract derivative
		wOutput -= (kd * wInputDerivative);

		// Clamping outputs
		if (wOutput > outMax)
			wOutput = outMax;
		else if (wOutput < outMin)
			wOutput = outMin;

		// Run low-pass filter if enabled
		if (lpfEnabled)
			doLPF();

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

	if (outputSum > outMax)
		outputSum = outMax;
	else if (outputSum < outMin)
		outputSum = outMin;
}

void FSMC3::PID::setTunings(double kp_in, double ki_in, double kd_in)
{
	if (kp_in < 0 || ki_in < 0 || kd_in < 0)
		return;

	rawKp = kp_in;
	rawKi = ki_in;
	rawKd = kd_in;

	double intervalTimeInSec = (static_cast<double>(intervalTime)) / 1000000;
	kp = kp_in;
	ki = ki_in * intervalTimeInSec;
	kd = kd_in / intervalTimeInSec;
}

void FSMC3::PID::setIntervalTime(int intervalMicros_in)
{
	if (intervalMicros_in > 0)
	{
		intervalTime = static_cast<unsigned long>(intervalMicros_in);
		setTunings(rawKp, rawKi, rawKd);
	}
}

void FSMC3::PID::setLPF(bool lpfEnabled_in)
{
	lpfEnabled = lpfEnabled_in;
}

void FSMC3::PID::setLPFCutoffFreq(int16_t lpfCutoffFreq_in)
{
	lpfCutoffFreq = lpfCutoffFreq_in;
	lpfCalcTimeConstant();
}

// Status Funcions
double FSMC3::PID::getKp() { return rawKp; }
double FSMC3::PID::getKi() { return rawKi; }
double FSMC3::PID::getKd() { return rawKd; }
