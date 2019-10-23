package com.balabas.smarthouse.server.security;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Repository;

@Repository
public class SecurityContextRepositoryImpl implements SecurityContextRepository {

	private Map<String, DeviceSecurityContext> cont = Collections.synchronizedMap(new HashMap<>());
	
	@Override
	public DeviceSecurityContext put(String deviceId, String serverHash, String deviceHash, String serverToken) {
		DeviceSecurityContext result = get(deviceId);
		
		if(result==null){
			result = new DeviceSecurityContext(serverHash, deviceHash, serverToken);
			cont.put(deviceId, result);
		}else{
			result.setDeviceKeyHash(deviceHash);
			result.setServerKeyHash(serverHash);
		}
		
		return result;
	}

	@Override
	public DeviceSecurityContext get(String deviceId) {
		return cont.getOrDefault(deviceId, null);
	}

}
