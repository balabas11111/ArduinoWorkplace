package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.stereotype.Component;

@Component
public class AlarmStateChangeEventProcessorSendStateChanged extends AlarmStateChangeEventProcessor {

	@Override
	public void process(IItemEvent event) {
		super.process(event);
	}

}
