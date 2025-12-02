#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <cstdint>
#include "SPI.h"
#include "proto_fsmc3.h"

namespace FSMC3Config
{
	enum AxisByName
	{
		AXISA,
		AXISB,
		AXISC
	};
	struct SystemHW
	{
		uint16_t eStopPin;
		uint16_t SPI_COPI;
		uint16_t SPI_CIPO;
		uint16_t SPI_SCLK;
		uint16_t pwmResolution;
		uint16_t pwmFrequency;
		uint16_t commandBitDepth;
		SPISettings *spiSettings;
		int16_t pidFrequency;
		int16_t pidLPFCutoff;
	};
	struct Driver
	{
		uint16_t enablePin;
		uint16_t pwmChAPin;
		uint16_t pwmChBPin;
		HardwareTimer *hwtimer;
	};
	struct Axis
	{
		uint16_t encoderPPR;
		int16_t encoderPinA;
		int16_t encoderPinB;
		HardwareTimer *encoderTimer;
		int16_t sensorPinCS;
		uint16_t rangeDegrees;
		FSMC3Config::Driver *driver;
	};
}
#endif // __HARDWARE_H__