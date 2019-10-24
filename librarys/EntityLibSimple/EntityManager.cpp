/*
 * EntityManager.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityManager.h>

#include <Arduino.h>
#include <ArduinoJson.h>

EntityManager::EntityManager(Entity* entities[], int count, SettingsStorage* conf, std::function<void(void)> onEntitiesChanged) {
	for (int i = 0; i < count; i++) {
		registerAndPreInitEntity(entities[i]);
	}
	this->onEntitiesChanged = onEntitiesChanged;

	this->conf = conf;
}

void EntityManager::registerAndPreInitEntity(Entity* entity) {
	Serial.print(FPSTR("register "));
	entity->print();

	entity->preInitialize(this->count,
								[this]() {markEntitiesAsChanged();},
								[this](const char* group, const char* name, const char* key, JsonVariant value) {putToBuffer(group, name, key, value);});

	if(entity->preValidate()){
		this->entities.push_back(entity);
		this->count++;
	}else{
		this->failPreinit++;
		Serial.print(FPSTR("PreValidation failed for entity = "));
		Serial.println(entity->getName());
	}

}

void EntityManager::init() {
	this->conf = conf;
	this->buffer = new EntityJsonRequestResponse();

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

void EntityManager::markEntitiesAsChanged() {
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
	const char* group = JsonObjectUtil::getFieldIfKeyExistsOrDefault(params, DEVICE_FIELD_GROUP,
			(const char*)DEFAULT_VALUE);
	const char* name = JsonObjectUtil::getFieldIfKeyExistsOrDefault(params, DEVICE_FIELD_ENTITY_NAME,
			(const char*)DEFAULT_VALUE);
	/*Serial.print(FPSTR("group="));
	Serial.print(group);
	Serial.print(FPSTR(" name="));
	Serial.println(name);
*/
	return getEntityByGroupAndName(group, name);
}

void EntityManager::addNotAllowed(JsonObject& response, const char* method) {
	JsonObjectUtil::setField(response, MESSAGE, NOT_ALLOWED);
	JsonObjectUtil::setField(response, METHOD, method);
}

void EntityManager::executeMethod(EntityJsonRequestResponse* reqResp, const char* method){
	executeMethod(reqResp->getRequest(), reqResp->getResponse(), method);
}

void EntityManager::executeMethod(JsonObject& params, JsonObject& response,
		const char* method) {

	JsonObjectUtil::printWithPreffix(PARAMETERS, params);
		JsonObjectUtil::printWithPreffix(RESPONSE, response);

	Serial.print(method);
	Serial.print(FPSTR(" "));


	/*
	JsonObject& json = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response,
										_DEVICE, _INFO);

	Serial.println(FPSTR("Add device info"));
	if(this->getConf()!=nullptr){
		Serial.println(FPSTR("not nullptr"));
	}
*/

	this->getConf()->addDeviceInfoToJson(response);
	Serial.println(FPSTR("info added"));
	bool changed = false;

	if (hasNoGroupNoName(params) || hasAllGroupNoName(params)) {
		changed = executeMethodOnAll(params, response, method);
	}else
	if (hasGroupNoName(params)) {
		changed = executeMethodOnGroup(params, response, method);
	}else
	if (hasGroupName(params)) {
		//Serial.println(FPSTR("HasGroupName"));
		Entity* entity = getEntityByGroupAndNameFromParams(params);

		changed = executeMethodOnEntity(params, response, entity, method);
	}

	//JsonObjectUtil::printWithPreffix(RESPONSE, response);

	this->entitiesChanged = (this->entitiesChanged || changed);

	if(this->entitiesChanged){
		Serial.print(FPSTR("chg="));
		Serial.println(this->entitiesChanged);
	}
}

bool EntityManager::executeMethodOnAll(JsonObject& params,
		JsonObject& response, const char* method) {

	bool changed = false;

	for (Entity* entity : this->entities) {
		changed = executeMethodOnEntity(params, response, entity, method) || changed;
	}

	return changed;
}

bool EntityManager::executeMethodOnGroup(JsonObject& params,
		JsonObject& response, const char* method) {

	bool changed = false;

	int totalCount = 0;

	const char* group = params[DEVICE_FIELD_GROUP].as<char*>();

	for (Entity* entity : this->entities) {
		if(strcmp(entity->getGroup(),group)==0){
			changed = executeMethodOnEntity(params, response, entity, method) || changed;

			totalCount++;
		}
	}

	if (totalCount == 0) {
		JsonObjectUtil::setField(response, MESSAGE, NOT_FOUND);
		return false;;
	}

	return changed;
}

bool EntityManager::executeGetMethodOnEntity(EntityJsonRequestResponse* data, Entity* entity){
	return executeMethodOnEntity(data->getRequest(), data->getResponse(), entity, REQUEST_GET);
}

bool EntityManager::executeMethodOnEntity(JsonObject& params, JsonObject& response, Entity* entity,
												const char* method) {

	if (entity == nullptr) {
		Serial.println(FPSTR("Entity not found"));
		JsonObjectUtil::setField(response, MESSAGE, NOT_FOUND);
		return false;
	}

	bool allowed = false;
	bool changed = false;

	if (strcmp(method, REQUEST_GET) == 0) {
		if (entity->hasGetMethod()) {
			entity->executeGet(params,
					JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response,
							entity->getGroup(), entity->getName()));
			allowed = true;
		}
	} else if (strcmp(method, REQUEST_POST) == 0) {
		Serial.print(FPSTR("POST on entity"));
		if (entity->hasPostMethod()) {
			Serial.print(FPSTR("entity="));
			Serial.println(entity->getName());

			JsonObject& paramsToPost = JsonObjectUtil::getFieldIfKeyExistsOrDefault<JsonObject&>(
					params, BODY, params);
			JsonObject& responceToPost = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response,
					entity->getGroup(), entity->getName());

			JsonObjectUtil::printWithPreffix("paramsToPost", paramsToPost);
			JsonObjectUtil::printWithPreffix("responceToPost", responceToPost);

			entity->executePost(paramsToPost, responceToPost);
			allowed = true;
			changed = entity->isMarkedAsChanged();
		} else {
			Serial.println(FPSTR("Post is not allowed"));
		}
	} else {
		JsonObjectUtil::setField(response, MESSAGE, BAD_METHOD);
		return false;
	}

	if(allowed){
		return changed;
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
#ifdef SETTINGS_ENTITIES_PERSISTANCE_ENABLED
	Serial.println(FPSTR("Load entities"));
	persist(
			[this](JsonObject& json, Entity* entity) {executeLoadOnEntity(json,entity);},
			nullptr);
#endif
}

void EntityManager::saveEntitiesToFile() {
#ifdef SETTINGS_ENTITIES_PERSISTANCE_ENABLED
	Serial.println(FPSTR("Save entities"));
	persist(
			[this](JsonObject& json, Entity* entity) {executeSaveOnEntity(json,entity);},
			[this](JsonObject& json) {FileUtils::saveJsonToFileIfDiff(FILE_PATH, json);});
#endif
}

void EntityManager::groupNameToParam(char* group, char* name,
		EntityJsonRequestResponse* json) {
	json->addRequestParam((char*)DEVICE_FIELD_GROUP, group);
	json->addRequestParam((char*)DEVICE_FIELD_ENTITY_NAME, name);
}

EntityJsonRequestResponse* EntityManager::createEntityJsonRequestResponse() {
	//DeviceUtils::printHeap();
	return new EntityJsonRequestResponse();
}

void EntityManager::deleteEntityJsonRequestResponse(
		EntityJsonRequestResponse* json) {
	delete json;
	//DeviceUtils::printHeap();
}

bool EntityManager::isEntitiesChanged(){
	return this->entitiesChanged;
}

bool EntityManager::processChangedEntities(EntityJsonRequestResponse* data){
	if(!this->entitiesChanged){
		return false;
	}
	Serial.print(FPSTR("---E Chg:  "));
	unsigned long start = millis();

	bool result = false;
	bool toSave = false;

	for(Entity* entity: entities){
		if(entity->isSaveRequired()){
			entity->setSaveRequired(false);
			toSave = true;
		}
		if(entity->isMarkedAsChanged()){
			Serial.print(entity->getName());
			Serial.print(FPSTR("; "));
			result = true;
			entity->markEntityAsUnChanged();

			if (data != nullptr){
				executeGetMethodOnEntity(data, entity);
			}
		}
	}

	if(toSave){
		Serial.println(FPSTR("Save is required"));
		saveEntitiesToFile();
	}

	if(result){
		if(onEntitiesChanged!=nullptr){
			onEntitiesChanged();
		}
	}

	this->entitiesChanged = false;

	DeviceUtils::printTimeHeap(start);
	Serial.println(FPSTR("---"));
	return result;
}

char* EntityManager::getSensorsGroup() {
	return (char*)GROUP_SENSORS;
}

bool EntityManager::hasNoGroupNoName(JsonObject& params) {
	return !JsonObjectUtil::hasFieldWithAnyType(params, DEVICE_FIELD_GROUP)
				&& !JsonObjectUtil::hasFieldWithAnyType(params, DEVICE_FIELD_ENTITY_NAME);
}

bool EntityManager::hasGroupNoName(JsonObject& params) {
	return JsonObjectUtil::hasFieldWithAnyType(params, DEVICE_FIELD_GROUP)
					&& !JsonObjectUtil::hasFieldWithAnyType(params, DEVICE_FIELD_ENTITY_NAME);
}

bool EntityManager::hasGroupName(JsonObject& params) {
	return JsonObjectUtil::hasFieldWithAnyType(params, DEVICE_FIELD_GROUP)
					&& JsonObjectUtil::hasFieldWithAnyType(params, DEVICE_FIELD_ENTITY_NAME);
}

bool EntityManager::hasAllGroupNoName(JsonObject& params) {
	return JsonObjectUtil::hasFieldAndValueEquals(params, (char*)DEVICE_FIELD_GROUP, (char*)GROUP_ALL);
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

SettingsStorage* EntityManager::getConf() {
	return this->conf;
}

void EntityManager::persist(
		std::function<void(JsonObject& json, Entity* entity)> onEntityFunction,
		std::function<void(JsonObject& json)> postPersistFunction) {

	Serial.println(FPSTR("-------------"));
	EntityJsonRequestResponse* req = new EntityJsonRequestResponse();

	JsonObject& jsonTemp = FileUtils::loadJsonFromFile(FILE_PATH, req->getBuffer(), req->getRequest());


	Serial.println(FPSTR("Loaded entities"));
	JsonObjectUtil::print(jsonTemp);
	Serial.println(FPSTR("-------------"));

	for (Entity* entity : entities) {
		JsonObject& json = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(
				jsonTemp, entity->getGroup(), entity->getName());
		/*entity->print();
		JsonObjectUtil::print(json);
		*/
		onEntityFunction(json, entity);
	}

	if (postPersistFunction != nullptr) {
		postPersistFunction(jsonTemp);
		Serial.println(FPSTR("Execute post persist function"));
	}

	delete req;

	Serial.println(FPSTR("------persisted-------"));
}

void EntityManager::get(EntityJsonRequestResponse* reqResp) {
	executeMethod(reqResp->getRequest(),reqResp->getResponse(), REQUEST_GET);
}

void EntityManager::post(EntityJsonRequestResponse* reqResp) {
	executeMethod(reqResp->getRequest(),reqResp->getResponse(), REQUEST_POST);
}

EntityJsonRequestResponse* EntityManager::getBuffer() {
	return this->buffer;
}

void EntityManager::setBuffer(EntityJsonRequestResponse* buffer) {
	this->buffer = buffer;
}

void EntityManager::putToBuffer(const char* group, const char* name,
		const char* key, JsonVariant value) {
	if(this->buffer!=nullptr){
		JsonObject& obj =
		JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(this->buffer->getResponse(), group, name);

		obj[key] = value;
	}
}
