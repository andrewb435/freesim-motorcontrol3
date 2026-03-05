#ifndef __FSMC_HANDLER_H__
#define __FSMC_HANDLER_H__

#include "hw_blackpill.h"
#include "fsmc_controller.h"
#include "fsmc_communicator.h"
#include "fsmc_parser.h"
#include "fsmc_storage.h"

namespace FSMC3
{
	class Handler
	{
	private:
		FSMC3Config::fwversion *version;
		FSMC3Config::Hardware *hardware;
		FSMC3::Controller controller;
		FSMC3::Communicator communicator;
		FSMC3::Parser parser;
		FSMC3::StorageController storage;
		void cmdReport();
		void cmdStatic();
		void errorState();

	public:
		Handler(FSMC3Config::Hardware *hardware_in, FSMC3Config::fwversion *version_in, SPIClass *SPI_in);
		void init();
		void processLoop();
	};
}

#endif // __FSMC_HANDLER_H__