package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public class AlarmV2CheckerAbstractEntityFieldNumber {

	public Class<?> getItemClass() {
		return IEntityField.class;
	}
	
	public boolean isParameterValid(String value) {
		return isValidFloat(value);
	}
	
	protected Float getAsFloat(String parameter) {
		return Float.valueOf(parameter);
	}
	
	protected Float getValueTmpAsFloat(IEntityField entityField) {
		return entityField.getValueTmpAsFloat();
	}
	
	protected Integer getValueTmpAsInteger(IEntityField entityField) {
		return Integer.valueOf(entityField.getValueTmpAsInteger());
	}
	
	protected Float getEntityFieldValueFloat(IItemAbstract item) {
		IEntityField entityField = (IEntityField) item;
		return getEntityFieldValueFloat(entityField);
	}
	
	protected Float getEntityFieldValueFloat(IEntityField entityField) {
		return ((Number) entityField.getValue()).floatValue();
	}
	
	protected boolean isValidFloat(String value) {
		try {
			getAsFloat(value);
		} catch(Exception e) {
			return false;
		}
		return true;
	}
}
