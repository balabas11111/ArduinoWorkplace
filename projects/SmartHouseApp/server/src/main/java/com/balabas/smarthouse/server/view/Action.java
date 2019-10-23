package com.balabas.smarthouse.server.view;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import org.json.JSONObject;
import org.thymeleaf.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Getter;
import lombok.Setter;

@AllArgsConstructor
@Builder
public class Action {
	
	public static final String CALLBACK_SPLITTER = ";";
	public static final String CALLBACK_TYPE_ACTION = "Act";
	
	public static final String ACTION_TYPE_UNKNOWN                   = "NUL";
	public static final String ACTION_TYPE_VIEW_DEVICE_LIST          = "vdl";
	public static final String ACTION_TYPE_VIEW_HELP                 = "vhp";
	public static final String ACTION_TYPE_EDIT_DEVICE_SELECT_LIST   = "eds";
	public static final String ACTION_TYPE_VIEW_GROUPS_OF_DEVICE     = "vgd";
	public static final String ACTION_TYPE_VIEW_ENTITIES_OF_GROUP    = "veg";
	public static final String ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE   = "eeg";
	public static final String ACTION_TYPE_EDIT_ENTITITY             = "een";
	public static final String ACTION_TYPE_EDIT_ENTITITY_FIELD       = "eef";
	public static final String ACTION_TYPE_VIEW_ENTITITY             = "ven";
	public static final String ACTION_TYPE_SEND_DATA_TO_DEVICE       = "sdd";
	public static final String ACTION_TYPE_GET_DATA_FROM_DEVICE      = "gdd";
	
	public static final String ACTION_DATA_FIELD_NAME                = "adfn";
	public static final String ACTION_DATA_FIELD_DESCRIPTION         = "adfd";
	public static final String ACTION_DATA_FIELD_VALUE               = "adfv";
	
	public static final String ID_TYPE_DEVICE         = "_dev"; 
	public static final String ID_TYPE_GROUP          = "_grp";
	public static final String ID_TYPE_ENTITY         = "_ent";
	public static final String ID_TYPE_ENTITY_FIELD   = "_fld";
	public static final String ID_TYPE_ENABLED_VALUE  = "_val";
	
	public static final String validActions[] = {
													ACTION_TYPE_VIEW_DEVICE_LIST,
													ACTION_TYPE_EDIT_DEVICE_SELECT_LIST,
													ACTION_TYPE_VIEW_GROUPS_OF_DEVICE,
													ACTION_TYPE_VIEW_ENTITIES_OF_GROUP,
													ACTION_TYPE_VIEW_ENTITITY
												};
	@Getter
	private String action;
	@Getter @Setter
	private String description;
	@Getter
	private String data;
	@Getter
	private String deviceName;
	@Getter
	private String groupName;
	@Getter
	private String entityName;
	@Getter
	private String idType;
	@Getter @Setter
	private Long targetId;
	
	@Getter
	private String callbackData;

	@Getter
	private boolean valid;
	
	public Action() {
		this.valid = false;
	}
	
	public Action(String...cols) {
		this.action = getColValueOrNull(1, cols );
		this.data = getColValueOrNull(2, cols );
		this.deviceName = getColValueOrNull(3, cols );
		this.groupName = getColValueOrNull(4, cols );
		this.entityName = getColValueOrNull(5, cols );
		this.idType = getColValueOrNull(6, cols );
		
		String tId = getColValueOrNull(7, cols );
		if( !StringUtils.isEmpty(tId)) {
			this.targetId = Long.valueOf(tId); 
		}
			
		this.callbackData = buildCallBackData();
		this.valid = isActionValid();
	}
	
		
	public Action(String action, String data, String deviceName, String groupName, String entityName) {
		
		this.action = action;
		this.deviceName = deviceName;
		this.groupName = groupName;
		this.entityName = entityName;
		this.data = data;
		
		this.callbackData = buildCallBackData();
		
		this.valid = isActionValid();
	}
	
	public Action(String action, String data, String description, String idType, Long targetId) {
		
		this.action = action;
		this.data = data;
		this.description = description;
		this.idType = idType;
		this.targetId = targetId;
		
		this.callbackData = buildCallBackData();
		
		this.valid = isActionValid();
	}
		
	public void setData(String data) {
		this.data = clear(data);
		this.callbackData = buildCallBackData();
	}
	
	public void setDeviceName(String deviceId) {
		this.deviceName = clear(deviceId);
		this.callbackData = buildCallBackData();
	}
	
	public void setGroupName(String groupId) {
		this.groupName = clear(groupId);
		this.callbackData = buildCallBackData();
	}
	
	public void setEntityName(String entityId) {
		this.entityName = clear(entityId);
		this.callbackData = buildCallBackData();
	}
	
	public void setValueByKeyRemoveKey(String keyToSelectAndRemove, String fieldValue) {
		JSONObject dataJson = new JSONObject(getData());
		String fieldName = dataJson.getString(keyToSelectAndRemove);
		dataJson.remove(keyToSelectAndRemove);
		
		dataJson.put(fieldName, fieldValue);
		
		this.setData(dataJson.toString());
	}
	
	public static Action fromDevice(String action, IDevice device, String data) {
		return new Action(action, data, device.getName());
	}
	
	public static Action fromGroup(String action, IGroup group, String data) {
		return new Action(action, data, group.getDevice().getName(), group.getName());
	}
	
	public static Action fromEntity(String action, IEntity entity, String data) {
		return new Action(action, data, entity.getGroup().getDevice().getName(), entity.getGroup().getName(), entity.getName());
	}
	
	@SuppressWarnings("rawtypes")
	public static Action fromEntityFieldEnabledValue(String actionName, IEntityFieldEnabledValue entityFieldEnabledValue) {
		Entity entity = entityFieldEnabledValue.getEntityField().getEntity();
		
		String description = Optional.ofNullable(entityFieldEnabledValue.getEmoji()).orElse(Emoji.EMPTY_EMOJI).toString()
				+ entity.getDescription() + " : " + entityFieldEnabledValue.getActionDescription();
		
		return new Action(actionName, entityFieldEnabledValue.buildDataForCallBack(), description,
				ID_TYPE_ENTITY, entity.getId());
	}
	
	public static Action fromColumnList(String...cols) {
		List<String> list = new ArrayList<String>();
		
		if(cols==null || cols.length<1 || !cols[0].equals(CALLBACK_TYPE_ACTION)) {
			list.add(CALLBACK_TYPE_ACTION);
		}
		if(cols!=null) {
			for(String str:cols) {
				list.add(str);
			}
		}
		
		String[] strArr = list.toArray(new String[list.size()]);
		return new Action(strArr);
	}
	
	public static String callback(String actionName, String data, IEntity entity) {
		return (Action.fromColumnList(actionName, data, entity.getGroup().getDevice().getName(), entity.getGroup().getName(), entity.getName())).getCallbackData();
	}
	
	public static String callback(String...cols) {
		return (Action.fromColumnList(cols)).getCallbackData();
	}
		
	public static Action fromCallbackData(String str) {
		if(str == null || str.isEmpty()) {
			return new Action();
		}
		
		String[] col = str.split(CALLBACK_SPLITTER);
		
		if(col==null || col.length <2 || !CALLBACK_TYPE_ACTION.equals(col[0])) {
			return new Action();
		}
		
		return new Action(col);
	}
		
	@Override
	public String toString() {
		return this.callbackData;
	}
		
	private String getColValueOrNull(int i, String...cols) {
		if(cols.length>=i+1) {
			return cols[i];
		}
		return null;
	}
			
		
	private boolean isActionValid() {
		for(String va:validActions) {
			return va.equals(action);
		}
		return false;
	}
	
	private String buildCallBackData() {
		return buildCallbackData(CALLBACK_TYPE_ACTION, action, data, deviceName, groupName, entityName, idType, (targetId == null)?"":targetId.toString());
	}
	
	private static String buildCallbackData(String... arg){
		StringBuilder result = new StringBuilder("");
		
		for(int i=0; i<arg.length; i++){
			result.append(arg[i]!=null?arg[i]:"");
			result.append(CALLBACK_SPLITTER);
		}
		
		return result.toString();
	}
	
	private static String clear(String str) {
		return (str==null)?"":str;
	}

	public void setActionRebuildData(String action) {
		this.action = action;
		this.callbackData = buildCallBackData();
	}
	
	public void rebuildCallbackData() {
		this.callbackData = buildCallBackData();
	}

}