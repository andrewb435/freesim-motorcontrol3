#ifndef __FSMC_HANDLER_H__
#define __FSMC_HANDLER_H__

#include "hw_blackpill.h"
#include "fsmc_controller.h"
#include "fsmc_communicator.h"
#include "fsmc_parser.h"
#include "fsmc_eeprom.h"

namespace FSMC3
{
	class Handler
	{
	private:
		FSMC3Config::Hardware *hardware;
		FSMC3::Communicator communicator;
		FSMC3::Parser parser;
		FSMC3::EEPROMController eeprom;
		void cmdReport();
		// void cmdEEPROMSave();
		// void cmdEEPROMLoad();
		// void cmdEEPROMWipe();
	public:
		// HACK: Put Controller back in private after debugging hack in main
		FSMC3::Controller controller;
		// Handler(FSMC3::Controller *controller_in, FSMC3::Communicator *communicator_in);
		Handler(FSMC3Config::Hardware *hardware_in, FSMC3Config::fwversion *version_in, SPIClass *SPI_in);
		void init();
		void processLoop();
	};
}

#endif // __FSMC_HANDLER_H__