/*
 * EntityApplication.cpp
 *
 *  Created on: 30 ���. 2019 �.
 *      Author: Vitaliy
 */

#include <EntityApplication.h>

EntityApplication::EntityApplication(const char* firmWare, Entity* entities[],
		int entityCount, EntityUpdate* entityUpdate[], int entityUpdateCount,
		std::function<void(void)> onEntityChanged,
		WiFiSettingsBox* conf, std::function<void(void)> onWiFiConnected,
		std::function<void(void)> onWiFiDisConnected) {

	bool newConf = conf == nullptr;
	if (newConf) {
		Serial.println(FPSTR("New WiFISettingsBox will be created"));
	}

	this->conf = (newConf) ? new WiFiSettingsBox(firmWare) : conf;
	this->entityManager = new EntityManager(entities, entityCount, onEntityChanged);

	if (newConf) {
		this->entityManager->registerAndPreInitEntity(this->conf);
	}

	this->entityUpdateManager = new EntityUpdateManager(entityUpdate,
			entityUpdateCount);

	this->wifiManager = new WiFiManager(this->conf, onWiFiConnected,
			onWiFiDisConnected);
	this->wifiServerManager = new WiFiServerManager(this->entityManager,
			this->conf);

}

void EntityApplication::init(bool initSerial,
		bool initWiFi, bool initServer,
		bool initFs,
		bool deleteFs,
		bool initI2C, uint8_t clockPin, uint8_t dataPin) {

	if (initSerial) {
		ObjectUtils::initSerial();
	}

	if (initFs) {
		FileUtils::init();
	}
	if (deleteFs) {
		FileUtils::deleteAllFiles("/data/");
	}

	if(initI2C){
		I2C_utils::initStatic(clockPin, dataPin);
	}

	Serial.println(FPSTR("--------------------"));
	Serial.print(FPSTR("Init application "));
	Serial.println(conf->deviceFirmWare());

	ObjectUtils::printHeap();
	ObjectUtils::printMillis();

	entityManager->init();
	entityUpdateManager->init(this->conf->refreshInterval());

	if(initWiFi){
		startWiFi();
	}
	if(initServer){
		startServer();
	}

	Serial.println(FPSTR("Application Init done"));

	ObjectUtils::printHeap();
	ObjectUtils::printMillis();
	Serial.println(FPSTR("===================================="));
}

void EntityApplication::initWithWiFi(bool deleteFs, bool initI2C,
		uint8_t clockPin, uint8_t dataPin) {
	init(true, true, true, true,
				deleteFs, initI2C, clockPin, dataPin);
}

void EntityApplication::initWithoutWiFi(bool deleteFs, bool initI2C,
		uint8_t clockPin, uint8_t dataPin) {
	init(true, false, false, true,
					deleteFs, initI2C, clockPin, dataPin);
}

void EntityApplication::loop() {
	this->entityManager->loop();
	this->entityUpdateManager->loop();
	wifiServerManager->loop();
}
