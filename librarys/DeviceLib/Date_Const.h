/*
 * DateConst.h
 *
 *  Created on: 22 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_DATE_CONST_H_
#define LIBRARIES_DEVICELIB_DATE_CONST_H_

#define YEAR0 1970
#define LEAP_YEAR(Y)     ( ((YEAR0+Y)>0) && !((YEAR0+Y)%4) && ( ((YEAR0+Y)%100) || !((YEAR0+Y)%400) ) )

#define PREPARED_TIME_LENGTH 6
#define PREPARED_DATE_LENGTH 10

#define TIME_LENGTH 3
#define DATE_LENGTH 5

//preparedTime indexes
#define INDEX_HOUR1 0
#define INDEX_HOUR2 1
#define INDEX_MINUTE1 2
#define INDEX_MINUTE2 3
#define INDEX_SECUNDA1 4
#define INDEX_SECUNDA2 5
//time indexes
#define INDEX_HOUR 0
#define INDEX_MINUTE 1
#define INDEX_SECUNDA 2
//preparedDate indexes
#define INDEX_YEAR1 0
#define INDEX_YEAR2 1
#define INDEX_YEAR3 2
#define INDEX_YEAR4 3
#define INDEX_MONTH1 4
#define INDEX_MONTH2 5
#define INDEX_DAY1 6
#define INDEX_DAY2 7
#define INDEX_DAY_OF_WEEK_PREP 8
#define INDEX_LEAP_YEAR_PREP 9

//date indexes
#define INDEX_YEAR 0
#define INDEX_MONTH 1
#define INDEX_DAY 2
#define INDEX_DAY_OF_WEEK 3
#define INDEX_LEAP_YEAR 4

const char CALENDAR_DATE_DIVIDER[] PROGMEM ="-";
const char CALENDAR_TIME_DIVIDER[] PROGMEM =":";

static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};

const char CALENDAR_DAY_MONDAY_SHORT[] PROGMEM ="Mo";
const char CALENDAR_DAY_TUESDAY_SHORT[] PROGMEM ="Tu";
const char CALENDAR_DAY_WENDESDAY_SHORT[] PROGMEM ="We";
const char CALENDAR_DAY_THURSDAY_SHORT[] PROGMEM ="Th";
const char CALENDAR_DAY_FRIDAY_SHORT[] PROGMEM ="We";
const char CALENDAR_DAY_SATURDAY_SHORT[] PROGMEM ="Sa";
const char CALENDAR_DAY_SUNDAY_SHORT[] PROGMEM ="Su";

const char CALENDAR_DAY_MONDAY[] PROGMEM ="Monday";
const char CALENDAR_DAY_TUESDAY[] PROGMEM ="Tuesday";
const char CALENDAR_DAY_WENDESDAY[] PROGMEM ="Wednesday";
const char CALENDAR_DAY_THURSDAY[] PROGMEM ="Thursday";
const char CALENDAR_DAY_FRIDAY[] PROGMEM ="Friday";
const char CALENDAR_DAY_SATURDAY[] PROGMEM ="Saturday";
const char CALENDAR_DAY_SUNDAY[] PROGMEM ="Sunday";

const char CALENDAR_DAY_MONDAY_SHORT_RU[] PROGMEM ="��";
const char CALENDAR_DAY_TUESDAY_SHORT_RU[] PROGMEM ="��";
const char CALENDAR_DAY_WENDESDAY_SHORT_RU[] PROGMEM ="��";
const char CALENDAR_DAY_THURSDAY_SHORT_RU[] PROGMEM ="��";
const char CALENDAR_DAY_FRIDAY_SHORT_RU[] PROGMEM ="��";
const char CALENDAR_DAY_SATURDAY_SHORT_RU[] PROGMEM ="��";
const char CALENDAR_DAY_SUNDAY_SHORT_RU[] PROGMEM ="��";

const char CALENDAR_DAY_MONDAY_RU[] PROGMEM ="�����������";
const char CALENDAR_DAY_TUESDAY_RU[] PROGMEM ="�������";
const char CALENDAR_DAY_WENDESDAY_RU[] PROGMEM ="�����";
const char CALENDAR_DAY_THURSDAY_RU[] PROGMEM ="�������";
const char CALENDAR_DAY_FRIDAY_RU[] PROGMEM ="�������";
const char CALENDAR_DAY_SATURDAY_RU[] PROGMEM ="�������";
const char CALENDAR_DAY_SUNDAY_RU[] PROGMEM ="�����������";

const char* DAYS_OF_WEEK_SHORT[7]={CALENDAR_DAY_SUNDAY_SHORT,CALENDAR_DAY_MONDAY_SHORT,
							 CALENDAR_DAY_TUESDAY_SHORT,CALENDAR_DAY_WENDESDAY_SHORT,
							 CALENDAR_DAY_THURSDAY_SHORT,CALENDAR_DAY_FRIDAY_SHORT,CALENDAR_DAY_SATURDAY_SHORT};

const char* DAYS_OF_WEEK[7]={CALENDAR_DAY_SUNDAY,CALENDAR_DAY_MONDAY,
							 CALENDAR_DAY_TUESDAY,CALENDAR_DAY_WENDESDAY,
							 CALENDAR_DAY_THURSDAY,CALENDAR_DAY_FRIDAY,
							 CALENDAR_DAY_SATURDAY};

class Date_Const {
public:
	Date_Const();
	virtual ~Date_Const();

};

#endif /* LIBRARIES_DEVICELIB_DATE_CONST_H_ */
