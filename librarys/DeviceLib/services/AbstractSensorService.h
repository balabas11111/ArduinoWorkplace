/*
 * AbstractSensorService.h
 *
 *  Created on: Feb 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_SERVICES_ABSTRACTSENSORSERVICE_H_
#define LIBRARIES_DEVICELIB_SERVICES_ABSTRACTSENSORSERVICE_H_

#include <AbstractSensor.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#include <ESP_Consts.h>
#include <StatusMessage/StatusMessage.h>

//File path constants
const PROGMEM char ABSTRACT_SENSORS_BASE_PATH[]="/settings/sensors/";
const PROGMEM char TXT[]=".txt";
const PROGMEM char SLASH[]="/";

class AbstractSensorService {
public:
	AbstractSensorService(AbstractSensor** sensors,uint8_t sensorsSize);
	virtual ~AbstractSensorService();

	AbstractSensor** getSensors();
	uint8_t getSensorsCount();

	void printAbstractSensorServiceDetails();

	AbstractSensor* getSensorByName(String sensorName);
	AbstractSensor* getSensorById(uint8_t sensorId);

	void updateSensors();

	boolean loadSensors();
	boolean saveSensors();

	StatusMessage putAbstractSensorFromJson(String json,boolean setDescr,boolean setVals,boolean save);
	boolean loadAbstractSensorFromFile(AbstractSensor* sensor);
	boolean saveAbstractSensorToFile(AbstractSensor* sensor);


	static String getAbstractSensorFilePath(AbstractSensor* sensor);
	static String getAbstractSensorFileFolder(AbstractSensor* sensor);

protected:
	boolean loadSensorSettingsFromFile(AbstractSensor** sensors,uint8_t size);
	boolean saveSensorSettingsToFile(AbstractSensor** sensors,uint8_t size);

	static String getAbstractSensorFilePreffix(AbstractSensor* sensor);

	String getAbstractSensorsAsString();

private:
	AbstractSensor** sensors;
	uint8_t sensorsCount;

};

#endif /* LIBRARIES_DEVICELIB_SERVICES_ABSTRACTSENSORSERVICE_H_ */