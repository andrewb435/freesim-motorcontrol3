#include "fsmc_parser.h"

FSMC3::Parser::Parser() {}

void FSMC3::Parser::parseBuffer(char *buffer_in)
{
	// Command type
	// char buff[ProtocolStructure::MAXIMUM_LENGTH_BYTES] = &buffer_in;
	uint8_t temp = buffer_in[ProtocolStructure::COMMAND_BYTE_POSITION];
	this->motorCommand = static_cast<Command>(temp);
	// Data
	uint16_t upper = 0;
	uint16_t lower = 0;
	uint8_t upper_bits_data_offset = ProtocolStructure::AXIS_DATA_START;
	uint8_t lower_bits_data_offset = upper_bits_data_offset + 1;
	for (int i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++)
	{
		upper = 0;
		lower = 0;
		upper_bits_data_offset = (i * ProtocolStructure::AXIS_DATA_OFFSET) + ProtocolStructure::AXIS_DATA_START;
		lower_bits_data_offset = upper_bits_data_offset + 1;
		upper = buffer_in[upper_bits_data_offset] << 8;
		lower = buffer_in[lower_bits_data_offset];
		this->data.data[i] = upper | lower;
	}
}

FSMC3::Command FSMC3::Parser::getMode()
{
	return this->motorCommand;
}

FSMC3::ProtocolData *FSMC3::Parser::getData()
{
	return &this->data;
}
