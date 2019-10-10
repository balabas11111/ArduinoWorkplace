/*
 * DisplayManager.cpp
 *
 *  Created on: Oct 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <display/DisplayManager.h>

DisplayManager::DisplayManager(PageToDisplayAdapter* displayAdapter,
		DisplayPage** pages, unsigned char pageCount, SettingsStorage* conf) {
	this->displayAdapter = displayAdapter;
	this->pages = pages;
	this->pageCount = pageCount;
	this->conf = conf;
	this->currentPage = DISPLAY_MANAGER_STATUS_PAGE_INDEX;
}

void DisplayManager::init() {
	if (!displayAdapter->init()){
		return;
	}
	initDone = true;

	conf->setOnDeviceStatusChanged([this](){switchToStatusPageReturnToPrevious();});
	conf->setDeviceStatus((char*)DEVICE_STATUS_INITIALIZING);

	int initDoneCount = 0;

	for(int i = 1; i <= this->pageCount; i++){
		if((this->pages[i-1])->init()){
			initDoneCount++;
		}
	}
	Serial.print(FPSTR("DisplayManager pages="));
	Serial.println(initDoneCount);
}

void DisplayManager::switchToStatusPage(bool backToPrev) {
	this->currentPage = DISPLAY_MANAGER_STATUS_PAGE_INDEX;

	displayAdapter->setPowerOn();
	renderStatusPage();

	if(backToPrev && this->currentPage != this->previousPage){
		this->currentPage = this->previousPage;
		renderCurrentPage();
	}
}

void DisplayManager::switchToNextPage() {
	if(this->pageChangeEnabled && this->initDone){
		this->previousPage = this->currentPage;

		if(this->currentPage == this->pageCount){
			switchToStatusPage(false);
		} else {
			this->currentPage++;
			renderCurrentPage();
		}
	}
}

void DisplayManager::setPageChangeEnabled(bool pageChangeEnabled) {
	this->pageChangeEnabled = pageChangeEnabled;
}

void DisplayManager::renderCurrentPage() {
	if(!(this->pageChangeEnabled && this->initDone)){
		return;
	}

	this->pages[this->currentPage-1]->render(this->displayAdapter);
}

void DisplayManager::loop() {
	this->displayAdapter->loop();
}

void DisplayManager::renderStatusPage() {
	if(!this->initDone){
		return;
	}
	EntityJsonRequestResponse* resp = new EntityJsonRequestResponse();

	JsonArray& pageData = resp->getResponse().createNestedArray(DISPLAY_DATA);

	pageData.add(EMPTY_LINE);
	if(WiFiUtils::isConnected()){
		pageData.add(conf->getCurrentIp());
	}
	pageData.add(conf->getDeviceStatus());

	this->displayAdapter->renderPage(DISPLAY_MANAGER_STATUS_PAGE_HEADER, pageData);

	delete resp;
}

void DisplayManager::switchToStatusPageReturnToPrevious() {
	switchToStatusPage(true);
}