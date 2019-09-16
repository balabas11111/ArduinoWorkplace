/*
 * EntityManagerSerialNotifier.h
 *
 *  Created on: 5 ����. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERNOTIFIER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERNOTIFIER_H_

#include <Arduino.h>
#include <EntityManager.h>
#include <JsonObjectUtil.h>

#include <ObjectUtils.h>
#include <Notifiers/Notifier.h>

#include <EntityJsonRequestResponse.h>

class EntityManagerNotifier: public Notifier  {
public:
	EntityManagerNotifier(NotificationTarget* target = nullptr, EntityManager* manager = nullptr):Notifier("EntityManagerNotifier", target, manager){

	}
	virtual ~EntityManagerNotifier(){};

	virtual void notify(char* group = nullptr, char* name = nullptr, NotificationTarget* notifTarget = nullptr){
		if(!initialized()){
			Serial.println(FPSTR("No entity Manager was set"));
			return;
		}

		unsigned long start = startNotification();
		EntityJsonRequestResponse* json = this->manager->createEntityJsonRequestResponse();

		json->addRequestParam((char*)GROUP, group);
		json->addRequestParam((char*)NAME, name);

		this->manager->get(json);

		toTarget(json->getResponse(), notifTarget);
		this->manager->deleteEntityJsonRequestResponse(json);

		finishNotification(start);
	}

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERNOTIFIER_H_ */
