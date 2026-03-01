#ifndef __FSMC_STORAGE_H__
#define __FSMC_STORAGE_H__

#include <cstdint>
#include "proto_fsmc3.h"
#include "fsmc_controller.h"
#include "fwversion.h"
#include <SPIFlash.h>

namespace FSMC3
{
	class StorageController
	{
	private:
		const static uint8_t STORAGE_START_OFFSET = 0;
		const static uint8_t BYTECOUNT_VERSION = (sizeof(uint8_t) * 3); // major, minor, patch = 3
		const static uint8_t BYTECOUNT_AXESENABLE = (sizeof(int16_t) * FSMC3::ProtocolStructure::MAXIMUM_AXIS_COUNT);
		const static uint8_t BYTECOUNT_AXESCENTER = (sizeof(double) * FSMC3::ProtocolStructure::MAXIMUM_AXIS_COUNT);
		const static uint8_t BYTECOUNT_MOTORPIDS = (sizeof(int16_t) * FSMC3::ProtocolStructure::MAXIMUM_AXIS_COUNT);
		const static uint16_t BYTECOUNT_TOTAL = BYTECOUNT_VERSION +
												BYTECOUNT_AXESENABLE +
												BYTECOUNT_AXESCENTER +
												BYTECOUNT_MOTORPIDS +
												BYTECOUNT_MOTORPIDS +
												BYTECOUNT_MOTORPIDS;

		struct StorageData
		{
			FSMC3Config::fwversion version;
			FSMC3::ProtocolData axesEnables;
			FSMC3::ProtocolDataDoubles axesCenters;
			FSMC3::ProtocolData motorPs;
			FSMC3::ProtocolData motorIs;
			FSMC3::ProtocolData motorDs;
		};
		FSMC3::Controller *controller;
		StorageData dataSystem;
		StorageData dataFlash;
		uint16_t dataLength = sizeof(StorageData);
		SPIFlash flash;
		bool checkDataVersion();
		void fillDataSystem();

	public:
		StorageController(FSMC3::Controller *controller_in, FSMC3Config::fwversion *version_in, int16_t pinCS_in);
		void init();
		void systemToStorage();
		void storageToSystem();
		void eepromWipe();
	};
};

#endif // __FSMC_STORAGE_H__