package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import javax.persistence.CascadeType;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToMany;
import javax.persistence.MappedSuperclass;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

import lombok.Getter;
import lombok.Setter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@MappedSuperclass
public abstract class AlarmV2 implements IAlarmV2 {

	public static final Integer NO_MESSAGE_SEND_REPEATS = -1;
	public static final String ALARM_FINISHED_STATUS = "показатели в норме";
	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Getter @Setter
	private Long id;
	/*
	@Getter @Setter
	private Long itemId;
	*/
	@Getter @Setter
	private String checkerName;
	
	@Getter @Setter
	private String description;
	
	@Getter @Setter
	private String parameter;
	
	@Getter @Setter
	private Integer messageInterval = NO_MESSAGE_SEND_REPEATS;
	
	@Setter
	@ManyToMany(targetEntity = AlarmStateChangeAction.class, fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	private Set<IAlarmStateChangeAction> actions;
	
	@Transient
	@Getter @Setter
	private AlarmV2Checker checker;
	
	@Transient
	private AlarmState alarmState;
	
	@Setter
	@Transient
	private AlarmState previousAlarmState;
	
	@Override
	public void setAlarmState(AlarmState newState) {
		this.previousAlarmState = alarmState;
		this.alarmState = newState;
	}

	@Override
	public Set<IAlarmStateChangeAction> getActions() {
		return this.actions;
	}
	
	@Override
	public List<IAlarmStateChangeAction>	getActionsList() {
		Set<IAlarmStateChangeAction> result = getActions();
		List<IAlarmStateChangeAction> list = result.stream().collect(Collectors.toList());
		return list;
	}
	
	@Override
	public boolean check(IItemAbstract item) {
		setItem(item);
		if(checker!=null) {
			return checker.check(this);
		} else {
			log.error("Null pointer as checker");
		}
		return false;
	}
	
	@Override
	public AlarmState getAlarmState() {
		return Optional.ofNullable(this.alarmState).orElse(AlarmState.NO_DATA);
	}
	
	@Override
	public 	AlarmState getPreviousAlarmState() {
		return Optional.ofNullable(this.previousAlarmState).orElse(AlarmState.NO_DATA);
	}
	
	@Override
	public boolean isInBadState() {
		return this.getAlarmState().badState;
	}
	
	@Override
	public boolean isRepeatable() {
		return !NO_MESSAGE_SEND_REPEATS.equals(messageInterval) && messageInterval > 0;
	}
	
	@Override
	public List<IAlarmStateChangeAction> getCurrentActions() {
		
		List<IAlarmStateChangeAction> result = getActions().stream().filter(
				action -> action.accepts(getPreviousAlarmState(), getAlarmState())
				).collect(Collectors.toList());
		
		return result;
	}
	
	@Override
	public boolean setAlarmStateByState(AlarmState alarmState) {
		AlarmState state1 = getAlarmState();
		
		this.setPreviousAlarmState(getAlarmState());
		this.setAlarmState(Optional.ofNullable(alarmState).orElse(AlarmState.NO_DATA));
		
		return state1.equals(getAlarmState());
	}
	
	@Override
	public boolean setAlarmStateByBooleanFlag(boolean alarmed) {
		if(alarmed) {
			return setAlarmStateByState(AlarmState.ALARM);
		} else {
			return setAlarmStateByState(AlarmState.OK);
		}
	}
	
	@Override
	public Long getItemId() {
		IItemAbstract ia = getItem(); 
		return ia != null?ia.getId():null;
	}
/*
	@Override
	public void setItemId(Long itemId) {
		// TODO Auto-generated method stub
		
	}
	*/
	
}
