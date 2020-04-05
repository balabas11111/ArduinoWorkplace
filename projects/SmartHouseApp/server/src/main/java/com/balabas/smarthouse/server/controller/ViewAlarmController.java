package com.balabas.smarthouse.server.controller;

import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_SERVER_NAME;

import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmOfDevice;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmOfEntity;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmOfEntityField;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeAction;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmStateChangeAction;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2Service;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.util.DateTimeUtil;

@Controller
public class ViewAlarmController {

	@Value("${smarthouse.server.view.page.device.refresh.interval.sec:60}")
	private Long deviceViewRefreshInterval;
	
	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IAlarmV2Service alarmService;
	
	@GetMapping("/alarmsList")
	public String getAllAlarms(@RequestParam(name = "itemType", required = false) String itemType, Model model) {
		if(StringUtils.isEmpty(itemType)) {
			itemType = ItemType.DEVICE.name();
		}
		Map<String, List<IAlarmV2>> alarms = alarmService.getAlarmsGrouppedByItemClassNameWithAlarmDescriptions();
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute("itemType", itemType);
		model.addAttribute("alarms", alarms);
		if (deviceViewRefreshInterval != null && deviceViewRefreshInterval > 0) {
			model.addAttribute(ControllerConstants.ATTR_PAGE_REFRESH_INTERVAL, deviceViewRefreshInterval);
		}
		model.addAttribute("dateTime", DateTimeUtil.getDateTimeStr());

		return "alarms/alarmsList.html";
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	@GetMapping(value = "/editAlarm")
	public String editAlarm(@RequestParam(name = "id", required = true) Long id,
			@RequestParam(name = "itemType", required = false) String itemType, Model model) {

		if (id == null || (id == 0 && StringUtils.isEmpty(itemType))) {
			throw new IllegalArgumentException("New alarm should contain ItemType");
		}

		ItemType it = ItemType.getItemTypeByName(itemType);
		IAlarmV2 alarm = id > 0 ? alarmService.getAlarm(id, it) : alarmService.newAlarm(it);
		List<AlarmV2Checker> checkers = alarmService.getCheckersByTargetItemClass(alarm.getTargetItemClass());

		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute("alarm", alarm);
		model.addAttribute("itemType", itemType);
		model.addAttribute("currentCheckerName", alarm.getCheckerName());
		model.addAttribute("checkers", checkers);
		model.addAttribute("targets", alarmService.getEnabledAlarmAbstractTargets(alarm));
		if (!alarm.isNew()) {
			model.addAttribute("alarmEvent", new AlarmStateChangeAction());
			model.addAttribute("alarmStates", AlarmState.getList());
			model.addAttribute("alarmStateChangedEventProcessors",
					alarmService.getAlarmStateChangedEventProcessors(alarm));
			
			Set<IAlarmStateChangeAction> actions = alarm.getActions();
			
			alarm.setActions(actions==null?new HashSet():new HashSet(actions));
		}else {
			alarm.setActions(new HashSet());
		}
		
		return "alarms/editAlarm.html";
	}

	@PostMapping("/saveAlarmEvent")
	public String saveAlarmEvent(@ModelAttribute("alarmEvent") AlarmStateChangeAction action,
			@RequestParam("alarmId") Long alarmId, @RequestParam("oldStateName") String oldStateName,
			@RequestParam("newStateName") String newStateName,
			@RequestParam("itemType") String itemType, Model model) {

		AlarmState oldState = AlarmState.getByName(oldStateName);
		AlarmState newState = AlarmState.getByName(newStateName);

		action.setOldState(oldState);
		action.setNewState(newState);

		ItemType it = ItemType.getItemTypeByName(itemType);
		
		alarmService.addAlarmStateChangeActionToAlarm(alarmId, it, action);

		return "redirect:/editAlarm?id=" + alarmId+ "&itemType=" + itemType;
	}
	
	@GetMapping("/deleteAction")
	public String deleteAlarmEvent(@ModelAttribute("alarmEvent") AlarmStateChangeAction action,
			@RequestParam("id") Long actionId,
			@RequestParam("alarmId") Long alarmId,
			@RequestParam("itemType") String itemType, Model model) {
		
		ItemType it = ItemType.getItemTypeByName(itemType);
		
		alarmService.deleteAlarmStateChangeActionFromAlarm(alarmId, it, actionId);
		
		return "redirect:/editAlarm?id=" + alarmId+ "&itemType=" + itemType;
	}

	@PostMapping(value = "/saveDeviceAlarm")
	public String saveDeviceAlarm(@ModelAttribute("alarm") AlarmOfDevice alarm,
			@RequestParam(value = "itemId", required = true) Long itemId, Model model) {
		return saveAlarm(alarm, itemId);
	}

	@PostMapping(value = "/saveEntityAlarm")
	public String saveEntityAlarm(@ModelAttribute("alarm") AlarmOfEntity alarm,
			@RequestParam(value = "itemId", required = true) Long itemId, Model model) {
		return saveAlarm(alarm, itemId);
	}

	@PostMapping(value = "/saveEntityFieldAlarm")
	public String saveEntityFieldAlarm(@ModelAttribute("alarm") AlarmOfEntityField alarm,
			@RequestParam(value = "itemId", required = true) Long itemId, Model model) {
		return saveAlarm(alarm, itemId);
	}

	@GetMapping("/deleteAlarm_{id}")
	public String deleteEntityFieldEnabledValue(@PathVariable(name = "id", required = true) Long id, Model model) {
		alarmService.deleteAlarm(id);

		return "redirect:/alarmsList";
	}

	private String saveAlarm(IAlarmV2 alarm, Long itemId) {
		alarmService.createOrUpdateAlarm(alarm, itemId);
		return "redirect:/alarmsList";
	}

}
