package com.balabas.smarthouse.server.entity.model;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_STATUS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SWG;
import static com.balabas.smarthouse.server.DeviceConstants.HTTP_PREFFIX;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE_INFO;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_URL_DATA;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_URL_DATA;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_FIRMWARE;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_COUNT;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_C;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_FIELDS_ARRAY;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ITEM_CLASS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_KEYS_ARRAY;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_CLASS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_CLASS_VIEW;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_ENTITY_FIELDS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ACTION_DESCR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCR_FIELD;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_EMOJI;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ENABLED_VALUES;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_NAME;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ID;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_READ_ONLY;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EMPTY_STR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.FALSE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.HIGH;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.TRUE;

import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import org.json.JSONArray;
import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityClass;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassType;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.entity.model.descriptor.ActionTimer;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.balabas.smarthouse.server.model.request.DeviceRequest;

import lombok.experimental.UtilityClass;
import lombok.extern.log4j.Log4j2;

@SuppressWarnings("rawtypes")
@UtilityClass
@Log4j2
public class SmartHouseEntityBuilder {

	private static List<String> notUpdateableEntityFields = Arrays.asList(ENTITY_FIELD_STATUS,
			ENTITY_FIELD_SENSOR_ITEMS, ENTITY_FIELD_SWG, ENTITY_FIELD_ITEM_CLASS);

	private static List<String> notParsedAsEntityNames = Arrays.asList(ENTITY_FIELD_DESCRIPTION, EDC_FIELD_EMOJI);

	public static IDevice buildDeviceFromRequest(DeviceRequest request) {
		Device device = new Device();

		device.setName(request.getDeviceId());
		device.setDescription(request.getDeviceDescr());

		device.setIp(request.getIp());

		String baseUrl = HTTP_PREFFIX + request.getIp();

		String deviceDataUrl = DEVICE_URL_DATA;

		if (request.hasData()) {
			String data = request.getData();

			JSONObject dataJson = new JSONObject(data);

			if (dataJson.has(DEVICE_FIELD_URL_DATA)) {
				deviceDataUrl = dataJson.optString(DEVICE_FIELD_URL_DATA, DEVICE_URL_DATA);
			}
		}

		if (!deviceDataUrl.startsWith(HTTP_PREFFIX)) {
			deviceDataUrl = baseUrl + deviceDataUrl;
		}

		device.setDataUrl(deviceDataUrl);
		device.setState(State.CONSTRUCTED);

		device.setTimer(buildTimer(ItemType.DEVICE));

		return device;
	}

	private static ActionTimer buildTimer(ItemType itemType) {
		Long updateInterval = itemType.refreshInterval;

		if (updateInterval > 0) {
			ActionTimer updateTimer = new ActionTimer(updateInterval);
			updateTimer.setActionForced(true);
			return updateTimer;
		}

		return null;
	}

	public static boolean updateDeviceEntityValuesFromJson(IDevice device, JSONObject deviceJson) {

		boolean isOk = true;

		for (String groupName : JSONObject.getNames(deviceJson)) {

			IGroup group = device.getGroup(groupName);
			if (group != null) {
				JSONObject groupJson = deviceJson.optJSONObject(groupName);

				boolean groupOk = updateGroupEntityValuesFromJson(group, groupJson);

				isOk = isOk && groupOk;
			}
		}
		if(isOk) {
			device.getTimer().setActionSuccess();
		}
		return isOk;
	}

	private static boolean updateGroupEntityValuesFromJson(IGroup group, JSONObject groupJson) {
		boolean isOk = true;
		for (String entityName : JSONObject.getNames(groupJson)) {

			IEntity entity = group.getEntity(entityName);
			JSONObject entityJson = groupJson.optJSONObject(entityName);

			if (entity != null && entityJson != null && !entityJson.isEmpty()) {
				boolean entityOk = updateEntityValuesFromJson(entity, entityJson);

				if (entityOk) {
					entity.setState(State.OK);
				} else {
					entity.setState(State.BAD_DATA);
				}

				isOk = isOk && entityOk;
			}
		}
		if(isOk) {
			group.getTimer().setActionSuccess();
		}
		return isOk;
	}

	@SuppressWarnings("unchecked")
	private static boolean updateEntityValuesFromJson(IEntity entity, JSONObject entityJson) {
		boolean setOk = true;

		for (String entityFieldName : JSONObject.getNames(entityJson)) {

			if (!notUpdateableEntityFields.contains(entityFieldName)) {
				try {
					IEntityField entityField = entity.getEntityField(entityFieldName);
					Object valueObj = entityJson.get(entityFieldName);

					if(entity.getDescriptionField().equals(entityFieldName)) {
						entity.setDescription(valueObj.toString());
					}
					
					if (entityField != null) {
						try {
							entityField.setValueWithNoCheck(valueObj);
						} catch (BadValueException e) {
							log.error(e);
							setOk = false;
						}

					} else {
						log.warn("New field " + entityFieldName + " entity = " + entity.getName() + " device = "
								+ entity.getDeviceName());
						try {
							IEntityField field = buildEntityFieldFromJson(entityJson, entityFieldName);
							entity.getEntityFields().add(field);
						} catch (BadValueException e) {
							log.error(e);
						}

					}
				} catch (Exception e) {
					log.error("field " + entityFieldName + " : ", e);
				}
			}
		}

		return setOk;
	}

	public static void processDeviceInfo(IDevice device, JSONObject deviceJson) {
		if (deviceJson.has(DEVICE_FIELD_DEVICE)) {
			JSONObject deviceDeviceJson = deviceJson.getJSONObject(DEVICE_FIELD_DEVICE);

			if (deviceDeviceJson.has(DEVICE_FIELD_DEVICE_INFO)) {
				JSONObject info = Optional.ofNullable(deviceDeviceJson.getJSONObject(DEVICE_FIELD_DEVICE_INFO))
						.orElse(new JSONObject());

				device.setDescription(info.optString(ENTITY_DEVICE_DEVICE_DESCRIPTION, device.getDescription()));
				device.setFirmware(info.optString(ENTITY_DEVICE_DEVICE_FIRMWARE, device.getFirmware()));

				if (!info.has(EDC_FIELD_EMOJI)) {
					device.setEmoji(ItemType.DEVICE.getEmoji());
				} else {
					String emojiStr = info.optString(EDC_FIELD_EMOJI, ItemType.DEVICE.getEmoji().code);

					device.setEmoji(Emoji.getByCode(emojiStr));
				}
			}
		}
	}

	public static boolean buildDeviceFromJson(IDevice device, JSONObject deviceJson) {
		Set<IGroup> groups = SmartHouseEntityBuilder.buildGroupsFromJson(device.getName(), deviceJson);
		boolean initOk = !groups.isEmpty();

		if (initOk) {
			for (IGroup group : groups) {
				Set<IEntity> entities = SmartHouseEntityBuilder.buildEntitiesForGroup(group, deviceJson);

				group.setChildren(entities);
				initOk = initOk && !entities.isEmpty();
			}
		}

		device.setChildren(groups);

		return initOk;
	}

	public static Set<IGroup> buildGroupsFromJson(String deviceName, JSONObject deviceJson) {
		Set<IGroup> groups = new LinkedHashSet<>();

		for (String groupName : JSONObject.getNames(deviceJson)) {
			if (!DEVICE_FIELD_DEVICE.equals(groupName)) {
				ItemType type = ItemType.getItemTypeByName(groupName);

				if (!ItemType.CUSTOM.equals(type)) {
					String description = deviceJson.getJSONObject(groupName).optString(ENTITY_FIELD_DESCRIPTION,
							type.description);
					String emojiDescr = deviceJson.getJSONObject(groupName).optString(EDC_FIELD_EMOJI, null);
					Emoji groupEmoji = (emojiDescr != null) ? Emoji.getByCode(emojiDescr) : type.getEmoji();

					IGroup group = new Group();

					group.setEmoji(groupEmoji);
					group.setDeviceName(deviceName);
					group.setDescription(description);
					group.setName(groupName);
					group.setType(type);
					group.setTimer(buildTimer(type));

					groups.add(group);
				}
			}
		}

		return groups;
	}

	public static Set<IEntity> buildEntitiesForGroup(IGroup group, JSONObject deviceJson) {
		try {
			return buildEntitiesForGroup(deviceJson, group.getDeviceName(), group.getName());
		} catch (BadValueException e) {
			log.error(e);
			return Collections.emptySet();
		}
	}

	public static Set<IEntity> buildEntitiesForGroup(JSONObject deviceJson, String deviceName, String groupName)
			throws BadValueException {
		Set<IEntity> entities = new LinkedHashSet<>();

		JSONObject groupJson = deviceJson.optJSONObject(groupName);

		for (String entityName : JSONObject.getNames(groupJson)) {
			if (!notParsedAsEntityNames.contains(entityName)) {
				IEntity entity = buildEntityFromJson(groupJson, deviceName, groupName, entityName);
				entities.add(entity);
			}
		}

		return entities;
	}

	public static IEntity buildEntityFromJson(JSONObject groupJson, String deviceName, String groupName,
			String entityName) throws BadValueException {

		JSONObject entityJson = groupJson.optJSONObject(entityName);
		JSONObject descriptorJson = Optional.ofNullable(entityJson.optJSONObject(ENTITY_FIELD_SWG))
				.orElse(new JSONObject());

		String description = entityJson.optString(ENTITY_FIELD_DESCRIPTION, null);
		Emoji emoji = Emoji.getByCode(descriptorJson.optString(EDC_FIELD_EMOJI, null));
		String descriptionField = descriptorJson.optString(EDC_FIELD_DESCR_FIELD, ENTITY_FIELD_DESCRIPTION);
		int remoteId = descriptorJson.optInt(EDC_FIELD_ID, -1);
		EntityClass renderer = EntityClass.getByKey(
				entityJson.optString(ENTITY_FIELD_ITEM_CLASS, descriptorJson.optString(ENTITY_FIELD_ITEM_CLASS, "")));

		IEntity entity = new Entity();

		entity.setName(entityName);
		entity.setDeviceName(deviceName);
		entity.setGroupName(groupName);
		entity.setDescription(description);
		entity.setEmoji(emoji);
		entity.setDescriptionField(descriptionField);
		entity.setRemoteId(remoteId);
		entity.setRenderer(renderer);

		Set<IEntityField> children = new LinkedHashSet<>();

		for (String fieldName : JSONObject.getNames(entityJson)) {
			if (ENTITY_FIELD_STATUS.equals(fieldName)) {
				processEntityStatus(entity, entityJson);
			} else if (ENTITY_FIELD_SENSOR_ITEMS.equals(fieldName)) {
				processGrouppedValues(entity, entityJson);
			} else if (!ENTITY_FIELD_SWG.equals(fieldName) && !ENTITY_FIELD_ITEM_CLASS.equals(fieldName)) {
				IEntityField field = buildEntityFieldFromJson(entityJson, fieldName);
				children.add(field);
			}
		}

		entity.setChildren(children);

		return entity;
	}

	private static void processEntityStatus(IEntity entity, JSONObject entityJson) {
		JSONObject statusJson = entityJson.optJSONObject(ENTITY_FIELD_STATUS);
		if (statusJson != null) {
			int stat = entityJson.optInt(EDC_FIELD_ID, 1);
			String descr = entityJson.optString(EDC_FIELD_DESCRIPTION, "OK");

			entity.setStatus(new EntityStatus(stat, descr));
		}
	}

	@SuppressWarnings("unchecked")
	private static void processGrouppedValues(IEntity entity, JSONObject entityJson) {
		JSONObject sensorItemsJson = entityJson.getJSONObject(ENTITY_FIELD_SENSOR_ITEMS);

		log.debug("sensorItemsJson : " + sensorItemsJson);

		int count = sensorItemsJson.optInt(ENTITY_FIELD_COUNT, sensorItemsJson.optInt(ENTITY_FIELD_C, 0));

		Set<String> grouppedFieldsNames = Optional.ofNullable(sensorItemsJson.optJSONArray(ENTITY_FIELD_FIELDS_ARRAY))
				.orElse(new JSONArray()).toList().stream().map(Object::toString).collect(Collectors.toSet());

		Set<String> grouppedFieldsIds = Optional.ofNullable(sensorItemsJson.optJSONArray(ENTITY_FIELD_KEYS_ARRAY))
				.orElse(new JSONArray()).toList().stream().map(Object::toString).collect(Collectors.toSet());

		if (count == 0 && !grouppedFieldsIds.isEmpty()) {
			count = grouppedFieldsIds.size();
		}

		if (grouppedFieldsIds.isEmpty() && count > 0) {
			IntStream.range(0, count).forEach(i -> grouppedFieldsIds.add(Integer.toString(i)));
		}

		if (count == 0 || grouppedFieldsNames.isEmpty() || grouppedFieldsIds.size() != count) {
			log.error("Bad SensorItems count=" + count + " fields=" + grouppedFieldsNames + " grouppedFieldsIds = "
					+ grouppedFieldsIds);
		} else {
			entity.setGrouppedFieldsIds(grouppedFieldsIds);
			entity.setGrouppedFieldsNames(grouppedFieldsNames);

			try {
				IEntityField countField = new EntityFieldInteger();
				countField.setName(ENTITY_FIELD_COUNT);
				countField.setReadOnly(true);
				countField.setValueWithNoCheck(Integer.valueOf(count));

				entity.addGeneratedField(countField);
			} catch (BadValueException e) {
				log.error(e);
			}

			log.debug("Sensor items added");
		}

	}

	@SuppressWarnings("unchecked")
	public static IEntityField buildEntityFieldFromJson(JSONObject entityJson, String fieldName) throws BadValueException {
		JSONObject descriptorJson = entityJson.optJSONObject(ENTITY_FIELD_SWG);
		JSONObject allFieldsDecriptor = descriptorJson.optJSONObject(EDC_ENTITY_FIELDS);

		JSONObject fieldDecriptor = Optional.ofNullable(allFieldsDecriptor.optJSONObject(fieldName))
				.orElse(new JSONObject());

		String value = entityJson.optString(fieldName);

		EntityFieldClassType fieldClassType = Optional.ofNullable(
				EntityFieldClassType.from(fieldDecriptor.optString(EDC_CLASS, null))).orElse(EntityFieldClassType.STRING); 
		
		String name = fieldDecriptor.optString(EDC_FIELD_NAME, fieldName);
		String description = fieldDecriptor.optString(EDC_FIELD_DESCRIPTION, EMPTY_STR);
		EntityFieldClassView viewClass = EntityFieldClassView.from(fieldDecriptor.optString(EDC_CLASS_VIEW, null));
		boolean readOnly = booleanFromString(fieldDecriptor.optString(EDC_READ_ONLY, FALSE));
		Emoji emoji = Emoji.getByCode(fieldDecriptor.optString(EDC_FIELD_EMOJI, null));

		Set<IEntityField> enabledValues = getEnabledFieldValues(fieldDecriptor, name, fieldClassType, viewClass);

		IEntityField entityField = createEntityFieldByClass(fieldClassType);

		entityField.setName(name);
		entityField.setDescription(description);
		entityField.setViewClass(viewClass);
		entityField.setReadOnly(readOnly);
		entityField.setEmoji(emoji);
		entityField.setValueStr(value);
		entityField.setEnabledValues(enabledValues);

		return entityField;
	}

	@SuppressWarnings("unchecked")
	private static Set<IEntityField> getEnabledFieldValues(JSONObject fieldDecriptor, String fieldName, EntityFieldClassType fieldClassType,
			EntityFieldClassView parentViewClass) throws BadValueException {
		if (fieldDecriptor.has(EDC_FIELD_ENABLED_VALUES)) {
			JSONObject enabledFieldJson = fieldDecriptor.getJSONObject(EDC_FIELD_ENABLED_VALUES);

			Set<IEntityField> enabledValues = new LinkedHashSet<>();
			long id = 0;

			for (Entry<String, Object> entry : enabledFieldJson.toMap().entrySet()) {
				String enabledValueStr = entry.getKey();

				Map<String, String> enabledValueBody = (Map<String, String>) entry.getValue();

				String description = enabledValueBody.getOrDefault(EDC_FIELD_DESCRIPTION, null);
				String actionDescription = enabledValueBody.getOrDefault(EDC_FIELD_ACTION_DESCR, null);
				Emoji emoji = Emoji.getByCode(enabledValueBody.getOrDefault(EDC_FIELD_EMOJI, null));
				EntityFieldClassView viewClass = EntityFieldClassView
						.from(enabledValueBody.getOrDefault(EDC_CLASS_VIEW, parentViewClass.getKey()));

				IEntityField enabledValue = createEntityFieldByClass(fieldClassType);
				enabledValue.setId(id);
				enabledValue.setName(fieldName);
				enabledValue.setDescription(description);
				enabledValue.setViewClass(viewClass);
				enabledValue.setActionDescription(actionDescription);
				enabledValue.setEmoji(emoji);
				enabledValue.setValueStr(enabledValueStr);

				enabledValues.add(enabledValue);

				id++;
			}

			return enabledValues;
		}

		return Collections.emptySet();

	}

	private static IEntityField createEntityFieldByClass(EntityFieldClassType fieldClassType) {
		if (EntityFieldClassType.BOOLEAN.equals(fieldClassType)) {
			return new EntityFieldBoolean();
		} else if (EntityFieldClassType.INTEGER.equals(fieldClassType)) {
			return new EntityFieldInteger();
		} else if (EntityFieldClassType.FLOAT.equals(fieldClassType)) {
			return new EntityFieldFloat();
		} else if (EntityFieldClassType.LONG.equals(fieldClassType)) {
			return new EntityFieldLong();
		} else if (EntityFieldClassType.IP.equals(fieldClassType)) {
			return new EntityFieldIp();
		} else if (EntityFieldClassType.PASSWORD.equals(fieldClassType)) {
			return new EntityFieldPassword();
		} else {
			return new EntityFieldString();
		}
	}

	private static boolean booleanFromString(String value) {
		return (TRUE.equalsIgnoreCase(value) || HIGH.equalsIgnoreCase(value)
				|| value != null && !value.isEmpty() && value.startsWith(HIGH));
	}
}