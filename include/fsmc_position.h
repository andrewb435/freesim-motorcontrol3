#ifndef __FSMC_ENCODER_H__
#define __FSMC_ENCODER_H__

#include "hardware.h"
#include "encoders/mt6835/MagneticSensorMT6835.h"
#include "encoders/stm32hwencoder/STM32HWEncoder.h"
#include "fsmc_helpers.h"

namespace FSMC3
{
	class Position
	{
	private:
		bool posInvert;
		double posOffset;
		double posMin;
		double posMax;
		double posCenter;
		double posCurrent;
		FSMC3Config::Axis *axisData;
		SPIClass *spi;
		MagneticSensorMT6835 sensor;
		STM32HWEncoder encoder;
		void hwEncoderWorkaround();
	public:
		Position(FSMC3Config::Axis *axisData_in, SPIClass *SPI_in, SPISettings *spiSettings_in);
		void init();
		double processLoop();
		/// @brief Takes radians of total range sets minimum and maximum
		/// @param range_in double radians of total travel
		void setRange(double range_in);
		void nudgeCenter(double nudge_in);
		double getAbsoluteAngle();
		double getEncoderAngle();
		double getPositionCenter();
		int16_t getAbsoluteAngle16(int16_t rangeLow_in, int16_t rangeHigh_in);
		int16_t getEncoderAngle16(int16_t rangeLow_in, int16_t rangeHigh_in);
	};
}
#endif // __FSMC_ENCODER_H__