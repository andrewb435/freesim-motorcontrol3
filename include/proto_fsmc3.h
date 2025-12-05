#ifndef __PROTO_FSMC3__
#define __PROTO_FSMC3__

#include <cstdint>

// FreeSim Motor Control 3x
namespace FSMC3
{
	enum class Command : uint8_t
	{
		/// @brief Command character [ "~" ] for doing nothing at all
		COMMAND_NONE = 0x7E,
		/// @brief Command character [ "N" ] for setting an axis to enabled or
		/// disabled state.
		COMMAND_ENABLE = 0x4E,
		/// @brief Command character [ "M" ] for requesting a move from the
		/// controller. Values expected are a signed 16 bit integer describing the
		/// position within the maximum range of the axis.
		COMMAND_MOVE = 0x4D,
		/// @brief Command character [ "R" ] for requesting a full update report
		/// from the controller
		COMMAND_REPORT = 0x52,
		/// @brief Command character [ "C" ] for nudging the centerpoint:
		/// Values are a signed 16 bit integer instructing a number of degrees
		/// (positive or negative) to adjust the centerpoint by. This is clamped
		/// by the controller to +-10 degrees max per command. (e.g. 0x0001 would
		/// instruct the controller to move the centerpoint +1 degree where 0xFFFF
		/// would command -1 degree)
		COMMAND_NUDGE_CENTER = 0x43,
		/// @brief Command character [ "P" ] for setting the P tuning value:
		/// Values will still be signed 16 bit integer but describing a double
		/// within a range set in fsmc_pid.h
		COMMAND_SET_P = 0x50,
		/// @brief Command character [ "I" ] for setting the I tuning value:
		/// Values will still be signed 16 bit integer but describing a double
		/// within a range set in fsmc_pid.h
		COMMAND_SET_I = 0x49,
		/// @brief Command character [ "D" ] for setting the D tuning value:
		/// Values will still be signed 16 bit integer but describing a double
		/// within a range set in fsmc_pid.h
		COMMAND_SET_D = 0x44,
		/// @brief Command character [ "X" ] for saving system state to EEPROM
		COMMAND_EEPROM_SAVE = 0x58,
		/// @brief Command character [ "Y" ] for loading system state from EEPROM
		COMMAND_EEPROM_LOAD = 0x59,
		/// @brief Command character [ "X" ] for wiping onboard EEPROM
		COMMAND_EEPROM_WIPE = 0x5A
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