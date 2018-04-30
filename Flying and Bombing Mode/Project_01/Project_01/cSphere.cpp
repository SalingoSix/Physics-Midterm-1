#include "cSphere.h"
#include "cGameObject.h"

cSphere::cSphere()
{
	this->Alerter = cCollisionAlert::getInstance();
	this->hasExploded = false;
	return;
}

cSphere::~cSphere()
{
	return;
}

void cSphere::physicsStep(double deltaTime)
{
	if (!this->pObject->bIsUpdatedInPhysics)
		return;

	if (hasExploded)
	{
		this->pObject->diffuseColour.r = 1.0f;
		this->pObject->diffuseColour.g = 0.6f;
		this->pObject->diffuseColour.b = 0.0f;
		this->radius += 0.025;
		this->pObject->scale += 0.025f;

		if (this->radius > 1.5f)
		{
			this->pObject->bIsUpdatedInPhysics = false;
			this->pObject->scale = 0.0f;
			this->pObject->wireFrame = true;
			this->pObject->position.x = 20.0f;
		}

		return;
	}

	const glm::vec3 GRAVITY = glm::vec3(0.0f, -2.0f, 0.0f);

	glm::vec3 deltaPosition = (float)deltaTime * this->pObject->vel;
	this->pObject->position += deltaPosition;

	// New velocity is based on acceleration over time
	glm::vec3 deltaVelocity = ((float)deltaTime * this->pObject->accel)
		+ ((float)deltaTime * GRAVITY);

	this->pObject->vel += deltaVelocity;

	if (pObject->position.y < -30.0f)
	{
		this->pObject->bIsUpdatedInPhysics = false;
		this->hasExploded = true;
		this->pObject->wireFrame = true;
	}

	return;
}

int cSphere::shapeType()
{
	return 2;
}

void cSphere::setVelocity(glm::vec3 newVelocity)
{
	this->pObject->vel = newVelocity;
}

void cSphere::setPosition(glm::vec3 newPos)
{
	this->pObject->position = newPos;
}

glm::vec3 cSphere::getVelocity()
{
	return this->pObject->vel;
}

glm::vec3 cSphere::getPosition()
{
	return this->pObject->position;
}

void cSphere::issueAlert(int pos1, int pos2, int type2)
{
	Alerter->makeAlert(pos1, pos2, this->shapeType(), type2);
	return;
}