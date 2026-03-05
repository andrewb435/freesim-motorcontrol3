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
		/// @brief Generate Position-related debug spam. THIS WILL BREAK NORMAL COMMUNICATION!
		bool debugPosition;
		/// @brief Generate PID-related debug spam. THIS WILL BREAK NORMAL COMMUNICATION!
		bool debugPID;
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
		int16_t spiFlashCS;
		int16_t errorLEDpin;
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
		/// @brief If the ABZ and SPI positions diverge, invert this
		bool flipABZSPIDir;
		/// @brief If the feedback (ABZ + SPI) are matched but backwards (down = up), invert this
		bool invertFeedbackDirection;
		/// @brief If the motor is backwards from feedback, invert this
		bool invertMotorDirection;
		/// @brief Total range of movement in degrees
		uint16_t rangeDegrees;
		/// @brief Encoder PPR 1-indexed (aka 1-16384 NOT 0-16383 for a 14 bit encoder)
		uint16_t encoderPPR;
		int16_t encoderPinA;
		int16_t encoderPinB;
		HardwareTimer *encoderTimer;
		int16_t sensorPinCS;
		FSMC3Config::Driver *driver;
	};
}
#endif // __HARDWARE_H__