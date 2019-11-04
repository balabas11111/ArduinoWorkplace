package com.balabas.smarthouse.telegram.bot.message;

import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.stream.Collectors;

import org.apache.logging.log4j.util.Strings;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardRemove;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.entity.alarm.IEntityAlarm;
import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.alarm.IEntityFieldAlarm;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractByDescriptionComparator;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.view.Action;
import com.google.common.collect.Lists;

import static com.balabas.smarthouse.server.view.Action.ACTION_DATA_FIELD_NAME;
import static com.balabas.smarthouse.server.view.Action.ACTION_DATA_FIELD_CLASS;

import static com.balabas.smarthouse.telegram.bot.message.ItemTextHelper.getEntityFieldButtonText;

import lombok.Getter;

@Component
@SuppressWarnings("rawtypes")
public class SendMessageBuilder {

	@Value("${telegram.bot.server.display.address:#{null}}")
	private String serverDisplayAddress;
	
	@Autowired
	private IDeviceManageService deviceService;

	@Autowired
	private IEntityAlarmService entityAlarmService;

	@Autowired
	private InlineKeyboardBuilder inlineKeyboard;

	@Autowired
	@Getter
	private ReplyKeyboardBuilder replyKeyboard;

	@Autowired
	private EntityViewBuilder itemRendererBuilder;

	@Autowired
	private ItemTextHelper itemTextHelper;

	Comparator<ItemAbstract> itemAbstractComparator = new ItemAbstractByDescriptionComparator();

	public List<SendMessage> createServerStartedMessages(Long chatId, String serverName) {
		List<SendMessage> msgs = Lists.newArrayList();
		String text = String.format(BotMessageConstants.BOT_REGISTERED_MSG, Emoji.PERSON_RAISING_ONE_HAND, serverName,
				new Date());
		msgs.add(createRefreshDevicesListReplyKeyboard(chatId, text));
		return msgs;
	}

	public List<SendMessage> createDeviceRegisteredMessages(String deviceName, Long chatId) {
		List<SendMessage> msgs = Lists.newArrayList();

		String text = String.format(BotMessageConstants.DEVICE_REGISTERED_MSG, Emoji.CHECK_MARK, deviceName,
				new Date());

		msgs.add(createRefreshDevicesListReplyKeyboard(chatId, text));
		return msgs;
	}

	public List<SendMessage> createRefreshDevicesListReplyKeyboard(ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();
		msgs.add(context.createMsg(replyKeyboard.getMainMenuReplyKeyboard()));

		return msgs;
	}

	public SendMessage createRefreshDevicesListReplyKeyboard(Long chatId, String text) {
		return ReplyContext.createMsg(replyKeyboard.getMainMenuReplyKeyboard(), null, chatId, text, true);
	}

	public List<Device> getDevices() {
		return deviceService.getDevicesInitialized().stream().sorted(itemAbstractComparator)
				.collect(Collectors.toList());
	}

	public List<SendMessage> createDevicesListView(Action action, ReplyContext cont) {
		String serverAddress = Optional.ofNullable(serverDisplayAddress).orElse("");
		
		String serverName = action.getServerName();
		List<SendMessage> msgs = Lists.newArrayList();
		List<Device> devices = getDevices();

		cont.setText((devices.isEmpty()) ? String.format(BotMessageConstants.NO_DEVICE_MSG, Emoji.WARNING, serverAddress)
				: String.format(BotMessageConstants.SERVER_SELECT_DEVICE_VIEW_MSG, Emoji.OUTBOX_TRAY, serverName, serverAddress));

		msgs.add(cont.createMsg(inlineKeyboard.getDevicesOfServerInlineKeyboardView(devices)));

		return msgs;
	}

	public List<SendMessage> createSetupMenu(Action action, ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();

		context.setText(String.format(BotMessageConstants.SELECT_SETUP_ACTION, Emoji.WARNING));
		msgs.add(context.createMsg(inlineKeyboard.getSetupMenuKeyboard()));

		return msgs;
	}

	public List<SendMessage> createEditAlarmsMenu(Action action, ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();
		List<Device> devices = getDevices();

		context.setText(String.format(BotMessageConstants.SELECT_DEVICE_TO_EDIT_ALARMS, Emoji.WARNING));
		msgs.add(context.createMsg(inlineKeyboard.getAlarmsMenuKeyboard(devices)));

		return msgs;
	}

	public List<SendMessage> createEditAlarmsOfDevice(Action action, ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();

		IDevice device = deviceService.getDeviceByName(action.getDeviceName());

		List<IEntity> entities = entityAlarmService.getEntitiesWithPossibleAlarms(device);

		context.setText(String.format(BotMessageConstants.SELECT_ENTITY_TO_EDIT_ALARMS, Emoji.ERROR, device.getName()));
		msgs.add(context.createMsg(inlineKeyboard.getEntitiesAlarmsOfDeviceMenuKeyboard(entities)));

		return msgs;
	}

	public List<SendMessage> createEditAlarmsOfEntity(Action action, ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();

		IDevice device = deviceService.getDeviceByName(action.getDeviceName());
		IEntity entity = device.getEntity(action.getEntityName());

		List<IEntityField> entityFields = entityAlarmService.getEntityFieldsWithPossibleAlarms(entity);

		IEntityAlarm entityAlarm = entityAlarmService.getAlarm(entity);

		StringBuilder buf = new StringBuilder();

		if (entityAlarm == null) {
			buf.append(BotMessageConstants.NO_ENTITY_ALARMS_MESSAGE);
		} else {
			buf.append(String.format(BotMessageConstants.SELECT_ENTITY_FIELD_TO_EDIT_ALARMS,
					Optional.ofNullable(entity.getEmoji()).orElse(Emoji.PAGER), entity.getDescription()));

			if (!entityAlarm.isActivated()) {
				buf.append(BotMessageConstants.ENTITY_ALARM_NOT_ACTIVATED_MESSAGE);
			} else {
				buf.append(BotMessageConstants.ENTITY_ALARM_ACTIVATED_MESSAGE);

				if (entityAlarm.isNotificationRepeatable()) {
					buf.append(String.format(BotMessageConstants.ENTITY_HAS_ALARM_INTERVAL_MESSAGE,
							entityAlarm.getMessageInterval()));
				} else {
					buf.append(BotMessageConstants.ENTITY_HAS_NO_ALARM_INTERVAL_MESSAGE);
				}

				buf.append(entityAlarm.isSound() ? BotMessageConstants.ENTITY_SOUND_ACTIVATED_MESSAGE
						: BotMessageConstants.ENTITY_SOUND_NOT_ACTIVATED_MESSAGE);

				buf.append("\n");
				buf.append(
						String.format(BotMessageConstants.ENTITY_ALARM_COUNT_MESSAGE, entityAlarm.getAlarms().size()));
			}
			buf.append("\n");

			for (IEntityFieldAlarm entityFieldAlarm : entityAlarm.getAlarms()) {
				buf.append(String.format(BotMessageConstants.ENTITY_ALARM_DISPLAY_MESSAGE,
						entityFieldAlarm.getWatchedItem().getNameDescriptionByDescriptionField(),
						entityFieldAlarm.getTriggerDescription()));

			}
		}

		context.setText(buf.toString());

		msgs.add(context.createMsg(
				inlineKeyboard.getEntityFieldsAlarmsOfEntityMenuKeyboard(entity, entityAlarm, entityFields)));

		return msgs;
	}

	public SendMessage createAlarmUpdatedMessage(Action action, ReplyContext context) {
		String message = String.format(BotMessageConstants.ENTITY_ALARM_SAVED_MESSAGE, deviceService
				.getDeviceByName(action.getDeviceName()).getEntity(action.getEntityName()).getDescription());
		return createHtmlMessage(context.getChatId(), message);
	}

	public SendMessage getAlarmToBeAddedMessage(Action action, ReplyContext context) {

		IDevice device = deviceService.getDeviceByName(action.getDeviceName());
		IEntity entity = device.getEntity(action.getEntityName());

		IEntityField entityField = entity.getEntityField(action.getTargetId());

		Integer alarmClassIndex = Integer.valueOf(action.getDataJsonField(ACTION_DATA_FIELD_CLASS));
		String alarmClassName = entityAlarmService.getEntityFieldAllowedClassByIndex(alarmClassIndex).getSimpleName();

		String message = String.format(BotMessageConstants.ENTITY_ENTITY_FIELD_ALARM_ADD_VALUE_MESSAGE, alarmClassName,
				entity.getName(), getEntityFieldButtonText(entityField));
		return createHtmlMessage(context.getChatId(), message);
	}

	public SendMessage getAlarmToBeSavedMessage(Action action, ReplyContext context) {

		IEntityFieldAlarm entityFieldAlarm = entityAlarmService.getEntityAlarmFieldById(action.getTargetId());

		IEntityField entityField = entityFieldAlarm.getWatchedItem();

		IEntity entity = entityField.getEntity();

		String alarmClassName = entityFieldAlarm.getClass().getSimpleName();
		String currentValue = entityFieldAlarm.getTriggerDescription();

		String message = String.format(BotMessageConstants.ENTITY_ENTITY_FIELD_ALARM_EDIT_VALUE_MESSAGE, alarmClassName,
				entity.getName(), entityField.getNameDescriptionByDescriptionField(), currentValue);
		return createHtmlMessage(context.getChatId(), message);
	}

	public SendMessage getAlarmIntervalToBeSavedMessage(Action action, ReplyContext context) {
		String message = String.format(BotMessageConstants.ENTITY_ENTITY_ALARM_INTERVAL_VALUE_MESSAGE, deviceService
				.getDeviceByName(action.getDeviceName()).getEntity(action.getEntityName()).getDescription());
		return createHtmlMessage(context.getChatId(), message);
	}

	public List<SendMessage> createEditAlarmsOfEntityField(Action action, ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();

		IDevice device = deviceService.getDeviceByName(action.getDeviceName());
		IEntity entity = device.getEntity(action.getEntityName());
		IEntityAlarm entityAlarm = entityAlarmService.getAlarm(entity);
		IEntityField entityField = entity.getEntityField(action.getTargetId());

		Map<Integer, Class> enabledAlarmClasses = entityAlarmService.getEnabledAlarmsForField(entityField);

		StringBuilder buf = new StringBuilder();

		buf.append(String.format(BotMessageConstants.SELECT_ENTITY_FIELD_EDIT_ALARMS, Emoji.WARNING,
				entityField.getNameDescriptionByDescriptionField()));

		for (IEntityFieldAlarm entityFieldAlarm : entityAlarm.getAlarms()) {
			buf.append(String.format(BotMessageConstants.ENTITY_FIELD_ALARM_DISPLAY_MESSAGE,
					entityFieldAlarm.getTriggerDescription()));
		}

		context.setText(buf.toString());

		msgs.add(context.createMsg(
				inlineKeyboard.getEntityFieldsAlarmEditMenuKeyboard(entityAlarm, entityField, enabledAlarmClasses)));

		return msgs;
	}
	// edit device properties functionality

	public SendMessage createDevicesListEdit(Action action, ReplyContext cont) {
		String serverName = action.getServerName();
		List<Device> devices = getDevices();

		cont.setText((devices.isEmpty()) ? String.format(BotMessageConstants.NO_DEVICE_MSG, Emoji.WARNING)
				: String.format(BotMessageConstants.SERVER_SELECT_DEVICE_EDIT_MSG, Emoji.OUTBOX_TRAY, serverName));

		return cont.createMsg(inlineKeyboard.getDevicesOfServerInlineKeyboardEdit(devices));
	}

	public SendMessage createServerWillBeRestartedMsg(ReplyContext context) {
		return createHtmlMessage(context.getChatId(), BotMessageConstants.SERVER_WILL_BE_RESTARTED_MESSAGE);
	}

	public SendMessage createHelpMsg(ReplyContext context) {
		return createHtmlMessage(context.getChatId(), BotMessageConstants.HELP_MESSAGE);
	}

	public SendMessage createGroupsOfDeviceInlineKeyboard(Action action, ReplyContext cont) {
		IDevice device = deviceService.getDeviceByName(action.getDeviceName());

		cont.setText(String.format(BotMessageConstants.SELECT_GROUP_MSG, Emoji.OUTBOX_TRAY, device.getDescription(),
				device.getFirmware()));

		return cont.createMsg(inlineKeyboard.getGroupsOfDeviceInlineKeyboard(device));
	}

	public SendMessage getEntitiesOfGroupInlineKeyboard(String deviceName, String groupName, ReplyContext cont) {
		IDevice device = deviceService.getDeviceByName(deviceName);
		IGroup group = device.getGroup(groupName);

		Emoji groupEmoji = itemTextHelper.getEmojiByGroupName(groupName);
		String trans = itemTextHelper.getGroupNameTranslation(groupName);

		String text = String.format(BotMessageConstants.BUTTON, groupEmoji.toString(), trans);

		cont.setText(text);

		return cont.createMsg(inlineKeyboard.getEntitiesOfGroupInlineKeyboard(device, group));
	}

	public SendMessage getEntitiesOfDeviceToEdit(Action action, ReplyContext cont) {
		IDevice device = deviceService.getDeviceByName(action.getDeviceName());

		String text = String.format(BotMessageConstants.EDIT_DEVICE_SELECT_ENTITY, Emoji.GEAR, device.getDescription());

		cont.setText(text);

		return cont.createMsg(inlineKeyboard.getEntitiesOfDeviceInlineKeyboard(device));
	}

	public SendMessage getFieldsOfEntityToEdit(Action action, ReplyContext cont) {
		IDevice device = deviceService.getDeviceByName(action.getDeviceName());
		IEntity entity = device.getEntity(action.getEntityName());

		String text = String.format(BotMessageConstants.EDIT_DEVICE_SELECT_FIELD, Emoji.GEAR, device.getDescription(),
				entity.getDescription());

		cont.setText(text);

		return cont.createMsg(inlineKeyboard.getFieldsOfEntityInlineKeyboard(entity));
	}

	public SendMessage getDeviceDescriptionToEdit(Action action, ReplyContext context) {
		IDevice device = deviceService.getDeviceByName(action.getDeviceName());

		String text = String.format(BotMessageConstants.EDIT_DEVICE_SELECTED_FIELD, device.getEmoji().toString(),
				device.getDescription(), DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION, "Название",
				device.getDescription());

		return createHtmlMessage(context.getChatId(), text);
	}

	public SendMessage getFieldToEdit(Action action, ReplyContext context) {
		IDevice device = deviceService.getDeviceByName(action.getDeviceName());
		IEntity entity = device.getEntity(action.getEntityName());

		String fieldName = new JSONObject(action.getData()).getString(ACTION_DATA_FIELD_NAME);

		IEntityField<?> entityField = entity.getEntityField(fieldName);

		String text = String.format(BotMessageConstants.EDIT_DEVICE_SELECTED_FIELD, device.getEmoji().toString(),
				entity.getDescription(), entityField.getName(), entityField.getDescription(),
				entityField.getValueStr());

		return createHtmlMessage(context.getChatId(), text);
	}

	public List<SendMessage> createGroupView(Action action, ReplyContext context) {
		List<SendMessage> result = Lists.newArrayList();

		IDevice device = deviceService.getDeviceByName(action.getDeviceName());
		if (device != null) {
			IGroup group = device.getGroup(action.getGroupName());

			StringBuilder builder = new StringBuilder();

			itemRendererBuilder.buildDeviceGroupHeaderView(device, group, builder);

			group.getEntities().stream()// .filter(e -> EntityClass.DEFAULT.equals(e.getRenderer()))
					.sorted(itemAbstractComparator).forEach(ent -> itemRendererBuilder.buildEntityView(ent, builder));

			List<IEntityAlarm> alarms = entityAlarmService.getEntityAlarmsWithAlarmDetected(device);

			if (!alarms.isEmpty()) {

				builder.append("\n---------------------\n");
				builder.append(Emoji.ERROR.toString());
				builder.append("<code> Режим ТРЕВОГИ </code>\n\n");

				group.getEntities().stream().forEach(entity -> alarms.stream()
						.filter(alarm -> entity.getName().equals(alarm.getEntity().getName()))
						.forEach(alarm -> Optional.ofNullable(alarm.getAlarmStartedText()).ifPresent(builder::append)));
			}

			result.add(createHtmlMessage(context.getChatId(), builder.toString()));

			Optional.ofNullable(buildGroupCommandInterface(device, group, context.getChatId())).ifPresent(result::add);
		}
		return result;
	}

	private SendMessage buildGroupCommandInterface(IDevice device, IGroup group, Long chatId) {

		List<Action> actions = EntityViewBuilder.getCommandButtonsForGroup(Action.ACTION_TYPE_SEND_DATA_TO_DEVICE,
				group);

		if (actions.isEmpty()) {
			return null;
		}

		return ReplyContext.createMsg(inlineKeyboard.getCommandButtonsByEnabledFieldCommandButtonList(actions), chatId,
				itemRendererBuilder.buildDeviceGroupHeaderCommandsView(device, group));
	}

	public SendMessage createUnknown(ReplyContext context) {
		String text = String.format(BotMessageConstants.DONT_UNDERSTAND_MSG, Emoji.FACE_WITH_TONGUE_AND_CLOSED_ONE_EYE,
				(context.getText() == null || context.getText().isEmpty()) ? "" : context.getText());

		return createHtmlMessage(context.getChatId(), text, context.getReplyToMessageIdIfUnknown());

	}

	public SendMessage createUnauthorized(Long chatId) {
		String text = String.format(BotMessageConstants.UNAUTHORIZED_MSG, Emoji.ERROR);

		return createHtmlMessage(chatId, text);

	}

	public SendMessage createServerError(String msg, Long chatId) {
		return createMessage(BotMessageConstants.MESSAGE_SERVER_ERROR, Emoji.STOP.toString(), chatId, msg);
	}

	public SendMessage createDeviceError(String msg, Long chatId) {
		return createMessage(BotMessageConstants.MESSAGE_DEVICE_ERROR, Emoji.STOP.toString(), chatId, msg);
	}

	public SendMessage createDeviceRefreshed(String msg, Long chatId) {
		return createMessage(BotMessageConstants.MESSAGE_DEVICE_REFRESHED, Emoji.REFRESH.toString(), chatId, msg);
	}

	public SendMessage createDataSentToDevice(String msg, Long chatId) {
		return createMessage(BotMessageConstants.MESSAGE_DATA_WAS_SENT_TO_DEVICE, Emoji.PHONE_WITH_ARROW.toString(),
				chatId, msg);
	}

	public SendMessage createDeviceDataSavedOnServer(String msg, Long chatId) {
		return createMessage(BotMessageConstants.MESSAGE_DATA_WAS_SAVED_ON_SERVER, Emoji.FLOPPY.toString(), chatId,
				msg);
	}

	public SendMessage createMessage(String format, String emoji, Long chatId, String msg) {
		String text = String.format(format, emoji, (msg == null || msg.isEmpty()) ? "" : msg);
		return createHtmlMessage(chatId, text);
	}

	public SendMessage createHideReplyKeyboardMessage(Long chatId, Integer messageId, String msgText) {
		SendMessage sendMessage = new SendMessage();
		sendMessage.setChatId(chatId.toString());
		sendMessage.enableMarkdown(true);
		sendMessage.setReplyToMessageId(messageId);

		String text = Strings.isNotEmpty(msgText) ? msgText : Emoji.WAVING_HAND_SIGN.toString();

		sendMessage.setText(text);

		ReplyKeyboardRemove replyKeyboardRemove = new ReplyKeyboardRemove();
		replyKeyboardRemove.setSelective(true);
		sendMessage.setReplyMarkup(replyKeyboardRemove);

		return sendMessage;
	}

	public SendMessage createHtmlMessage(Long chatId, String text) {
		SendMessage msg = createTextMessage(chatId, text);

		msg.enableHtml(true);

		return msg;
	}

	public SendMessage createHtmlMessage(Long chatId, String text, Integer replyToMessageId) {
		SendMessage msg = createHtmlMessage(chatId, text);
		msg.setReplyToMessageId(replyToMessageId);

		return msg;
	}

	public SendMessage createTextMessage(Long chatId, String text) {
		SendMessage msg = new SendMessage();
		msg.setChatId(chatId.toString());
		msg.setText(text);

		return msg;
	}

}
