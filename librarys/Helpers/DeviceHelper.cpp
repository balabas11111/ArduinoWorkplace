/*
 * Looper.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#include <DeviceHelper.h>
#include "ESP_Consts.h"
#include "ESP8266WebServer.h"

DeviceHelper::DeviceHelper(Loopable** _loopItems,uint8_t _loopItemsSize,long minAlarmInterval){
	this->loopItems=_loopItems;
	this->loopItemsSize=_loopItemsSize;

	displayDetails();
	Serial.println();

	this->alarmMode=false;
	this->minAlarmInterval=minAlarmInterval*1000;
	this->lastAlarmTime=0;
}

String DeviceHelper::displayDetails(){
	Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_LOOPERS));

	Serial.print(FPSTR(MESSAGE_DEVICE_HELPER_LOOP_SIZE));
	Serial.println(loopItemsSize);

	for(uint8_t i=0;i<loopItemsSize;i++){
		loopItems[i]->displayDetails();
	}

	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

	return "";
}


boolean DeviceHelper::loop(){
	boolean result=false;

	for(uint8_t i=0;i<loopItemsSize;i++){
		bool currentLoop=loopItems[i]->loop();
		result=result | currentLoop;
	}
	#ifdef DISPLAY_LOOPS
		Serial.println("DeviceHelper loop="+String(result));
	#endif

	return result;
}

void DeviceHelper::startDevice(String deviceId){
  Serial.begin(115200);
  yield();

  Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_STARTED));
  Serial.print(FPSTR(MESSAGE_DEVICE_START_DEVICE_ID));Serial.println(deviceId);
  Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

boolean DeviceHelper::init(Initializable** initItems,uint8_t initItemsSize){
	Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_INITIALIZING));
	Serial.print(FPSTR(MESSAGE_DEVICE_INIT_STARTED_COUNT));Serial.println(initItemsSize);
	uint8_t initOk=0;

	for(uint8_t i=0;i<initItemsSize;i++){
		boolean init=initItems[i]->init();
		if(init){
			initOk++;
		}
	}

	Serial.print(FPSTR(MESSAGE_DEVICE_INIT_COUNT));Serial.println(initOk);
	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
	displayDetails();

	return initOk==initItemsSize;
}

void DeviceHelper::printDeviceDiagnostic(){
	yield();
	Serial.println(FPSTR(MESSAGE_DEVICE_DIAGNOSTIC_BEGIN));
	Serial.print(FPSTR(MESSAGE_DEVICE_FREE_HEAP));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getFreeHeap());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	Serial.print(FPSTR(MESSAGE_DEVICE_CPU_MHZ));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getCpuFreqMHz());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	Serial.print(FPSTR(MESSAGE_DEVICE_FLASH_CHIP_SIZE));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getFlashChipSize());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	Serial.print(FPSTR(MESSAGE_DEVICE_FREE_SCETCH_SPACE));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getFreeSketchSpace());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	Serial.print(FPSTR(MESSAGE_DEVICE_RESET_REASON));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getResetReason());Serial.println(FPSTR(MESSAGE_DOT_COMMA));
	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

void DeviceHelper::update(AbstractItem** sensors, uint8_t sensorsSize){
	Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_UPDATE_EXECUTION));

	for(uint8_t i=0;i<sensorsSize;i++){
		sensors[i]->update();
		sensors[i]->checkForAlarm();
	}

	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
	printDeviceDiagnostic();
}

String DeviceHelper::processAlarm(AbstractItem** sensors, uint8_t sensorsSize){
	unsigned long now=millis();
	//Serial.println("lastAlarmTime="+String(lastAlarmTime)+" minAlarmInterval="+String(minAlarmInterval)+" now="+String(now)+" alarmMode="+String(alarmMode));

	if(minAlarmInterval==0){
		return "";
	}

	if(alarmMode && (lastAlarmTime+minAlarmInterval>now)){
		return "";
	}

	String alarmMessage="";
	boolean alarmStarted=alarmMode;

	for(uint8_t i=0;i<sensorsSize;i++){
		boolean alarm=sensors[i]->checkForAlarm();

		if(alarm){
			if(!alarmStarted){
				alarmStarted=true;
			}
			alarmMode=true;
			alarmMessage+=sensors[i]->generateAlarmText();
		}
	}

	if(alarmMessage!=""){
		if(alarmStarted){
			Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_STARTED));
		}else{
			Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_IDENTIFIED));
		}
		Serial.println(alarmMessage);
		lastAlarmTime=now;
	}else{
		//Serial.println("No alarm identified");
		if(alarmMode){
			Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_FINISHED));
			alarmMode=false;
			alarmMessage=FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_FINISHED_RESULT);
			lastAlarmTime=now;
		}
	}
	if(alarmMessage!=""){
		Serial.println(alarmMessage);
	}

	return alarmMessage;
}

String DeviceHelper::getJson(AbstractItem** sensors, uint8_t size) {
		String result="{\"sensors\":[";

			for(uint8_t i=0;i<size;i++){
				delay(1);
				result+=sensors[i]->getJson();
				if(i!=size-1){
					result+=",";
				}
			}
		result+="]}";

		return result;
}

