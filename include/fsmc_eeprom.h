#ifndef __FSMC_EEPROM_H__
#define __FSMC_EEPROM_H__

#include <cstdint>
#include "proto_fsmc3.h"
#include "fsmc_controller.h"
#include "fwversion.h"

namespace FSMC3
{
	class EEPROMStorage
	{
		const static uint8_t EEPROM_START_OFFSET = 10;

	private:
		FSMC3::Controller *controller;
		class EEPROMController
		{
		public:
			FSMC3Config::fwversion dataVersion;
			ProtocolData isEnables = {0};
			ProtocolData motorPs = {0};
			ProtocolData motorIs = {0};
			ProtocolData motorDs = {0};
			ProtocolDataDoubles axisCenters = {0};
		};
		FSMC3Config::fwversion firmwareVersion;
		EEPROMController data;
		void systemToEeprom();
		void eepromToSystem();
		bool checkDataVersion(EEPROMController tempData_in);

	public:
		EEPROMStorage(Controller *controller_in, FSMC3Config::fwversion *version_in);
		void SaveToEEPROM();
		void ReadFromEEPROM();
		void WipeEEPROM();
	};
};

#endif // __FSMC_EEPROM_H__