#ifndef __FSMC_COMMUNICATOR_H__
#define __FSMC_COMMUNICATOR_H__

#include <Arduino.h>
#include "proto_fsmc3.h"
#include "fwversion.h"

namespace FSMC3
{
	class Communicator
	{
	private:
		fwversion *version;
		bool hasData;
		uint8_t bufferIndex;
		char buffer[ProtocolStructure::MAXIMUM_LENGTH_BYTES] = {0};
		char output[ProtocolStructure::MAXIMUM_LENGTH_BYTES] = {0};
		void wipeBuffer(char *buffer_target);
		void checkBuffer();

	public:
		Communicator(fwversion *version_in);
		void setVersion(fwversion *version_in);
		void processLoop();
		bool checkForData();
		char *getBuffer();
		void reportData(Outputs outputType_in, ProtocolData *outputData_in);
		void clearData();
	};
};

#endif