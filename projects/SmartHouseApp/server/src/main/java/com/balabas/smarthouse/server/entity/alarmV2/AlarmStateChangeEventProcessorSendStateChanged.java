package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.stereotype.Component;

import lombok.extern.log4j.Log4j2;

@Component
@Log4j2
public class AlarmStateChangeEventProcessorSendStateChanged extends AlarmStateChangeEventProcessor {

	public AlarmStateChangeEventProcessorSendStateChanged() {
		this.setName("AlarmStateChangeEventProcessorSendStateChanged");
	}
	
	@Override
	public void processEvent(IAlarmStateChangeEvent event) {
		log.info("processEvent AlarmStateChangeEventProcessorSendStateChanged " + event);
	}

}
