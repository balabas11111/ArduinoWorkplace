package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Date;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

import lombok.Getter;
import lombok.Setter;

public class ItemChangeEvent implements IItemEvent {

	@Getter @Setter
	private Long alarmId;
	
	@Getter @Setter
	private Date date;
	
	@Getter @Setter
	private IItemAbstract item;
	
	@Getter @Setter
	private IAlarmStateChangeAction changeAction;

	public ItemChangeEvent(IItemAbstract item, IAlarmStateChangeAction changeAction, Long alarmId) {
		super();
		this.date = new Date();
		this.item = item;
		this.changeAction = changeAction;
		this.alarmId = alarmId;
	}
	
}
