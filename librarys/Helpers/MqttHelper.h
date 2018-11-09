/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_MQTTHELPER_H_
#define LIBRARIES_TIMETRIGGER_MQTTHELPER_H_

#include "Arduino.h"
#include <EspSettingsBox.h>
#include <PubSubClient.h>
#include "Loopable.h"
#include "Initializable.h"

class MqttHelper: public Loopable,public Initializable{

public:
	MqttHelper(EspSettingsBox *_settingsBox,String* _subscribeTopics,uint8_t _topicCount,Client& _client,std::function<void(String topic,String message)> _externalCallbackFunction);

	virtual ~MqttHelper();

	virtual boolean initialize(boolean _init) override;

	boolean isConnected();
	boolean connectIfNotConnected();
	void subscribe(String topicName);
	boolean publish(String message);
	boolean publish(String topicName,String message);

	String getName();
	String displayDetails();
	boolean loop();

	void callback(char* topic, uint8_t* payload, unsigned int length);
	PubSubClient getClient();

private:
	uint8_t topicCount;
	String* subscribeTopics;
	std::function<void(String topic,String message)> externalCallbackFunction;
	PubSubClient client;

	EspSettingsBox *espSettingsBox;
/*
	char* mqtt_server;
	uint16_t mqtt_port;
	char* mqtt_user;
	char* mqtt_pass;
	char* mqtt_topic;
	char* mqtt_startMessage;
	char* deviceId;
*/
	Client* wiFiClient;

	void connect();
};


#endif /* LIBRARIES_TIMETRIGGER_MQTTHELPER_H_ */
