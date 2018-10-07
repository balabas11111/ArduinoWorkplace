/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_BUTTONB_H_
#define LIBRARIES_TIMETRIGGER_BUTTONB_H_

#define COMMAND_PREFFIX_BUTTON_STATECHANGED "PIN_STATECHANGED"
#define COMMAND_PREFFIX_BUTTON_GET_STATE "PIN_STATECHANGED"

#include "Arduino.h"
#include "AbstractEvent.h"
#include <FunctionalInterrupt.h>

class ButtonB{

public:
	ButtonB(uint8_t pin);
	ButtonB(String name,uint8_t pin,std::function<String(AbstractEvent)> funcEvent);
	ButtonB(String name,uint8_t pin,std::function<String(AbstractEvent)> funcEvent,boolean _setActionOnlyIfChangedState);
	ButtonB(String name,uint8_t pin,uint8_t _pinMode,std::function<String(AbstractEvent)> funcEvent,int _buttonMode,boolean _resetActionFlagAfterExecution,boolean _setActionOnlyIfChangedState);
	uint8_t getPin();
	boolean isStateChanged();
	boolean getPressed();
	void processInterrupt();

	boolean processEvent(AbstractEvent event);
	boolean isDispatcherOfEvent(AbstractEvent event);

	boolean loop();

private:
	String _name;
	uint8_t _pin;
	boolean _previousStatePressed;
	boolean _changedToPressed;
	boolean actionFlagOn;
	boolean actionFlagOff;
	boolean resetActionFlagAfterExecution;
	boolean setActionOnlyIfChangedState;
	std::function<String(AbstractEvent)> externalFunctionEvent;

	void init(String name,uint8_t pin,uint8_t _pinMode,std::function<String(AbstractEvent)> funcEvent,int _buttonMode,boolean _resetActionFlagAfterExecution,boolean _setActionOnlyIfChangedState);
	void setPressed(boolean pressed);
	void handleExternalFunction(uint8_t isTurnedOn);
	String handleInternalFunction();
	AbstractEvent constructEvent(uint8_t state);

	boolean getActionFlagOn(boolean doReset);
	boolean getActionFlagOff(boolean doReset);

	void setActionFlagOn(boolean actionFlagVal);
	void setActionFlagOff(boolean actionFlagVal);
};


#endif /* LIBRARIES_TIMETRIGGER_BUTTONB_H_ */
