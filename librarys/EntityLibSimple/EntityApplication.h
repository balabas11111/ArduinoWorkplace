/*
 * EntityApplication.h
 *
 *  Created on: 30 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_

#include <Arduino.h>

#include <WiFi/WiFiSettingsBox.h>

#include <WiFi/WiFiManager.h>
#include <WiFi/WiFiServerManager.h>

#include <Entity.h>
#include <EntityManager.h>
#include <EntityUpdate.h>
#include <EntityUpdateManager.h>

#include <ObjectUtils.h>
#include <FileUtils.h>
#include <I2C_utils.h>

#include <functional>

#include <Ticker.h>

class EntityApplication {
public:
	EntityApplication(const char* firmWare, Entity* entities[], int entityCount,
			EntityUpdate* entityUpdate[], int entityUpdateCount,
			WiFiSettingsBox* conf = nullptr,
			std::function<void(void)> onWiFiConnected = nullptr,
			std::function<void(void)> onWiFiDisConnected = nullptr);
	virtual ~EntityApplication() {
	}
	;

	void init(bool initSerial = true, bool initWiFi = false, bool initServer = false, bool initFs = true,
			bool deleteFs = false, bool initI2C = false, uint8_t clockPin = SCL, uint8_t dataPin = SDA);

	void loop();

	void startWiFi(){
		wifiManager->begin();
	}
	void startServer(){
		wifiServerManager->begin();
	}

	EntityManager* getEntityManager() {
		return entityManager;
	}

	void registerTicker(uint32_t milliseconds, void (*callback)(void)){
		Ticker* ticker = new Ticker();
		ticker->attach_ms(milliseconds, callback);
	}


private:
	WiFiSettingsBox* conf;
	WiFiManager* wifiManager;
	WiFiServerManager* wifiServerManager;

	EntityManager* entityManager;
	EntityUpdateManager* entityUpdateManager;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_ */
