#include "TestComponent.h"
#include <iostream>
#include "Entity.h"
#include "Transform.h"


TestComponent::TestComponent(json& args): Component(args)
{
}

TestComponent::~TestComponent()
{
	Component::~Component();
}

void TestComponent::update() {
	Ogre::Vector3 pos = getEntity()->getComponent<Transform>("Transform")->getWorldPosition();
	pos.x += 0.7f;
	getEntity()->getComponent<Transform>("Transform")->setWorldPosition(pos);
}

bool TestComponent::ReceiveEvent(Event& event)
{
	if (event.type == EventType::TEXT) {
		TextEvent textEvent = static_cast<TextEvent&>(event);
		printf(textEvent.text.c_str());
	}
	return false;
}
