#include "fsmc_communicator.h"

void FSMC3::Communicator::wipeBuffer(char *buffer_target)
{
	for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_LENGTH_BYTES; i++)
	{
		buffer_target[i] = 0;
	}
}

void FSMC3::Communicator::checkBuffer()
{
	// [ M 00 00 00 ] ProtocolStructure::MAXIMUM_LENGTH_BYTES - 1 for zero index
	// TODO: There's gotta be a more efficient way to do this
	if (buffer[0] == ProtocolStructure::COMMAND_OPEN && buffer[ProtocolStructure::MAXIMUM_LENGTH_BYTES - 1] == ProtocolStructure::COMMAND_CLOSE)
	{
		for (uint8_t i = 0; i < ProtocolStructure::MAXIMUM_LENGTH_BYTES; i++)
		{
			output[i] = buffer[i];
		}
		wipeBuffer(buffer);
		hasData = true;
	}
	else
	{
		hasData = false;
	}
}

FSMC3::Communicator::Communicator()
{
	wipeBuffer(buffer);
	wipeBuffer(output);
	hasData = false;
	bufferIndex = 0;
}

void FSMC3::Communicator::processLoop()
{
	if (Serial.available() > 0)
	{
		char incoming = Serial.read();
		if (bufferIndex == 0 && incoming == ProtocolStructure::COMMAND_OPEN)
		{
			buffer[bufferIndex] = incoming;
			bufferIndex++;
		}
		else if (bufferIndex < ProtocolStructure::MAXIMUM_LENGTH_BYTES - 1)
		{
			buffer[bufferIndex] = incoming;
			bufferIndex++;
		}
		else if (bufferIndex == ProtocolStructure::MAXIMUM_LENGTH_BYTES - 1)
		{
			buffer[bufferIndex] = incoming;
			checkBuffer();
			bufferIndex = 0;
		}
	}
}

/// @brief returns TRUE if output[] has data, FALSE if last frame was garbage
bool FSMC3::Communicator::checkForData()
{
	return hasData;
}

char *FSMC3::Communicator::getBuffer()
{
	return output;
}

void FSMC3::Communicator::reportData(Outputs outputType_in, ProtocolData *outputData_in)
{
	Serial.print(static_cast<char>(ProtocolStructure::COMMAND_OPEN));
	Serial.print(static_cast<char>(outputType_in));
	uint8_t raw_upper = 0;
	uint8_t raw_lower = 0;
	for (int i = 0; i < ProtocolStructure::MAXIMUM_AXIS_COUNT; i++)
	{
		raw_upper = 0 | (outputData_in->data[i] >> 8);
		raw_lower = 0 | outputData_in->data[i];
		Serial.print(static_cast<char>(raw_upper));
		Serial.print(static_cast<char>(raw_lower));
	}
	Serial.println(static_cast<char>(ProtocolStructure::COMMAND_CLOSE));
}

void FSMC3::Communicator::clearData()
{
	wipeBuffer(buffer);
	wipeBuffer(output);
	hasData = false;
}
