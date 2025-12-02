#ifndef __FWVERSION_H__
#define __FWVERSION_H__

#include <cstdint>

struct fwversion
{
	uint8_t major = 0;
	uint8_t minor = 0;
	uint8_t patch = 0;
	fwversion() {}
	fwversion(uint8_t major_in, uint8_t minor_in, uint8_t patch_in)
	{
		this->major = major_in;
		this->minor = minor_in;
		this->patch = patch_in;
	}
};

#endif