#ifndef __FSMC_HANDLER_H__
#define __FSMC_HANDLER_H__

#include "fsmc_controller.h"
#include "fsmc_communicator.h"
#include "fsmc_parser.h"

namespace FSMC3
{
	class Handler
	{
	private:
		FSMC3::Controller *controller;
		FSMC3::Communicator *communicator;
		FSMC3::Parser parser;
		void cmdReport();
		// void cmdEEPROMSave();
		// void cmdEEPROMLoad();
		// void cmdEEPROMWipe();
	public:
		Handler(FSMC3::Controller *controller_in, FSMC3::Communicator *communicator_in);
		void processLoop();
	};
}

#endif // __FSMC_HANDLER_H__