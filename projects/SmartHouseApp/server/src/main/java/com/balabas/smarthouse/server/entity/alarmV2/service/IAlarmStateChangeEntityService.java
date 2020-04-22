package com.balabas.smarthouse.server.entity.alarmV2.service;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEntity;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.repository.AlarmStateChangeEntityRepository;
import com.balabas.smarthouse.server.util.DateTimeUtil;

public interface IAlarmStateChangeEntityService {

	AlarmStateChangeEntityRepository getAlarmStateChangeEntityRepository();
	
	void log(String string);
	
	default void saveAlarmStateChange(IItemEvent event) {
		if(event.getOldState()!=null && !event.getOldState().equals(event.getNewState())) {
			AlarmStateChangeEntity entity = buildEntity(event, event.getOldState(), null);
			getAlarmStateChangeEntityRepository().save(entity);
			
			entity = buildEntity(event, event.getNewState(), event.getDescription());
			getAlarmStateChangeEntityRepository().save(entity);
			
			log("State change saved old=" + event.getOldState() + " new=" + event.getNewState());
		}
	}
	
	default AlarmStateChangeEntity buildEntity(IItemEvent event, AlarmState alarmState, String description) {
		AlarmStateChangeEntity entity = new AlarmStateChangeEntity();
		
		entity.setDateTime(DateTimeUtil.getDate());
		entity.setAlarmState(alarmState);
		entity.setValueInt(alarmState.getValueInt());
		entity.setTargetAlarmId(event.getAlarm().getId());
		entity.setType(event.getItem().getItemType());
		
		return entity;
	}
}