#include "Physics/RigidBody.h"

#include "Entity/Entity.h"
#include "Graphics/Mesh.h"
#include "Entity/Transform.h"
#include "Physics/PhysicsManager.h"
#include <BtOgreExtras.h>
#include <btBulletCollisionCommon.h>
#include "checkML.h"

RigidBody::RigidBody(json& args) : Component(args)
{
}

RigidBody::~RigidBody()
{
	Component::~Component();
	btDynamicsWorld* w = PhysicsManager::getInstance()->getWorld();
	w->removeRigidBody(body);
	delete body->getCollisionShape();
	delete body;
}

void RigidBody::init(json& args)//mandar unha mesaxe a outros componentes (por exemplo render) mandando informacion do corpo rixido
{
	createRigidBody(args);

	RigidBodyInfoEvent event(body);
	EventManager::getInstance()->RegisterListener(getEntity(), "RIGIDBODY_INFO"); //rexistrar a entidade que alberga o componente para mandalo mesaxe os demais componentes desta
	EventManager::getInstance()->EmitEvent(event);
	EventManager::getInstance()->ClearListeners(event.type);
}

void RigidBody::redefine(json& args)
{
	if (args["shape"].is_null())
		args["shape"] = shape;

	if (args["mass"].is_null())
		args["mass"] = body->getMass();

	if (args["isStatic"].is_null())
		args["isStatic"] = isStatic;

	if (args["isKinematic"].is_null())
		args["isKinematic"] = isKinematic;

	if (args["disableDeactivation"].is_null())
		args["disableDeactivation"] = (body->getActivationState() == DISABLE_DEACTIVATION);

	if (args["friction"].is_null()) {
		args["friction"] = body->getFriction();
	}

	if (args["rollingFriction"].is_null()) {
		args["rollingFriction"] = body->getRollingFriction();
	}

	if (args["spinningFriction"].is_null()) {
		args["spinningFriction"] = body->getSpinningFriction();
	}


	body->setUserPointer(nullptr);
	PhysicsManager::getInstance()->getWorld()->removeRigidBody(body);
	delete body->getCollisionShape();
	delete body;

	init(args);
}

void RigidBody::preupdate()
{
	//asigna o rigidbody a posicion e rotacion que manda o transform
	
	Transform* tr = getEntity()->getComponent<Transform>("Transform");

	bakPosition = tr->getWorldPosition();
	bakRotation = tr->getWorldRotation();
	bakScale = tr->getWorldScale();
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	trans.setOrigin(tr->getWorldPosition());
	trans.setRotation(tr->getWorldRotation());
	body->getMotionState()->setWorldTransform(trans);
	body->setWorldTransform(trans);

	body->getCollisionShape()->setLocalScaling(bakScale);
	PhysicsManager::getInstance()->getWorld()->updateSingleAabb(body);
}

void RigidBody::physicsUpdate()
{
	//trala actualizacion das fisicas, o rigidbody volve a mandar sobre o transform
	Transform* tr = getEntity()->getComponent<Transform>("Transform");
	btTransform trans;
	trans = body->getWorldTransform();

	tr->setWorldPosition((Vector3)trans.getOrigin());
	tr->setWorldRotation((Quaternion)trans.getRotation());
	tr->setScale((Vector3)body->getCollisionShape()->getLocalScaling());
}

void RigidBody::onActivated()
{
	PhysicsManager::getInstance()->getWorld()->addRigidBody(body);
}

void RigidBody::onDeactivated()
{
	PhysicsManager::getInstance()->getWorld()->removeRigidBody(body);
}

btRigidBody* RigidBody::getRigidBody()
{
	return body;
}

void RigidBody::applyForce(ForceType type, Vector3 force, Vector3 relPos)
{
	body->activate(true);
	switch (type) {
		case CENTRAL_FORCE:
			body->applyCentralForce(force);
			break;
		case CENTRAL_IMPULSE:
			body->applyCentralImpulse(force);
			break;
		case FORCE:
			body->applyForce(force, relPos);
			break;
		case IMPULSE:
			body->applyImpulse(force, relPos);
			break;
		case PUSH_IMPULSE:
			body->applyImpulse(force, relPos);
			break;
		case TORQUE:
			body->applyTorque(force);
			break;
		case TORQUE_IMPULSE:
			body->applyTorqueImpulse(force);
			break;
		case TORQUE_TURN_IMPULSE:
			body->applyTorqueTurnImpulse(force);
			break;
		case LINEAR_VELOCITY:
			body->setLinearVelocity(force);
			break;
		case ANGULAR_VELOCITY:
			body->setAngularVelocity(force);
			break;
		default:
			break;
	}
}

void RigidBody::setGravity(Vector3 grav)
{
	body->setGravity(grav);
}

Vector3 RigidBody::getGravity()
{
	return Vector3(body->getGravity().x(), body->getGravity().y(), body->getGravity().z());
}

void RigidBody::createRigidBody(json& args)
{
	//default rigidbody shape is mesh
	shape = "mesh";
	float mass = 0.0;

	if (!args["shape"].is_null()) {
		std::string cast = args["shape"];
		shape = cast;
	}

	if (!args["mass"].is_null()) {
		//if it's kinematic or static, it won't have mass
		if ((args["isStatic"].is_null() || !args["isStatic"]) || (args["isKinematic"].is_null() || !args["isKinematic"]))
		{
			float cast = args["mass"];
			mass = cast;
		}
	}

	body = PhysicsManager::getInstance()->createRigidBody(shape, e_->getComponent<Transform>("Transform")->getPosition(), e_->getComponent<Mesh>("Mesh")->getOgreEntity(), mass, e_->getComponent<Mesh>("Mesh")->isMeshAnimated(), false);

	body->setUserPointer(this);

	if (!args["gravity"].is_null()) {
		float x, y, z;
		x = args["gravity"][0];
		y = args["gravity"][1];
		z = args["gravity"][2];
		body->setGravity(Vector3(x, y, z));
	}

	if (!args["friction"].is_null()) {
		float friction = args["friction"];
		body->setFriction(friction);
	}

	if (!args["rollingFriction"].is_null()) {
		float rollingFriction = args["rollingFriction"];
		body->setRollingFriction(rollingFriction);
	}

	if (!args["spinningFriction"].is_null()) {
		float spinningFriction = args["spinningFriction"];
		body->setSpinningFriction(spinningFriction);
	}

	if (!args["angularFactor"].is_null() && args["angularFactor"].is_array()) {
		body->setAngularFactor(btVector3(args["angularFactor"][0], args["angularFactor"][1], args["angularFactor"][2]));
	}

	if (!args["linearFactor"].is_null() && args["linearFactor"].is_array()) {
		body->setLinearFactor(btVector3(args["linearFactor"][0], args["linearFactor"][1], args["linearFactor"][2]));
	}

	if (!args["isTrigger"].is_null() && args["isTrigger"]) {

		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		isTrigger = true;
	}

	if (!args["isStatic"].is_null() && args["isStatic"])
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		isStatic = true;
	}
	else if(!args["isKinematic"].is_null() && args["isKinematic"])
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		isKinematic = true;
	}

	else {
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		if (!args["disableDeactivation"].is_null() && args["disableDeactivation"])
			body->setActivationState(DISABLE_DEACTIVATION);
	}
}

bool RigidBody::isRBStatic()
{
	return isStatic;
}

bool RigidBody::isRBKinematic()
{
	return isKinematic;
}
