package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

public interface IDevice extends IStateable, IUpdateable, IItemContainer<IGroup> {

	String getFirmware();

	void setFirmware(String firmware);

	boolean isRegistered();
	boolean isInitialized();
	void setInitialized(boolean initialized);

	Set<IGroup> getGroups();
	Set<IEntity> getEntities();
	
	String getDataUrl();
	
	IEntity getEntity(String entityName);
	IEntity getEntity(int remoteId);
	
	default IGroup getGroup(String groupName) {
		return getChild(groupName);
	}
}