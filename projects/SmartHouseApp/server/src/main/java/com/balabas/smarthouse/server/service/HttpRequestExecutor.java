package com.balabas.smarthouse.server.service;

import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.Map;

import org.springframework.http.HttpHeaders;
import org.springframework.http.ResponseEntity;
import org.springframework.util.MultiValueMap;

import com.balabas.smarthouse.server.model.Device;

public interface HttpRequestExecutor {

	ResponseEntity<String> executeGetRequest(String url, Map<String, String> params) throws UnsupportedEncodingException;
	
	ResponseEntity<Device> executeGetRequestDevice(String url, Map<String, String> params) throws UnsupportedEncodingException;
	
	ResponseEntity<List<Device>> executeGetRequestDeviceList(String url, Map<String, String> params) throws UnsupportedEncodingException;
	
	ResponseEntity<String> executePostRequest(String url, String body) throws UnsupportedEncodingException;

	ResponseEntity<String> executeGetRequest(String url, HttpHeaders headers, Map<String, String> params);

	ResponseEntity<String> executePostRequest(String url, HttpHeaders headers, String body);
	
	ResponseEntity<String> executePostRequest(String url, HttpHeaders headers, MultiValueMap<String, Object> map);
}
