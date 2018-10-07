/*
 * ButtonB.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "ButtonB.h"

ButtonB::ButtonB(uint8_t pin){
	init("",pin,INPUT,[this](AbstractEvent){return handleInternalFunction();},RISING,true,true);
}

ButtonB::ButtonB(String name,uint8_t pin,uint8_t _pinMode,std::function<String(AbstractEvent)> funcEvent,int _buttonMode,boolean _resetActionFlagAfterExecution,boolean _setActionOnlyIfChangedState){
	init(name,pin,_pinMode,funcEvent,_buttonMode,_resetActionFlagAfterExecution,_setActionOnlyIfChangedState);
}

ButtonB::ButtonB(String name,uint8_t pin,std::function<String(AbstractEvent)> funcEvent,boolean _setActionOnlyIfChangedState){
	init(name,pin,INPUT,funcEvent,CHANGE,true,_setActionOnlyIfChangedState);
	//init(name,pin,INPUT,funcEvent,FALLING,true,true);
}

ButtonB::ButtonB(String name,uint8_t pin,std::function<String(AbstractEvent)> funcEvent){
	init(name,pin,INPUT,funcEvent,CHANGE,true,true);
	//init(name,pin,INPUT,funcEvent,FALLING,true,true);
}

void ButtonB::init(String name,uint8_t pin,uint8_t _pinMode,std::function<String(AbstractEvent)> funcEvent,int _buttonMode,boolean _resetActionFlagAfterExecution,boolean _setActionOnlyIfChangedState){
	Serial.println("Start init button "+name);
	_pin=pin;
	_name=name;
	pinMode(_pin, _pinMode);
	attachInterrupt(_pin, [this](){processInterrupt();}, _buttonMode);
	externalFunctionEvent=funcEvent;

	_previousStatePressed=getPressed();
	_changedToPressed=false;

	resetActionFlagAfterExecution=_resetActionFlagAfterExecution;
	setActionOnlyIfChangedState=_setActionOnlyIfChangedState;
	actionFlagOn=false;
	actionFlagOff=false;

	Serial.println("Button "+_name+" initiated on pin "+_pin);
}

uint8_t ButtonB::getPin(){
	return _pin;
}

boolean ButtonB::getPressed(){
	return (digitalRead(_pin) == HIGH)?true:false;
}

void ButtonB::setPressed(boolean pressed){
	if(_previousStatePressed!=pressed){
		_previousStatePressed=pressed;
	}

	if(pressed && !_previousStatePressed){
		_changedToPressed=true;
	}

	if(!pressed && _previousStatePressed){
		_changedToPressed=false;
	}
}
boolean ButtonB::isStateChanged(){
	boolean result=getPressed()!=_previousStatePressed;
	Serial.print(" Changed="+String(result)+" previous="+_previousStatePressed+" current="+getPressed());

	return result;
}

void ButtonB::setActionFlagOn(boolean actionFlagVal){
	actionFlagOn=actionFlagVal;
}

void ButtonB::setActionFlagOff(boolean actionFlagVal){
	actionFlagOff=actionFlagVal;
}

boolean ButtonB::getActionFlagOn(boolean doReset){
	boolean result=actionFlagOn;
	if(result && doReset){
		actionFlagOn=false;
	}
	return result;
}

boolean ButtonB::getActionFlagOff(boolean doReset){
	boolean result=actionFlagOff;
		if(result && doReset){
			actionFlagOff=false;
		}
		return result;
}

void ButtonB::processInterrupt(){
	boolean nowPressed=getPressed();

	Serial.print("Interrupt "+_name+" pin="+_pin+" state="+nowPressed);

	if((setActionOnlyIfChangedState && isStateChanged()) || !setActionOnlyIfChangedState){
		Serial.println("...event dispatched");
		setActionFlagOn(nowPressed);
		setActionFlagOff(!nowPressed);
	}else{
		Serial.println("...no event");
	}

	setPressed(nowPressed);
}

boolean ButtonB::processEvent(AbstractEvent event){
	event.print();
	return false;
}

boolean ButtonB::isDispatcherOfEvent(AbstractEvent event){

	if(_name.equals(event.getDispatcher())){
		return true;
	}

	return false;
}

boolean ButtonB::loop(){
	boolean result=false;

	if(getActionFlagOn(resetActionFlagAfterExecution)){
		handleExternalFunction(1);
		result=true;
	}

	if(getActionFlagOff(resetActionFlagAfterExecution)){
		handleExternalFunction(0);
		result=true;
	}

	return result;
}

void ButtonB::handleExternalFunction(uint8_t isTurnedOn){
	if(externalFunctionEvent!=NULL){
		Serial.print("Button "+_name+" isTurnedOn="+isTurnedOn);
		externalFunctionEvent(constructEvent(isTurnedOn));
		Serial.println(" ...completed ");
	}
}

String ButtonB::handleInternalFunction(){
	Serial.println("Internal handle function executed");
	return "NotImplemented";
}

AbstractEvent ButtonB::constructEvent(uint8_t state){
	return AbstractEvent(COMMAND_PREFFIX_BUTTON_STATECHANGED,_pin,state,"StateChanged",_name);
}


