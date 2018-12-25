/*
 * NtpTimeClientService.h
 *
 *  Created on: 28 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_NTPTIMECLIENTSERVICE_H_
#define LIBRARIES_HELPERS_NTPTIMECLIENTSERVICE_H_

#include "Initializable.h"
#include "Loopable.h"

#include "EspSettingsBox.h"
#include "ESPSett_Ntp.h"
#include "ESPExtraSettingsBox.h"

#include "ESP_Consts.h"
#include "Date_Const.h"
#include "TimeTrigger.h"
#include "WiFiUdp.h"
#include "NTPClient.h"
#include "FunctionalInterrupt.h"

const char NTP_TIME_CLIENT_SERVICE_NAME[] PROGMEM ="NtpTimeClientService";
const char NTP_TIME_CLIENT_SERVICE_TYPE[] PROGMEM ="NtpTime";

class NtpTimeClientService: public Initializable, public Loopable {
public:

	NtpTimeClientService(EspSettingsBox* _espSettingsBox){
		construct(_espSettingsBox,nullptr);
	}

	NtpTimeClientService(EspSettingsBox* _espSettingsBox,std::function<void(void)> _externalFunction){//, ulong externalFunctionInterval){
		construct(_espSettingsBox,_externalFunction);
	}

	~NtpTimeClientService(){};

	virtual boolean initialize(boolean _init){
		if(_init){

			Serial.println(FPSTR("-----Init Ntp Client-----"));
			int boxIndex=espSettingsBox->getExtraBoxIndex(FPSTR(NTP_SETTINGS_BOX_NAME));
			String poolServerName=espSettingsBox->getExtraValue(boxIndex, NTP_poolServerName);
			int timeOffset=espSettingsBox->getExtraValueInt(boxIndex, NTP_timeOffset);
			int updateInterval=1000*espSettingsBox->getExtraValueInt(boxIndex, NTP_updateInterval);

			timeClient->setPoolServerName(poolServerName.c_str());
			timeClient->setTimeOffset(timeOffset);
			timeClient->setUpdateInterval(updateInterval);

			timeClient->begin();
			bool result=timeClient->forceUpdate();

			Serial.print(FPSTR("timeClient->begin() status="));
			Serial.println(result);

			timeClient->printDetails();
			Serial.print(FPSTR("initialized="));
			Serial.println(_init);

			timeClient->end();

			initialized=_init;
			displayDetails();
		}

		return initialized;
	}

	virtual boolean loop(){
		if(initialized){
			return update();
		}

		return false;
	}

	virtual void displayDetails(){
		if(!initialized){
			return;
		}
		Serial.print(FPSTR("Epoch time="));
		Serial.print(timeClient->getEpochTime());
		Serial.print(FPSTR("TimeStamp="));
		Serial.print(getTimeStamp());
		Serial.print(FPSTR(" timeReceived="));
		Serial.println(isTimeReceived());
	}

	ulong now(){
		return timeClient->getEpochTime();
	}

	boolean update(){
		if(!initialized){
			return false;
		}

		if(timeClient->update()){
			Serial.println(FPSTR("TimeUpdate FAILED"));
			timeClient->printDetails();
			return false;
		}

		boolean timeChanged=parseTimeToCache();
		boolean dateChanged=parseDateToCache();

		if(timeChanged || dateChanged){
			onTimeChanged();
		}

		return timeChanged || dateChanged;
	}

	String getTimeStamp(){
		update();
		String result="";

		for(uint8_t i=0;i<=INDEX_DAY_OF_WEEK;i++){
			result+=String(currDate[i]);
			result+=FPSTR(CALENDAR_DATE_DIVIDER);
		}

		result.setCharAt(result.length(), ' ');

		for(uint8_t i=0;i<TIME_LENGTH;i++){
			result+=String(currTime[i]);
			result+=FPSTR(CALENDAR_DATE_DIVIDER);
		}

		result.remove(result.length());

		return result;
	}

	ulong DateTimeToEpochTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t seconds){

		uint8_t monthLength=monthDays[month];
		uint16_t yearLength=(LEAP_YEAR(year))?366:355;

		if (month==1) { // february
			if (LEAP_YEAR(year)){monthLength=29;} else {monthLength=28;}
		}

		ulong result=seconds+minute*60+hour*60*60+day*24*60*60+monthLength*24*60*60+yearLength*24*60*60;

		return result;
	}

	boolean getLeapYear(){
		return currDate[INDEX_LEAP_YEAR];
	}

	uint8_t getDayOfMonth(){
		return currDate[INDEX_DAY];
	}

	uint8_t getMonth(){
		return currDate[INDEX_MONTH];
	}

	uint16_t getYear(){
		return currDate[INDEX_YEAR];
	}

	uint8_t getDayOfWeek(){
		return currDate[INDEX_DAY_OF_WEEK];
	}

	String getDayOfWeekNameShort(){
		return DAYS_OF_WEEK_SHORT[getDayOfWeek()];
	}

	String getDayOfWeekName(){
		return DAYS_OF_WEEK[getDayOfWeek()];
	}

	int8_t* getCurrentTimePrepared(){
		update();
		return currTimePrepared;
	}

	int8_t* getCurrentDatePrepared(){
		update();
		return currDatePrepared;
	}

	int8_t* getCurrentMonthDate(){
		update();

		monthDate[0]=currDatePrepared[INDEX_MONTH1];
		monthDate[1]=currDatePrepared[INDEX_MONTH2];
		monthDate[2]=currDatePrepared[INDEX_DAY1];
		monthDate[3]=currDatePrepared[INDEX_DAY2];

		return monthDate;
	}

	boolean isTimeReceived(){
		return timeReceived;
	}

	String getJson(){
		update();

		String result="{\"id\":\"-1\",\
				\"name\":\""+String(FPSTR(NTP_TIME_CLIENT_SERVICE_NAME))+"\",\
				\"type\":\""+String(FPSTR(NTP_TIME_CLIENT_SERVICE_TYPE))+"\",\
				\"year\":\""+String(currDate[INDEX_YEAR])+"\",\
				\"month\":\""+String(currDate[INDEX_MONTH])+"\",\
				\"day\":\""+String(currDate[INDEX_DAY])+"\",\
				\"leapYear\":\""+String(currDate[INDEX_LEAP_YEAR])+"\",\
				\"dayOfWeek\":\""+String(currDate[INDEX_DAY_OF_WEEK])+"\",\
				\"dayOfWeekNameShort\":\""+getDayOfWeekNameShort()+"\",\
				\"dayOfWeekName\":\""+getDayOfWeekName()+"\",\
				\"epochTime\":\""+String(timeClient->getEpochTime())+"\",\
				\"hour\":\""+String(currTime[INDEX_HOUR])+"\",\
				\"minute\":\""+String(currTime[INDEX_MINUTE])+"\",\
				\"second\":\""+String(currTime[INDEX_SECUNDA])+"\"}";

		return result;
	}

private:
	WiFiUDP ntpUDP;
	int8_t currTime[TIME_LENGTH] = {0x00,0x00,0x00};
	int8_t currTimePrepared[PREPARED_TIME_LENGTH] = {0x00,0x00,0x00,0x00,0x00,0x00};

	uint32_t currDate[DATE_LENGTH]={0x00,0x00,0x00,0x00,0x00};
	int8_t currDatePrepared[PREPARED_DATE_LENGTH] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

	int8_t monthDate[4]={0x00,0x00,0x00,0x00};

	EspSettingsBox* espSettingsBox;
	NTPClient* timeClient;
	std::function<void(void)> externalFunction;
	boolean timeReceived=false;

	void construct(EspSettingsBox* _espSettingsBox,std::function<void(void)> _externalFunction){//, ulong externalFunctionInterval){
		Serial.println(FPSTR("Create time client"));

		this->timeClient=new NTPClient(ntpUDP,"pl.pool.ntp.org",7200,1000);
		this->espSettingsBox=_espSettingsBox;
		this->externalFunction=_externalFunction;
	}

	void onTimeChanged(){
		if(!timeReceived){
			timeReceived=true;
			Serial.print(FPSTR("----First Time received----"));
			displayDetails();
		}

		if(externalFunction!=nullptr){
			externalFunction();
		}
	}

	boolean parseTimeToCache(){
		boolean timeChanged=false;

		uint8_t h=timeClient->getHours();
		uint8_t m=timeClient->getMinutes();
		uint8_t s=timeClient->getSeconds();

		uint8_t tmpTime[TIME_LENGTH] = {0x00,0x00,0x00};
		int8_t tmpTimePrep[PREPARED_TIME_LENGTH] = {0x00,0x00,0x00,0x00,0x00,0x00};

		tmpTimePrep[INDEX_HOUR1] = h / 10;
		tmpTimePrep[INDEX_HOUR2] = h % 10;
		tmpTimePrep[INDEX_MINUTE1] = m / 10;
		tmpTimePrep[INDEX_MINUTE2] = m % 10;
		tmpTimePrep[INDEX_SECUNDA1] = s / 10;
		tmpTimePrep[INDEX_SECUNDA2] = s % 10;

		tmpTime[INDEX_HOUR] = h;
		tmpTime[INDEX_MINUTE] = m;
		tmpTime[INDEX_SECUNDA] = s;

		for(uint8_t i=0;i<PREPARED_TIME_LENGTH;i++){
			if(tmpTimePrep[i]!=currTimePrepared[i]){
				timeChanged=true;
			}

			currTimePrepared[i]=tmpTimePrep[i];
		}

		for(uint8_t i=0;i<TIME_LENGTH;i++){
			if(tmpTime[i]!=currTime[i]){
				timeChanged=true;
			}

			currTime[i]=tmpTime[i];
		}
		return timeChanged;
	}

	boolean parseDateToCache(){
		boolean dateUpdated=false;

		ulong time=now();

		uint32_t tmpDate[DATE_LENGTH] = {0x00,0x00,0x00,0x00,0x00};
		uint8_t tmpDatePrepared[PREPARED_DATE_LENGTH] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

		uint year = 0;
		uint month=0;
		uint days = 0;
		uint leapYear=0;
		uint dayOfWeek=0;

		time=time/60/60/24;

		dayOfWeek = ((time + 4) % 7);

		uint monthLength=0;
		while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
			year++;

		}
		leapYear = LEAP_YEAR(year);

		days -= LEAP_YEAR(year) ? 366 : 365;
		time  -= days; // now it is days in this year, starting at 0

		days=0;
		month=0;
		monthLength=0;

		for (month=0; month<12; month++) {
			if (month==1) { // february
				if (LEAP_YEAR(year)){monthLength=29;} else {monthLength=28;}
			} else {
			  monthLength = monthDays[month];
			}

			if (time >= monthLength){ time -= monthLength;}else{break;}
		}

		year+=YEAR0;
		month+=1;
		time+=1;

		tmpDate[INDEX_YEAR] = year;
		tmpDate[INDEX_MONTH] = month;  // jan is month 1
		tmpDate[INDEX_DAY] = time;     // day of month
		tmpDate[INDEX_DAY_OF_WEEK] = dayOfWeek;
		tmpDate[INDEX_LEAP_YEAR] = leapYear;

		tmpDatePrepared[INDEX_YEAR1] = year / 1000;
		tmpDatePrepared[INDEX_YEAR2] = (year % 1000)/100;
		tmpDatePrepared[INDEX_YEAR3] = (year % 1000)%100/10;
		tmpDatePrepared[INDEX_YEAR4] = (year % 1000)%100%10;

		tmpDatePrepared[INDEX_MONTH1] = month / 10;
		tmpDatePrepared[INDEX_MONTH2] = month % 10;

		tmpDatePrepared[INDEX_DAY1] = time / 10;
		tmpDatePrepared[INDEX_DAY2] = time % 10;

		tmpDatePrepared[INDEX_DAY_OF_WEEK_PREP] = dayOfWeek;
		tmpDatePrepared[INDEX_LEAP_YEAR_PREP] = leapYear;

		for(uint8_t i=0;i<PREPARED_DATE_LENGTH;i++){
			if(tmpDatePrepared[i]!=currDatePrepared[i]){
				dateUpdated=true;
			}

			currDatePrepared[i]=tmpDatePrepared[i];
		}

		for(uint8_t i=0;i<DATE_LENGTH;i++){
			if(tmpDate[i]!=currDate[i]){
				dateUpdated=true;
			}

			currDate[i]=tmpDate[i];
		}
		return dateUpdated;
	}

	 /*
		Serial.print("boxIndex=");
		Serial.println(boxIndex);
		Serial.print("poolServerName=");
		Serial.println(poolServerName);
		Serial.print("=poolServerName.c_str() =");
		Serial.println(poolServerName.c_str());
		Serial.print("=");
		Serial.print("timeOffset =");
		Serial.println(timeOffset);
		Serial.print("updateInterval=");
		Serial.println(updateInterval);
		Serial.print("triggerInterval=");
		Serial.println(triggerInterval);

		String getCurrentTimeAsString(const char divider){
			String result=String(currTimePrepared[INDEX_HOUR1])
					+String(currTimePrepared[INDEX_HOUR2])
					+String(divider)
					+String(currTimePrepared[INDEX_MINUTE1])
					+String(currTimePrepared[INDEX_MINUTE2])
					+String(divider)
					+String(currTimePrepared[INDEX_SECUNDA1])
					+String(currTimePrepared[INDEX_SECUNDA2]);

			return result;
		}

		String getCurrentTimeNoSecondsAsString(const char divider){
			String result=String(currTimePrepared[INDEX_HOUR1])
					+String(currTimePrepared[INDEX_HOUR2])
					+String(divider)
					+String(currTimePrepared[INDEX_MINUTE1])
					+String(currTimePrepared[INDEX_MINUTE2]);

			return result;
		}

		String getCurrentDateAsString(const char divider){
			return String(getYear())+String(divider)
				   +String(getMonth())+String(divider)
				   +String(getDayOfMonth())+String(divider)
				   +String(getLeapYear())+String(divider)
				   +String(getDayOfWeekNameShort())+String(divider);
		}
	*/
};

#endif /* LIBRARIES_HELPERS_NTPTIMECLIENTSERVICE_H_ */