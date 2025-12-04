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

// Serial debugging variables/functions
long timestamp = millis();
long now = millis();
uint16_t interval = 100;
#define DEBUG_AXIS 0
#define DEBUG_ENABLED 0
void report();
// End serial debugging variables/functions

void setup()
{
	Serial.begin(115200);
	handler.init();

	// HACK: Remove this hardcoded enable for the only axis I have wired up
	handler.controller.axes[DEBUG_AXIS].setEnable(0x1);
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
		if (DEBUG_ENABLED)
			report();
	}
}

void report()
{
	now = millis();
	if (now - timestamp > interval)
	{
		Serial.print(handler.controller.axes[DEBUG_AXIS].getAbsoluteAngle(), 5);
		Serial.print("  |  ");
		Serial.print(handler.controller.axes[DEBUG_AXIS].getEncoderAngle(), 5);
		Serial.print("  |  ");
		Serial.println(handler.controller.axes[DEBUG_AXIS].pidOutput, 5);
		timestamp = millis();
	}
}