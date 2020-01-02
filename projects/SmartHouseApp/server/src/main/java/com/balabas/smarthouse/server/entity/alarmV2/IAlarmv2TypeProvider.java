package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarmv2TypeProvider {

	List<Class<?>> getEnabledAlarms(IItemAbstract item);
}