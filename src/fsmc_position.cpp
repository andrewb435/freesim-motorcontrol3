#include "fsmc_position.h"

void FSMC3::Position::hwEncoderWorkaround()
{
	encoder._pinA = digitalPinToPinName(axisData->encoderPinA);
	encoder._pinB = digitalPinToPinName(axisData->encoderPinB);
}

FSMC3::Position::Position(FSMC3Config::Axis *axisData_in,
						  SPIClass *SPI_in,
						  SPISettings *spiSettings_in)
	: encoder{
		  STM32HWEncoder{
			  axisData_in->encoderPPR,
			  axisData_in->encoderPinA,
			  axisData_in->encoderPinB}},
	  sensor{MagneticSensorMT6835{axisData_in->sensorPinCS, *spiSettings_in}}
{
	axisData = axisData_in;
	posInvert = true;
	posOffset = 0.0f;
	posMin = 0.0f;
	posMax = 0.0f;
	posCenter = 1.68f;
	posCurrent = 0.0f;
	spi = SPI_in;
}

void FSMC3::Position::init()
{
	sensor.init();
	encoder.init();
	sensor.update();
	encoder.update();
	posOffset = encoder.getAngle() + sensor.getAngle();
	hwEncoderWorkaround();
}

double FSMC3::Position::processLoop()
{
	encoder.update();
	posCurrent = encoder.getAngle();
	if (posInvert)
	{
		posCurrent = abs(encoder.getAngle() - posOffset);
	}
	else
	{
		posCurrent = encoder.getAngle() + posOffset;
	}
	// TODO: Sanity check against SPI absolute angle every once in a while
	return this->posCurrent;
}

void FSMC3::Position::setRange(double range_in)
{
	posMax = posCenter + (range_in / 2);
	posMin = posCenter - (range_in / 2);
}

void FSMC3::Position::nudgeCenter(double nudge_in)
{
	posCenter = posCenter + nudge_in;
}

double FSMC3::Position::getAbsoluteAngle()
{
	sensor.update();
	return sensor.getAngle();
}

double FSMC3::Position::getEncoderAngle()
{
	return posCurrent;
}

double FSMC3::Position::getPositionCenter()
{
	return posCenter;
}

int16_t FSMC3::Position::getAbsoluteAngle16(int16_t rangeLow_in, int16_t rangeHigh_in)
{
	return FSMC3::Utils::mapDoubleToInt16(posCurrent, posMin, posMax, rangeLow_in, rangeHigh_in);
}

int16_t FSMC3::Position::getEncoderAngle16(int16_t rangeLow_in, int16_t rangeHigh_in)
{
	return FSMC3::Utils::mapDoubleToInt16(posCurrent, posMin, posMax, rangeLow_in, rangeHigh_in);
}