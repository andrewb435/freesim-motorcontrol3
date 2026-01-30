#include "fsmc_axis.h"

void FSMC3::Axis::setRangeFloat()
{
	position.setRange(rangeFloat);
}

FSMC3::Axis::Axis(FSMC3Config::SystemHW *system_in,
				  FSMC3Config::Axis *axis_in,
				  SPIClass *SPI_in)
	: pidController{
		  &pidInput,
		  &pidOutput,
		  &pidSetpoint,
		  FSMC3::PIDTuneDefault::DEFAULT_P,
		  FSMC3::PIDTuneDefault::DEFAULT_I,
		  FSMC3::PIDTuneDefault::DEFAULT_D},
	  driver{axis_in->driver, system_in}, position{axis_in, SPI_in, system_in->spiSettings}
{
	invertDirection = axis_in->invertDirection;
	pidSetpoint = 0.0;
	pidInput = 0.0;
	pidOutput = 0.0;
	rangeLow = 0;
	rangeHigh = pow(2, system_in->commandBitDepth) - 1;
	rangeCenter = rangeHigh / 2;
	rangeFloat = axis_in->rangeDegrees * (PI / 180.0);
	setRangeFloat();
	pidController.setIntervalTime(system_in->pidFrequency);
	pidController.setFilterCutoffFreq(system_in->pidLPFCutoff);
	this->debugPosition = system_in->debugPosition;
	this->debugPID = system_in->debugPID;
}

void FSMC3::Axis::init()
{
	driver.init();
	position.init();
	pidSetpoint = position.getPositionCenter();
	pidController.setOutputLimits(-1.0f, 1.0f);
}

void FSMC3::Axis::processLoop()
{
	if (isEnabled)
	{
		pidInput = position.processLoop();
		if (pidController.compute())
			driver.drive(pidOutput);
	}
	if (isEnabled && (debugPosition || debugPID))
	{
		Serial.print("Axis ");
		Serial.print((uintptr_t)this, HEX);
		Serial.print(": ");
		if (debugPosition)
		{
			Serial.print("abs pos: ");
			Serial.print(this->position.getAbsoluteAngle(), 5);
			Serial.print(", enc pos: ");
			Serial.print(this->position.getEncoderAngle(), 5);
		}
		if (debugPosition && debugPID)
		{
			Serial.print(", ");
		}
		if (debugPID)
		{
			Serial.print("out: ");
			Serial.print(this->pidOutput, 5);
		}
		Serial.println();
	}
}

void FSMC3::Axis::eepromSetCenter(double data_in)
{
	position.setCenter(data_in);
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
	// TODO: Hot path, see about optimizing this
	if (!invertDirection)
	{
		pidSetpoint = FSMC3::Utils::mapInt16ToDouble(
			target_in,
			rangeLow,
			rangeHigh,
			position.getRadsMin(),
			position.getRadsMax());
	}
	else
	{
		pidSetpoint = FSMC3::Utils::mapInt16ToDouble(
			target_in,
			rangeLow,
			rangeHigh,
			position.getRadsMax(),
			position.getRadsMin());
	}
}

void FSMC3::Axis::setP(int16_t valP_in)
{
	pidController.setKp(valP_in);
}

void FSMC3::Axis::setI(int16_t valI_in)
{
	pidController.setKi(valI_in);
}

void FSMC3::Axis::setD(int16_t valD_in)
{
	pidController.setKd(valD_in);
}

void FSMC3::Axis::setCenterToCurrent(int16_t set_in)
{
	if (set_in > 0)
		position.setCenterToCurrent();
}

void FSMC3::Axis::nudgeCenter(int16_t nudge_in)
{
	if (nudge_in > 10)
		nudge_in = 10;
	if (nudge_in < -10)
		nudge_in = -10;
	if (nudge_in > 0)
		position.nudgeCenter(AxisConst::NUDGE_RAD);
	else if (nudge_in < 0)
		position.nudgeCenter(-1 * AxisConst::NUDGE_RAD);
	pidSetpoint = position.getPositionCenter();
	position.setRange(rangeFloat);
}

double FSMC3::Axis::getAbsoluteAngle()
{
	return position.getAbsoluteAngle();
}

double FSMC3::Axis::getEncoderAngle()
{
	return position.getEncoderAngle();
}

int16_t FSMC3::Axis::getEnabled()
{
	if (isEnabled)
		return AxisConst::IS_ENABLED;
	else
		return AxisConst::IS_DISABLED;
}

double FSMC3::Axis::getCenter()
{
	return position.getPositionCenter();
}

int16_t FSMC3::Axis::getAbsoluteAngle16()
{
	if (!invertDirection)
	{
		return FSMC3::Utils::mapDoubleToInt16(
			position.getAbsoluteAngle(),
			position.getRadsMin(),
			position.getRadsMax(),
			rangeLow,
			rangeHigh);
	}
	else
	{
		return FSMC3::Utils::mapDoubleToInt16(
			position.getAbsoluteAngle(),
			position.getRadsMin(),
			position.getRadsMax(),
			rangeHigh,
			rangeLow);
	}
}

int16_t FSMC3::Axis::getEncoderAngle16()
{
	if (!invertDirection)
	{
		return FSMC3::Utils::mapDoubleToInt16(
			position.getEncoderAngle(),
			position.getRadsMin(),
			position.getRadsMax(),
			rangeLow,
			rangeHigh);
	}
	else
	{
		return FSMC3::Utils::mapDoubleToInt16(
			position.getEncoderAngle(),
			position.getRadsMin(),
			position.getRadsMax(),
			rangeHigh,
			rangeLow);
	}
}

int16_t FSMC3::Axis::getMoveTarget16()
{
	return FSMC3::Utils::mapDoubleToInt16(
		pidSetpoint,
		position.getRadsMin(),
		position.getRadsMax(),
		rangeLow,
		rangeHigh);
}

int16_t FSMC3::Axis::getAxisP()
{
	return FSMC3::Utils::mapDoubleToInt16(
		pidController.getKp(),
		FSMC3::PIDConst::KPID_LO,
		FSMC3::PIDConst::KP_HI,
		FSMC3::Utils::INT16_LO,
		FSMC3::Utils::INT16_HI);
}

int16_t FSMC3::Axis::getAxisI()
{
	return FSMC3::Utils::mapDoubleToInt16(
		pidController.getKi(),
		FSMC3::PIDConst::KPID_LO,
		FSMC3::PIDConst::KI_HI,
		FSMC3::Utils::INT16_LO,
		FSMC3::Utils::INT16_HI);
}

int16_t FSMC3::Axis::getAxisD()
{
	return FSMC3::Utils::mapDoubleToInt16(
		pidController.getKd(),
		FSMC3::PIDConst::KPID_LO,
		FSMC3::PIDConst::KD_HI,
		FSMC3::Utils::INT16_LO,
		FSMC3::Utils::INT16_HI);
}
