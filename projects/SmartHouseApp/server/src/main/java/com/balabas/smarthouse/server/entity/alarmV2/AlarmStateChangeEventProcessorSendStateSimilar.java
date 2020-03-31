package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.stereotype.Component;

@Component
public class AlarmStateChangeEventProcessorSendStateSimilar extends AlarmStateChangeEventProcessor {

	@Override
	public void process(IItemEvent event) {
		alarmStateService.removeAllStateDescription(event);
		alarmStateService.printStateDescription(event.getItem());
		super.process(event);
	}

}
