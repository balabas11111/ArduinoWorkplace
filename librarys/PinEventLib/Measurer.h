/*
 * Measurer.h
 *
 *  Created on: 14 îêò. 2018 ã.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_MEASURER_H_
#define LIBRARIES_MEASURER_MEASURER_H_

#include <ESP_Consts.h>
#include "Arduino.h"
#include <Measureable.h>
#include "FunctionalInterrupt.h"
#include <PinEventProcessor.h>
#include <Initializable.h>

class Measurer: public Measureable,public PinEventProcessor,public Initializable{

public:

	virtual void getExternal()=0;

	Measurer(String _id,String _name,String _kind,String _val,String _descr,uint8_t _count,boolean _quoteValue)
	:Measureable(_id, _name, _kind, _val,_descr){
		itemsCount=_count;
		lastMeasureTime=-120000;
		minMeasureInterval=120000;
		quoteValue=_quoteValue;

		items=new Measureable[itemsCount];

		initialized=false;
	}

	String getChildrenJson(){

		//Serial.println("Measure childrens of "+name);

		String result="{";

		for(uint8_t i=0;i<itemsCount;i++){
			result+="\""+String(items[i].id)+"\":";
			result+="\""+items[i].val+"\"";
			if(i!=itemsCount-1){
				result+=",";
			}
		}

		result+="}";
		return result;
	}

	String getName() override{
		return name;
	}

	boolean loop(){
		if(initialized){
			measure();
		}

		return true;
	}

	Measureable* getItems(){
		return items;
	}

	Measureable* measure(){
		uint now=millis();
		if(lastMeasureTime+minMeasureInterval<millis()){
			Serial.println("-measure "+name);
			getExternal();
			lastMeasureTime=now;
		}else{
			ulong left=(lastMeasureTime+minMeasureInterval-now)/1000;
			Serial.print("-measure is too often left=");
			Serial.print(String(left));
			Serial.print(" name=");
			Serial.println(name);
		}
		return getItems();
	}

	uint getLastMeasureTime(){
		return millis()-lastMeasureTime;
	}

	String getMeasurableAsJson(){
		return getMeasurableAsJson(String(getLastMeasureTime()));
	}

	String getMeasurableAsJson(String timeStamp){
		String result="{"+getMeasuableAsJson(*(this),true)+","
						+getStringAsJson("itemsCount", String(itemsCount), false)+","
						+getStringAsJson("time",timeStamp)+","
						+getStringAsJson("items",getMeasurableArrayAsJson(measure(),itemsCount,quoteValue),false)+
						"}";

		return result;
	}

	boolean processEvent(PinEvent event) override{
		Serial.println(event.getText());
		return false;
	}
	PinEvent processEventNow(PinEvent event) override{
		if(event.isValid())
			if(isTargetOfEvent(event)){
				//Serial.println(name+" processNow "+event.getText());
				Serial.println("Measurer ("+name+") - PinEvent("+event.getText()+")");
				if(event.getKind().equals(PIN_EVENT_STATE_GET)){
					return constructEvent(PIN_EVENT_STATE_CURRENT,true);
				}
			}
		//Serial.println("Not my event");
		return PinEvent();
	}

	PinEvent constructEvent(String evName,boolean bubble) override{
		String str=getMeasurableAsJson();
		//PinEvent res=PinEvent(evName,true,0,0,0,str,name,PIN_EVENT_TARGET_ANY);
		//Serial.println("Event constructed "+res.getText());
		return PinEvent(evName,bubble,0,0,0,str,name,PIN_EVENT_TARGET_ANY);
	}

	boolean isDispatcherOfEvent(PinEvent event) override{
		if(name.equals(event.getDispatcherName())){
			return true;
		}

		return false;
	}

	boolean isTargetOfEvent(PinEvent event) override{
		if(name.equals(event.getTargetName())){
			return true;
		}

		return false;
	}
private:

	String getMeasurableArrayAsJson(Measureable* ar,uint count,boolean _quoteValue){
		String result="[";

		for (uint i=0;i<count;i++){
			result+="{"+getMeasuableAsJson(ar[i],_quoteValue)+"}";
			if(i!=count-1){
				result+=",";
			}
		}

		result+="]";

		return result;
	}

	String getMeasuableAsJson(boolean _quoteValue){
		return getMeasuableAsJson(*(this),_quoteValue);
	}

	String getMeasuableAsJson(Measureable m,boolean _quoteValue){
		return
				getStringAsJson(VAR_NAME(m.id),String(m.id))+","+
				getStringAsJson(VAR_NAME(m.name),m.name)+","+
				getStringAsJson(VAR_NAME(m.val),m.val,_quoteValue)+","+
				getStringAsJson(VAR_NAME(m.kind),m.kind)+","+
				getStringAsJson(VAR_NAME(m.descr),m.descr);
	}

	String getStringAsJson(String nodeName,String nodeVal){
		return getStringAsJson(nodeName, nodeVal, true);
	}

	String getStringAsJson(String nodeName,String nodeVal,boolean useQuotes){
		String quotes=useQuotes?"\"":"";
		return "\""+nodeName+"\":"+quotes+nodeVal+quotes;
	}

protected:
	boolean initialized;
	uint itemsCount;
	Measureable* items;

	ulong minMeasureInterval;
	ulong lastMeasureTime;
	boolean quoteValue;

	String getWsJson(){
			return "{\"wsId\":\""+getName()+"\",\"wsClass\":\"wsParent\",\"items\":"+getChildrenJson()+"}";
		}
};

#endif /* LIBRARIES_MEASURER_MEASURER_H_ */
