package com.balabas.smarthouse.server.service.events;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import lombok.extern.log4j.Log4j2;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.events.DeviceChangedEvent;
import com.balabas.smarthouse.server.events.EntityChangedEvent;
import com.balabas.smarthouse.server.events.GroupChangedEvent;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Entity;
import com.balabas.smarthouse.server.model.Group;

@Log4j2
@Service
public class EventProcessorsServiceImpl implements EventProcessorsService {

	@Autowired
	List<EventProcessor<Device, DeviceChangedEvent>> deviceEventsProcessors;
	
	@Autowired(required = false)
	List<EventProcessor<Group, GroupChangedEvent>> groupEventsProcessors;
	
	@Autowired(required = false)
	List<EventProcessor<Entity, EntityChangedEvent>> entityEventsProcessors;
	
    @Override
    public void processEvents(List<ChangedEvent<?>> events) {
        if(!events.isEmpty()){
            log.debug("Total "+events.size()+" events received");
            
            for(ChangedEvent<?> event: events){
                log.debug("Target "+
                        event.getClass().getSimpleName()+" "+
                        event.getTargetClass().getSimpleName()+"("+event.getTargetName()+") eventType="+
                            event.getEventType());
                processEvent(event);
            }
        }
    }
    
    public void processEvent(ChangedEvent<?> event){
        if(event.getClass().equals(DeviceChangedEvent.class)){
            processEvent((DeviceChangedEvent) event);
        }else if(event.getClass().equals(GroupChangedEvent.class)){
            processEvent((GroupChangedEvent) event);
        }else if(event.getClass().equals(EntityChangedEvent.class)){
            processEvent((EntityChangedEvent) event);
        }
    }
    
    public void processEvent(DeviceChangedEvent event){
    	if(groupEventsProcessors!=null && !deviceEventsProcessors.isEmpty()){
    		deviceEventsProcessors.stream()
    			.forEach(proc-> proc.processEvent(event));
    	}else{
    		log.warn("DeviceChangedEvent : " + event.toString());
    	}
    }
    
    public void processEvent(GroupChangedEvent event){
    	if(groupEventsProcessors!=null && !groupEventsProcessors.isEmpty()){
    		groupEventsProcessors.stream()
				.forEach(proc-> proc.processEvent(event));
    	}else{
    		log.warn("GroupChangedEvent : " + event.printDetails());
    	}
    }
    
    public void processEvent(EntityChangedEvent event){
    	if(entityEventsProcessors!=null && !entityEventsProcessors.isEmpty()){
    		entityEventsProcessors.stream()
				.forEach(proc-> proc.processEvent(event));
    	}else{
    		log.warn("EntityChangedEvent "+event.toString());
    	}
        
    }

}
