#ifndef __HW_BLACKPILL_H__
#define __HW_BLACKPILL_H__

#include "hardware.h"
#include "encoders/mt6835/MagneticSensorMT6835.h"

namespace FSMC3Config
{
	class Hardware
	{
	private:
		// PWM output timers
		HardwareTimer hwtimer1;
		HardwareTimer hwtimer2;

		// Encoder Input timers
		HardwareTimer hwtimer3;
		HardwareTimer hwtimer4;
		HardwareTimer hwtimer5;

		// SPI Settings for MT6835
		SPISettings spi_settings;

	public:
		Hardware();

		// config objects
		FSMC3Config::SystemHW configSystem;
		FSMC3Config::Driver configDrivers[FSMC3::ProtocolStructure::MAXIMUM_AXIS_COUNT];
		FSMC3Config::Axis configAxes[FSMC3::ProtocolStructure::MAXIMUM_AXIS_COUNT];
	};
}
#endif // __HW_BLACKPILL_H__