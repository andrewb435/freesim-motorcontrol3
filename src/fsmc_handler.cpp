#include "fsmc_handler.h"

void FSMC3::Handler::cmdReport()
{
	// TODO: Finsih report echoes
	communicator->reportData(FSMC3::Outputs::OUTPUT_SPI_POSITION, controller->getAbsoluteAngles16());
	communicator->reportData(FSMC3::Outputs::OUTPUT_ABZ_POSITION, controller->getEncoderAngles16());
	communicator->reportData(FSMC3::Outputs::OUTPUT_TARGET, controller->getMoveTargets16());
	// communicator->reportData(FSMC3::Outputs::OUTPUT_POSITION, controller.getMovePositions());
	// communicator->reportData(FSMC3::Outputs::OUTPUT_LOWER_LIMIT, controller.getLowerLimits());
	// communicator->reportData(FSMC3::Outputs::OUTPUT_UPPER_LIMIT, controller.getUpperLimits());
	// communicator->reportData(FSMC3::Outputs::OUTPUT_P, controller.getMotorP());
	// communicator->reportData(FSMC3::Outputs::OUTPUT_I, controller.getMotorI());
	// communicator->reportData(FSMC3::Outputs::OUTPUT_D, controller.getMotorD());
}

FSMC3::Handler::Handler(FSMC3::Controller *controller_in, FSMC3::Communicator *communicator_in)
{
	controller = controller_in;
	communicator = communicator_in;
}

void FSMC3::Handler::processLoop()
{
	communicator->processLoop();
	if (communicator->checkForData())
	{
		parser.parseBuffer(communicator->getBuffer());
		switch (parser.getMode())
		{
		case FSMC3::Command::COMMAND_NONE:
			break;
		case FSMC3::Command::COMMAND_ENABLE:
			controller->setEnables(parser.getData());
			break;
		case FSMC3::Command::COMMAND_MOVE:
			controller->setMoveTargets(parser.getData());
			break;
		case FSMC3::Command::COMMAND_REPORT:
			cmdReport();
			break;
		case FSMC3::Command::COMMAND_NUDGE_CENTER:
			controller->nudgeCenters(parser.getData());
			break;
		case FSMC3::Command::COMMAND_SET_P:
			controller->setP(parser.getData());
			break;
		case FSMC3::Command::COMMAND_SET_I:
			controller->setI(parser.getData());
			break;
		case FSMC3::Command::COMMAND_SET_D:
			controller->setD(parser.getData());
			break;
		}
		communicator->clearData();
	}
	controller->processLoop();
}
