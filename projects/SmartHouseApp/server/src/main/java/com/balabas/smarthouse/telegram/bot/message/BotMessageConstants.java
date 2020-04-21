package com.balabas.smarthouse.telegram.bot.message;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

import lombok.experimental.UtilityClass;

@UtilityClass
public class BotMessageConstants {
	
	public static final String BOT_MESSAGE_DEFAULT = "%s %s";

	public static final String BOT_TEMPLATES_PATH = "telegram/templates/text/";
	
	public static final String BOT_REGISTERED_MSG = "%s \"%s\" запущен в %tT";
	
	public static final String DEVICE_REGISTERED_MSG = "%s <b>Зарегистрировано устройство </b> \" %s \" время %tT";
	
	public static final String DONT_UNDERSTAND_MSG = "%s Я не понял что имелось ввиду:  %s";
	
	public static final String MESSAGE_SERVER_ERROR = "%s Ошибка сервера.  %s";
	public static final String MESSAGE_DEVICE_ERROR = "%s Ошибка устройства.  %s";
	
	public static final String MESSAGE_DEVICE_REFRESHED = "%s Устройство обновлено.  %s";
	public static final String MESSAGE_DEVICE_DATA_SAVED = "%s Данные сохранены.  %s";
	public static final String MESSAGE_DATA_WAS_SAVED_ON_SERVER = "%s Данные устройства сохранены.  %s";
	public static final String MESSAGE_DATA_WAS_SENT_TO_DEVICE = "%s Данные отправлены на устройство.  %s";
	
	public static final String BUTTON = "%s %s";
	
	public static final String VIEW_DEVICES_BUTTON = "%s Просмотр показателей";
	public static final String VIEW_METRICS_BUTTON = "%s Гаджеты";
	public static final String VIEW_DEVICES_LIST_BUTTON = "%s Устройства";
	public static final String SETUP_BUTTON = "%s Настройки";
	public static final String VIEW_ALARMS = "%s Опасности";
	
	public static final String RESTART_APPLICATION_BUTTON = "%s Перезапустить сервер";
	public static final String EDIT_ALARMS_BUTTON = "%s Режимы тревоги";
	public static final String EDIT_PROPERTIES_BUTTON = "%s Настройки описаний";
	
	public static final String EDIT_DEVICE_SELECT_ENTITY = "%s Редактировать %s. Выберите датчик для редактирования";
	public static final String EDIT_DEVICE_SELECT_FIELD = "%s Устройство %s. \n Датчик <b>%s</b>. Выберите поле для редактирования";
	public static final String EDIT_DEVICE_SELECTED_FIELD =  
			"%s %s\n Редактируем поле (%s) - <b>%s</b>\n Поточное значение %s\n"
			+ "введите новое значение и нажмите кнопку отправить";
	
	public static final String ADD_ALARM_BUTTON = "%s Добавить : %s";
	public static final String REMOVE_ALARM_BUTTON = "%s Удалить : %s";
	public static final String EDIT_ALARM_BUTTON = "%s Редактировать %s";
	
	public static final String EDIT_DEVICE_SELECT_ENTITY_FIELD_BUTTON = "(%s) Редактировать : %s";
	
	public static final String HELLO_MSG = "%s Привет %s, набери %s для просмотра устройств";
	
	public static final String UNAUTHORIZED_MSG = "%s No Access = No success. Suck, man";
	
	public static final String GROUP_VIEW_HEADER_MSG = "%s <b>%s</b>      %s <code>%s</code> \n";
	public static final String GROUP_COMMAND_HEADER_MSG = Emoji.JOYSTICK.toString() + 
								"<b>Управление</b>      %s <b>%s</b>";
	public static final String ENTITY_DISPLAY_HEADER_MSG = "--------------------\n %s <b>%s</b> :     ";
	
	public static final String SELECT_DEVICE_MSG = "%s Выберите устройство";
	public static final String SERVER_SELECT_ACTION = "Сервер %s \n %s";
	public static final String SERVER_SELECT_DEVICE_VIEW_MSG = "%s Сервер %s \n %s";
	public static final String SERVER_SELECT_DEVICE_EDIT_MSG = "%s Сервер %s - Редактировать параметры";
	public static final String NO_DEVICE_MSG = "%s На сервере не зарегистрировано ни одно устройство \n %s";
	public static final String SERVER_BY_BY_ACTION = "Сервер сказал бай бай";
	
	public static final String SELECT_GROUP_MSG = 
	        "%s Устройство \n"+
	        "<b>: %s </b>\n"+
	        ": %s ";
	public static final String SELECT_ALL_METRICS = " Метрики";
	public static final String SELECT_ALL_DEVICES = " Устройства";
	public static final String SELECT_SETUP_ACTION = "%s Выберите группу настроек";
	public static final String SELECT_DEVICE_TO_EDIT_ALARMS = "%s Выберите устройство для просмотра режима тревоги";
	public static final String SELECT_ENTITY_TO_EDIT_ALARMS = "%s Выберите датчик для просмотра режима тревоги. устройство %s";
	
	public static final String NO_ENTITY_ALARMS_MESSAGE = Emoji.FACE_WITH_TONGUE_AND_CLOSED_ONE_EYE.toString() + " нет проверок тревоги\n";
	public static final String SELECT_ENTITY_FIELD_TO_EDIT_ALARMS = 
			Emoji.BAR_CHART.toString() + "Редактировать режимы тревоги %s <b>%s</b> \n";
	public static final String ENTITY_ALARM_ACTIVATED_MESSAGE = Emoji.CHECK_MARK.toString() + " проверка активирована. ";
	public static final String ENTITY_ALARM_NOT_ACTIVATED_MESSAGE = Emoji.ERROR.toString() + " проверка НЕ активирована. \n";
	public static final String ENTITY_SOUND_ACTIVATED_MESSAGE = Emoji.BELL.toString() + " звуковое оповещение.\n";
	public static final String ENTITY_SOUND_NOT_ACTIVATED_MESSAGE = Emoji.BELL_CANCEL.toString() + " без звука.\n";
	public static final String ENTITY_HAS_ALARM_INTERVAL_MESSAGE = " Интервал оповещений %d сек\n";
	public static final String ENTITY_HAS_NO_ALARM_INTERVAL_MESSAGE = " Оповещение не повторяется\n";
	public static final String ENTITY_ALARM_COUNT_MESSAGE = "Проверок тревоги %d\n";
	public static final String ENTITY_ALARM_DISPLAY_MESSAGE = "<b>%s</b> значение %s;\n";
	public static final String ENTITY_FIELD_ALARM_DISPLAY_MESSAGE = "%s;\n";
	
	public static final String ENTITY_ALARM_SAVED_MESSAGE = Emoji.FLOPPY.toString() + " Настройки тревоги сохранены %s";
	public static final String ENTITY_ENTITY_ALARM_INTERVAL_VALUE_MESSAGE = 
			Emoji.GEAR.toString() + " Введите значение интервала оповещения для датчика %s";
	
	public static final String ENTITY_ENTITY_FIELD_ALARM_ADD_VALUE_MESSAGE = 
			Emoji.GEAR.toString() + " Введите Новое значение <b>%s</b> для <b>%s</b> -> %s";
	public static final String ENTITY_ENTITY_FIELD_ALARM_EDIT_VALUE_MESSAGE = 
			Emoji.GEAR.toString() + " Введите значение <b>%s</b> для <b>%s</b> -> %s  текущее значение %s";
	
	public static final String ENTITY_ALARM_CREATE_MESSAGE = Emoji.CHECK_MARK.toString() + " создать проверку тревоги";
	public static final String ENTITY_ALARM_ACTIVATE_MESSAGE = Emoji.ARROW_DOWN.toString() + " активировать проверку тревоги";
	public static final String ENTITY_ALARM_DEACTIVATE_MESSAGE = Emoji.CROSS_MARK_RED.toString() + " деактивировать проверку тревоги";
	public static final String ENTITY_ALARM_ENABLE_SOUND_MESSAGE = Emoji.BELL.toString() + " разрешить звук на сервере";
	public static final String ENTITY_ALARM_DISABLE_SOUND_MESSAGE = Emoji.BELL_CANCEL.toString() + "  отменить звук на сервере";
	public static final String ENTITY_ALARM_EDIT_INTERVAL_MESSAGE = Emoji.HOURGLASS.toString() + " редактировать интервал оповещений";
	public static final String ENTITY_ALARM_REMOVE_INTERVAL_MESSAGE = Emoji.CROSS_MARK_RED.toString() + " только одно оповещение";
			
	public static final String SELECT_ENTITY_FIELD_EDIT_ALARMS = "%s Режимы тревоги %s \n\n";
	public static final String ENTITY_ALARM_HEADER = "\n--------------------------\n" + Emoji.ERROR.toString() + "<code> Режим ТРЕВОГИ </code>\n\n";
	
	public static final String SPACE = " ";
	public static final String USER = "Пользователь";
	public static final String SELECTED = "Выбрано";
	public static final String NOT_FOUND = "Не найдено";
	
	public static final String COMMAND_HOME = "/home";
	public static final String COMMAND_ROOT = "/";
	public static final String COMMAND_HIDE = "/hide";
	public static final String COMMAND_SETUP = "/setup";
	public static final String COMMAND_ALARMS = "/alarms";
	public static final String COMMAND_ALARM = "/alarm";
	public static final String COMMAND_EDIT = "/alarm";
	public static final String COMMAND_HELP = "/help";
	
	public static final String TOOGLE_STATE = "сост";
	public static final String TOOGLE_ON = "Включено";
	public static final String TOOGLE_OF = "Отключено";
	
	public static final String SERVER_WILL_BE_RESTARTED_MESSAGE = Emoji.WAVING_HAND_SIGN.toString() + " Сервер перезапускается";
	public static final String HELP_MESSAGE = Emoji.HAND_OK.toString() + " Справка "
			+ COMMAND_SETUP + " - " + "Установки устройства" + " \n"
			+ COMMAND_ALARM + " - " + "Режимы тревоги" + " \n"
			+ COMMAND_EDIT + " - " + "Редактировать устройства" + " \n"
			+ COMMAND_HOME + " - " + "Датчики / управление устройством" + " \n"
			+ COMMAND_HELP + " - " + "Справка по коммандам" + " \n";
	
}
