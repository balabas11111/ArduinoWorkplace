package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class AlarmV2Service implements IAlarmV2Service {

	@Autowired
	IAlarmV2RepositoryDevice alarmRepositoryDevice;

	@Autowired
	IAlarmV2RepositoryEntity alarmRepositoryEntity;

	@Autowired
	IAlarmV2RepositoryEntityField alarmRepositoryEntityField;

	@Autowired
	IAlarmv2TypeProvider alarmTypeProvider;

	@Autowired
	IDeviceManageService deviceService;

	Map<String, List<IAlarmV2>> alarmMap = new HashMap<String, List<IAlarmV2>>();

	@Override
	public List<AlarmV2Checker> getEnabledAlarmCheckers(IItemAbstract item) {
		return alarmTypeProvider.getEnabledAlarms(item);
	}

	@PostConstruct
	public void loadAlarms() {
		alarmRepositoryDevice.findAll().forEach(this::putAlarmToCache);
		alarmRepositoryEntity.findAll().forEach(this::putAlarmToCache);
		alarmRepositoryEntityField.findAll().forEach(this::putAlarmToCache);
	}

	@Override
	public void saveAlarm(IAlarmV2 alarm) {
		getRepository(alarm).save(alarm);
		putAlarmToCache(alarm);
	}

	@Override
	public void deleteAlarm(IAlarmV2 alarm) {
		if (alarm != null) {
			getRepository(alarm).deleteById(alarm.getId());
			removeAlarmFromCache(alarm);
		}
	}

	@Override
	public void checkForAlarmsWithParentExecutePostActions(List<EntityFieldValue> changedValues) {
		Map<Long, IItemAbstract> entityFieldsChanged = new HashMap<>();
		Map<Long, IItemAbstract> entitiesChanged = new HashMap<>();
		Map<Long, IItemAbstract> groupsChanged = new HashMap<>();
		Map<Long, IItemAbstract> devicesChanged = new HashMap<>();

		for (IEntityFieldValue value : changedValues) {
			putById(entityFieldsChanged, value.getEntityField());
			putById(entitiesChanged, value.getEntityField().getEntity());
			putById(groupsChanged, value.getEntityField().getEntity().getGroup());
			putById(devicesChanged, value.getEntityField().getEntity().getGroup().getDevice());
		}

		checkForAlarmsExecutePostActions(devicesChanged.values());
		checkForAlarmsExecutePostActions(groupsChanged.values());
		checkForAlarmsExecutePostActions(entitiesChanged.values());
		checkForAlarmsExecutePostActions(entityFieldsChanged.values());
	}

	@Override
	public List<IItemEvent> checkForAlarmsExecutePostActions(Collection<IItemAbstract> items) {
		List<IItemEvent> list = new ArrayList<>();

		items.forEach(item -> list.addAll(checkForAlarmsExecutePostActions(item)));

		return list;
	}

	@Override
	public List<IItemEvent> checkForAlarmsExecutePostActions(IItemAbstract item) {
		List<IItemEvent> events = new ArrayList<IItemEvent>();

		getAlarmsByItemUid(item).forEach(alarm -> {
			String checkerName = alarm.getCheckerName();
			AlarmV2Checker checker = alarmTypeProvider.getAlarmV2checker(checkerName);
			if(checker == null) {
				log.error("checker is null");
			}
			
			alarm.setChecker(checker);
			alarm.setItem(item);
			events.addAll(checkForAlarm(alarm));
		});
		events.forEach(this::processEvent);

		return events;
	}

	private List<IItemEvent> checkForAlarm(IAlarmV2 alarm) {
		alarm.check();
		return buildEvent(alarm);
	}

	private void processEvent(IItemEvent event) {
		IAlarmStateChangeEventProcessor processor = alarmTypeProvider.getAlarmStateChangedEventProcessor(event);

		if (processor != null && processor.isTarget(event)) {
			processor.processEvent(event);
		}
	}

	private List<IItemEvent> buildEvent(IAlarmV2 alarm) {

		List<IItemEvent> events = new ArrayList<IItemEvent>();
		List<IAlarmStateChangeAction> action = alarm.getCurrentActions();

		action.stream().forEach( a -> {
			events.add(new ItemChangeEvent(alarm.getItem(), a));
		});
		return events;
	}

	private void putAlarmToCache(IAlarmV2 alarm) {
		List<IAlarmV2> alarms = getAlarmsByItemUid(alarm.getItem());

		int ind = getAlarmIndex(alarms, alarm);

		if (ind > -1) {
			alarms.set(ind, alarm);
		} else {
			alarms.add(alarm);
		}

		alarms.stream().forEach(alarmTypeProvider::setAlarmCheckerByName);
	}

	private void removeAlarmFromCache(IAlarmV2 alarm) {
		List<IAlarmV2> alarms = getAlarmsByItemUid(alarm.getItem());

		int ind = getAlarmIndex(alarms, alarm);

		if (ind > -1) {
			alarms.remove(ind);
		}
	}

	private List<IAlarmV2> getAlarmsByItemUid(IItemAbstract item) {
		String uid = item.getItemUid();

		if (!alarmMap.containsKey(uid)) {
			alarmMap.put(uid, new ArrayList<>());
		}

		List<IAlarmV2> result = alarmMap.get(uid);

		result.stream().forEach(alarm -> alarm.setItem(item));
		return result;
	}

	private int getAlarmIndex(List<IAlarmV2> alarms, IAlarmV2 source) {
		int ind = -1;
		for (IAlarmV2 al : alarms) {
			ind++;
			if (al.getId().equals(source.getId())) {
				return ind;
			}
		}

		return -1;
	}

	private void putById(Map<Long, IItemAbstract> map, IItemAbstract item) {
		map.put(item.getId(), item);
	}

	@Override
	public List<IAlarmV2> getAllAlarms() {
		return alarmMap.values().stream().flatMap(list -> list.stream()).filter(alarm -> alarm.getItem() != null)
				.collect(Collectors.toList());
	}

	@Override
	public List<IAlarmV2> getAlarmsForItemClass(Class<?> clazz) {
		return getAlarmsByFilter(
				alarm -> alarm.getItem() != null && clazz.isAssignableFrom(alarm.getTargetItemClass()));
	}

	@Override
	public Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassName() {
		return getAlarmsGrouppedBy(alarm -> alarm.getTargetItemClass().getSimpleName());
	}

	@Override
	public List<IAlarmV2> getAlarmsByAlarmState(AlarmState alarmState) {
		return getAlarmsByFilter(alarm -> alarm.getItem() != null && alarmState.equals(alarm.getAlarmState()));
	}

	@Override
	public Map<AlarmState, List<IAlarmV2>> getAlarmsGrouppedByAlarmState() {
		return getAlarmsGrouppedBy(alarm -> alarm.getAlarmState());
	}

	public Optional<IAlarmV2> getAlarmByFilter(Predicate<? super IAlarmV2> predicate) {
		return getAllAlarms().stream().filter(predicate).findFirst();
	}

	public List<IAlarmV2> getAlarmsByFilter(Predicate<? super IAlarmV2> predicate) {
		return getAllAlarms().stream().filter(predicate).sorted(AlarmV2Service::compareByItemDescriptionField)
				.collect(Collectors.toList());
	}

	public <T> Map<T, List<IAlarmV2>> getAlarmsGrouppedBy(Function<IAlarmV2, T> mapKeyFunc) {
		Map<T, List<IAlarmV2>> result = new HashMap<>();

		getAllAlarms().forEach(alarm -> {
			T key = mapKeyFunc.apply(alarm);

			if (!result.containsKey(key)) {
				result.put(key, new ArrayList<IAlarmV2>());
			}

			result.get(key).add(alarm);

		});

		for (T key : result.keySet()) {
			List<IAlarmV2> list = result.get(key).stream().sorted(AlarmV2Service::compareByItemDescriptionField)
					.collect(Collectors.toList());
			result.put(key, list);
		}

		return result;
	}

	public static int compareByItemDescriptionField(IAlarmV2 a1, IAlarmV2 a2) {
		return ItemAbstract.compareByDescriptionField(a1.getItem(), a2.getItem());
	}

	@Override
	public IAlarmV2 getAlarmOrDefault(Long id, IAlarmV2 def) {
		return getAlarmByFilter(alarm -> id.equals(alarm.getId())).orElse(def);
	}

	@Override
	public void deleteAlarm(Long id) {
		deleteAlarm(getAlarmOrDefault(id, null));
	}

	@Override
	public List<AlarmV2Checker> getCheckersByTargetItemClass(Class<?> targetItemClass) {
		return alarmTypeProvider.getCheckersByTargetItemClass(targetItemClass);
	}

	@Override
	public IAlarmV2Repository getRepository(IAlarmV2 alarm) {
		if (IDevice.class.isAssignableFrom(alarm.getTargetItemClass())) {
			return alarmRepositoryDevice;
		}
		if (IEntity.class.isAssignableFrom(alarm.getTargetItemClass())) {
			return alarmRepositoryEntity;
		}
		if (IEntityField.class.isAssignableFrom(alarm.getTargetItemClass())) {
			return alarmRepositoryEntityField;
		}

		return null;
	}

	@Override
	public IAlarmV2 newAlarm(ItemType itemType) {
		if (ItemType.DEVICE.equals(itemType)) {
			return new AlarmOfDevice();
		}
		if (ItemType.ENTITY.equals(itemType)) {
			return new AlarmOfEntity();
		}
		if (ItemType.ENTITY_FIELD.equals(itemType)) {
			return new AlarmOfEntityField();
		}

		return null;
	}

	@Override
	public Set<IItemAbstract> getEnabledAlarmTargets(IAlarmV2 alarm) {
		Set<IItemAbstract> result = null;

		if (IDevice.class.isAssignableFrom(alarm.getTargetItemClass())) {
			result = deviceService.getDevices().stream().collect(Collectors.toSet());
		}
		if (IEntity.class.isAssignableFrom(alarm.getTargetItemClass())) {
			result = deviceService.getEntities().stream().collect(Collectors.toSet());
		}
		if (IEntityField.class.isAssignableFrom(alarm.getTargetItemClass())) {
			result = deviceService.getEntityFields().stream().collect(Collectors.toSet());
		}

		return result;
	}
	
	@Override
	public IItemAbstract getEnabledAlarmTarget(Long itemId, Class<?> itemClass) {
		if (IDevice.class.isAssignableFrom(itemClass)) {
			return deviceService.getDeviceById(itemId);
		}
		if (IEntity.class.isAssignableFrom(itemClass)) {
			return deviceService.getEntityById(itemId);
		}
		if (IEntityField.class.isAssignableFrom(itemClass)) {
			return deviceService.getEntityFieldById(itemId);
		}
		
		return null;
	}

	@Override
	public void createOrUpdateAlarm(IAlarmV2 alarm, Long itemId) {
		IItemAbstract item = getEnabledAlarmTarget(itemId, alarm.getTargetItemClass());
		alarm.setItem(item);
		
		saveAlarm(alarm);
		log.debug("Alarm saved " + alarm);
	}

	@Override
	public Set<IAlarmStateChangeEventProcessor> getAlarmStateChangedEventProcessors(IAlarmV2 alarm) {
		return alarmTypeProvider.getAlarmStateChangedEventProcessors();
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	@Override
	public void addAlarmStateChangeActionToAlarm(Long alarmId, AlarmStateChangeAction action) {
		IAlarmV2 alarm = getAlarmOrDefault(alarmId, null);
		
		Set<IAlarmStateChangeAction> set = Optional.ofNullable(alarm.getActions()).orElse(new LinkedHashSet());
		set.add(action);
		alarm.setActions(set);
		
		saveAlarm(alarm);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	@Override
	public void deleteAlarmStateChangeActionFromAlarm(Long alarmId, Long actionId) {
		IAlarmV2 alarm = getAlarmOrDefault(alarmId, null);
		
		Set<IAlarmStateChangeAction> set = Optional.ofNullable(alarm.getActions()).orElse(new LinkedHashSet());
		
		IAlarmStateChangeAction action = set.stream().filter( a-> a.getId().equals(a.getId())).findFirst().orElse(null);
		
		Optional.ofNullable(action).ifPresent( a -> set.remove(a));
		
		alarm.setActions(set);
		saveAlarm(alarm);
	}

	@Override
	public IAlarmV2 getAlarm(Long id, ItemType it) {
		if(it == null && id!=null && id>0) {
			IAlarmV2 al = getAlarmByFilter(alarm -> id.equals(alarm.getId())).orElse(null);
			if(al!=null) {
				return al;
			}
			
		}
		return getAlarmOrDefault(id, newAlarm(it));
	}

}
