#include <Arduino.h>
// MT6835 drivers from SimpleFOCDrivers library
// https://github.com/simplefoc/Arduino-FOC-drivers
#include "SimpleFOC.h"
#include "SimpleFOCDrivers.h"
// Arduino PID Library
// https://github.com/br3ttb/Arduino-PID-Library
#include "PID_v1.h"

// FSMC Constants
#include "hw_blackpill.h"
#include "fsmc_communicator.h"
#include "fsmc_parser.h"
#include "fsmc_controller.h"
#include "fsmc_handler.h"

fwversion version(0,0,1);

void report();
void reportAngles();

FSMC3Config::Hardware hardware;

FSMC3::Communicator communicator(&version);
FSMC3::Controller controller(&hardware, &SPI);
FSMC3::Handler handler(&controller, &communicator);

long timestamp = millis();
uint16_t interval = 100;

void setup()
{
	Serial.begin(115200);
	SPI.setMISO(hardware.configSystem.SPI_CIPO);
	SPI.setMOSI(hardware.configSystem.SPI_COPI);
	SPI.setSCLK(hardware.configSystem.SPI_SCLK);
	controller.init();

	// TODO: Remove this hardcoded enable for the only axis I have wired up
	controller.axes[2].setEnable(0x1);
}

void loop()
{
	// hardware.configSystem.eStopPin being low (NC to GND through estop switch) is the only time the loop should run
	// If the pinEStop is ever high (aka not NC to GND through estop switch), reset the whole system
	if (digitalRead(hardware.configSystem.eStopPin)){
		NVIC_SystemReset();
	} else {
		communicator.processLoop();
		if (communicator.checkForData()) {
			handler.processLoop();
		}
		controller.processLoop();
	}
	// report();
}

void report()
{
	long now = millis();
	if (now - timestamp > interval) {
		// reportAngles();
		timestamp = millis();
	}
}

void reportAngles()
{
	Serial.print(controller.axes[2].getAbsoluteAngle(), 5);
	Serial.print("  |  ");
	Serial.println(controller.axes[2].getEncoderAngle(), 5);
}
