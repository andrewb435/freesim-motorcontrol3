#include <Arduino.h>
// MT6835 drivers from SimpleFOCDrivers library
// https://github.com/simplefoc/Arduino-FOC-drivers

// FSMC Constants
#include "hw_blackpill.h"
#include "fsmc_communicator.h"
#include "fsmc_controller.h"
#include "fsmc_handler.h"

FSMC3Config::fwversion version(0, 0, 1);
FSMC3Config::Hardware hardware;

FSMC3::Handler handler(&hardware, &version, &SPI);

void setup()
{
	Serial.begin(115200);
	handler.init();
}

void loop()
{
	// hardware.configSystem.eStopPin being low (NC to GND through estop switch) is the only time the loop should run
	// If the pinEStop is ever high (aka not NC to GND through estop switch), reset the whole system
	if (digitalRead(hardware.configSystem.eStopPin))
	{
		NVIC_SystemReset();
	}
	else
	{
		handler.processLoop();
	}
}