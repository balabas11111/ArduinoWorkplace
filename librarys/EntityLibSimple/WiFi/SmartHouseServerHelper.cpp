/*
 * SmartHouseServerHelper.cpp
 *
 *  Created on: 21 ����. 2019 �.
 *      Author: Vitaliy
 */

#include <WiFi/SmartHouseServerHelper.h>

SmartHouseServerHelper::SmartHouseServerHelper(EntityManager* manager) {
	this->manager = manager;
}

void SmartHouseServerHelper::loop() {
	this->executeOnServerRegisterIfTriggered();
	this->executeOnServerDataChangedIfTriggered();
}

void SmartHouseServerHelper::triggerOnServerRegister() {
	this->triggeredOnServerRegister = true;
}

void SmartHouseServerHelper::triggerOnServerDeviceChanged(bool addData) {
	this->triggeredOnServerDeviceChanged = true;
	this->addDataToChangedRequest = addData;
}

bool SmartHouseServerHelper::connectedAndRegistered(){
	return WiFi.status() == WL_CONNECTED
			&& this->registered;
}

void SmartHouseServerHelper::executeOnServerRegisterIfTriggered() {
	if(WiFi.status() == WL_CONNECTED){
		if(!this->registered){

			if(this->triggeredOnServerRegister){

			if(timeoutExecuteRegister && nextRegisterExecution + MAX_CONNECTION_ERRORS_REACHED_TIMEOUT > millis()){
				return;
			}

				if(!serverRegisterRuns){
					this->serverRegisterRuns = true;
					unsigned long start = millis();
					Serial.println(FPSTR("-------------------------"));
					Serial.println(FPSTR("Server register triggered"));
					EntityJsonRequestResponse* req = new EntityJsonRequestResponse();

					createDeviceRegistrationRequest(req->getRequest());

					int status = HttpUtils::executePostRequest(req, JSONKEY_url);

					if(status == 200){
						this->registered = true;
						this->registerErrors = 0;
						this->timeoutExecuteRegister = false;
						this->triggeredOnServerRegister = false;
						Serial.println(FPSTR("Device was REGISTERED on server"));
					}else{
						Serial.println(FPSTR("ERROR REGISTER on server"));
						this->registerErrors++;
					}

					delete req;

					ObjectUtils::printTimeHeap(start);
					Serial.println(FPSTR("-------------------------"));

					this->serverRegisterRuns = false;
				}
			}

			if(registerErrors>=MAX_REGISTER_ERRORS){
				this->registerErrors = 0;
				this->timeoutExecuteRegister = true;
				this->nextRegisterExecution = millis();

				Serial.print(FPSTR("Registration will be timedOut at "));
				Serial.print(nextRegisterExecution+ MAX_CONNECTION_ERRORS_REACHED_TIMEOUT);
				Serial.println(FPSTR(")"));
			}
		}
	}
}

void SmartHouseServerHelper::executeOnServerDataChangedIfTriggered() {
	if(connectedAndRegistered()){

		if(!serverDataChangedRuns){
			if(this->triggeredOnServerDeviceChanged){
				this->serverDataChangedRuns = true;
				unsigned long start = millis();
				Serial.println(FPSTR("-------------------------"));
				Serial.println(FPSTR("Trigerring DataChanged"));
				EntityJsonRequestResponse* req = new EntityJsonRequestResponse();

				if(this->addDataToChangedRequest){
					JsonObject& data = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(req->getResponse(), DATA);
					manager->groupNameToParam(manager->getSensorsGroup(), nullptr, req);
					manager->get(req);
				}

				createDeviceDataChangedRequest(req->getRequest());

				JsonObjectUtil::printWithPreffix("DataChanged Request =", req->getRequest());

				int status = HttpUtils::executePostRequest(req, JSONKEY_url);

				delete req;
				ObjectUtils::printTimeHeap(start);
				if(status == 200){
					Serial.println(FPSTR("DataChange was dispatched"));
					this->connectionErrors = 0;
				}else if(status==403){
					this->registered = false;
					this->triggerOnServerRegister();
				} else{
					Serial.println(FPSTR("DataChange dispatch error"));
					this->connectionErrors++;
				}
				Serial.println(FPSTR("-------------------------"));

				this->triggeredOnServerDeviceChanged = false;
				this->serverDataChangedRuns = false;
			}
		}

		if(this->connectionErrors>=MAX_CONNECTION_ERRORS){
			this->connectionErrors = 0;
			this->registered = false;
		}
	}
}

void SmartHouseServerHelper::createDeviceRegistrationRequest(
		JsonObject& request) {

	JsonObjectUtil::setField(request, _DEVICE_ID, this->manager->getConf()->deviceId());
	JsonObjectUtil::setField(request, _DEVICE_FIRMWARE, this->manager->getConf()->deviceFirmWare());
	JsonObjectUtil::setField(request, _DEVICE_DESCR, this->manager->getConf()->deviceDescr());
	JsonObjectUtil::setField(request, KEY_SERVER_KEY, getServerKeyHashed());
	JsonObjectUtil::setField(request, KEY_DEVICE_KEY, getDeviceKeyHashed());

	JsonObjectUtil::setField(request, KEY_ROOT_URL, getDeviceRootUrl());
	JsonObjectUtil::setField(request, KEY_DATA_URL, getDeviceDataUrl());

	JsonObjectUtil::setField(request, JSONKEY_url, getServerRegisterUrl());
}

void SmartHouseServerHelper::createDeviceDataChangedRequest(
		JsonObject& request) {
	JsonObjectUtil::setField(request, _DEVICE_ID, this->manager->getConf()->deviceId());
	JsonObjectUtil::setField(request, KEY_DEVICE_KEY, getDeviceKeyHashed());

	JsonObjectUtil::setField(request, JSONKEY_url, getServerDataChangedUrl());
}

char* SmartHouseServerHelper::getDeviceBaseUrl() {
	if(this->deviceBaseUrl == nullptr){
		String deviceBaseUrlStr=HTTP_PREFFIX;
		deviceBaseUrlStr += this->manager->getConf()->getCurrentIp();
		this->deviceBaseUrl = strdup(deviceBaseUrlStr.c_str());
	}
	return this->deviceBaseUrl;
}

char* SmartHouseServerHelper::getDeviceRootUrl() {
	if(this->deviceRootUrl == nullptr){
		this->deviceRootUrl = strdup((String(getDeviceBaseUrl()) + URL_ROOT).c_str());
	}
	return this->deviceRootUrl;
}

char* SmartHouseServerHelper::getDeviceDataUrl() {
	if(this->deviceDataUrl == nullptr){
			this->deviceDataUrl = strdup((String(getDeviceBaseUrl()) + URL_DATA).c_str());
		}
		return this->deviceDataUrl;
}

bool SmartHouseServerHelper::validateServersKeyHash(char* serverKey) {
	return strcmp(serverKey, getServerKeyHashed())==0;
}

char* SmartHouseServerHelper::getServerKeyHashed() {
	if(this->serverToken == nullptr){
		String serverTokenHash = sha1(this->manager->getConf()->smartServerKey());
		this->serverToken = strdup(serverTokenHash.c_str());
	}
	return this->serverToken;
}

char* SmartHouseServerHelper::getDeviceKeyHashed() {
	if(this->deviceToken == nullptr){
		generateDeviceToken();
	}
	return this->deviceToken;
}

void SmartHouseServerHelper::generateDeviceToken() {
	Serial.print(FPSTR("Generate deviceToken ="));
	String deviceToken = "";
	deviceToken += ESP.getChipId();
	deviceToken += ESP.getCoreVersion();
	deviceToken += this->manager->getConf()->deviceId();
	deviceToken += this->manager->getConf()->deviceFirmWare();
	deviceToken += random(1, 10000);

	Serial.print(deviceToken);

	String deviceTokenHash = sha1(deviceToken);

	this->deviceToken = strdup(deviceTokenHash.c_str());

	Serial.print(FPSTR(" hash ="));
	Serial.println(this->deviceToken);
}

bool SmartHouseServerHelper::validateDeviceKeyHash(String& deviceToken) {
	return deviceToken == getDeviceKeyHashed();
}

char* SmartHouseServerHelper::getServerRegisterUrl() {
	if(this->serverRegisterUrl == nullptr){
		String baseUrl = this->manager->getConf()->smartServerAddr();

		if(!baseUrl.startsWith(HTTP_PREFFIX)){
			baseUrl = HTTP_PREFFIX + baseUrl;
		}

		baseUrl += SMART_HOUSE_SERVER_URL_BASE;

		baseUrl += SMART_HOUSE_SERVER_URL_REGISTER;

		this->serverRegisterUrl = strdup(baseUrl.c_str());
	}

	return this->serverRegisterUrl;
}

char* SmartHouseServerHelper::getServerDataChangedUrl() {
	if(this->serverDataChangedUrl == nullptr){
		String baseUrl = this->manager->getConf()->smartServerAddr();

		if(!baseUrl.startsWith(HTTP_PREFFIX)){
			baseUrl = HTTP_PREFFIX + baseUrl;
		}

		baseUrl += SMART_HOUSE_SERVER_URL_BASE;

		baseUrl += SMART_HOUSE_SERVER_URL_SENDDATA;

		this->serverDataChangedUrl = strdup(baseUrl.c_str());
	}

	return this->serverDataChangedUrl;
}
