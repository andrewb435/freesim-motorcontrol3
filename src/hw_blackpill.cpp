#include "hw_blackpill.h"
namespace FSMC3Config
{
	Hardware::Hardware() :
		hwtimer1{TIM1},
		hwtimer2{TIM2},
		hwtimer3{TIM3},
		hwtimer4{TIM4},
		hwtimer5{TIM5},
		spi_settings{500000, MT6835_BITORDER, SPI_MODE3},
		configSystem
		{
			.eStopPin = PB1,
			.SPI_COPI = PB15,
			.SPI_CIPO = PB14,
			.SPI_SCLK = PB10,
			.pwmResolution = 32767,
			.pwmFrequency = 20000,
			.commandBitDepth = 12,
			.spiSettings = &spi_settings
		},
		configDrivers
		{
			FSMC3Config::Driver
			{
				.enablePin = PB_13,
				.pwmChAPin = PA_8,		// TIM1 CH1
				.pwmChBPin = PA_9,		// TIM1 CH2
				.hwtimer = &hwtimer1
			},
			FSMC3Config::Driver
			{
				.enablePin = PB_4,
				.pwmChAPin = PA_5,		// TIM2 CH1
				.pwmChBPin = PB_3,		// TIM2 CH2
				.hwtimer = &hwtimer2
			},
			FSMC3Config::Driver
			{
				.enablePin = PA_4,
				.pwmChAPin = PA_2,		// TIM2 CH3
				.pwmChBPin = PA_3,		// TIM2 CH4
				.hwtimer = &hwtimer2
			},
		},
		configAxes
		{
			FSMC3Config::Axis
			{
				.encoderPPR = 16384,
				.encoderPinA = PA_6,		// TIM3 CH1
				.encoderPinB = PA_7_ALT1,	// TIM3 CH2 ALT1
				.encoderTimer = &hwtimer3,
				.sensorPinCS = PB0,			// MT6835 CS pin
				.rangeDegrees = 80,			// Total (min to max) degree range of the actuator
				.driver = &configDrivers[FSMC3Config::AxisByName::AXISA]
			},
			FSMC3Config::Axis
			{
				.encoderPPR = 16384,
				.encoderPinA = PB_6,		// TIM4 CH1
				.encoderPinB = PB_7,		// TIM4 CH2
				.encoderTimer = &hwtimer4,
				.sensorPinCS = PB_5,		// MT6835 CS pin
				.rangeDegrees = 80,			// Total (min to max) degree range of the actuator
				.driver = &configDrivers[FSMC3Config::AxisByName::AXISB]
			},
			FSMC3Config::Axis
			{
				.encoderPPR = 16384,
				.encoderPinA = PA_0_ALT1,	// TIM5 CH1 ALT1
				.encoderPinB = PA_1_ALT1,	// TIM5 CH2 ALT1
				.encoderTimer = &hwtimer5,
				.sensorPinCS = PC15,		// MT6835 CS pin
				.rangeDegrees = 80,			// Total (min to max) degree range of the actuator
				.driver = &configDrivers[FSMC3Config::AxisByName::AXISC]
			}
		}
	{
		// Empty constructor body, configuration is all in the inline
	}
}