/*
 * EntityManager.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityManager.h>

#include <Arduino.h>
#include <ArduinoJson.h>

EntityManager::EntityManager(Entity* entities[], int count, std::function<void(void)> onEntitiesChanged) {
	for (int i = 0; i < count; i++) {
		registerAndPreInitEntity(entities[i]);
	}
	this->onEntitiesChanged = onEntitiesChanged;
}

void EntityManager::registerAndPreInitEntity(Entity* entity) {
	Serial.print(FPSTR("register "));
	entity->print();

	entity->preInitialize(this->count,
								[this](int val) {processEntityChangedEvent(val);});

	if(entity->preValidate()){
		this->entities.push_back(entity);
		this->count++;
	}else{
		this->failPreinit++;
		Serial.print(FPSTR("PreValidation failed for entity = "));
		Serial.println(entity->getName());
	}

}

void EntityManager::init(WiFiSettingsBox* conf, SmartHouseServerHelper* serverHelper) {
	this->conf = conf;
	this->serverHelper = serverHelper;

	Serial.println(FPSTR("----------------------------------"));
	Serial.println(FPSTR("Init entityManager"));
	FileUtils::init();

	for (Entity* entity : entities) {
		if(entity->validate()){
			Serial.print(FPSTR("init "));
			Serial.println(entity->getName());
			entity->init();
			this->initOk++;
		}else{
			Serial.print(FPSTR("validation FAILED "));
			Serial.println(entity->getName());
			this->failValidate++;
		}
	}

	loadEntitiesFromFile();
	saveEntitiesToFile();

	Serial.println(FPSTR("Init entityManager completed"));
	Serial.println(FPSTR("==============================="));
}

void EntityManager::processEntityChangedEvent(int entityIndex) {
	//Serial.print(FPSTR("Change event id="));
	//Serial.println(entityIndex);
	this->entitiesChanged = true;
}

Entity* EntityManager::getEntityByGroupAndName(const char* group,
		const char* name) {

	if(strcmp(group,DEFAULT_VALUE)==0 || strcmp(name,DEFAULT_VALUE)==0){
		return nullptr;
	}

	for (Entity* entity : this->entities) {
		if (entity->isTarget(group, name)) {
			return entity;
		}
	}

	return nullptr;
}

Entity* EntityManager::getEntityByGroupAndNameFromParams(JsonObject& params){
	return getEntityByGroupAndName(JsonObjectUtil::getFieldIfKeyExistsOrDefault(params, GROUP,
								DEFAULT_VALUE),
						JsonObjectUtil::getFieldIfKeyExistsOrDefault(params, NAME,
								DEFAULT_VALUE));
}

void EntityManager::addNotAllowed(JsonObject& response, const char* method) {
	JsonObjectUtil::setField(response, MESSAGE, NOT_ALLOWED);
	JsonObjectUtil::setField(response, METHOD, method);
}

void EntityManager::executeHttpMethod(EntityJsonRequestResponse* reqResp,
		const char* method) {

	executeHttpMethod(reqResp->getRequest(),
			(!reqResp->getRequest().containsKey(PARAM))?
					reqResp->getResponse()
					:reqResp->getResponse().createNestedObject(reqResp->getRequest().get<char*>(PARAM)),
			 method);
}

void EntityManager::executeHttpMethod(JsonObject& params, JsonObject& response,
		const char* method) {

	Serial.print(method);
	Serial.print(FPSTR(" "));

	JsonObjectUtil::printWithPreffix(PARAMETERS, params);

	JsonObject& json = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response,
										_DEVICE, _INFO);
	this->serverHelper->addDeviceInfoToJson(json);

	bool changed = false;

	if (hasNoGroupNoName(params) || hasAllGroupNoName(params)) {
		changed = executeMethodOnAll(params, response, method);
	}else
	if (hasGroupNoName(params)) {
		changed = executeMethodOnGroup(params, response, method);
	}else
	if (hasGroupName(params)) {
		Entity* entity = getEntityByGroupAndNameFromParams(params);

		changed = executeMethodOnEntity(params, response, entity, method);
	}

	JsonObjectUtil::printWithPreffix(RESPONSE, response);

	this->entitiesChanged = (this->entitiesChanged || changed);

	if(this->entitiesChanged){
		Serial.print(FPSTR("changed="));
		Serial.println(this->entitiesChanged);
	}
}

bool EntityManager::executeMethodOnAll(JsonObject& params,
		JsonObject& response, const char* method) {

	bool changed = false;

	for (Entity* entity : this->entities) {
		executeMethodOnEntity(params, response, entity, method);
		changed = changed || entity->isChanged();
	}

	return changed;
}

bool EntityManager::executeMethodOnGroup(JsonObject& params,
		JsonObject& response, const char* method) {

	bool changed = false;

	int totalCount = 0;

	const char* group = params[GROUP].as<char*>();

	for (Entity* entity : this->entities) {
		if(strcmp(entity->getGroup(),group)==0){
			executeMethodOnEntity(params, response, entity, method);

			changed = changed || entity->isChanged();

			totalCount++;
		}
	}

	if (totalCount == 0) {
		JsonObjectUtil::setField(response, MESSAGE, NOT_FOUND);
		return false;;
	}

	return changed;
}

bool EntityManager::executeMethodOnEntity(JsonObject& params, JsonObject& response, Entity* entity,
												const char* method) {

	if (entity == nullptr) {
		JsonObjectUtil::setField(response, MESSAGE, NOT_FOUND);
		return false;
	}

	bool allowed = false;

	if (strcmp(method, REQUEST_GET) == 0) {
		if (entity->hasGetMethod()) {
			entity->executeGet(params,
					JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response,
							entity->getGroup(), entity->getName()));
			allowed = true;
		}
	} else if (strcmp(method, REQUEST_POST) == 0) {
		if (entity->hasPostMethod()) {
			entity->executePost(
					JsonObjectUtil::getFieldIfKeyExistsOrDefault<JsonObject&>(
							params, BODY, params),
					JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response,
							entity->getGroup(), entity->getName()));
			allowed = true;
		}
	} else {
		JsonObjectUtil::setField(response, MESSAGE, BAD_METHOD);
		return false;
	}

	if(allowed){
		return entity->isChanged();
	}

	addNotAllowed(response, method);
	return false;
}

void EntityManager::executeLoadOnEntity(JsonObject& jsonFromFile,
		Entity* entity) {
	if (entity->canLoadState()) {
		entity->setJsonToEntity(jsonFromFile);
	}
}

void EntityManager::executeSaveOnEntity(JsonObject& jsonToFile,
		Entity* entity) {
	if (entity->canSaveState()) {
		entity->getJsonToSave(jsonToFile);
	}
}

void EntityManager::loadEntitiesFromFile() {
	Serial.println(FPSTR("Load entities"));
	persist(
			[this](JsonObject& json, Entity* entity) {executeLoadOnEntity(json,entity);},
			nullptr);
}

void EntityManager::saveEntitiesToFile() {
	Serial.println(FPSTR("Save entities"));
	persist(
			[this](JsonObject& json, Entity* entity) {executeSaveOnEntity(json,entity);},
			[this](JsonObject& json) {FileUtils::saveJsonToFileIfDiff(FILE_PATH, json);});
}

void EntityManager::groupNameToParam(char* group, char* name,
		EntityJsonRequestResponse* json) {
	json->addRequestParam((char*)GROUP, group);
	json->addRequestParam((char*)NAME, name);
}

EntityJsonRequestResponse* EntityManager::createEntityJsonRequestResponse() {
	ObjectUtils::printHeap();
	return new EntityJsonRequestResponse();
}

void EntityManager::deleteEntityJsonRequestResponse(
		EntityJsonRequestResponse* json) {
	delete json;
	ObjectUtils::printHeap();
}

bool EntityManager::processEntitiesChange(EntityJsonRequestResponse* collector){
	if(!this->entitiesChanged){
		return false;
	}
	Serial.println(FPSTR("-------------------------"));
	unsigned long start = millis();
	//dispatchAllChangedEntities();
	collectAllChangedEntities(collector);

	bool result = finishChangesProcess();
	ObjectUtils::printTimeHeap(start);
	Serial.println(FPSTR("-------------------------"));
	return result;
}

void EntityManager::dispatchAllChangedEntities() {
	for(Entity* entity: entities){
		entity->dispatchChangeEventIfChanged();
	}
}

void EntityManager::collectAllChangedEntities(EntityJsonRequestResponse* collector){
	if(collector!=nullptr){
		collectAllChangedEntities(collector->getRequest(),collector->getResponse());
	}
}

void EntityManager::collectAllChangedEntities(JsonObject& params, JsonObject& response){
	for(Entity* entity: entities){
		if(entity->isChanged()){
			executeMethodOnEntity(params, response,
					entity);
		}
	}
}

bool EntityManager::finishChangesProcess(){
	bool dispatch = false;
	bool toSave = false;

	for(Entity* entity: entities){
		if(entity->isChanged()){
			entity->setChanged(false);
			if(entity->isApplicationDispatcher()){
				Serial.print(FPSTR("Entity "));
				Serial.print(entity->getName());
				Serial.println(FPSTR(" changed"));
				dispatch = true;
			}
		}
		if(entity->isSaveRequired()){
			entity->setSaveRequired(false);
			toSave = true;
		}
	}

	if(toSave){
		Serial.println(FPSTR("Save is required"));
		//saveEntitiesToFile();
	}

	if(dispatch){
		if(onEntitiesChanged!=nullptr){
			onEntitiesChanged();
		}
	}

	this->entitiesChanged = false;

	return dispatch;
}

bool EntityManager::hasNoGroupNoName(JsonObject& params) {
	return !JsonObjectUtil::hasFieldWithAnyType(params, GROUP)
				&& !JsonObjectUtil::hasFieldWithAnyType(params, NAME);
}

bool EntityManager::hasGroupNoName(JsonObject& params) {
	return JsonObjectUtil::hasFieldWithAnyType(params, GROUP)
					&& !JsonObjectUtil::hasFieldWithAnyType(params, NAME);
}

bool EntityManager::hasGroupName(JsonObject& params) {
	return JsonObjectUtil::hasFieldWithAnyType(params, GROUP)
					&& JsonObjectUtil::hasFieldWithAnyType(params, NAME);
}

bool EntityManager::hasAllGroupNoName(JsonObject& params) {
	return JsonObjectUtil::hasFieldAndValueEquals(params, (char*)GROUP, (char*)GROUP_ALL);
}

void EntityManager::setOnEntitiesChanged(
		std::function<void(void)> onEntitiesChanged) {
	this->onEntitiesChanged = onEntitiesChanged;
}

void EntityManager::print() {
	Serial.print(FPSTR("EntityManager "));
	Serial.print(FPSTR("count = "));
	Serial.print(count);
	Serial.print(FPSTR(" failed preinit = "));
	Serial.print(failPreinit);
	Serial.print(FPSTR(" initOk = "));
	Serial.print(initOk);
	Serial.print(FPSTR(" failValidate = "));
	Serial.println(failValidate);

}

WiFiSettingsBox* EntityManager::getConf() {
	return this->conf;
}

SmartHouseServerHelper* EntityManager::getServerHelper() {
	return this->serverHelper;
}

void EntityManager::persist(
		std::function<void(JsonObject& json, Entity* entity)> onEntityFunction,
		std::function<void(JsonObject& json)> postPersistFunction) {

	Serial.println(FPSTR("-------------"));
	JsonObject& jsonEmpty = buf.parse("{}").as<JsonObject>();

	JsonObject& jsonTemp = FileUtils::loadJsonFromFile(FILE_PATH, &buf, jsonEmpty);


	Serial.println(FPSTR("Loaded entities"));
	JsonObjectUtil::print(jsonTemp);
	Serial.println(FPSTR("-------------"));

	for (Entity* entity : entities) {
		JsonObject& json = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(
				jsonTemp, entity->getGroup(), entity->getName());
		entity->print();
		JsonObjectUtil::print(json);
		onEntityFunction(json, entity);
	}

	if (postPersistFunction != nullptr) {
		postPersistFunction(jsonTemp);
		Serial.println(FPSTR("Execute post persist function"));
	}

	//buf->clear();

	Serial.println(FPSTR("------persisted-------"));
}

void EntityManager::get(EntityJsonRequestResponse* reqResp) {
	executeHttpMethod(reqResp, REQUEST_GET);
}

void EntityManager::post(EntityJsonRequestResponse* reqResp) {
	executeHttpMethod(reqResp, REQUEST_POST);
}

