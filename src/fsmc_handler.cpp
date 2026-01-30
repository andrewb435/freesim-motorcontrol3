#include "fsmc_handler.h"

void FSMC3::Handler::cmdReport()
{
	communicator.reportData(FSMC3::Outputs::OUTPUT_SPI_POSITION, controller.getAbsoluteAngles16());
	communicator.reportData(FSMC3::Outputs::OUTPUT_ABZ_POSITION, controller.getEncoderAngles16());
	communicator.reportData(FSMC3::Outputs::OUTPUT_TARGET, controller.getMoveTargets16());
}

void FSMC3::Handler::cmdStatic()
{
	communicator.reportData(FSMC3::Outputs::OUTPUT_SPI_POSITION, controller.getAbsoluteAngles16());
	communicator.reportData(FSMC3::Outputs::OUTPUT_ABZ_POSITION, controller.getEncoderAngles16());
	communicator.reportData(FSMC3::Outputs::OUTPUT_TARGET, controller.getMoveTargets16());
	communicator.reportData(FSMC3::Outputs::OUTPUT_GET_P, controller.getAxesP());
	communicator.reportData(FSMC3::Outputs::OUTPUT_GET_I, controller.getAxesI());
	communicator.reportData(FSMC3::Outputs::OUTPUT_GET_D, controller.getAxesD());
	communicator.reportData(FSMC3::Outputs::OUTPUT_GET_EN, controller.getEnables());
}

FSMC3::Handler::Handler(FSMC3Config::Hardware *hardware_in, FSMC3Config::fwversion *version_in, SPIClass *SPI_in)
	: controller{hardware_in, SPI_in},
	  storage{&controller, version_in, hardware_in->configSystem.spiFlashCS}
{
	hardware = hardware_in;
}

void FSMC3::Handler::init()
{
	SPI.setMISO(hardware->configSystem.SPI_CIPO);
	SPI.setMOSI(hardware->configSystem.SPI_COPI);
	SPI.setSCLK(hardware->configSystem.SPI_SCLK);
	controller.init();
	storage.init();
}

void FSMC3::Handler::processLoop()
{
	communicator.processLoop();
	if (communicator.checkForData())
	{
		parser.parseBuffer(communicator.getBuffer());
		switch (parser.getMode())
		{
		case FSMC3::Command::COMMAND_NONE:
			break;
		case FSMC3::Command::COMMAND_ENABLE:
			controller.setEnables(parser.getData());
			break;
		case FSMC3::Command::COMMAND_MOVE:
			controller.setMoveTargets(parser.getData());
			break;
		case FSMC3::Command::COMMAND_REPORT:
			cmdReport();
			break;
		case FSMC3::Command::COMMAND_STATIC:
			cmdStatic();
			break;
		case FSMC3::Command::COMMAND_SET_CENTER:
			controller.setCenterToCurrent(parser.getData());
			break;
		case FSMC3::Command::COMMAND_NUDGE_CENTER:
			controller.nudgeCenters(parser.getData());
			break;
		case FSMC3::Command::COMMAND_SET_P:
			controller.setP(parser.getData());
			break;
		case FSMC3::Command::COMMAND_SET_I:
			controller.setI(parser.getData());
			break;
		case FSMC3::Command::COMMAND_SET_D:
			controller.setD(parser.getData());
			break;
		case FSMC3::Command::COMMAND_STORAGE_SAVE:
			storage.systemToStorage();
			break;
		case FSMC3::Command::COMMAND_STORAGE_LOAD:
			storage.storageToSystem();
			break;
		case FSMC3::Command::COMMAND_STORAGE_WIPE:
			storage.eepromWipe();
			break;
		}
		communicator.clearData();
	}
	controller.processLoop();
}
