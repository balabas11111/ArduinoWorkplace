/*
 * DS18D20Sensor.h
 *
 *  Created on: 3 нояб. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_SENSORSLIB_DS18D20_SENSOR_H_
#define LIBRARIES_SENSORSLIB_DS18D20_SENSOR_H_

#include "Arduino.h"
#include "Initializable.h"
#include "AbstractItem.h"
#include "Initializable.h"
#include "Measurable.h"

#include "ESP_Consts.h"

#include "OneWire.h"
#include "DallasTemperature.h"

const char SENSOR_DS18D20_NAME[] PROGMEM ="DS18D20_Sensor";
const char SENSOR_DS18D20_DESCRIPTION[] PROGMEM ="DS18D20 Sensor";
const char SENSOR_DS18D20_SIZE[] PROGMEM ="C";
const char SENSOR_DS18D20_DESCRIPTION_RU[] PROGMEM ="Температура DS18D20";

const char HEADER_DS18D20[] PROGMEM ="-------DS18D20 initialize------------";
const char INIT_STR_DS18D20[] PROGMEM ="Init DS18D20";

class DS18D20_Sensor: public AbstractItem, public Initializable, public Measurable {
public:
	DS18D20_Sensor(String name, uint8_t pin, uint8_t fieldId,String queueName)
				: AbstractItem(pin,name,FPSTR(SENSOR_DS18D20_DESCRIPTION),FPSTR(SENSOR_DS18D20_SIZE),FPSTR(SENSOR_DS18D20_DESCRIPTION_RU),
						0, 0,fieldId,queueName,-512,512){

		oneWire=new OneWire(pin);
		dallasTemperature=new DallasTemperature(oneWire);
	};

	virtual ~DS18D20_Sensor(){};

	boolean initialize(boolean _init){
		if(_init){
			Serial.println(FPSTR(HEADER_DS18D20));
			Serial.print(FPSTR(INIT_STR_DS18D20));

			initSensor();
			update();

			Serial.println(FPSTR(MESSAGE_DONE));
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		}
		initialized=_init;
		return initialized;
	}

	void initSensor(){
		dallasTemperature->begin();
		dallasTemperature->setDescription(getName());

		childCount=dallasTemperature->getDeviceCount();

		initializeChildren();

		Serial.print("Sensors count=");
		Serial.println(childCount);

		for(uint8_t i=0;i<childCount;i++){

			String devAddressStr=getDeviceAddress(i);
			String name=String(FPSTR(TEMPERATURE))+devAddressStr;

			//uint8_t field=i+1;
			items[i]={i,name,FPSTR(MEASURE_CELSIUS_DEGREES),FPSTR(MEASURE_CELSIUS_DEGREES_RU),devAddressStr,-127,0,-512,512};
		}

		Serial.println("complete init bus");

	}

	void update(){
		dallasTemperature->requestTemperatures();

		for(uint8_t i=0;i<childCount;i++){
			items[i].val=dallasTemperature->getTempCByIndex(i);
		}
	}

	void print(){
		printValues();
	}

private:
	OneWire* oneWire;
	DallasTemperature* dallasTemperature;

	String getDeviceAddress(uint8_t index){
		DeviceAddress deviceAddress;
		dallasTemperature->getAddress(deviceAddress,index);
		uint8_t size=sizeof(deviceAddress);

		String addrStr=deviceAddressToString(deviceAddress,size);

		Serial.print("index=");
		Serial.print(index);
		Serial.print(" address=");
		for(uint8_t k=0;k<size;k++){
			Serial.print(deviceAddress[k]);
		}
		Serial.print(" addrStr=");
		Serial.println(addrStr);

		return addrStr;
	}

	String deviceAddressToString(DeviceAddress deviceAddress,uint8_t size){
		String devAddrStr="";

		for(uint8_t k=0;k<size;k++){
			devAddrStr+=String(deviceAddress[k]);
		}

		return devAddrStr;
	}
};

#endif /* LIBRARIES_SENSORSLIB_DS18D20_SENSOR_H_ */