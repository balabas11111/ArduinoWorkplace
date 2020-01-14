package com.balabas.smarthouse.server.service;

import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_GROUP;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_ENTITY_NAME;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SWG;

import java.util.Collections;
import java.util.Map;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.util.LinkedMultiValueMap;
import org.springframework.util.MultiValueMap;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.service.DeviceSecurityService;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceRequestorServiceImpl implements DeviceRequestorService {

	@Autowired
	HttpRequestExecutor executor;

	@Autowired
	DeviceSecurityService securityService;

	@Override
	public String executeGetDataOnDevice(IDevice device, String groupName) {
		if (device != null) {
			if (device.getDataUrl() != null) {
				Map<String, String> params = (groupName != null && !groupName.isEmpty())
						? Collections.singletonMap(DEVICE_FIELD_GROUP, groupName)
						: Collections.emptyMap();

				HttpHeaders headers = new HttpHeaders();
				headers.set(HttpHeaders.AUTHORIZATION, securityService.getServerKey());
				headers.set(DeviceConstants.HEADER_SREQUEST, DeviceConstants.HEADER_SREQUEST);

				String url = getDeviceDataUrl(device);

				ResponseEntity<String> result = executor.executeGetRequest(url, headers, params);

				if (result.getStatusCode().equals(HttpStatus.OK)) {
					return result.getBody();
				}

			} else {
				log.error("Empty data url deviceId=" + device.getName());
			}
		}
		
		log.error("Empty device");

		return null;
	}

	@Override
	public String executePostDataOnDevice(IDevice device, JSONObject json) {
		HttpHeaders headers = new HttpHeaders();
		headers.setContentType(MediaType.APPLICATION_JSON);
		headers.add(HttpHeaders.AUTHORIZATION, securityService.getServerKey());
		headers.set(DeviceConstants.HEADER_SREQUEST, DeviceConstants.HEADER_SREQUEST);
		headers.add(ENTITY_FIELD_SWG, "1");

		return executor.executePostRequest(device.getDataUrl(), headers, json.toString()).getBody();
	}

	@Override
	public String executePostDataOnDeviceEntity(IDevice device, IEntity entity, Map<String, Object> values) {
		HttpHeaders headers = new HttpHeaders();
		headers.setContentType(MediaType.APPLICATION_FORM_URLENCODED);
		headers.add(HttpHeaders.AUTHORIZATION, securityService.getServerKey());
		headers.set(DeviceConstants.HEADER_SREQUEST, DeviceConstants.HEADER_SREQUEST);

		MultiValueMap<String, Object> map = new LinkedMultiValueMap<String, Object>();
		map.add(DEVICE_FIELD_GROUP, entity.getGroup().getName());
		map.add(DEVICE_FIELD_ENTITY_NAME, entity.getName());
		map.add(ENTITY_FIELD_SWG, 1);

		values.entrySet().stream().forEach(e -> map.add(e.getKey(), e.getValue()));

		return executor.executePostRequest(device.getDataUrl(), headers, map).getBody();
	}
	
	private String getDeviceDataUrl(IDevice device) {
		return //(device.isInitialized()) ? device.getDataUrl(): 
				device.getDataUrl() + DeviceConstants.ENTITY_FIELD_SERVER_REQUEST_EQ_1;
	}
}