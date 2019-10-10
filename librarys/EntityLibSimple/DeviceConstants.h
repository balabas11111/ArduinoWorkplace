/*
 * NetConstants.h
 *
 *  Created on: Sep 20, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_WIFI_NETCONSTANTS_H_
#define LIBRARIES_ENTITYLIBSIMPLE_WIFI_NETCONSTANTS_H_

#include "Arduino.h"

// default json keys for values in request context
#define DEVICE_FIELD_GROUP "group"
#define DEVICE_FIELD_ENTITY_NAME "name"
#define DEVICE_FIELD_DATA "data"

#define DEVICE_FIELD_DATA_PARAM_GET "&data="

#define PARAM "param"
#define JSON "json"
#define BODY "body"

#define GROUP_ALL "all"
#define GROUP_SENSORS "sensors"
#define GROUP_SETTINGS "settings"
#define GROUP_SERVICES "services"
#define GROUP_DEVICE "device"

#define ENTITY_FIELD_DESCRIPTION "d"
#define ENTITY_FIELD_SENSOR_ITEMS "si"
#define ENTITY_FIELD_ITEM_CLASS "ic"

#define ENTITY_ITEM_CLASS_TOOGLE_BUTTON "tb"

#define EMPTY_LINE ""

class DeviceConstants {
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_WIFI_NETCONSTANTS_H_ */