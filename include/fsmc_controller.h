#ifndef __FSMC3_CONTROLLER_H__
#define __FSMC3_CONTROLLER_H__

#include <SPI.h>

#include "hardware.h"
#include "proto_fsmc3.h"
#include "fsmc_axis.h"

namespace FSMC3
{
	class Controller
	{
	public:
		FSMC3::Axis axes[ProtocolStructure::MAXIMUM_AXIS_COUNT];
		FSMC3::ProtocolData dataOutput;

	public:
		/**
		 * Controller constructor takes a FSMC3Hardware::hardwareConfig (containing
		 * 1 system-wide configuration block and 3 motor axis definitions), and the SPI bus
		 */
		Controller(FSMC3Config::Hardware *hardwareConfig_in, SPIClass *spi_in);
		void init();
		void processLoop();

		/*
		SETTER FUNCTIONS
		*/
		/// @brief Sets axis ranges by total range in degrees; e.g. 80 = +40 and -40 from center
		/// @param data_in ProtocolData containing [axis count]{int16_t} total travel rotation in degrees
		void setRanges(ProtocolData *data_in);
		/// @brief Sets enable flags on axes, >0 is enabled, <=0 is disabled
		/// @param data_in ProtocolData containing [axis count]{int16_t} enabled (>0) or disabled (<=0)
		void setEnables(ProtocolData *data_in);
		/// @brief Sets move targets for the axes from 0 to (2^commandBitDepth - 1),
		/// e.g hardwareConfig.configSystem.commandBitDepth = 12 means the range is 0-4095
		/// @param data_in ProtocolData containing [axis count]{int16_t} move target within the range
		void setMoveTargets(ProtocolData *data_in);
		/// @brief Nudge centerpoints +- 1 degree
		/// @param data_in ProtocolData containing [axis count]{int16_t} positive nudges +1 degree, negative nudges -1 degree
		void nudgeCenters(ProtocolData *data_in);
		/// @brief Set P value of axes PID controllers
		/// @param data_in ProtocolData containing [axis count]{int16_t}
		void setP(ProtocolData *data_in);
		/// @brief Set I value of axes PID controllers
		/// @param data_in ProtocolData containing [axis count]{int16_t}
		void setI(ProtocolData *data_in);
		/// @brief Set D value of axes PID controllers
		/// @param data_in ProtocolData containing [axis count]{int16_t}
		void setD(ProtocolData *data_in);

		/*
		REPORTING FUNCTIONS
		*/
		FSMC3::ProtocolData *getAbsoluteAngles16();
		FSMC3::ProtocolData *getEncoderAngles16();
		FSMC3::ProtocolData *getMoveTargets16();
		FSMC3::ProtocolData *getAxisP();
		FSMC3::ProtocolData *getAxisI();
		FSMC3::ProtocolData *getAxisD();

		/*
		EEPROM FUNCTIONS
		*/
		void eepromSetCenters(ProtocolDataDoubles *data_in);
	};
};
#endif