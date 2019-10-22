/*
 * DeviceConfig.h
 *
 *  Created on: Oct 9, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_DEVICECONFIG_H_
#define LIBRARIES_ENTITYLIBSIMPLE_DEVICECONFIG_H_

//#define SETTINGS_DISPLAY_ENABLED
//#define SETTINGS_ENTITIES_PERSISTANCE_ENABLED

//#define SENSORS_MOCKED 1

#define SETTINGS_WIFI_SSSID "balabasKiev5"
#define SETTINGS_WIFI_PASSWORD "wuWylKegayg2wu22"

#define SETTINGS_SMART_SERVER_ADDRESS "192.168.0.101"
#define SETTINGS_SMART_SERVER_KEY "SomeServerKey"
//box
//#define SETTINGS_DEVICE_EMOJI "1F4BC"
//thermo
#define SETTINGS_DEVICE_EMOJI EMOJI_THERMOMETER

#define DEVICE_MANAGER_MIN_HEAP_RESTART 2000
#define REQUEST_SECURITY_DISABLED 1

#ifndef SETTINGS_SERVER_MQTT_DISABLED

#define MQTT_PUBLISHER "!!SmartHouse-device"
#define MQTT_HOST "192.168.0.101"
#define MQTT_PORT 1883
#define MQTT_USER "mqttuser"
#define MQTT_PASSWORD "mqttpassword"

//#define SETTINGS_SERVER_CONNECTION_DISABLED 1
#define SETTINGS_SERVER_HTTP_DATA_UPDATE_DISPATCH_DISABLED
#endif

#define MQTT_REGISTRATION_DISABLED 1

#ifndef SETTINGS_SERVER_CONNECTION_DISABLED

	//#define SETTINGS_SERVER_SEND_DATA_METHOD_GET 1

	#ifndef SETTINGS_SMART_SERVER_ADDRESS
		#define SETTINGS_SMART_SERVER_ADDRESS "192.168.0.101"
	#endif
	#ifndef SETTINGS_SMART_SERVER_KEY
		#define SETTINGS_SMART_SERVER_KEY "SomeServerKey"
	#endif
#endif

#ifndef SERVER_CONNECTION_PING_FAILED_TIMEOUT
	#define SERVER_CONNECTION_PING_FAILED_TIMEOUT 10000
#endif

#ifndef SERVER_CONNECTION_REGISTRATION_FAILED_FIRST_TIMEOUT
	#define SERVER_CONNECTION_REGISTRATION_FAILED_FIRST_TIMEOUT 60000
#endif

#ifndef SERVER_CONNECTION_REGISTRATION_FAILED_NEXT_COUNT
	#define SERVER_CONNECTION_NEXT_REGISTRATION_NEXT_COUNT 1
#endif

#ifndef SERVER_CONNECTION_REGISTRATION_FAILED_NEXT_TIMEOUT
	#define SERVER_CONNECTION_REGISTRATION_FAILED_NEXT_TIMEOUT 600000
#endif

#ifndef SERVER_CONNECTION_REGISTRATION_FAILED_MIN_HEAP_TO_RESTART
	#define SERVER_CONNECTION_REGISTRATION_FAILED_MIN_HEAP_TO_RESTART 18000
#endif

#ifndef SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_TIMEOUT
	#define SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_TIMEOUT 10000
#endif

#ifndef SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_NOT_DEREGISTER_TIMEOUT
	#define SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_NOT_DEREGISTER_TIMEOUT 5000
#endif

#ifndef SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_NOT_DEREGISTER_COUNT
	#define SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_NOT_DEREGISTER_COUNT 3
#endif

class DeviceConfig {
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_DEVICECONFIG_H_ */
