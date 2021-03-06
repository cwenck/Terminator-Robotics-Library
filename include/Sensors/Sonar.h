/*
 * Sonar.h
 *
 *  Created on: Apr 3, 2015
 *      Author: cwenck
 */

#ifndef SONAR_H_
#define SONAR_H_

#include "../TRL/TRL_BaseIncludes.h"
#include "SensorRegistry.h"
#include "Sensor.h"

#define SONAR_MAX_CM 200
#define SONAR_MAX_INCH (SONAR_MAX_CM/2.54f)

namespace TRL {

	class Sonar : public Sensor {
	private:
		DigitalPort inputPort;
		DigitalPort outputPort;
		PROS_Sonar prosSonar;
		SonarUnit unit;
		UnefinedBehavior behavior;

	public:
		Sonar();
		Sonar(DigitalPort inputPort, DigitalPort outputPort);
		virtual ~Sonar();

		bool destroy();

		int getValue();
		int getValueInInches();
		int getValueInCM();

		Sonar* setUndefinedReturnBehavior(UnefinedBehavior behavior);
		Sonar* setUnit(SonarUnit unit);

		SensorType getSensorType();
		string getSensorName();

	};
}

#endif /* SONAR_H_ */
