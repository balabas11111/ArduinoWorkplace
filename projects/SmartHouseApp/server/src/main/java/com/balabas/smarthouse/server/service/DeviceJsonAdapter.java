package com.balabas.smarthouse.server.service;

import java.util.List;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.model.Device;

@SuppressWarnings("rawtypes")
public interface DeviceJsonAdapter {

    List<ChangedEvent> adapt(String data, Device device);
    
}