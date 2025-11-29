#include "fsmc_controller.h"

FSMC3::Controller::Controller(FSMC3Config::Hardware *hardware_in, SPIClass *spi_in) :
	axes{
		Axis{
			&hardware_in->configSystem, 
			&hardware_in->configAxes[FSMC3Config::AxisByName::AXISA],
			spi_in},
		Axis{
			&hardware_in->configSystem, 
			&hardware_in->configAxes[FSMC3Config::AxisByName::AXISB],
			spi_in},
		Axis{
			&hardware_in->configSystem, 
			&hardware_in->configAxes[FSMC3Config::AxisByName::AXISC],
			spi_in}
		}
{
	/*
	 * IF ESTOP IS NOT CONNECTED TO GND, DEAD STOP INITIALIZATION
	*/
	while (digitalRead(hardware_in->configSystem.eStopPin)) {};
	/*
	 * IF ESTOP IS NOT CONNECTED TO GND, DEAD STOP INITIALIZATION
	*/
}

void FSMC3::Controller::init()
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->axes[i].init();
	}
}

void FSMC3::Controller::processLoop()
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->axes[i].processLoop();
	}
}

void FSMC3::Controller::setCenter(ProtocolData *data_in)
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		if (data_in->data[i] > 0) {
			this->axes[i].setCenter();
		}
	}
}

void FSMC3::Controller::setMaxAngle(ProtocolData *data_in)
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->axes[i].setRange(data_in->data[i]);
	}
}

void FSMC3::Controller::setEnables(ProtocolData *data_in)
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->axes[i].setEnable(data_in->data[i]);
	}
}

FSMC3::ProtocolData *FSMC3::Controller::getAbsoluteAngles16()
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->dataOutput.data[i] = this->axes[i].getAbsoluteAngle16();
	}
	return &this->dataOutput;
}
FSMC3::ProtocolData *FSMC3::Controller::getEncoderAngles16()
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->dataOutput.data[i] = this->axes[i].getEncoderAngle16();
	}
	return &this->dataOutput;
}

FSMC3::ProtocolData *FSMC3::Controller::getMoveTargets16()
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->dataOutput.data[i] = this->axes[i].getMoveTarget16();
	}
	return &this->dataOutput;
}

void FSMC3::Controller::setMoveTargets(ProtocolData *data_in)
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->axes[i].setMoveTarget(data_in->data[i]);
	}
}

void FSMC3::Controller::setP(ProtocolData *data_in)
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->axes[i].setP(data_in->data[i]);
	}
}

void FSMC3::Controller::setI(ProtocolData *data_in)
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->axes[i].setI(data_in->data[i]);
	}
}

void FSMC3::Controller::setD(ProtocolData *data_in)
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++) {
		this->axes[i].setD(data_in->data[i]);
	}
}
