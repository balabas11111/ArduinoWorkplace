package com.balabas.smarthouse.server.entity.model;

import java.util.Date;
import java.util.Set;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import javax.persistence.CascadeType;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true, exclude = "groups")
@Table(name = "devices")
@javax.persistence.Entity
public class Device extends ItemAbstract implements IDevice {

	private String firmware;
	private String ip;
	private String dataUrl;

	@Enumerated(EnumType.STRING)
	private State state;

	private Date registrationDate;

	@OneToMany(mappedBy = "device", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	protected Set<Group> groups;

	@Transient
	private boolean initialized;

	@Transient
	private ActionTimer timer;

	public Device() {
		this.timer = new ActionTimer(ItemType.DEVICE.getRefreshInterval());
	}

	@Override
	public Group getGroup(String groupName) {
		if (getGroups() == null) {
			return null;
		}
		return getGroups().stream().filter(g -> g.getName().equalsIgnoreCase(groupName)).findFirst().orElse(null);
	}

	@JsonIgnore
	@Override
	public Set<Entity> getEntities() {
		return getGroups().stream().flatMap(group -> group.getEntities().stream()).collect(Collectors.toSet());
	}

	@Override
	public Entity getEntity(String entityName) {
		return getEntityByFilter(e -> e.getName() != null && e.getName().equals(entityName));
	}

	@Override
	public Entity getEntity(Long entityId) {
		return getEntityByFilter(e -> e.getId() != null && e.getId().equals(entityId));
	}
	
	@Override
	public Entity getEntityByRemoteId(int remoteId) {
		return getGroups().stream().flatMap(group -> group.getEntities().stream())
				.filter(e -> e.getRemoteId() == remoteId).findFirst().orElse(null);
	}

	@Override
	public boolean isRegistered() {
		return state != null && state.compareTo(State.REGISTERED) >= 0;
	}

	@Override
	public Device update(Device device) {
		this.firmware = device.getFirmware();
		this.description = device.getDescription();
		this.ip = device.getIp();
		this.dataUrl = device.getDataUrl();
		this.timer = device.getTimer();
		return this;
	}
	
	private Entity getEntityByFilter(Predicate<? super Entity> entityPredicate) {
		if (getGroups() == null) {
			return null;
		}
		return getGroups().stream().flatMap(group -> group.getEntities().stream())
				.filter(entityPredicate).findFirst().orElse(null);
	}

}
