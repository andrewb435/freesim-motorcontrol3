#include "fsmc_eeprom.h"

void FSMC3::EEPROMController::checkDataVersion()
{
	if ((eepromFWVersion.major == compiledFWVersion->major))
		versionMatched = true;
	else
		versionMatched = false;
}

void FSMC3::EEPROMController::writeDataToEEPROM()
{
	EEPROM.put(EEPROM_START_OFFSET, data);
}

void FSMC3::EEPROMController::readEEPROMToData()
{
	EEPROM.get(EEPROM_START_OFFSET, data);
}

FSMC3::EEPROMController::EEPROMController(FSMC3::Controller *controller_in, FSMC3Config::fwversion *version_in)
{
	controller = controller_in;
	compiledFWVersion = version_in;
	readEEPROMToData();
	checkDataVersion();
}

void FSMC3::EEPROMController::systemToEeprom()
{
	data.axesEnables = *controller->getEnables();
	data.axesCenters = *controller->getCenters();
	data.motorPs = *controller->getAxesP();
	data.motorIs = *controller->getAxesI();
	data.motorDs = *controller->getAxesD();
}

int16_t FSMC3::EEPROMController::eepromToSystem()
{
	if (versionMatched)
	{
		controller->setEnables(&data.axesEnables);
		controller->eepromSetCenters(&data.axesCenters);
		controller->setP(&data.motorPs);
		controller->setI(&data.motorIs);
		controller->setD(&data.motorDs);
		return 0x1;
	}
	else
	{
		return 0x0;
	}
}

void FSMC3::EEPROMController::eepromWipe()
{
	for (int i = 0; i < EEPROM.length(); i++)
		EEPROM.write(i, 0);
}
