/*
 * TimeIntervalService.h
 *
 *  Created on: 24 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_TIME_TIMEINTERVALSERVICE_H_
#define LIBRARIES_HELPERS_TIME_TIMEINTERVALSERVICE_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Time/NtpTimeClientService.h"
#include "EspSettingsBox.h"
#include "JSONprovider.h"
#include "Loopable.h"
#include "Initializable.h"
#include "FunctionalInterrupt.h"
#include "DS3231.h"
#include "TimeTrigger.h"

#define ALARM_SIZE 10
#define SEC_IN_DAY 24*60*60
#define SEC_IN_WEEK 7*24*60*60

const char* const IntervalState_Names[] PROGMEM={
		"NEW", "WAIT", "ACTIVE", "FINISHED", "TO_DELETE", "MISSED", "FAILED"
};
const char* const IntervalCompNow_Names[] PROGMEM={
		"NOW_IN_FUTURE_OF_INTERVAL", "NOW_IN_INTERVAL", "NOW_IN_PAST_OF_INTERVAL"
};
const char* const IntervalType_Names[] PROGMEM={
		"ONCE", "PERIODIC", "DAILY", "MULTIDAILY", "WEEKLY", "MONTHLY", "QUATERLY"
};

const char TimeIntervalService_ID[] PROGMEM ="123";
const char TimeIntervalService_NAME[] PROGMEM ="TimeIntervalService";
const char TimeIntervalService_DESCR[] PROGMEM ="Service to schedule periodic executions";

typedef enum {
  NOW_IN_FUTURE_OF_INTERVAL, NOW_IN_INTERVAL, NOW_IN_PAST_OF_INTERVAL
} IntervalCompNow;

typedef enum {
  ONCE, PERIODIC, DAILY, MULTIDAILY, WEEKLY, MONTHLY, QUATERLY
} IntervalType;

typedef enum {
  NEW, WAIT, ACTIVE, FINISHED, TO_DELETE, MISSED, FAILED
} IntervalState;

typedef struct timeIntervalDetail{
  uint8_t id;
  String name;
  uint8_t type;
  uint32_t startTime;
  uint32_t endTime;
  uint32_t time;
  uint8_t state;
  String days;
  String param;
}TimeIntervalDetail;

class TimeIntervalService: public Initializable, public Loopable, public JSONprovider {
public:
	virtual ~TimeIntervalService(){};

	TimeIntervalService(EspSettingsBox* espSettingsBox,NtpTimeClientService* ntpTimeClientService,std::function<void(TimeIntervalDetail)> externalFunction){
		this->espSettingsBox=espSettingsBox;
		this->timeService=ntpTimeClientService;
		this->waitForTimeReceive=false;
		this->externalFunction=externalFunction;
	}

	virtual boolean initialize(boolean _init){
		Serial.println(FPSTR("---Init TimeIntervalService---"));
		if(!isTimeServiceHasTime()){
			if(_init){
				Serial.println(FPSTR("Wait for TimeService"));
				waitForTimeReceive=true;
			}
			return false;
		}else{
			return finishInit();
		}

		return initialized;
	}

	virtual boolean loop(){
		if(waitForTimeReceive && isTimeServiceHasTime()){
			finishInit();
		}

		alarmSwitcer.loop();
		processIntervals();

		return true;
	}

	ulong now(){
		return timeService->getnow();
	}

	boolean finishInit(){
		Serial.println(FPSTR("---Finish TimeIntervalService initialization"));
		loadFromFile();
		processIntervals();

		waitForTimeReceive=false;

		addTestInterval();

		return true;
	}

	TimeIntervalDetail getInterval(uint8_t ind){
		if(!isInitComplete() || ind>=itemCount){
			ind=itemCount;
		}

		return items[ind];
	}

	virtual String getName(){
		return FPSTR("TimeIntervalService");
	}

	virtual String getJson(){
		String result="{"+getItemJson()+",\"items\":[";

			for(uint8_t i=0;i<itemCount;i++){
					result+=getItemJson(i);
					if(i!=itemCount-1){
						result+=",";
					}
			}

			result+="]}";

		return result;
	}

	virtual void add(uint32_t start,uint32_t end,IntervalType type,uint16_t time,String days){
		items[itemCount].state=WAIT;
		items[itemCount].type=type;
		items[itemCount].startTime=start;
		items[itemCount].endTime=end;
		items[itemCount].time=type==PERIODIC?time:0;
		items[itemCount].days=type==MULTIDAILY?days:"";

		Serial.println(FPSTR("Interval added"));
		printTimeInterval(items[itemCount]);
		itemCount++;
		saveToFile();
	}

	void addTestInterval(){
		itemCount=0;

		uint32_t start=DateTime(2019,1,1,22,0,0).unixtime();

		ulong testTime=timeService->getnow()+40;
		add(start,start+50,ONCE,0,"");
	}

	int getActiveAlarms(){
		for(uint8_t i=0;i<itemCount;i++){
			if(items[i].state==ACTIVE){
				return i;
			}
		}

		return -1;
	}

	boolean getAlarmActive(){
		return alarmActive;
	}

	void stopAlarm(){
		alarmSwitcer.stop();
		alarmActive=false;
	}

protected:
	boolean loadFromFile(){

		if(!espSettingsBox->isSettingsFileExists(FPSTR(TimeIntervalService_NAME))){
			Serial.println(FPSTR("---TimeInterval file NOT exists---"));
			return false;
		}

		String fileName=espSettingsBox->getSettingsFileFileName(FPSTR(TimeIntervalService_NAME));

		Serial.print(FPSTR("Load TimeIntervals from file "));
		Serial.println(fileName);

     	StaticJsonBuffer<1024> jsonBuffer;
		delay(1);

		File file = SPIFFS.open(fileName, "r");
		size_t size = file.size();

		std::unique_ptr<char[]> buf (new char[size]);
		file.readBytes(buf.get(), size);
		JsonObject& root = jsonBuffer.parseObject(buf.get());

		if (!root.success()) {
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
		  }else{
			Serial.println();
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_READ_FROM_FILE_COMPLETE));
			String vals="";
			root.printTo(vals);
			Serial.println(vals);

			itemCount=root["itemCount"];

			Serial.print(FPSTR("itemCount="));
			Serial.println(itemCount);

			for(uint8_t i=0;i<itemCount;i++){
				items[i].id=i;
				items[i].name=root["items"][i]["name"].as<char*>();
				items[i].type=root["items"][i]["typeInt"];
				items[i].state=root["items"][i]["type"];
				items[i].startTime=root["items"][i]["minVal"];
				items[i].endTime=root["items"][i]["maxVal"];
				items[i].days=root["items"][i]["days"].as<char*>();
				items[i].param=root["items"][i]["param"].as<char*>();

				Serial.println(getItemJson(i));
			}

			Serial.println(FPSTR("TimeIntervalService load complete"));
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		}

		  file.close();
		  return true;
	}

	void saveToFile(){
		Serial.println();
		espSettingsBox->saveSettingToFile(FPSTR(TimeIntervalService_NAME),getJson());
	}

	void processIntervals(){
		boolean doSave=false;

		uint8_t dispatched=0;
		uint8_t rescheduled=0;
		uint8_t updated=0;

		for(uint8_t i=0;i<itemCount;i++){
			ulong now=timeService->getnow();

			boolean reschedule=false;
			boolean dispatch=false;
			boolean update=false;


			switch(items[i].state){
				case NEW: reschedule=true; break;
				case WAIT:
					{	IntervalCompNow compRes=compareIntervalAndNow(now,items[i]);
						switch(compRes){
							case NOW_IN_PAST_OF_INTERVAL:
									Serial.println(FPSTR("WAIT is in past"));
									setState(i,FINISHED);
									reschedule=true;
									break;
							case NOW_IN_INTERVAL:
									Serial.println(FPSTR("WAIT is in interval"));
									setState(i,ACTIVE);
									dispatch=true;
									break;
						}
						break;
					}
				case ACTIVE:
					{	IntervalCompNow compRes=compareIntervalAndNow(now,items[i]);
						switch(compRes){
							case NOW_IN_PAST_OF_INTERVAL:
									Serial.println(FPSTR("ACTIVE is in past"));
									setState(i,FINISHED);
									reschedule=true;
									dispatch=true;
									break;
						}
						break;
					}
				case FINISHED:
					{	IntervalCompNow compRes=compareIntervalAndNow(now,items[i]);
						switch(compRes){
							case NOW_IN_PAST_OF_INTERVAL:
									Serial.println(FPSTR("FINISHED is in past"));
									reschedule=true;
									break;
							case NOW_IN_INTERVAL:
									Serial.println(FPSTR("FINISHED is in interval"));
									setState(i,ACTIVE);
									dispatch=true;
									break;
							case NOW_IN_FUTURE_OF_INTERVAL:
									Serial.println(FPSTR("FINISHED is in future"));
									setState(i,WAIT);
									update=true;
									break;
						}
						break;
					}

			}

			doSave= reschedule || update;

			if(dispatch || doSave){
				Serial.print(FPSTR("check interval "));
				printTimeInterval(items[i]);
			}

			if(update){
				updated++;
			}

			if(dispatch){
				dispatched++;
				dispatchExternalFunction(i);
			}

			if(reschedule){
				doSave=scheduleNextExecution(i) || doSave;
				rescheduled++;
				Serial.print(FPSTR("reschedule "));
				Serial.println(getItemJson(i));
			}

		}

		if(doSave){
			saveToFile();

			Serial.print(FPSTR(" updated="));
			Serial.print(updated);
			Serial.print(FPSTR(" rescheduled="));
			Serial.print(rescheduled);
			Serial.print(FPSTR(" dispatched="));
			Serial.println(dispatched);
		}
	}

	void dispatchExternalFunction(uint8_t ind){
		Serial.print(FPSTR("dispatch "));
		Serial.println(getItemJson(ind));

		boolean started=items[ind].state==ACTIVE;
		alarmActive=started;

		this->alarmSwitcer.setActive(started);

		Serial.print(FPSTR("alarmActive="));
		Serial.println(alarmActive);

		if(externalFunction!=nullptr){
			externalFunction(items[ind]);
		}
	}

	boolean scheduleNextExecution(uint8_t ind){

		boolean result=false;

		Serial.println("Before change");
		printTimeInterval(items[ind]);

		if(items[ind].type==ONCE){
			items[ind].state=TO_DELETE;
			result= true;
		}
		if(items[ind].type==PERIODIC){
			items[ind].startTime+=items[ind].time;
			items[ind].endTime+=items[ind].time;
			items[ind].state=WAIT;
			result= true;
		}
		if(items[ind].type==DAILY){
			items[ind].startTime+=SEC_IN_DAY;
			items[ind].endTime+=SEC_IN_DAY;
			items[ind].state=WAIT;
			result= true;
		}
		if(items[ind].type==MULTIDAILY){
			int8_t days[7]={0,0,0,0,0,0,0};

			String d=items[ind].days;
			uint8_t lastPos=0;

			for(uint8_t i=0;i<d.length();i++){
				if(d.substring(i,i+1)==","){
					uint8_t day=d.substring(lastPos,i-1).toInt();
					days[day]=1;
				}
			}

			for(uint8_t i=0;i<7;i++){
				Serial.print(days[i]);
			}

			Serial.println();

			DateTime dt1=DateTime(items[ind].startTime);
			uint8_t todayDay=dt1.dayOfTheWeek();

			Serial.print("todayDay=");
			Serial.println(todayDay);

			int nextDay=-1;

			for(uint8_t i=todayDay+1;i<7;i++){
				if(days[i]==1){
					nextDay=i;
					break;
				}
			}

			if(nextDay==-1){
				for(uint8_t i=0;i<=todayDay;i++){
					if(days[i]==1){
						nextDay=i;
						break;
					}
				}
			}

			Serial.print("nextDay=");
			Serial.println(nextDay);

			uint8_t dayDif=0;

			if(nextDay>todayDay){
				dayDif=nextDay-todayDay;
			}else{
				dayDif=6-todayDay+nextDay+1;
			}

			Serial.print("dayDif=");
			Serial.println(dayDif);

			items[ind].startTime+=dayDif*SEC_IN_DAY;
			items[ind].endTime+=dayDif*SEC_IN_DAY;
			items[ind].state=WAIT;
			result= true;
		}
		if(items[ind].type==WEEKLY){
			items[ind].startTime+=7*SEC_IN_DAY;
			items[ind].endTime+=7*SEC_IN_DAY;
			items[ind].state=WAIT;
			result= true;
		}
		if(items[ind].type==MONTHLY){

			DateTime dt1= DateTime(items[ind].startTime);


			uint newMonth=dt1.month();
			uint16_t newYear=dt1.year();

			if(newMonth==12){
				newMonth=1;
				newYear+=1;
			}else{
				newMonth+=1;
			}

			DateTime dt2=DateTime(newYear,newMonth,dt1.day(),dt1.hour(),dt1.minute(),dt1.second());

			uint32_t increment=dt2.unixtime()-items[ind].startTime;

			items[ind].startTime+=increment;
			items[ind].endTime+=increment;
			items[ind].state=WAIT;
			result= true;
		}

		Serial.println("After change");
		printTimeInterval(items[ind]);

		return result;
	}

	void printTimeInterval(TimeIntervalDetail time){
		Serial.print("type=");
		Serial.print(time.type);
		Serial.print(" state=");
		Serial.print(time.state);
		Serial.print(" startTime=");
		printDateTime(time.startTime);
		Serial.print(" endTime=");
		printDateTime(time.endTime);
		Serial.println();
	}

	void printDateTime(uint32_t interval){
		DateTime dt1=DateTime(interval);
		Serial.print(dt1.getFormattedIsoDateTime());
	}



	void onAlarmActiveChange(){
		Serial.println("alarmActive changed");
		alarmActive=!alarmActive;
	}



private:

	boolean alarmActive;
	boolean waitForTimeReceive=false;
	EspSettingsBox* espSettingsBox;
	NtpTimeClientService* timeService;
	TimeTrigger alarmSwitcer=TimeTrigger(0,1000,false,[this](){onAlarmActiveChange();});

	std::function<void(TimeIntervalDetail)> externalFunction;

	uint8_t itemCount=0;
	TimeIntervalDetail items[ALARM_SIZE];

	String getItemJson(){
		return "\"id\":\"123\",\"name\":\"TimeIntervalService\",\"type\":\"ScheduleService\",\"size\":\"1\",\
				\"descr\":\"TimeIntervalService_DESCR\",\"itemCount\":\""+String(itemCount)+"\"";

	}

	String getItemJson(uint8_t index){
		TimeIntervalDetail item=items[index];

		return "{\"id\":\""+String(item.id)+"\","
					+"\"name\":\""+item.name+"\","
					+"\"type\":\""+getTypeName(item.type)+"\","
					+"\"typeInt\":\""+String(item.type)+"\","
					+"\"state\":\""+getStateName(item.state)+"\","
					+"\"stateInt\":\""+String(item.state)+"\","
					+"\"minVal\":\""+String(item.startTime)+"\","
					+"\"maxVal\":\""+String(item.endTime)+"\","
					+"\"days\":\""+item.days+"\","
					+"\"param\":\""+item.param+"\"}";
	}

	String getTypeName(uint8_t id){
		return String(FPSTR(IntervalType_Names[id]));
	}
	String getStateName(uint8_t id){
		return String(FPSTR(IntervalState_Names[id]));
	}

	void setState(uint8_t ind,IntervalState newState){
		items[ind].state=newState;
	}

	IntervalCompNow compareIntervalAndNow(ulong now,TimeIntervalDetail interval){
		if(interval.endTime>now && interval.startTime>now){
			return NOW_IN_FUTURE_OF_INTERVAL;
		}

		if(interval.endTime>=now && now>=interval.startTime){
			return NOW_IN_INTERVAL;
		}

		return NOW_IN_PAST_OF_INTERVAL;
	}

	boolean isTimeServiceHasTime(){
		return timeService->isTimeReceived();
	}

	boolean isInitComplete(){
		return initialized && !waitForTimeReceive;
	}
};

#endif /* LIBRARIES_HELPERS_TIME_TIMEINTERVALSERVICE_H_ */
