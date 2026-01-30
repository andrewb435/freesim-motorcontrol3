#include "hw_blackpill.h"
namespace FSMC3Config
{
	Hardware::Hardware()
		: hwtimer1{TIM1},
		  hwtimer2{TIM2},
		  hwtimer3{TIM3},
		  hwtimer4{TIM4},
		  hwtimer5{TIM5},
		  spi_settings{1000000, MT6835_BITORDER, SPI_MODE3},
		  configSystem{
			  .debugPosition = false,
			  .debugPID = false,
			  .eStopPin = PC13,
			  .SPI_COPI = PA7,
			  .SPI_CIPO = PA6,
			  .SPI_SCLK = PA5,
			  .pwmResolution = 32767,
			  .pwmFrequency = 20000,
			  .commandBitDepth = 12,
			  .spiSettings = &spi_settings,
			  .pidFrequency = 10000, // 10kHz
			  .pidLPFCutoff = 100,
			  .spiFlashCS = PA4}, // 100Hz // W25Q64JV_IQ 8MB
		  configDrivers{
			  FSMC3Config::Driver{
				  .enablePin = PB8,
				  .pwmChAPin = PB6, // TIM4 CH1 AF02
				  .pwmChBPin = PB7, // TIM4 CH2 AF02
				  .hwtimer = &hwtimer4},
			  FSMC3Config::Driver{
				  .enablePin = PB15, // PB15 and PB_15 resolves to a different pin - appears to be a bug in stm32hal,
									 // 30U (PB15) is the correct pin for the blackpill variant
				  .pwmChAPin = PA15, // TIM2 CH1 AF01
				  .pwmChBPin = PB3,	 // TIM2 CH2 AF01
				  .hwtimer = &hwtimer2},
			  FSMC3Config::Driver{
				  .enablePin = PC15,
				  .pwmChAPin = PA2, // TIM2 CH3 AF01
				  .pwmChBPin = PA3, // TIM2 CH4 AF01
				  .hwtimer = &hwtimer2},
		  },
		  configAxes{
			  FSMC3Config::Axis{
				  .invertDirection = false,
				  .invertEncoderDir = true,
				  .encoderPPR = 16384,
				  .encoderPinA = PA8, // TIM1 CH1 AF01
				  .encoderPinB = PA9, // TIM1 CH2 AF01
				  .encoderTimer = &hwtimer1,
				  .sensorPinCS = PB9, // MT6835 CS pin
				  .rangeDegrees = 80, // Total (min to max) degree range of the actuator
				  .driver = &configDrivers[FSMC3Config::AxisByName::AXISA]},
			  FSMC3Config::Axis{
				  .invertDirection = false,
				  .invertEncoderDir = false,
				  .encoderPPR = 16384,
				  .encoderPinA = PB4, // TIM3 CH1 AF02
				  .encoderPinB = PB5, // TIM3 CH2 AF02
				  .encoderTimer = &hwtimer3,
				  .sensorPinCS = PB14, // MT6835 CS pin
				  .rangeDegrees = 80,  // Total (min to max) degree range of the actuator
				  .driver = &configDrivers[FSMC3Config::AxisByName::AXISB]},
			  FSMC3Config::Axis{
				  .invertDirection = false,
				  .invertEncoderDir = false,
				  .encoderPPR = 16384,
				  .encoderPinA = PA0_ALT1, // TIM5 CH1 AF02
				  .encoderPinB = PA1_ALT1, // TIM5 CH2 AF02
				  .encoderTimer = &hwtimer5,
				  .sensorPinCS = PC14, // MT6835 CS pin
				  .rangeDegrees = 80,  // Total (min to max) degree range of the actuator
				  .driver = &configDrivers[FSMC3Config::AxisByName::AXISC]}}
	{
		// Empty constructor body, configuration is all inline
	}
}