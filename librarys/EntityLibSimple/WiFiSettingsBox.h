/*
 * ServerSettingsBox.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_WSERVERSETTINGSBOX_H_
#define LIBRARIES_ENTITYLIB_WSERVERSETTINGSBOX_H_
//device info
#define _DEVICE_ID "deviceId"
#define _DEVICE_FIRMWARE "deviceFirmware"
#define _DEVICE_DESCR "deviceDescr"
//network settings
#define _SSID "ssid"
#define _PASS "pass"

#define _SSID_AP "ssidAP"
#define _PASS_AP "passAP"

#define _IS_AP "isAp"
#define _IS_STAT_IP "isStatIp"
#define _DISCONNECT_ON_START_IF_CONN "disStIfCon"

#define _IP "ip"
#define _IP_AP "ipAP"
#define _IP_GATEWAY "gateway"
#define _IP_SUBNET "subnet"
#define _IP_DNS "dns"
#define _IP_DNS2 "dns2"
#define _SMART_HOUSE_SERVER_IP "smrtServIp"
//access settings
#define _USER_LOGIN "userLogin"
#define _USER_PASS "userPass"
#define _ADMIN_LOGIN "adminLogin"
#define _ADMIN_PASS "adminPass"
#define _REFRESH_INTERVAL "interval"
// AP default settings
#define _AP_CHANNEL  "apCH"
#define _AP_HIDDEN   "apH"
#define _AP_MAXCONN  "apMC"

#define _DEFAULT_AP_PASSWORD ""
#define _DEFAULT_AP_CHANNEL  1
#define _DEFAULT_AP_HIDDEN   0
#define _DEFAULT_AP_MAXCONN  4

#define _MASKED_VALUE "******"

#define _WIFI_SETTINGS_DEF_NAME "WiFiSettings"
#define _WIFI_SETTINGS_DEF_DESCR "Wifi Server Device settings"

#include <Entity.h>
#include <IPAddress.h>
#include <sstream>
#include "ObjectUtils.h"

class WiFiSettingsBox: public Entity {
public:
	WiFiSettingsBox(const char* firmware) :
			Entity(GROUP_SETTINGS, _WIFI_SETTINGS_DEF_NAME,
					(char*)_WIFI_SETTINGS_DEF_DESCR) {
		this->_firmware = firmware;
		this->_devId = "ESP_Dev_ID";

		this->_ssidAp = strdup(deviceId());
	}
	virtual ~WiFiSettingsBox() {
	}

	virtual void init() override {
		//String devIdTmp="ESP_"+String(ESP.getChipId());
		//this->devId=strdup(devIdTmp.c_str());

	}

	void resetToAp() {
		this->_isAp = 1;
		this->_isStatIp = 1;
	}

	void printApConf() {
		Serial.print(FPSTR(" ssidAP="));
		Serial.print(ssidAP());
		Serial.print(FPSTR(" passwordAP="));
		Serial.print(passwordAP());
		Serial.print(FPSTR(" chann="));
		Serial.print(channelAP());
		Serial.print(FPSTR(" hidden="));
		Serial.print(hiddenAP());
		Serial.print(FPSTR(" maxCon="));
		Serial.print(maxConnAP());
		Serial.print(FPSTR(" ipAp="));
		Serial.print(IPaccessPoint());
		Serial.print(FPSTR(" ipStatic="));
		Serial.print(isStaticIp());
		Serial.print(FPSTR(" gateway="));
		Serial.print(gateway());
		Serial.print(FPSTR(" subnet="));
		Serial.print(subnet());
		Serial.print(FPSTR(" dns="));
		Serial.print(dns());
		Serial.print(FPSTR(" dns2="));
		Serial.print(dns2());
		Serial.print(FPSTR(" smrtServIp="));
		Serial.print(smartHouseServerIp());
		Serial.println();
	}

	void printStaConf() {
		Serial.print(FPSTR(" ssid="));
		Serial.print(ssid());
		Serial.print(FPSTR(" pass="));
		Serial.print(password());
		Serial.print(FPSTR(" ip="));
		Serial.print(IPstation());
		Serial.print(FPSTR(" ipStatic="));
		Serial.print(isStaticIp());
		Serial.print(FPSTR(" gateway="));
		Serial.print(gateway());
		Serial.print(FPSTR(" subnet="));
		Serial.print(subnet());
		Serial.print(FPSTR(" dns="));
		Serial.print(dns());
		Serial.print(FPSTR(" dns2="));
		Serial.print(dns2());
		Serial.println();
	}

	const char* ssid() {
		return this->_ssid;
	}
	const char* password() {
		return this->_pass;
	}

	const char* ssidAP() {
		return this->_ssidAp;
	}
	const char* passwordAP() {
		return this->_passAp;
	}
	int channelAP() {
		return this->_apCH;
	}
	int hiddenAP() {
		return this->_apH;
	}
	int maxConnAP() {
		return this->_apMC;
	}

	int refreshInterval() {
		return this->_interval;
	}

	bool isAccessPoint() {
		return this->_isAp;
	}
	bool isStaticIp() {
		return this->_isStatIp;
	}
	bool isDisconnectOnStartIfConn() {
		return this->_disStIfCon;
	}

	IPAddress IPstation() {
		return this->_ip;
	}
	IPAddress IPaccessPoint() {
		return this->_ipAP;
	}
	IPAddress gateway() {
		return this->_gateway;
	}
	IPAddress subnet() {
		return this->_subnet;
	}
	IPAddress dns() {
		return this->_dns;
	}
	IPAddress dns2() {
		return this->_dns2;
	}
	IPAddress smartHouseServerIp() {
		return this->_smrtServIp;
	}

	const char* userLogin() {
		return this->_userLogin;
	}
	const char* userPassword() {
		return this->_userPassword;
	}

	const char* adminLogin() {
		return this->_adminLogin;
	}
	const char* adminPassword() {
		return this->_adminPassword;
	}

	const char* deviceId() {
		return this->_devId;
	}
	const char* deviceFirmWare() {
		return this->_firmware;
	}
	char* deviceDescr() {
		return this->_deviceDescr;
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		itemsToJson(response, true);
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);
		setChanged(jsonToItems(params));
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
		jsonToItems(jsonFromFile);
	}

	virtual void doSave(JsonObject& jsonToFile) override {
		itemsToJson(jsonToFile, false);
	}

protected:
	const char* _devId;
	const char* _firmware;
	char* _deviceDescr = (char*)"Default Device description";

	char* _ssid = (char*)"balabasKiev5";
	char* _pass = (char*)"wuWylKegayg2wu22";
	char* _ssidAp = (char*)"";
	char* _passAp = (char*)"";
	bool _isAp = 0;bool _isStatIp = 0;bool _disStIfCon = 0;

	uint8_t _apCH = 1;
	uint8_t _apH = 0;
	uint8_t _apMC = 4;

	IPAddress _ip = new IPAddress(192,168,0,120);
	IPAddress _ipAP = new IPAddress(192,168,0,4);
	IPAddress _gateway = new IPAddress(192,168,0,1);
	IPAddress _subnet = new IPAddress(255,255,255,0);
	IPAddress _dns = new IPAddress(192,168,0,1);
	IPAddress _dns2 = new IPAddress(192,168,0,1);
	IPAddress _smrtServIp = new IPAddress(192,168,0,5);

	char* _userLogin = (char*)"";
	char* _userPassword = (char*)"";
	char* _adminLogin = (char*)"admin";
	char* _adminPassword = (char*)"admin";
	uint16_t _interval = 60;

	bool jsonToItems(JsonObject& json){
		bool chg = false;

		chg = getKeyValueIfExistsAndNotEquals(json, _DEVICE_DESCR, &this->_deviceDescr)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _SSID, &this->_ssid)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _PASS, &this->_pass)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _SSID_AP, &this->_ssidAp)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _PASS_AP, &this->_passAp)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _IS_AP, &this->_isAp)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IS_STAT_IP, &this->_isStatIp)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _DISCONNECT_ON_START_IF_CONN, &this->_disStIfCon)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _IP, &this->_ip)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IP_AP, &this->_ipAP)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IP_GATEWAY, &this->_gateway)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IP_SUBNET, &this->_subnet)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IP_DNS, &this->_dns)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IP_DNS2, &this->_dns2)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _SMART_HOUSE_SERVER_IP, &this->_smrtServIp)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _USER_LOGIN, &this->_userLogin)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _USER_PASS, &this->_userPassword)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _ADMIN_LOGIN, &this->_adminLogin)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _ADMIN_PASS, &this->_adminPassword)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _REFRESH_INTERVAL, &this->_interval)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _AP_CHANNEL, &this->_apCH)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _AP_HIDDEN, &this->_apH)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _AP_MAXCONN, &this->_apMC)?true:chg;

		return chg;
	}

	void itemsToJson(JsonObject& json, bool mask) {
		setJsonField(json, _DEVICE_ID, this->_devId);
		setJsonField(json, _DEVICE_FIRMWARE, this->_firmware);
		setJsonField(json, _DEVICE_DESCR, this->_deviceDescr);

		setJsonField(json, _SSID, this->_ssid);
		setJsonField(json, _PASS, mask?_MASKED_VALUE:_pass);

		setJsonField(json, _SSID_AP, this->_ssidAp);
		setJsonField(json, _PASS_AP, mask?_MASKED_VALUE:_passAp);

		setJsonField(json, _IS_AP, this->_isAp);
		setJsonField(json, _IS_STAT_IP, this->_isStatIp);
		setJsonField(json, _DISCONNECT_ON_START_IF_CONN, this->_disStIfCon);

		setJsonField(json, _IP, (this->_ip).toString().c_str());
		setJsonField(json, _IP_AP, (this->_ipAP).toString().c_str());
		setJsonField(json, _IP_GATEWAY, (this->_gateway).toString().c_str());
		setJsonField(json, _IP_SUBNET, (this->_subnet).toString().c_str());
		setJsonField(json, _IP_DNS, (this->_dns).toString().c_str());
		setJsonField(json, _IP_DNS2, (this->_dns2).toString().c_str());
		setJsonField(json, _SMART_HOUSE_SERVER_IP, (this->_smrtServIp).toString().c_str());

		setJsonField(json, _USER_LOGIN, this->_userLogin);
		setJsonField(json, _USER_PASS, mask?_MASKED_VALUE:_userPassword);
		setJsonField(json, _ADMIN_LOGIN, this->_adminLogin);
		setJsonField(json, _ADMIN_PASS, mask?_MASKED_VALUE:_adminPassword);
		setJsonField(json, _REFRESH_INTERVAL, this->_interval);
		setJsonField(json, _AP_CHANNEL, this->_apCH);
		setJsonField(json, _AP_HIDDEN, this->_apH);
		setJsonField(json, _AP_MAXCONN, this->_apMC);
	}

};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_ */
