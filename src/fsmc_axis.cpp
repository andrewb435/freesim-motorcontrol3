#include "fsmc_axis.h"

void FSMC3::Axis::setPIDTunings()
{
	pidController.setTunings(pidKp, pidKi, pidKd);
}

FSMC3::Axis::Axis(FSMC3Config::SystemHW *system_in,
				  FSMC3Config::Axis *axis_in,
				  SPIClass *SPI_in)
	: pidController{
		  &pidInput,
		  &pidOutput,
		  &pidSetpoint,
		  pidKp,
		  pidKi,
		  pidKd},
	  driver{axis_in->driver, system_in}, position{axis_in, SPI_in, system_in->spiSettings}
{
	pidKp = 1.0f;
	pidKi = 0.01f;
	pidKd = 0.01f;
	pidSetpoint = 0.0;
	pidInput = 0.0;
	pidOutput = 0.0;
	rangeLow = 0;
	rangeHigh = pow(2, system_in->commandBitDepth) - 1;
	rangeCenter = rangeHigh / 2;
	setRange(axis_in->rangeDegrees);
	pidController.setIntervalTime(system_in->pidFrequency);
	pidController.setLPFCutoffFreq(system_in->pidLPFCutoff);
}

void FSMC3::Axis::init()
{
	driver.init();
	position.init();
	pidSetpoint = position.getPositionCenter();
	pidController.setOutputLimits(-1.0f, 1.0f);
	pidController.setTunings(this->pidKp, this->pidKi, this->pidKd);
}

void FSMC3::Axis::processLoop()
{
	if (isEnabled)
	{
		pidInput = position.processLoop();
		if (pidController.compute())
		{
			driver.drive(pidOutput);
		}
	}
}

void FSMC3::Axis::eepromSetCenter(double data_in)
{
}

void FSMC3::Axis::setRange(int16_t range_in)
{
	double rangeFloat = range_in * (PI / 180.0);
	position.setRange(rangeFloat);
}

void FSMC3::Axis::setEnable(int16_t enable_in)
{
	if (enable_in > 0)
	{
		isEnabled = true;
		driver.setEnable(true);
	}
	else
	{
		isEnabled = false;
		driver.setEnable(false);
	}
}

void FSMC3::Axis::setMoveTarget(int16_t target_in)
{
	pidSetpoint = FSMC3::Utils::mapInt16ToDouble(target_in, rangeLow, rangeHigh, -1.0, 1.0);
}

void FSMC3::Axis::setP(int16_t valP_in)
{
	// TODO: This will definitely need some kind of division to make sense with the PID controller
	pidKp = static_cast<double>(valP_in);
	setPIDTunings();
}

void FSMC3::Axis::setI(int16_t valI_in)
{
	// TODO: This will definitely need some kind of division to make sense with the PID controller
	pidKi = static_cast<double>(valI_in);
	setPIDTunings();
}

void FSMC3::Axis::setD(int16_t valD_in)
{
	// TODO: This will definitely need some kind of division to make sense with the PID controller
	pidKd = static_cast<double>(valD_in);
	setPIDTunings();
}

void FSMC3::Axis::nudgeCenter(int16_t nudge_in)
{
	if (nudge_in > 0)
	{
		position.nudgeCenter(NUDGE);
	}
	else if (nudge_in < 0)
	{
		position.nudgeCenter(-1 * NUDGE);
	}
}

double FSMC3::Axis::getAbsoluteAngle()
{
	return position.getAbsoluteAngle();
}

double FSMC3::Axis::getEncoderAngle()
{
	return position.getEncoderAngle();
}

int16_t FSMC3::Axis::getAbsoluteAngle16()
{
	return position.getAbsoluteAngle16(rangeLow, rangeHigh);
}

int16_t FSMC3::Axis::getEncoderAngle16()
{
	return position.getEncoderAngle16(rangeLow, rangeHigh);
}

int16_t FSMC3::Axis::getMoveTarget16()
{
	return FSMC3::Utils::mapDoubleToInt16(pidSetpoint, -1.0, 1.0, rangeLow, rangeHigh);
}
