#ifndef __PROTO_FSMC3__
#define __PROTO_FSMC3__

#include <cstdint>

// FreeSim Motor Control 3x
namespace FSMC3
{
	enum class Command : uint8_t
	{
		COMMAND_NONE = 0x7E,		 // "~"
		COMMAND_ENABLE = 0x4E,		 // "N"
		COMMAND_MOVE = 0x4D,		 // "M"
		COMMAND_REPORT = 0x52,		 // "R"
		COMMAND_NUDGE_CENTER = 0x43, // "C"
		COMMAND_SET_P = 0x50,		 // "P"
		COMMAND_SET_I = 0x49,		 // "I"
		COMMAND_SET_D = 0x44,		 // "D"
		COMMAND_EEPROM_SAVE = 0x58,	 // "X"
		COMMAND_EEPROM_LOAD = 0x59,	 // "Y"
		COMMAND_EEPROM_WIPE = 0x5A	 // "Z"
	};
	enum class Outputs : uint8_t
	{
		OUTPUT_SPI_POSITION = 0x52, // "R"
		OUTPUT_ABZ_POSITION = 0x45, // "E"
		OUTPUT_TARGET = 0x54,		// "T"
		OUTPUT_GET_P = 0x50,		// "P"
		OUTPUT_GET_I = 0x49,		// "I"
		OUTPUT_GET_D = 0x44,		// "D"
	};
	namespace ProtocolStructure
	{
		// FSMC3::ProtocolStructure::COMMAND_OPEN (1) + FSMC3::Command (1)
		// + 3 axes with 2 bytes data each (3*2 = 6) + FSMC3::ProtocolStructure::COMMAND_CLOSE (1)
		static const uint8_t COMMAND_OPEN_POSITION = 0;
		static const uint8_t COMMAND_OPEN = 0x5B;  // "["
		static const uint8_t COMMAND_CLOSE = 0x5D; // "]"
		static const uint8_t COMMAND_BYTE_POSITION = 1U;
		static const uint8_t AXIS_DATA_START = 2U;
		static const uint8_t AXIS_DATA_OFFSET = 2U;
		static const uint8_t MAXIMUM_AXIS_COUNT = 3U;
		static const uint8_t MAXIMUM_LENGTH_BYTES = 1 + 1 + (MAXIMUM_AXIS_COUNT * AXIS_DATA_OFFSET) + 1;
	};
	struct ProtocolData
	{
		int16_t data[ProtocolStructure::MAXIMUM_AXIS_COUNT];
	};
	struct ProtocolDataDoubles
	{
		double data[ProtocolStructure::MAXIMUM_AXIS_COUNT];
	};
};

#endif