/*
 * PortRegistry.h
 *
 *  Created on: Apr 4, 2015
 *      Author: cwenck
 */

#ifndef PORTREGISTRY_H_
#define PORTREGISTRY_H_

#include "Sensor.h"
#include "../TRL/TRL_BaseIncludes.h"

namespace TRL {

	class Sensor;

	class SensorRegistry {
	private:
		static Sensor* sensors[21];
	public:
		SensorRegistry();
		virtual ~SensorRegistry();

		static void resetRegistry();
		static void printRegistryEntry(UniversalPort port);
		static void printSensorRegistry();

		static bool deleteRegistryEntry(UniversalPort port);
		static bool deleteRegistryEntry(AnalogPort port);
		static bool deleteRegistryEntry(DigitalPort port);

		static bool registerSensor(Sensor* sensor);

		static bool isSensorRegistered(UniversalPort port);
		static bool isSensorRegistered(AnalogPort port);
		static bool isSensorRegistered(DigitalPort port);

		static short getNumberOfRegisteredSensors();
		static short getNumberOfRegisteredSensorsWithoutDuplicates();

		//The full array of registered motors is returned
		//This is a copy of the array so it is readOnly
		//and should be deleted with delete
		static Array<Sensor*>* getRegisteredSensorsArray();
		static Array<Sensor*>* getRegisteredSensorsArrayWithoutDuplicates();

	};
}

#endif /* PORTREGISTRY_H_ */
