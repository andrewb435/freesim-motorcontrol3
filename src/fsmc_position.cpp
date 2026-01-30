#include "fsmc_position.h"

void FSMC3::Position::calcRangeEnds()
{
	posMax = posCenter + (range / 2);
	posMin = posCenter - (range / 2);
}

FSMC3::Position::Position(FSMC3Config::Axis *axisData_in,
						  SPIClass *SPI_in,
						  SPISettings *spiSettings_in)
	: encoder{
		  STM32HWEncoder{
			  axisData_in->encoderPPR,
			  (axisData_in->invertEncoderDir ? axisData_in->encoderPinB : axisData_in->encoderPinA),
			  (axisData_in->invertEncoderDir ? axisData_in->encoderPinA : axisData_in->encoderPinB)}},
	  sensor{MagneticSensorMT6835{axisData_in->sensorPinCS, *spiSettings_in}}
{
	axisData = axisData_in;
	posInvert = axisData_in->invertEncoderDir;
	posOffset = 0.0f;
	posMin = 0.0f;
	posMax = 0.0f;
	posCenter = 0.0f;
	posCurrent = 0.0f;
	spi = SPI_in;
}

void FSMC3::Position::init()
{
	sensor.init();
	encoder.init();
	setCenterToCurrent();
}

double FSMC3::Position::processLoop()
{
	encoder.update();
	posCurrent = encoder.getAngle();
	posCurrent = abs(encoder.getAngle() + posOffset);
	return this->posCurrent;
}

void FSMC3::Position::setRange(double range_in)
{
	range = range_in;
	calcRangeEnds();
}

void FSMC3::Position::setCenterToCurrent()
{
	sensor.update();
	encoder.update();
	posCenter = sensor.getAngle();
	posOffset = encoder.getAngle() + sensor.getAngle();
	posOffset = posOffset * (posInvert ? 1 : -1);
	calcRangeEnds();
}

void FSMC3::Position::setCenter(double center_in)
{
	posCenter = center_in;
	calcRangeEnds();
}

void FSMC3::Position::nudgeCenter(double nudge_in)
{
	posCenter = posCenter + nudge_in;
	calcRangeEnds();
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

double FSMC3::Position::getRadsMin()
{
	return posMin;
}

double FSMC3::Position::getRadsMax()
{
	return posMax;
}