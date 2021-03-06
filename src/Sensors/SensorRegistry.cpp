/*
 * PortRegistry.cpp
 *
 *  Created on: Apr 4, 2015
 *      Author: cwenck
 */

#include "Sensors/SensorRegistry.h"

namespace TRL {

Sensor* SensorRegistry::sensors[21];

SensorRegistry::SensorRegistry() {

}

SensorRegistry::~SensorRegistry() {

}

bool SensorRegistry::registerSensor(Sensor* sensor) {
	UniversalPort portOne = sensor->getMainPort();
	UniversalPort portTwo = sensor->getSecondaryPort();

	bool twoPortSensor = sensor->isTwoPortSensor();
	bool portOneOpen = false;
	bool portTwoOpen = false;

	if (portOne <= 20) {
		if (sensors[portOne] == NULL) {
			portOneOpen = true;
		}
		if (!twoPortSensor && portOneOpen) {
			sensors[portOne] = sensor;
			return true;
		} else if (twoPortSensor) {
			if (sensors[portTwo] == NULL) {
				portTwoOpen = true;
			}

			if (portOneOpen && portTwoOpen) {
				sensors[portOne] = sensor;
				sensors[portTwo] = sensor;
				return true;
			}
		}

	}

	println(ERROR, "PortRegistry", "registerPort", "Failed to register port.");
	println(ERROR, "PortRegistry", "registerPort", "Port already taken.");
	return false;
}

bool SensorRegistry::deleteRegistryEntry(UniversalPort port) {
	if (port <= 20) {
		if (sensors[port] == NULL) {
			println(WARNING, "PortRegistry", "deleteRegistryEntry",
					"Port %d was never registered.", port);
			return false;
		}
		sensors[port] = NULL;
		return true;
	}
	return false;
}

bool SensorRegistry::deleteRegistryEntry(AnalogPort port) {
	return deleteRegistryEntry(Port::getUniversalPortNumber(port));
}

bool SensorRegistry::deleteRegistryEntry(DigitalPort port) {
	return deleteRegistryEntry(Port::getUniversalPortNumber(port));
}

void SensorRegistry::resetRegistry() {
	for (int i = 0; i < 21; i++) {
		sensors[i] = NULL;
	}
}

void SensorRegistry::printRegistryEntry(UniversalPort port) {
	if (port <= 20) {
		print(Port::getPortName(port));
		print(": ");
		if (sensors[port] == NULL) {
			println("Not Registered");
		} else {
			println(sensors[port]->getSensorName());
		}
	}
}

void SensorRegistry::printSensorRegistry() {
	delay(5);
	println("|------------------------------|");
	delay(5);
	println("|        PORT REGISTRY         |");
	delay(5);
	println("|------------------------------|");
	for (int i = 0; i <= 21; i++) {
		delay(5);
		printRegistryEntry(i);
		delay(5);
	}
	println("================================");
	delay(5);
}

bool SensorRegistry::isSensorRegistered(UniversalPort port) {
	return !(sensors[port] == NULL);
}

bool SensorRegistry::isSensorRegistered(AnalogPort port) {
	return isSensorRegistered(Port::getUniversalPortNumber(port));
}

bool SensorRegistry::isSensorRegistered(DigitalPort port) {
	return isSensorRegistered(Port::getUniversalPortNumber(port));
}

short SensorRegistry::getNumberOfRegisteredSensors() {
	int count = 0;
	for (int i = 0; i < 21; i++) {
		if (sensors[i] != NULL) {
			count++;
		}
	}
	return count;
}

Array<Sensor*>* SensorRegistry::getRegisteredSensorsArray() {
	Array<Sensor*>* registeredSensors = new Array<Sensor*>(getNumberOfRegisteredSensors());
	int currentFillingIndex = 0;
	for (int i = 0; i < 21; i++) {
		if (sensors[i] != NULL) {
			registeredSensors->at(currentFillingIndex) = sensors[i];
			currentFillingIndex++;
		}
	}
	return registeredSensors;
}

short SensorRegistry::getNumberOfRegisteredSensorsWithoutDuplicates() {
	int numSensors = getNumberOfRegisteredSensors();
	Array<Sensor*>* registeredSensors = getRegisteredSensorsArray();
	int numDuplicates = 0;
	for (int i = 0; i < numSensors; i++) {
		if (registeredSensors->at(i)->isTwoPortSensor()) {
			numDuplicates++;
		}
	}
	numDuplicates /= 2;
	return numSensors - numDuplicates;
}

Array<Sensor*>* SensorRegistry::getRegisteredSensorsArrayWithoutDuplicates() {
	int size = getNumberOfRegisteredSensorsWithoutDuplicates();

	Array<Sensor*>* sensorsWithoutDuplicates = new Array<Sensor*>(size);
	Array<Sensor*>* allSensors = getRegisteredSensorsArray();

	int currentFillingIndex = 0;
	for (int i = 0; i < getNumberOfRegisteredSensors(); i++) {
		Sensor* sensorToCheckFor = allSensors->at(i);
		bool foundInArrayAlready = false;
		for (int j = 0; j < size; j++) {
			if(sensorsWithoutDuplicates->at(i) == sensorToCheckFor){
				foundInArrayAlready = true;
				break;
			}
		}
		if(!foundInArrayAlready){
			sensorsWithoutDuplicates->at(currentFillingIndex) = sensorToCheckFor;
			currentFillingIndex++;
		}
	}
	delete allSensors;
	return sensorsWithoutDuplicates;
}

} /* namespace TRL */
