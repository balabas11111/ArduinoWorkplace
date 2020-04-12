package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.LinkedHashMap;
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
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.entity.service.IActionTimerService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.util.DateTimeUtil;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
@SuppressWarnings("rawtypes")
public class AlarmV2Service implements IAlarmV2Service {

	@Autowired
	IAlarmV2RepositoryDevice alarmRepositoryDevice;

	@Autowired
	IAlarmV2RepositoryEntity alarmRepositoryEntity;

	@Autowired
	IAlarmV2RepositoryEntityField alarmRepositoryEntityField;

	@Autowired
	IAlarmStateChangeActionRepository alarmStateChangeActionRepository;

	@Autowired
	IAlarmv2TypeProvider alarmTypeProvider;

	@Autowired
	IDeviceManageService deviceService;

	@Autowired
	IItemAlarmStateService itemAlarmStateService;
	
	@Autowired
	IActionTimerService actionTimerService;
	
	@Autowired
	AlarmStateChangeEntityRepository alarmStateChangeEntityRepository;

	Map<String, List<IAlarmV2>> alarmMap = new HashMap<String, List<IAlarmV2>>();

	@Override
	public List<AlarmV2Checker> getEnabledAlarmCheckers(IItemAbstract item) {
		return alarmTypeProvider.getEnabledAlarms(item);
	}

	@PostConstruct
	public void loadAlarms() {
		alarmRepositoryDevice.findAll().forEach(this::initAlarm);
		alarmRepositoryEntity.findAll().forEach(this::initAlarm);
		alarmRepositoryEntityField.findAll().forEach(this::initAlarm);
	}

	@Override
	public Map<String, List<AlarmV2>> getAlarmsMap(List<IItemAbstract> items, List<AlarmState> states,
			boolean withStateDescriptionOnly) {
		Map<String, List<AlarmV2>> result = new LinkedHashMap<>();

		List<IAlarmV2> resultList = new ArrayList<>();

		Predicate<IAlarmV2> filter = withStateDescriptionOnly
				? alarm -> isAlarmInState(alarm, states) && attachAlarmDescriptions(alarm)
				: alarm -> isAlarmInState(alarm, states);

		items.stream().forEach(
				item -> getAlarmsByItemUid(item).stream().filter(filter).forEach(alarm -> resultList.add(alarm)));

		return result;
	}

	@Override
	public boolean isAlarmInState(IAlarmV2 alarm, List<AlarmState> states) {
		return states.contains(alarm.getAlarmState());
	}

	@Override
	public boolean attachAlarmDescriptions(IAlarmV2 alarm) {
		List<String> descriptions = itemAlarmStateService.getStateDescriptions(alarm);

		alarm.setStateDescriptions(descriptions);

		return descriptions != null && descriptions.size() > 0;
	}

	@Override
	public void saveAlarm(IAlarmV2 alarm) {
		getRepository(alarm).save(alarm);
		putAlarmToCache(alarm);
		actionTimerService.initAlarmScheduling(alarm);
	}

	@Override
	public void deleteAlarm(IAlarmV2 alarm) {
		if (alarm != null) {
			getRepository(alarm).deleteById(alarm.getId());
			removeAlarmFromCache(alarm);
		}
	}

	@Override
	public void checkForAlarmsWithParentExecutePostActionsForFields(List<IEntityField> changedValues) {
		Map<Long, IItemAbstract> entityFieldsChanged = new HashMap<>();
		Map<Long, IItemAbstract> entitiesChanged = new HashMap<>();
		Map<Long, IItemAbstract> groupsChanged = new HashMap<>();
		Map<Long, IItemAbstract> devicesChanged = new HashMap<>();

		for (IEntityField value : changedValues) {
			putById(entityFieldsChanged, value);
			putById(entitiesChanged, value.getEntity());
			putById(groupsChanged, value.getEntity().getGroup());
			putById(devicesChanged, value.getEntity().getGroup().getDevice());
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
	public List<IItemEvent> checkForAlarmsExecutePostActions(IAlarmV2 alarm) {
		
		IItemAbstract item =deviceService.getItemAbstract(alarm.getItem());
		List<IItemEvent> events = new ArrayList<IItemEvent>();

		alarm.setItem(item);
		events.addAll(checkForAlarm(alarm));
		
		events.forEach(this::processEvent);

		return events;
	}

	@Override
	public List<IItemEvent> checkForAlarmsExecutePostActions(IItemAbstract item) {
		List<IItemEvent> events = new ArrayList<IItemEvent>();

		getAlarmsByItemUid(item).forEach(alarm -> {
			alarm.setItem(item);
			events.addAll(checkForAlarm(alarm));
		});
		events.forEach(this::processEvent);

		return events;
	}

	private List<IItemEvent> checkForAlarm(IAlarmV2 alarm) {
		String checkerName = alarm.getCheckerName();
		AlarmV2Checker checker = alarmTypeProvider.getAlarmV2checker(checkerName);

		if (checker == null) {
			log.error("Null checker for alarm id=" + alarm.getId());
		}

		checker.process(alarm);
		
		actionTimerService.nextAlarmScheduling(alarm);
		
		return buildEvent(alarm);
	}

	private void processEvent(IItemEvent event) {
		IAlarmStateChangeEventProcessor processor = alarmTypeProvider.getAlarmStateChangedEventProcessor(event);

		if (processor != null && processor.isTarget(event)) {
			processor.process(event);
		}
		
		saveAlarmStateChange(event);
	}

	private void saveAlarmStateChange(IItemEvent event) {
		if(event.getOldState()!=null && !event.getOldState().equals(event.getNewState())) {
			AlarmStateChangeEntity entity = buildEntity(event, event.getOldState(), null);
			alarmStateChangeEntityRepository.save(entity);
			
			entity = buildEntity(event, event.getNewState(), event.getDescription());
			alarmStateChangeEntityRepository.save(entity);
			
			log.info("State change saved old=" + event.getOldState() + " new=" + event.getNewState());
		}
	}
	
	private AlarmStateChangeEntity buildEntity(IItemEvent event, AlarmState alarmState, String description) {
		AlarmStateChangeEntity entity = new AlarmStateChangeEntity();
		
		entity.setDateTime(DateTimeUtil.getDate());
		entity.setAlarmState(alarmState);
		entity.setValueInt(alarmState.getValueInt());
		entity.setTargetAlarmId(event.getAlarm().getId());
		entity.setType(event.getItem().getItemType());
		if(description!=null) {
			entity.setDescription(description);
		}
		
		return entity;
	}

	private List<IItemEvent> buildEvent(IAlarmV2 alarm) {

		List<IItemEvent> events = new ArrayList<IItemEvent>();
		List<IAlarmStateChangeAction> action = alarm.getCurrentActions();

		action.stream().forEach(a -> {
			if (alarm.getMessageInterval() != null && alarm.getMessageInterval() > 0) {
				a.setInterval(alarm.getMessageInterval());
			}
			events.add(new ItemChangeEvent(alarm.getItem(), a, alarm));
		});
		return events;
	}
	
	private void initAlarm(IAlarmV2 alarm) {
		putAlarmToCache(alarm);
		actionTimerService.initAlarmScheduling(alarm);
	}

	private void putAlarmToCache(IAlarmV2 alarm) {
		List<IAlarmV2> alarms = getAlarmsByItemUid(alarm.getItem());

		int ind = getAlarmIndex(alarms, alarm);

		if (ind > -1) {
			alarms.set(ind, alarm);
		} else {
			alarms.add(alarm);
		}
		
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
	
	private List<IAlarmV2> getAlarmsRequiredCheckBySchedule() {
		return alarmMap.values().stream().flatMap(list -> list.stream()).filter(
				actionTimerService::requireScheduledCheck)
				.collect(Collectors.toList());
	}
	
	@Scheduled(fixedRateString = "${smarthouse.server.alarm.request.interval:10000}")
	public void checkScheduledAlarms() {
		getAlarmsRequiredCheckBySchedule().stream().forEach(this::checkForAlarmsExecutePostActions);
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
	public List<IAlarmV2> getAlarmsForItemClassWithDescriptions(Class<?> clazz) {
		List<IAlarmV2> result = getAlarmsForItemClass(clazz);
		result.forEach(this::attachAlarmDescriptions);

		return result;
	}

	@Override
	public Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassName() {
		return getAlarmsGrouppedBy(alarm -> alarm.getTargetItemClass().getSimpleName());
	}

	@Override
	public Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassNameWithAlarmDescriptions() {
		Map<String, List<IAlarmV2>> result = getAlarmsGrouppedBy(alarm -> alarm.getTargetItemClass().getSimpleName());

		result.values().stream().forEach(list -> list.forEach(this::attachAlarmDescriptions));

		return result;
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

	public List<IAlarmV2> getAlarmsByFilterNotSorted(Predicate<? super IAlarmV2> predicate) {
		return getAllAlarms().stream().filter(predicate).collect(Collectors.toList());
	}

	public List<IAlarmV2> getAlarmsByFilterWithDescriptions(Predicate<? super IAlarmV2> predicate) {
		List<IAlarmV2> result = getAllAlarms().stream().filter(predicate)
				.sorted(AlarmV2Service::compareByItemDescriptionField).collect(Collectors.toList());

		result.forEach(this::attachAlarmDescriptions);

		return result;
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
			result = deviceService.getEntityFields().stream()
					.filter(ef -> !ef.getClazz().equals(String.class)
							&& DeviceConstants.GROUP_SENSORS.equalsIgnoreCase(ef.getEntity().getGroup().getName()))
					.collect(Collectors.toSet());
		}

		return result;
	}

	@Override
	public List<ItemAbstractDto> getEnabledAlarmAbstractTargets(IAlarmV2 alarm) {
		return getEnabledAlarmTargets(alarm).stream().sorted(ItemAbstract::compareByParentNameChain).map(ia -> ItemAbstractDto.fromItem(ia)).collect(Collectors.toList());
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

	@SuppressWarnings({ "unchecked" })
	@Override
	public void addAlarmStateChangeActionToAlarm(Long alarmId, ItemType it, AlarmStateChangeAction action) {
		IAlarmV2 alarm = getAlarm(alarmId, it);

		Set<IAlarmStateChangeAction> set = Optional.ofNullable(alarm.getActions()).orElse(new LinkedHashSet());
		set.add(action);
		alarm.setActions(set);

		saveAlarm(alarm);
	}

	@SuppressWarnings({ "unchecked" })
	@Override
	public void deleteAlarmStateChangeActionFromAlarm(Long alarmId, ItemType it, Long actionId) {
		IAlarmV2 alarm = getAlarm(alarmId, it);

		Set<IAlarmStateChangeAction> set = Optional.ofNullable(alarm.getActions()).orElse(new LinkedHashSet());

		IAlarmStateChangeAction action = set.stream().filter(a -> actionId.equals(a.getId())).findFirst().orElse(null);

		Optional.ofNullable(action).ifPresent(set::remove);

		alarm.setActions(set);
		saveAlarm(alarm);
		try {
			alarmStateChangeActionRepository.deleteById(actionId);
		} catch (Exception e) {
			log.error("Error delete actionId=" + actionId);
		}
	}

	@Override
	public IAlarmV2 getAlarm(Long id, ItemType it) {
		if (id != null && id > 0) {
			IAlarmV2 al = getAlarmByFilter(alarm -> alarm.getItemType().equals(it) && id.equals(alarm.getId()))
					.orElse(null);
			if (al != null) {
				return al;
			}

		}
		return getAlarmOrDefault(id, newAlarm(it));
	}

	@Override
	public AlarmV2Container getAlarmsContainerWithChildren(IItemAbstract parent) {
		List<IItemAbstract> children = parent.getAllChildren();
		children.add(parent);

		List<IAlarmV2> alarms = new ArrayList<>();

		children.stream().forEach(item -> alarms.addAll(getAlarmsByItemUid(item)));

		AlarmV2Container result = new AlarmV2Container();

		alarms.forEach(alarm -> {
			itemAlarmStateService.getStateDescriptions(alarm);
			result.putAlarm(alarm);
		});

		return result;
	}

	@Override
	public void checkForAlarmsWithParentExecutePostActions(List<IEntityFieldValue> changedValues) {
		List<IEntityField> entityFields = changedValues.stream().map(value -> value.getEntityField())
				.collect(Collectors.toList());
		checkForAlarmsWithParentExecutePostActionsForFields(entityFields);
	}

}
