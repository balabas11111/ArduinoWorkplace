/*
 * DisplayManager.h
 *
 *  Created on: Oct 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYMANAGER_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "functional"
#include "SettingsStorage.h"
#include "utils/WiFiUtils.h"
#include "display/PageToDisplayAdapter.h"
#include "display/DisplayPage.h"
#include "EntityJsonRequestResponse.h"
#include "DeviceConstants.h"

#define DISPLAY_MANAGER_STATUS_PAGE_INDEX 0
#define DISPLAY_MANAGER_STATUS_PAGE_HEADER "Balabas-soft"
#define DISPLAY_MANAGER_STATUS_PAGE_EMPTY_LINE ""

class DisplayManager {
public:
	DisplayManager(PageToDisplayAdapter* displayAdapter, DisplayPage** pages, unsigned char pageCount, SettingsStorage* conf);
	virtual ~DisplayManager(){}
	virtual void init();

	virtual void switchToStatusPageReturnToPrevious();
	virtual void switchToStatusPage(bool backToPrev);
	virtual void switchToNextPage() ;

	virtual void switchToNextNonStatusPage(){
		if(this->currentPage == this->pageCount) {
			Serial.println(FPSTR("max page reached"));
			this->currentPage = 0;
		}
		turnPowerOn();
		switchToNextPage();
	}

	void setPageChangeEnabled(bool pageChangeEnabled = true);

	void loop();

	void switchToPage(int page) {
		Serial.print(FPSTR("switchToPage ="));
		Serial.println(page);

		this->currentPage = page;
		renderCurrentPage();
	}

	void switchToNextPageOrTurnPowerOn() {
		Serial.println(FPSTR("switchToNextPageOrTurnPowerOn"));
		PageToDisplayAdapter* adapter = getCurrentPage()->getAdapter();

		if(adapter!=nullptr) {
			if(adapter->isPowerOn()) {
				switchToNextNonStatusPage();
			} else {
				turnPowerOn();
				renderCurrentPage();
			}
		}
	}

	virtual void renderCurrentPage();

	DisplayPage* getCurrentPage();

	void turnPowerOn() {
		DisplayPage* page = getCurrentPage();
		if(page != nullptr) {
			PageToDisplayAdapter* adapter = page->getAdapter();
			if(adapter!=nullptr) {
				adapter->setPowerOn();
			}
		}
	}

protected:
	virtual void renderStatusPage();
private:
	PageToDisplayAdapter* displayAdapter;
	DisplayPage** pages;
	unsigned char pageCount = 0;
	SettingsStorage* conf;
	unsigned char currentPage = DISPLAY_MANAGER_STATUS_PAGE_INDEX;
	unsigned char previousPage = DISPLAY_MANAGER_STATUS_PAGE_INDEX;

	bool initDone = false;
	bool pageChangeEnabled = false;

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYMANAGER_H_ */
