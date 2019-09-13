/*
 * NotificationTarget.h
 *
 *  Created on: 12 ����. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFICATIONTARGET_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFICATIONTARGET_H_

#include <Arduino.h>
#include <ArduinoJson.h>

class NotificationTarget {
public:
	NotificationTarget(){};
	virtual ~NotificationTarget(){};

	virtual void toTarget(JsonObject& json) = 0;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFICATIONTARGET_H_ */
