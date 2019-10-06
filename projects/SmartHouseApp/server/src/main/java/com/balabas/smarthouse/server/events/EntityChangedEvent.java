package com.balabas.smarthouse.server.events;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import lombok.Getter;
import lombok.ToString;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Entity;

@ToString(callSuper = true)
public class EntityChangedEvent extends ChangedEvent<Entity> {

    @Getter
    private List<ValuesChangeEvent> events =new ArrayList<>();
    
    @Getter
    private Device device;
    
    public EntityChangedEvent(Device device, Entity target, DeviceEventType eventType) {
    	super(target, eventType);
    	this.device = device;
    }
    
    public EntityChangedEvent(Entity target, DeviceEventType eventType, List<ValuesChangeEvent> events) {
        super(target, eventType);
        this.events = events;
    }
    
    public void addEvent(ValuesChangeEvent event){
        if(event!=null && event.hasValues()){
            events.add(event);
        }
    }
    
    public static EntityChangedEvent build(Entity target, DeviceEventType eventType, List<ValuesChangeEvent> events){
        List<ValuesChangeEvent>  nonNulls = events.stream().filter(ev->(ev!=null && ev.hasValues())).collect(Collectors.toList());
        
        if(nonNulls.isEmpty() && eventType.equals(DeviceEventType.UPDATED)){
            return null;
        }
        
        return new EntityChangedEvent(target, eventType, nonNulls);
    }

}
