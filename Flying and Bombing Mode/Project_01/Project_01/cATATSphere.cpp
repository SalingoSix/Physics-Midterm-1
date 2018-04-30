#include "cATATSphere.h"
#include "cGameObject.h"

cATATSphere::cATATSphere()
{
	this->Alerter = cCollisionAlert::getInstance();
	return;
}

cATATSphere::~cATATSphere()
{
	return;
}

void cATATSphere::physicsStep(double deltaTime)
{
		return;
}

int cATATSphere::shapeType()
{
	return 3;
}

void cATATSphere::setPosition(glm::vec3 newPos)
{
	this->pObject->position = newPos;
}

glm::vec3 cATATSphere::getPosition()
{
	return this->pObject->position;
}

void cATATSphere::issueAlert(int pos1, int pos2, int type2)
{
	Alerter->makeAlert(pos1, pos2, this->shapeType(), type2);
	return;
}

void cATATSphere::ATATHit()
{
	this->myATAT->diffuseColour.r = 0.0f;
	this->myATAT->diffuseColour.g = 0.0f;
	this->myATAT->diffuseColour.b = 0.0f;
	this->pObject->scale = 0.0f;
	return;
}