#include <Arduino.h>
// MT6835 drivers from SimpleFOCDrivers library
// https://github.com/simplefoc/Arduino-FOC-drivers

// FSMC Constants
#include "hw_blackpill.h"
#include "fsmc_communicator.h"
#include "fsmc_controller.h"
#include "fsmc_handler.h"

fwversion version(0, 0, 1);

FSMC3Config::Hardware hardware;

FSMC3::Communicator communicator(&version);
FSMC3::Controller controller(&hardware, &SPI);
FSMC3::Handler handler(&controller, &communicator);

// Serial debugging variables/functions
long timestamp = millis();
long now = millis();
uint16_t interval = 100;
#define DEBUG_AXIS 0
void report();
// End serial debugging variables/functions

void setup()
{
	Serial.begin(115200);
	SPI.setMISO(hardware.configSystem.SPI_CIPO);
	SPI.setMOSI(hardware.configSystem.SPI_COPI);
	SPI.setSCLK(hardware.configSystem.SPI_SCLK);
	controller.init();

	// HACK: Remove this hardcoded enable for the only axis I have wired up
	controller.axes[DEBUG_AXIS].setEnable(0x1);
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
		report();
	}
}

void report()
{
	now = millis();
	if (now - timestamp > interval)
	{
		Serial.print(controller.axes[DEBUG_AXIS].getAbsoluteAngle(), 5);
		Serial.print("  |  ");
		Serial.print(controller.axes[DEBUG_AXIS].getEncoderAngle(), 5);
		Serial.print("  |  ");
		Serial.println(controller.axes[DEBUG_AXIS].pidOutput, 5);
		timestamp = millis();
	}
}