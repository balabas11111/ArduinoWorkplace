package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

public interface IDevice extends IStateable, IUpdateable, IItemContainer<IGroup> {

	String getFirmware();

	void setFirmware(String firmware);

	boolean isInitialized();

	Set<IGroup> getGroups();
	
	String getDataUrl();
	
	default IGroup getGroup(String groupName) {
		return getChild(groupName);
	}
}
