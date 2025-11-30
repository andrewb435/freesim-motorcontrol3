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
		void setCenter(ProtocolData *data_in);
		/**
		 * Sets axis boundary from center in degrees
		 * 
		 * 45 = +45 -45 for 90 total
		 * @param data_in FSMC3::ProtocolData with n int16_t data fields each containing
		 * an axis. Zero is ignored, >0 is set for that respective axis
		*/
		void setMaxAngle(ProtocolData *data_in);
		void setEnables(ProtocolData *data_in);
		// double getAbsoluteRadians();
		// double getEncoderRadians();
		FSMC3::ProtocolData *getAbsoluteAngles16();
		FSMC3::ProtocolData *getEncoderAngles16();
		FSMC3::ProtocolData *getMoveTargets16();
		void setMoveTargets(ProtocolData *data_in);
		void setP(ProtocolData *data_in);
		void setI(ProtocolData *data_in);
		void setD(ProtocolData *data_in);
	};
};
#endif