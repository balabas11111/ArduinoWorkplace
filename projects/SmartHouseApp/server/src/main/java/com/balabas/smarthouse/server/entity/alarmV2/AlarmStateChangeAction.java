package com.balabas.smarthouse.server.entity.alarmV2;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import javax.persistence.Column;
import javax.persistence.Entity;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;

import lombok.Getter;
import lombok.Setter;

@Entity
public class AlarmStateChangeAction extends ItemAbstract implements IAlarmStateChangeAction {

	@Setter
	private AlarmState oldState;
	
	@Setter
	private AlarmState newState;
	
	@Getter @Setter
	@Column(name = "interval", nullable = true)
	private Long interval = -1l;
	
	@Override
	public AlarmState getOldState() {
		return Optional.ofNullable(oldState).orElse(AlarmState.NO_DATA);
	}
	
	@Override
	public AlarmState getNewState() {
		return Optional.ofNullable(newState).orElse(AlarmState.NO_DATA);
	}
	
	@Getter @Setter
	private String stringFormatted;
	
	@Getter @Setter
	private String stringParametersFormatted;
	
	@Getter
	@Setter
	boolean dispatchIfSameState;
	
	@Getter @Setter
	private String targetBeanName;
	
	@Getter 
	final private String hint = "getDescriptionByDescriptionField;getDescription;getName;getValueStr;getValueTmp";
	
	@Override
	public String getAlarmDescription(IItemAbstract item) {
		return executeAction(item);
	}
	
	@Override
	public String executeAction(IItemAbstract item) {
		if(!StringUtils.isEmpty(this.stringParametersFormatted)) {
			String[] methodNames = stringParametersFormatted.split(";");
			
			List<Object> params = new ArrayList<>();
			Object obj = item;
			
			for(String methodName : methodNames) {
				try {
					Method method = obj.getClass().getMethod(methodName);
					Object result = method.invoke(obj);
					params.add(result);
				} catch (Exception e) {
					e.printStackTrace();
				} 
			}
			
			if(params.size()>0) {
				return String.format(stringFormatted, params.toArray());
			} else {
				return stringFormatted;
			}
		} else {
			return stringFormatted;
		}
	}
	
	@Override
	public boolean accepts(AlarmState oldState, AlarmState newState) {
		if(this.oldState == null) {
			if(newState.equals(oldState)) {
				return false;
			}
			return this.newState.equals(newState);
		}
		if(this.newState == null) {
			if(newState.equals(oldState)) {
				return false;
			}
			return this.newState.equals(newState);
		}
		return this.oldState.equals(oldState) && this.newState.equals(newState);
	}

}
