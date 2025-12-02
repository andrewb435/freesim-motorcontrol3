#ifndef __FSMC_PARSER_H__
#define __FSMC_PARSER_H__

#include "proto_fsmc3.h"

namespace FSMC3
{
	class Parser
	{
	private:
		Command motorCommand;
		ProtocolData data;

	public:
		Parser();
		void parseBuffer(char *buffer_in);
		Command getMode();
		ProtocolData *getData();
	};
};

#endif // __FSMC_PARSER_H__