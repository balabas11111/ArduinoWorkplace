package com.balabas.smarthouse.server.mqtt.subscribers;

import com.balabas.smarthouse.server.entity.service.IDeviceMqService;
import com.balabas.smarthouse.server.mqtt.MqttMessageSubscribtion;

import lombok.extern.log4j.Log4j2;

@Log4j2
public class DataDeviceSubscribtion extends MqttMessageSubscribtion {

	public DataDeviceSubscribtion(String topicName, IDeviceMqService messageService){
		this.topicName = topicName;
		this.service = messageService;
	}
	
	@Override
	public void onMessageReceived(String topic, String message) {
		log.info("Data from topic " + topicName + " message: " + message);
	}

}