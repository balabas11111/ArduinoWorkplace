package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.ActionTimer;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
public class Group extends ItemAbstract implements IGroup {

	private ItemType type;
	
	private String deviceName;

	@JsonIgnore
	private ActionTimer timer;
	
	@JsonIgnore
	protected Set<IEntity> children;
	
	@Override
	public Set<IEntity> getEntities() {
		return getChildren();
	}
	
	@Override
	public void setEntities(Set<IEntity> entities) {
		setChildren(entities);
	}
	
}
