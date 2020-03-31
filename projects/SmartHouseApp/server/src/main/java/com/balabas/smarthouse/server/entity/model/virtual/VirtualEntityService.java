package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldBoolean;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldFloat;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.entity.service.IEntityFieldService;
import com.balabas.smarthouse.server.entity.service.IEntityService;
import com.balabas.smarthouse.server.entity.service.IGroupService;

@Service
@SuppressWarnings("rawtypes")
public class VirtualEntityService implements IVirtualEntityService {

	public static final String VIRTUAL_DEVICE_NAME = "VirtualDevice";
	public static final String VIRTUAL_DEVICE_DESCR = "Виртуальное устройство";
	public static final String VIRTUAL_DEVICE_FIRMWARE = "VirtualDevice";

	public static final String VIRTUAL_GROUP_NAME = "sensors";
	public static final String VIRTUAL_GROUP_DESCR = "Виртуальные датчики";

	public static final String VIRTUAL_ENTITY_NAME = "entity";
	public static final String VIRTUAL_ENTITY_DESCR = "Виртуальный датчик";

	public static final String VIRTUAL_ENTITY_FIELD_NAME = "entityField";
	public static final String VIRTUAL_ENTITY_FIELD_DESCR = "Виртуальный показатель";

	@Autowired
	private IDeviceManageService deviceService;

	@Autowired
	private IGroupService groupService;

	@Autowired
	private IEntityService entityService;
	
	@Autowired
	private IEntityFieldService entityFieldService;

	protected void setNameDescription(String name, String description, IItemAbstract item) {
		item.setId(0L);
		item.setName(name);
		item.setDescription(description);
		item.setVirtualized(true);
	}

	@Override
	public IEntityField createEntityFieldFloat(String name, String description) {
		IEntityField item = new EntityFieldFloat();
		setNameDescription(name, description, item);
		return item;
	}
	
	@Override
	public IEntityField createEntityFieldBoolean(String name, String description) {
		IEntityField item = new EntityFieldBoolean();
		setNameDescription(name, description, item);
		return item;
	}

	@Override
	public IEntityField getEntityFieldById(Long id) {
		IEntityField result = deviceService.getEntityFieldById(id);
		
		if(result == null) {
			result = entityFieldService.getEntityFieldById(id).orElse(null); 
		}
		return result;
	}

	@Override
	public IEntity createEntity(String name, String description) {
		IEntity item = new Entity();
		setNameDescription(name, description, item);
		return item;
	}

	@Override
	public IEntity getEntityById(Long id) {
		return Optional.ofNullable((IEntity)deviceService.getEntityById(id)).orElse(entityService.loadEntityById(id));
	}

	@Override
	public IGroup createGroup(String name, String description) {
		IGroup item = new Group();
		setNameDescription(name, description, item);
		return item;
	}

	@Override
	public IGroup getGroupById(Long id) {
		return Optional.ofNullable((IGroup)deviceService.getGroupById(id)).orElse(groupService.loadGroupById(id));
	}

	@Override
	public IDevice createDevice(String name, String description, String firmware) {
		IDevice item = new Device();
		item.setFirmware(firmware);
		setNameDescription(name, description, item);

		return item;
	}

	@Override
	public IDevice getDeviceById(Long id) {
		return deviceService.getDeviceById(id);
	}

	@Override
	public List<IDevice> getDevices() {
		return deviceService.getDevices().stream().filter(dev -> dev.isVirtualized()).collect(Collectors.toList());
	}

	@Override
	public List<IGroup> getGroups() {
		return groupService.loadVirtualized();
	}

	@Override
	public List<IEntity> getEntities() {
		return entityService.loadVirtualized();
	}

	@Override
	public List<IEntityField> getEntityFields() {
		return deviceService.getDevices().stream().flatMap(device -> device.getGroups().stream())
				.flatMap(group -> group.getEntities().stream()).flatMap(entity -> entity.getEntityFields().stream())
				.filter(dev -> dev.isVirtualized()).collect(Collectors.toList());
	}

	@Override
	public void save(IDevice device) {
		device.setVirtualized(true);
		deviceService.save(device);
	}

	@Override
	public void save(IGroup group) {
		if(group.getType()==null) {
			group.setType(ItemType.SENSORS);
		}
		group.setVirtualized(true);
		deviceService.save(group);
	}

	@Override
	public void save(IEntity entity) {
		entity.setVirtualized(true);
		deviceService.save(entity);
	}

	@Override
	public void save(IEntityField entityField) {
		entityField.setVirtualized(true);
		deviceService.save(entityField);
	}

	@SuppressWarnings("unchecked")
	@Override
	public Set<IEntityFieldEnabledValue> getEntityFieldEnabledValues(Long entityFieldId) {
		IEntityField field = deviceService.getEntityFieldById(entityFieldId);
		
		if(field == null || field.getEntityFieldEnabledValues() == null) {
			return Collections.emptySet();
		}
		
		return field.getEntityFieldEnabledValues();
	}

	@Override
	public void save(IEntityFieldEnabledValue entityFieldEnabledValue) {
		deviceService.save(entityFieldEnabledValue);
	}

	@Override
	public void deleteEntityFieldEnabledValue(Long id) {
		deviceService.deleteEntityFieldEnabledValue(id);
	}

	@Override
	public List<IEntityField> getEntityFieldsNotVirtual() {
		return deviceService.getEntityFieldsNotVirtual();
	}

	@Override
	public List<IEntityField> getEntityFieldsNotVirtualCommandButtons() {
		return deviceService.getEntityFieldsNotVirtualCommandButtons();
	}

}
