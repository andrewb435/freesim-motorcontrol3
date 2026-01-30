#ifndef __FSMC_AXIS_H__
#define __FSMC_AXIS_H__

#include "fsmc_utils.h"
#include "fsmc_driver.h"
#include "fsmc_position.h"
#include "fsmc_pid.h"

namespace FSMC3
{
	class Axis
	{
	private:
		bool debugPosition;
		bool debugPID;
		bool isEnabled;
		bool invertDirection;
		uint8_t commandBitDepth;
		int16_t rangeLow;
		int16_t rangeCenter;
		int16_t rangeHigh;
		double rangeFloat;
		double pidSetpoint;
		double pidInput;
		FSMC3::PID pidController;
		FSMC3::PWMDriver driver;
		FSMC3::Position position;
		void setRangeFloat();
		void debugReport();

	public:
		double pidOutput;
		Axis(FSMC3Config::SystemHW *system_in, FSMC3Config::Axis *axis_in, SPIClass *spi_in);
		void init();
		void processLoop();
		void eepromSetCenter(double data_in);
		/// @brief Takes total range in degrees (e.g. center = 0, max +40, min -40 = 80 degrees)
		/// @param range_in
		void setRange(int16_t range_in);
		/// @brief Enables/Disables axis
		/// @param enable_in >=1 enable, <=0 disable
		void setEnable(int16_t enable_in);
		/// @brief Takes command position in integer 0-(2^commandBitDepth), maps to double
		///	 +1.0 to -1.0, feeds to the PID controller
		/// @param target_in command position in range 0 to 2^commandBitDepth
		void setMoveTarget(int16_t target_in);
		void setP(int16_t valP_in);
		void setI(int16_t valI_in);
		void setD(int16_t valD_in);
		/// @brief Set centerpoint to current encoder position
		/// @param set_in int16_t > 0 = set
		void setCenterToCurrent(int16_t set_in);
		/// @brief Nudge centerpoint +- 1 degree
		/// @param nudge_in int16_t: Positive nudges +1 degree, negative nudges -1 degree
		void nudgeCenter(int16_t nudge_in);
		double getAbsoluteAngle();
		double getEncoderAngle();

		int16_t getEnabled();
		double getCenter();
		int16_t getAbsoluteAngle16();
		int16_t getEncoderAngle16();
		int16_t getMoveTarget16();
		int16_t getAxisP();
		int16_t getAxisI();
		int16_t getAxisD();
	};
	namespace AxisConst
	{
		static const double NUDGE_RAD = (PI / 180);
		static const double NUDGE_MAX = 10;
		static const int16_t IS_ENABLED = 0x1;
		static const int16_t IS_DISABLED = 0x0;
	};
}
#endif // __FSMC_AXIS_H__