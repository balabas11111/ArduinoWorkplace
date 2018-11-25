/*
 * ButtonB.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "PinDigital.h"
#include "TimeTrigger.h"

PinDigital::~PinDigital(){
	Serial.println("destructed");
}

void PinDigital::construct(uint8_t pin,std::function<void(void)> onChanged,uint8_t pinModeInOut,uint8_t changeMode,uint8_t pinVal,uint8_t turnOffLevel,long clickMaxTime){
	this->pin=pin;
	this->onChanged=onChanged;
	this->pinModeInOut=pinModeInOut;
	this->changeMode=changeMode;
	this->pinVal=pinVal;
	this->turnOffLevel=turnOffLevel;
	changed=false;

	oldVal=getVal();

	pinMode(pin, pinModeInOut);

	if(pinModeInOut==OUTPUT){
		digitalWrite(pin, pinVal);
	}else{
		attachInterrupt(pin, [this](){processInterrupt();}, changeMode);
	}

	oldVal=getVal();
	this->turnOffLevel=turnOffLevel;
	float val=isOn();

	processValueFromMqtt=(pinModeInOut==OUTPUT);

	items[0]={0,name,type,size,descr,val,0,-2,2,"",processValueFromMqtt};

	isClick=(clickMaxTime!=0);

	if(isClick){
		clickTrigger=new TimeTrigger(0,clickMaxTime,false, [this](){processClick(true);});
	}
}

uint16_t PinDigital::getVal(){
	return digitalRead(pin);
}

bool PinDigital::setVal(uint8_t _val){
	if(pinModeInOut==OUTPUT){

		if(_val!=getVal()){
			digitalWrite(pin, _val);
			//dispatchState=true;
		}
		items[0].val=isOn();
		return _val==digitalRead(pin);
	}
	return false;
}

void PinDigital::processClick(boolean fromTimer){
	if(!fromTimer){
		int8_t on=isOn();

		if(!on && clickTrigger->checkIsBeforeTrigger()){
			Serial.println("CLick Finished");
			clickTrigger->stop();
			changed=true;

			return;
		}

		if(on && !clickTrigger->isActive()){
			Serial.println("CLick started");
			clickTrigger->start();

			return;
		}
	}

	Serial.println("Timer stopped");
	clickTrigger->stop();
}

void PinDigital::processInterrupt(){

	uint16_t now=getVal();
	items[0].val=isOn();

	/*Serial.print("interrupted ");	Serial.println(name);*/
	//Serial.println(" old="+String(oldVal)+" now="+String(now));

	if(now!=oldVal){
		if(isClick==false){
			changed=true;
		}else{
			processClick(false);
		}
		#ifdef DIGITAL_PIN_DISPLAY_CHANGE_EVENT
			Serial.print(printState()+" now="+String(now));
			Serial.println("...event dispatched");
		#endif
	}else{
		//Serial.println(" not changed");
		#ifdef DIGITAL_PIN_DISPLAY_NO_CHANGE_EVENT
		Serial.print(printState()+" now="+String(now));
		Serial.println("...no event");
		#endif
	}
	oldVal=now;
}

boolean PinDigital::handleLoop(){
	bool result=false;
	#ifdef DISPLAY_LOOPS
		Serial.println("Digital pin loop ");
	#endif

	if(changed){
		if(onChanged!=nullptr){
			onChanged();
		}

		changed=false;
		result=true;
	}
	if(dispatchState){
		if(onChanged!=nullptr){
			onChanged();
		}
		dispatchState=false;
		result=true;
	}

	return result;
}

uint8_t PinDigital::change(){
	setVal(getOpposite(getVal()));

	return getVal();
}
uint8_t PinDigital::changeAndDelay(uint delayTime){
	change();
	delay(delayTime);

	return getVal();
}

uint8_t PinDigital::changeAndDelay(uint delayTime,uint _count){
	if(_count<1){
		return changeAndDelay(delayTime);
	}

	for(uint8_t i=1;i<=_count;i++){
		changeAndDelay(delayTime);
	}

	return getVal();
}

uint8_t PinDigital::turnOn(){
	setVal(getOpposite(turnOffLevel));

	return getVal();
}
uint8_t PinDigital::turnOff(){
	setVal(turnOffLevel);

	return getVal();
}

uint8_t PinDigital::turnOnOff(boolean _turnOn){
	if(_turnOn){
		return turnOn();
	}else{
		return turnOff();
	}
}

boolean PinDigital::isOn(){
	return turnOffLevel!=getVal();
}

void PinDigital::setTurnOffLevel(uint8_t turnOffLevel) {
	this->turnOffLevel=turnOffLevel;
}

uint8_t PinDigital::getOpposite(uint8_t _val){
	return (_val==HIGH)?LOW:HIGH;
}
