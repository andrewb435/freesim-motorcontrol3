#ifndef __FSMC_EEPROM_H__
#define __FSMC_EEPROM_H__

#include <cstdint>
#include <EEPROM.h>
#include "proto_fsmc3.h"
#include "fsmc_controller.h"
#include "fwversion.h"

namespace FSMC3
{
	class EEPROMController
	{
		const static uint8_t EEPROM_START_OFFSET = 10;

	private:
		FSMC3::Controller *controller;
		FSMC3Config::fwversion eepromFWVersion;
		FSMC3Config::fwversion *compiledFWVersion;
		bool versionMatched;
		class EEPROMData
		{
		public:
			FSMC3Config::fwversion dataVersion;
			ProtocolData axesEnables = {0};
			ProtocolDataDoubles axesCenters = {0};
			ProtocolData motorPs = {0};
			ProtocolData motorIs = {0};
			ProtocolData motorDs = {0};
		};
		EEPROMData data;
		void checkDataVersion();
		void writeDataToEEPROM();
		void readEEPROMToData();

	public:
		EEPROMController(FSMC3::Controller *controller_in, FSMC3Config::fwversion *version_in);
		void systemToEeprom();
		int16_t eepromToSystem();
		void eepromWipe();
	};
};

#endif // __FSMC_EEPROM_H__