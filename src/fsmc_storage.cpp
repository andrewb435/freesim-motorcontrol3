#include "fsmc_storage.h"

bool FSMC3::StorageController::checkDataVersion()
{
	if (dataFlash.version.patch == dataSystem.version.patch)
		return true;
	else
		return false;
}

void FSMC3::StorageController::fillDataSystem()
{
	memcpy(&dataSystem.axesEnables, controller->getEnables(), BYTECOUNT_AXESENABLE);
	memcpy(&dataSystem.axesCenters, controller->getCenters(), BYTECOUNT_AXESCENTER);
	memcpy(&dataSystem.motorPs, controller->getAxesP(), BYTECOUNT_MOTORPIDS);
	memcpy(&dataSystem.motorIs, controller->getAxesI(), BYTECOUNT_MOTORPIDS);
	memcpy(&dataSystem.motorDs, controller->getAxesD(), BYTECOUNT_MOTORPIDS);
}

FSMC3::StorageController::StorageController(FSMC3::Controller *controller_in, FSMC3Config::fwversion *version_in, int16_t pinCS_in)
	: flash{static_cast<uint8_t>(pinCS_in)}
{
	controller = controller_in;
	memcpy(&dataSystem.version, version_in, BYTECOUNT_VERSION);
}

void FSMC3::StorageController::init()
{
	flash.setClock(10000000);
	flash.begin();
	storageToSystem();
}

void FSMC3::StorageController::systemToStorage()
{
	fillDataSystem();
	flash.eraseSection(STORAGE_START_OFFSET, sizeof(this->dataSystem));
	flash.writeAnything(STORAGE_START_OFFSET, dataSystem, true);
}

void FSMC3::StorageController::storageToSystem()
{
	fillDataSystem();
	flash.readAnything(STORAGE_START_OFFSET, dataFlash, false);
	if (checkDataVersion())
	{
		controller->setEnables(&dataFlash.axesEnables);
		controller->setCentersFromStorage(&dataFlash.axesCenters);
		controller->setP(&dataFlash.motorPs);
		controller->setI(&dataFlash.motorIs);
		controller->setD(&dataFlash.motorDs);
	}
}

void FSMC3::StorageController::eepromWipe()
{
	flash.eraseChip();
}
